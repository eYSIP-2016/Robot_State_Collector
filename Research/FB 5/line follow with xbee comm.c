/*********************************************************************************************************************
* Objective :  Implementing wireless serial communication via Xbee module.
* Description: Through this program we are sending the sensor values of robot (while white line sensing program is
running in the robot) to laptop via wireless serial communication after every 1 second using xbee module.
Now we are not storing the state value in robot,we are directly sending it to the laptop via xbee module.
It solved our state constraint problem.
* Timer 4 interrupt is used to collect the state after every 1 seconds.
* Bug: sometimes correct data is not received at receiver side i.e. some digits of an integer are missing.

*************************************************************************************************************************/

 #define __OPTIMIZE__ -O0
 #define F_CPU 14745600
 //header files
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include "lcd.h"

int read = 1;
unsigned char SHARP_1;
unsigned char dt;
char a[3];
unsigned char flag = 0;
unsigned char data;
unsigned char ADC_flag;
unsigned char ADC_Conversion(unsigned char);
unsigned char ADC_Value;
unsigned char Left_white_line = 0;     //variable to store left white line sensor value
unsigned char Center_white_line = 0;   //variable to store center white line sensor value
unsigned char Right_white_line = 0;    //variable to store right white line sensor value
unsigned long int ShaftCountLeft = 0; //to keep track of left position encoder 
unsigned long int ShaftCountRight = 0; //to keep track of right position encoder
unsigned int Degrees; //to accept angle in degrees for turning

