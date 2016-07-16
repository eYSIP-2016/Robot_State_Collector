/********************************************************************************
 Written by: Vinod Desai, NEX Robotics Pvt. Ltd.
 Edited by: Sachitanand Malewar, NEX Robotics Pvt. Ltd.
 AVR Studio Version 4.17, Build 666

 Date: 26th December 2010

 This experiment demonstrates use of position encoders.

 Concepts covered: External Interrupts, Position control
 
 Microcontroller pins used:
 PORTA3 to PORTA0: Robot direction control
 PL3, PL4: Robot velocity control. Currently set to 1 as PWM is not used
 PE4 (INT4): External interrupt for left motor position encoder 
 PE5 (INT5): External interrupt for the right position encoder

 Note: 
 
 1. Make sure that in the configuration options following settings are 
 	done for proper operation of the code

 	Microcontroller: atmega2560
    Frequency: 14745600
 	Optimization: -O0  (For more information read section: Selecting proper optimization 
 					options below figure 2.22 in the Software Manual)

 2.	It is observed that external interrupts does not work with the optimization level -Os

 3. Auxiliary power can supply current up to 1 Ampere while Battery can supply current up to 
 	2 Ampere. When both motors of the robot changes direction suddenly without stopping, 
	it produces large current surge. When robot is powered by Auxiliary power which can supply
	only 1 Ampere of current, sudden direction change in both the motors will cause current 
	surge which can reset the microcontroller because of sudden fall in voltage. 
	It is a good practice to stop the motors for at least 0.5seconds before changing 
	the direction. This will also increase the useable time of the fully charged battery.
	the life of the motor.

*********************************************************************************/

/********************************************************************************

   Copyright (c) 2010, NEX Robotics Pvt. Ltd.                       -*- c -*-
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

   * Source code can be used for academic purpose. 
	 For commercial use permission form the author needs to be taken.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. 

  Software released under Creative Commence cc by-nc-sa licence.
  For legal information refer to: 
  http://creativecommons.org/licenses/by-nc-sa/3.0/legalcode

********************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>


#include "lcd.c"

int read = 1;
unsigned char SHARP_1;
unsigned char dt;
char a[3];
unsigned char data;
unsigned char ADC_flag;
unsigned char ADC_Conversion(unsigned char);
unsigned char ADC_Value;
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

 Direction &= 0x0F; 		// removing upper nibbel for the protection
 PortARestore = PORTA; 		// reading the PORTA original status
 PortARestore &= 0xF0; 		// making lower direction nibbel to 0
 PortARestore |= Direction; // adding lower nibbel for forward command and restoring the PORTA status
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
	ADCSRA = 0x86;		//ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}

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

//TIMER4 initialize - prescale:1024
// WGM: 0) Normal, TOP=0xFFFF
// desired value: 1Hz
// actual value:  1.000Hz (0.0%)
void timer4_init(void)
{
 TCCR4B = 0x00; //stop
 TCNT4H = 0x1F; //Counter higher 8 bit value
 TCNT4L = 0x01; //Counter lower 8 bit value
 OCR4AH = 0x00; //Output Compair Register (OCR)- Not used
 OCR4AL = 0x00; //Output Compair Register (OCR)- Not used
 OCR4BH = 0x00; //Output Compair Register (OCR)- Not used
 OCR4BL = 0x00; //Output Compair Register (OCR)- Not used
 OCR4CH = 0x00; //Output Compair Register (OCR)- Not used
 OCR4CL = 0x00; //Output Compair Register (OCR)- Not used
 ICR4H  = 0x00; //Input Capture Register (ICR)- Not used
 ICR4L  = 0x00; //Input Capture Register (ICR)- Not used
 TCCR4A = 0x00; 
 TCCR4C = 0x00;
 TCCR4B = 0x04; //start Timer
}


int	 store[1][10]; 

int count = 0;
//This ISR can be used to schedule events like refreshing ADC data, LCD data
ISR(TIMER4_OVF_vect)
{
	cli();
 TCNT4H = 0x1F; //reload counter high value
 TCNT4L = 0x01; //reload counter low value
 if (read == 1)
 {
 	lcd_print(1, 1, count, 3);
  sensor_data_interpretation();
  lcd_print(1, 10, converttomm_41sk(SHARP_1), 3);
  store[0][count]=converttomm_41sk(SHARP_1);
  count++;
 }   
 sei();
} 


void uart2_init(void)
{
 UCSR2B = 0x00; //disable while setting baud rate
 UCSR2A = 0x00;
 UCSR2C = 0x06;
 UBRR2L = 0x5F; //set baud rate lo
 UBRR2H = 0x00; //set baud rate hi
 UCSR2B = 0x98;
}


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
 UDR2 = a[2] + 48;
 UDR2 = a[1] + 48;
 _delay_ms(10);
 UDR2 = a[0] + 48;
 UDR2 = 32;
}

SIGNAL(SIG_USART2_RECV) 		// ISR for receive complete interrupt
{
    cli();

	data = UDR2; 				//making copy of data from UDR1 in 'data' variable				//echo data back to PC

		if(data == 0x35) //ASCII value of 5
		{
		  lcd_init();


		for (int  i = 0 ; i < count ; i++)
		{
          lcd_print(2, 10, store[0][i], 3);
          lcd_print(2, 14, i, 3);
			_delay_ms(1000);
			
 		  send(store[0][i]);
        }
	}

sei();

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

//Function to initialize all the devices
void init_devices()
{
 cli(); //Clears the global interrupt
 port_init();  //Initializes all the ports
 left_position_encoder_interrupt_init();
 right_position_encoder_interrupt_init();
 timer4_init();
 adc_init();
 uart2_init();
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
    forward();
	while(count <= 6);
	stop();
	read = 0;
	TIMSK4 = 0x00;
	TCCR4A = 0x00; 
	TCCR4C = 0x00;
 	TCCR4B = 0x00;
	lcd_init();
	lcd_print(1,1 , store[0][0], 3);
	lcd_print(1,5 , store[0][1], 3);
	lcd_print(1,9 , store[0][2], 3);
	lcd_print(1,13 , store[0][3], 3);
	lcd_print(2,1 , store[0][4], 3);
	lcd_print(2,5 , store[0][5], 3);
	while(1);
	 
	
}





