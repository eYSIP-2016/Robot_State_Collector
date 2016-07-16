
#define __OPTIMIZE__ -O0
#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>
#include "lcd.h"
unsigned char sp1;
char a[3];
unsigned char flag = 0;
unsigned char data;
unsigned char ADC_flag;
unsigned char conv_adc(unsigned char);
unsigned char lwl = 0;
unsigned char cwl = 0;
unsigned char rwl = 0;
// variables of main program
void port_init();
void timer5_init();
void velocity(unsigned char, unsigned char);
void motors_delay();

unsigned char ADC_Conversion(unsigned char);
unsigned char ADC_Value;
//unsigned char flag = 0;
unsigned char Left_white_line = 0;
unsigned char Center_white_line = 0;
unsigned char Right_white_line = 0;

void adcpinconfig (void)
{
	DDRF = 0x00;  //set PORTF direction as input
	PORTF = 0x00; //set PORTF pins floating
	DDRK = 0x00;  //set PORTK direction as input
	PORTK = 0x00; //set PORTK pins floating
}

//ADC initialize
// Conversion time: 56uS
void adcinit(void)
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;		//MUX5 = 0
	ADMUX = 0x20;		//Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;		//ADEN=1 --- ADIE=0 --- ADPS2:0 = 1 1 0
}
unsigned char conv_adc(unsigned char ch)
{
	unsigned char a;
	if(ch>7)
	{
		ADCSRB = 0x08;
	}
	ch = ch & 0x07;			  //Store only 3 LSB bits
	ADMUX= 0x20 | ch;		 //Select the ADC channel with left adjust select
	ADCSRA = ADCSRA | 0x40;	          //Set start conversion bit
	while((ADCSRA&0x10)==0);	  //Wait for ADC conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10;        //clear ADIF (ADC Interrupt Flag) by writing 1 to it
	ADCSRB = 0x00;
	return a;
}

unsigned int converttomm(unsigned int a )
{
	double b;
	b=a;
	b=pow(b,1.1546);
	b=1/b;
	b=b*2799.6;
	b=10*b;
	b=(int) b;
	return b;

}

unsigned int converttomm_41sk(unsigned int a)
{
	double b;
	b=a;
	b = b*0.001240875;
	b = b+0.005;
	b = 10/b;
	b = b-24.2;
	b = (int)b;
	return b;
}

//TIMER4 initialize - pre scaler:256
// WGM: 0) Normal, TOP=0x7080
// desired value: 2Hz
// actual value:  2.000Hz (0.0%)
void timer4_init(void)
{
	TCCR4B = 0x00; //stop
	OCR4AH = 0x00; //Output Compare Register (OCR)- Not used
	OCR4AL = 0x00; //Output compare Register (OCR)- Not used
	OCR4BH = 0x00; //Output compare Register (OCR)- Not used
	OCR4BL = 0x00; //Output compare Register (OCR)- Not used
	OCR4CH = 0x00; //Output compare Register (OCR)- Not used
	OCR4CL = 0x00; //Output compare Register (OCR)- Not used
	ICR4H  = 0x70; //Input Capture Register (ICR)
	ICR4L  = 0x80; //Input Capture Register (ICR)
	TCCR4C = 0x00;
	TCCR4A = 0b00000010;
	TCCR4B = 0b00011100; //start Timer
}
//conversion of integer into character for serial transmission
void send ( int n)
{
	int z = 0;
	int c = n;
	a[0] = 0;
	a[1] = 0;
	a[2] = 0;
	while (c)
	{ // loop till there's nothing left
		a[z++] = (char)(c % 10); // assign the last digit
		c /= 10; // "right shift" the number
	}
	
	UDR0 = a[2] + 48;
	while(!(UCSR0A & (1<<UDRE0)))
	{
		//Do nothing
	}
	UDR0 = a[1] + 48;
	while(!(UCSR0A & (1<<UDRE0)))
	{
		//Do nothing
	}
	UDR0 = a[0] + 48;
	while(!(UCSR0A & (1<<UDRE0)))
	{
		//Do nothing
	}
	UDR0 = 32;
	while(!(UCSR0A & (1<<UDRE0)))
	{
		//Do nothing
	}
}

int count = 0;
//This ISR can be used to schedule events like refreshing ADC data, LCD data
ISR(TIMER4_OVF_vect)
{
	cli();
	
		sp1 = ADC_Conversion(9);
		lwl= ADC_Conversion(3);	        //Getting data of Left WL Sensor
		cwl = ADC_Conversion(2);	//Getting data of Center WL Sensor
		rwl = ADC_Conversion(1);	//Getting data of Right WL Sensor

		//sensor_data_interpretation();
		send(count);
		send(converttomm_41sk(sp1));
		send(lwl);
		send(cwl);
		send(rwl);
		
		count++;
		sei();
	
}

//Function To Initialize UART0
// desired baud rate:9600
// actual baud rate:9600 (error 0.0%)
// char size: 8 bit
// parity: Disabled
void uart0_init(void)
{
	UCSR0B = 0x00; //disable while setting baud rate
	UCSR0A = 0x00;
	UCSR0C = 0x06;
	UBRR0L = 0x5F; //set baud rate lo
	UBRR0H = 0x00; //set baud rate hi
	UCSR0B = 0x98;
}
//Function to initialize ports
void portinit()
{

	adcpinconfig();
}