//Function to configure ports to enable robot's motion
void motion_pin_config (void) 
{
 DDRA = DDRA | 0x0F;
 PORTA = PORTA & 0xF0;
 DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
 PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.
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

//Function to configure INT4 (PORTE 4) pin as input for the left position encoder
void left_encoder_pin_config (void)
{
 DDRE  = DDRE & 0xEF;  //Set the direction of the PORTE 4 pin as input
 PORTE = PORTE | 0x10; //Enable internal pull-up for PORTE 4 pin
}

//Function to configure INT5 (PORTE 5) pin as input for the right position encoder
void right_encoder_pin_config (void)
{
 DDRE  = DDRE & 0xDF;  //Set the direction of the PORTE 4 pin as input
 PORTE = PORTE | 0x20; //Enable internal pull-up for PORTE 4 pin
}

//Function to initialize ports
void port_init()
{
 motion_pin_config(); //robot motion pins config
 left_encoder_pin_config(); //left encoder pin config
 lcd_port_config();
 right_encoder_pin_config(); //right encoder pin config	
 adc_pin_config();
}

void left_position_encoder_interrupt_init (void) //Interrupt 4 enable
{
 cli(); //Clears the global interrupt
 EICRB = EICRB | 0x02; // INT4 is set to trigger with falling edge
 EIMSK = EIMSK | 0x10; // Enable Interrupt INT4 for left position encoder
 sei();   // Enables the global interrupt 
}

void right_position_encoder_interrupt_init (void) //Interrupt 5 enable
{
 cli(); //Clears the global interrupt
 EICRB = EICRB | 0x08; // INT5 is set to trigger with falling edge
 EIMSK = EIMSK | 0x20; // Enable Interrupt INT5 for right position encoder
 sei();   // Enables the global interrupt 
}

//ISR for right position encoder
ISR(INT5_vect)  
{
 ShaftCountRight++;  //increment right shaft position count
}


//ISR for left position encoder
ISR(INT4_vect)
{
 ShaftCountLeft++;  //increment left shaft position count
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

void forward (void) //both wheels forward
{
  motion_set(0x06);
}

void back (void) //both wheels backward
{
  motion_set(0x09);
}

void left (void) //Left wheel backward, Right wheel forward
{
  motion_set(0x05);
}

void right (void) //Left wheel forward, Right wheel backward
{
  motion_set(0x0A);
}

void soft_left (void) //Left wheel stationary, Right wheel forward
{
 motion_set(0x04);
}

void soft_right (void) //Left wheel forward, Right wheel is stationary
{
 motion_set(0x02);
}

void soft_left_2 (void) //Left wheel backward, right wheel stationary
{
 motion_set(0x01);
}

void soft_right_2 (void) //Left wheel stationary, Right wheel backward
{
 motion_set(0x08);
}

void stop (void)
{
  motion_set(0x00);
}
//ADC initialize
// Conversion time: 56uS
void adc_init(void)
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;		//MUX5 = 0
	ADMUX = 0x20;		//Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;		//ADEN=1 --- ADIE=0 --- ADPS2:0 = 1 1 0
}
//This Function accepts the Channel Number and returns the corresponding digital Value
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

//Function used for turning robot by specified degrees
void angle_rotate(unsigned int Degrees)
{
 float ReqdShaftCount = 0;
 unsigned long int ReqdShaftCountInt = 0;

 ReqdShaftCount = (float) Degrees/ 4.090; // division by resolution to get shaft count
 ReqdShaftCountInt = (unsigned int) ReqdShaftCount;
 ShaftCountRight = 0; 
 ShaftCountLeft = 0; 

 while (1)
 {
  if((ShaftCountRight >= ReqdShaftCountInt) | (ShaftCountLeft >= ReqdShaftCountInt))
  break;
 }
 stop(); //Stop robot
}


void sensor_data_interpretation(void) //ld, fd, rd, light int
{

 SHARP_1 = ADC_Conversion(9);
 Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
 Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
 Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
 
}


//function used for distance calculation(in mm) of sharp sensor
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

//function used for distance calculation(in mm) of 41sk type sharp sensor
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

//TIMER4 initialize - prescale:1024
// WGM: 0) Normal, TOP=0xFFFF
// desired value: 1Hz
// actual value:  1.000Hz (0.0%)
//timer 4 will overflow after 1 second.
void timer4_init(void)
{
	TCCR4B = 0x00; //stop
	TCNT4H = 0x1F; //Counter higher 8 bit value
	TCNT4L = 0x01; //Counter lower 8 bit value
	OCR4AH = 0x00; //Output compare Register (OCR)- Not used
	OCR4AL = 0x00; //Output compare Register (OCR)- Not used
	OCR4BH = 0x00; //Output Compare Register (OCR)- Not used
	OCR4BL = 0x00; //Output Compare Register (OCR)- Not used
	OCR4CH = 0x00; //Output Compare Register (OCR)- Not used
	OCR4CL = 0x00; //Output Compare Register (OCR)- Not used
	ICR4H  = 0x00; //Input Capture Register (ICR)- Not used
	ICR4L  = 0x00; //Input Capture Register (ICR)- Not used
	TCCR4A = 0x00;
	TCCR4C = 0x00;
	TCCR4B = 0x04; //start Timer
}

/*
* Function Name:	send
* Input:			3 digit integer value to be sent through serial communication.
* Output:           Integer is sent as string via serial communication.
* Logic:			convert t Numbers (Integers) to string and then send the string.
* Example Call:		send(345);
*
*/
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
  _delay_ms(10);
  UDR2 = a[2] + 48;   //sending first digit
  UDR2 = a[1] + 48;   //sending second digit
  _delay_ms(10);
  UDR2 = a[0] + 48;   //sending third digit
  UDR2 = 32;          //To give space between two integer

}
int count = 0;
//This ISR can be used to schedule events like refreshing ADC data, LCD data.
//This interrupt service routine will be called after every 1 second
ISR(TIMER4_OVF_vect)
{
 lcd_print(1, 1, count, 3);
 TCNT4H = 0x1F; //reload counter high value
 TCNT4L = 0x01; //reload counter low value
 if (read == 1)
 {
  //sensor_data_interpretation();
  send(converttomm_41sk(SHARP_1));
  send(Left_white_line);
  send(Center_white_line);
  send(Right_white_line);
  count++;
 }   
} 
/*Function To Initialize UART0
* wireless serial communication using xbee module
* desired baud rate:9600
* actual baud rate:9600 (error 0.0%)
* char size: 8 bit
* parity: Disabled
*/
void uart0_init(void)
{
 UCSR0B = 0x00; //disable while setting baud rate
 UCSR0A = 0x00;
 UCSR0C = 0x06;
 UBRR0L = 0x5F; //set baud rate lo
 UBRR0H = 0x00; //set baud rate hi
 UCSR0B = 0x98;
}

