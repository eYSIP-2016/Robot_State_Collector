/********************************************************************************
*Description:  Serial communication
*Objective:    Trying to send integer to laptop via USB serial communication
*Problem :     We can't send the integer through USART directly.
*Solution:     We converted the integer into digits & send them as characters using send(int) function.
*Bug     :     Some digits are missed during transmission which was later corrected by using delay but still this was not appropriate..
*********************************************************************************/


#include "lcd.c"
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include <math.h>

unsigned char data; //to store received data from UDR1
unsigned char SHARP_1;
unsigned char ADC_flag;
unsigned char ADC_Conversion(unsigned char);
unsigned char ADC_Value;
int d=0;
unsigned char dt;
int i = 0;
char a[3];

void buzzer_pin_config (void)
{
 DDRC = DDRC | 0x08;		//Setting PORTC 3 as output
 PORTC = PORTC & 0xF7;		//Setting PORTC 3 logic low to turnoff buzzer
}

void motion_pin_config (void)
{
 DDRA = DDRA | 0x0F;
 PORTA = PORTA & 0xF0;
 DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
 PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.
}


void spi_pin_config (void)
{
 DDRB = DDRB | 0x07;
 PORTB = PORTB | 0x07;
}


void lcd_port_config (void)
{
 DDRC = DDRC | 0xF7;    //all the LCD pin's direction set as output
 PORTC = PORTC & 0x80;  // all the LCD pins are set to logic 0 except PORTC 7
}



void adc_pin_config (void)
{
 DDRF = 0x00;  //set PORTF direction as input
 PORTF = 0x00; //set PORTF pins floating
 DDRK = 0x00;  //set PORTK direction as input
 PORTK = 0x00; //set PORTK pins floating
}


void port_init(void)
{
 motion_pin_config();
 spi_pin_config();
 lcd_port_config();
 buzzer_pin_config();           
 adc_pin_config();
}

void spi_init(void)
{
 SPCR = 0x53; //setup SPI
 SPSR = 0x00; //setup SPI
 SPDR = 0x00;
}

//ADC initialize
// Conversion time: 56uS
void adc_init(void)
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;		//MUX5 = 0
	ADMUX = 0x20;		//Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;		//ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}


unsigned char spi_master_tx_and_rx (unsigned char data)
{
 unsigned char rx_data = 0;

 PORTB = PORTB & 0xFE;        // make SS pin low
 SPDR = data;
 while(!(SPSR & (1<<SPIF)));  //wait for data transmission to complete

 _delay_ms(1);                //time for ADC conversion in the slave microcontroller
 
 SPDR = 0x50;                 // send dummy byte to read back data from the slave microcontroller
 while(!(SPSR & (1<<SPIF)));  //wait for data reception to complete
 rx_data = SPDR;
 PORTB = PORTB | 0x01;        // make SS high
 return rx_data;
}

//-------------------------------------------------------------------------------

unsigned char ADC_Conversion(unsigned char ch)
{
unsigned char a;
 if(ch>7)
	{
		ADCSRB = 0x08;
	}
 ch = ch & 0x07;			  //Store only 3 LSB bits
 ADMUX= 0x20 | ch;			  //Select the ADC channel with left adjust select
 ADC_flag = 0x00; 			  //Clear the user defined flag
 ADCSRA = ADCSRA | 0x40;	  //Set start conversion bit
 while((ADCSRA&0x10)==0);	  //Wait for ADC conversion to complete
 a=ADCH;
 ADCSRA = ADCSRA|0x10;        //clear ADIF (ADC Interrupt Flag) by writing 1 to it
 ADCSRB = 0x00;
 return a;
}


void sensor_data_interpretation(void) //ld, fd, rd, light int
{

 SHARP_1 = ADC_Conversion(9);
 
}

void buzzer_on (void)
{
 unsigned char port_restore = 0;
 port_restore = PINC;
 port_restore = port_restore | 0x08;
 PORTC = port_restore;
}

void buzzer_off (void)
{
 unsigned char port_restore = 0;
 port_restore = PINC;
 port_restore = port_restore & 0xF7;
 PORTC = port_restore;
}

//Function To Initialize UART2
// desired baud rate:9600
// actual baud rate:9600 (error 0.0%)
// char size: 8 bit
// parity: Disabled
void uart2_init(void)
{
 UCSR2B = 0x00; //disable while setting baud rate
 UCSR2A = 0x00;
 UCSR2C = 0x06;
 UBRR2L = 0x5F; //set baud rate lo
 UBRR2H = 0x00; //set baud rate hi
 UCSR2B = 0x98;
}
/*
* Function Name:	 converttomm_41sk
* Input:			Digital value of sharp sensor values
* Output:           Distance in mm
* Example Call:		converttomm_41sk(_conv_adc(9));
*
*/

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

/***Function to convert integer into digits and send them as character********/
void send ( int n)
{ 
  i = 0;
  while (n) 
 {   // loop till there's nothing left
    a[i++] = n % 10; // assign the last digit
    n /= 10;        // "right shift" the number
 }
 UDR2 = a[2] + 48;
 UDR2 = a[1] + 48;
 UDR2 = a[0] + 48;
}

SIGNAL(SIG_USART2_RECV) 		// ISR for receive complete interrupt
{

	data = UDR2; 				//making copy of data from UDR1 in 'data' variable				//echo data back to PC

		if(data == 0x35) //ASCII value of 5
		{
		  lcd_init();
	      lcd_cursor(1,1);
	      lcd_string("Data r");
	      sensor_data_interpretation();
		  lcd_print(2,1,converttomm_41sk(SHARP_1),3);
		  lcd_print(2,5,converttomm(SHARP_1),3);
		  d =converttomm(SHARP_1);
		  send(d);
}



}


//Function To Initialize all The Devices
void init_devices()
{
 cli(); //Clears the global interrupts
 port_init();  //Initializes all the ports
 uart2_init(); //Initialize UART1 for serial communication
 lcd_init();
 spi_init();
 lcd_set_4bit();
 adc_init();
sei();   //Enables the global interrupts
}

//Main Function
int main(void)
{
	init_devices();
	lcd_set_4bit();
	lcd_init();
	lcd_cursor(1,5);
	lcd_string("HI");
	lcd_print(2,1,d,3);
	while(1);
	
}
	