//Function to initialize all the devices
void initdevices()
{
	cli(); //Clears the global interrupt
	//portinit();  //Initializes all the ports
	timer4_init();
	//adcinit();
	uart0_init(); //Initialize UART1 for serial communication
	TIMSK4 = 0x01;
	sei();   // Enables the global interrupt
}

//main program

//Function to configure LCD port
void lcd_port_config (void)
{
 DDRC = DDRC | 0xF7; //all the LCD pin's direction set as output
 PORTC = PORTC & 0x80; // all the LCD pins are set to logic 0 except PORTC 7
}

//ADC pin configuration
void adc_pin_config (void)
{
 DDRF = 0x00; 
 PORTF = 0x00;
 DDRK = 0x00;
 PORTK = 0x00;
}

//Function to configure ports to enable robot's motion
void motion_pin_config (void) 
{
 DDRA = DDRA | 0x0F;
 PORTA = PORTA & 0xF0;
 DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
 PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.
}

//Function to Initialize PORTS
void port_init()
{
	lcd_port_config();
	adc_pin_config();
	motion_pin_config();	
}

// Timer 5 initialized in PWM mode for velocity control
// Prescale:256
// PWM 8bit fast, TOP=0x00FF
// Timer Frequency:225.000Hz
void timer5_init()
{
	TCCR5B = 0x00;	//Stop
	TCNT5H = 0xFF;	//Counter higher 8-bit value to which OCR5xH value is compared with
	TCNT5L = 0x01;	//Counter lower 8-bit value to which OCR5xH value is compared with
	OCR5AH = 0x00;	//Output compare register high value for Left Motor
	OCR5AL = 0xFF;	//Output compare register low value for Left Motor
	OCR5BH = 0x00;	//Output compare register high value for Right Motor
	OCR5BL = 0xFF;	//Output compare register low value for Right Motor
	OCR5CH = 0x00;	//Output compare register high value for Motor C1
	OCR5CL = 0xFF;	//Output compare register low value for Motor C1
	TCCR5A = 0xA9;	/*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
 					  For Overriding normal port functionality to OCRnA outputs.
				  	  {WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/
	
	TCCR5B = 0x0B;	//WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}

void adc_init()
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;		//MUX5 = 0
	ADMUX = 0x20;		//Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;		//ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}

//Function For ADC Conversion
unsigned char ADC_Conversion(unsigned char Ch) 
{
	unsigned char a;
	if(Ch>7)
	{
		ADCSRB = 0x08;
	}
	Ch = Ch & 0x07;  			
	ADMUX= 0x20| Ch;	   		
	ADCSRA = ADCSRA | 0x40;		//Set start conversion bit
	while((ADCSRA&0x10)==0);	//Wait for conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10; //clear ADIF (ADC Interrupt Flag) by writing 1 to it
	ADCSRB = 0x00;
	return a;
}

//Function To Print Sesor Values At Desired Row And Coloumn Location on LCD
void print_sensor(char row, char coloumn,unsigned char channel)
{
	
	ADC_Value = ADC_Conversion(channel);
	lcd_print(row, coloumn, ADC_Value, 3);
}

//Function for velocity control
void velocity (unsigned char left_motor, unsigned char right_motor)
{
	OCR5AL = (unsigned char)left_motor;
	OCR5BL = (unsigned char)right_motor;
}

//Function used for setting motor's direction
void motion_set (unsigned char Direction)
{
 unsigned char PortARestore = 0;

 Direction &= 0x0F; 		// removing upper nibble for the protection
 PortARestore = PORTA; 		// reading the PORTA original status
 PortARestore &= 0xF0; 		// making lower direction nibble to 0
 PortARestore |= Direction; // adding lower nibble for forward command and restoring the PORTA status
 PORTA = PortARestore; 		// executing the command
}

void forward (void) 
{
  motion_set (0x06);
}

void stop (void)
{
  motion_set (0x00);
}

void init_devices (void)
{
 	cli(); //Clears the global interrupts
	port_init();
	adc_init();
	timer5_init();
	sei();   //Enables the global interrupts
}

//Main Function
int main()
{
	
	init_devices();
	initdevices();
	lcd_set_4bit();
	lcd_init();
	
	while(1)
	{

		Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
		Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
		Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor

		flag=0;

		lcd_print(1,1,Left_white_line,3);	//Prints value of White Line Sensor1
	        lcd_print(1,5,Center_white_line,3);	//Prints Value of White Line Sensor2
		lcd_print(1,9,Right_white_line,3);	//Prints Value of White Line Sensor3
		
		

		if(Center_white_line<0x28)
		{
			flag=1;
			forward();
			velocity(150,150);
		}

		if((Left_white_line>0x28) && (flag==0))
		{
			flag=1;
			forward();
			velocity(130,50);
		}

		if((Right_white_line>0x28) && (flag==0))
		{
			flag=1;
			forward();
			velocity(50,130);
		}

		if(Center_white_line>0x28 && Left_white_line>0x28 && Right_white_line>0x28)
		{
			forward();
			velocity(0,0);
		}

	}
	
}	