//Function used for moving robot forward by specified distance

void linear_distance_mm(unsigned int DistanceInMM)
{
 float ReqdShaftCount = 0;
 unsigned long int ReqdShaftCountInt = 0;

 ReqdShaftCount = DistanceInMM / 5.338; // division by resolution to get shaft count
 ReqdShaftCountInt = (unsigned long int) ReqdShaftCount;
  
 ShaftCountRight = 0;
 while(1)
 {
  if(ShaftCountRight > ReqdShaftCountInt)
  {
  	break;
  }
 } 
 stop(); //Stop robot
}

void forward_mm(unsigned int DistanceInMM)
{
 forward();
 linear_distance_mm(DistanceInMM);
}

void back_mm(unsigned int DistanceInMM)
{
 back();
 linear_distance_mm(DistanceInMM);
}

void left_degrees(unsigned int Degrees) 
{
// 88 pulses for 360 degrees rotation 4.090 degrees per count
 left(); //Turn left
 angle_rotate(Degrees);
}

void right_degrees(unsigned int Degrees)
{
// 88 pulses for 360 degrees rotation 4.090 degrees per count
 right(); //Turn right
 angle_rotate(Degrees);
}
	

void soft_left_degrees(unsigned int Degrees)
{
 // 176 pulses for 360 degrees rotation 2.045 degrees per count
 soft_left(); //Turn soft left
 Degrees=Degrees*2;
 angle_rotate(Degrees);
}

void soft_right_degrees(unsigned int Degrees)
{
 // 176 pulses for 360 degrees rotation 2.045 degrees per count
 soft_right();  //Turn soft right
 Degrees=Degrees*2;
 angle_rotate(Degrees);
}

void soft_left_2_degrees(unsigned int Degrees)
{
 // 176 pulses for 360 degrees rotation 2.045 degrees per count
 soft_left_2(); //Turn reverse soft left
 Degrees=Degrees*2;
 angle_rotate(Degrees);
}

void soft_right_2_degrees(unsigned int Degrees)
{
 // 176 pulses for 360 degrees rotation 2.045 degrees per count
 soft_right_2();  //Turn reverse soft right
 Degrees=Degrees*2;
 angle_rotate(Degrees);
}

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


//Function for velocity control
void velocity (unsigned char left_motor, unsigned char right_motor)
{
	cli();
	OCR5AL = (unsigned char)left_motor;
	OCR5BL = (unsigned char)right_motor;
	sei();
}

//Function to initialize all the devices
void init_devices()
{
 cli(); //Clears the global interrupt
 port_init();  //Initializes all the ports
 left_position_encoder_interrupt_init();
 right_position_encoder_interrupt_init();
 timer4_init();
 adc_init();
 timer5_init();
 uart0_init(); //Initialize UART0 for serial communication
 TIMSK4 = 0x01;
 sei();   // Enables the global interrupt 
}


//Main Function

int main(void)
{
	init_devices();
	lcd_set_4bit();
	lcd_init();
	lcd_cursor(1,5);
	lcd_string("HI");
	lcd_print(2,1,read,3);
	while(1)
	{ 
	    sensor_data_interpretation();

		flag=0;
		

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
		if(count>=6)
		 break;
	}
	stop();
	read = 0;
	TIMSK4 = 0x00;
	TCCR4A = 0x00; 
	TCCR4C = 0x00;
 	TCCR4B = 0x00;
	while(1);
	 
	
}





