/********************************************************************************
 Written by: Vinod Desai, NEX Robotics Pvt. Ltd.
 Edited by: Sachitanand Malewar, NEX Robotics Pvt. Ltd.
 AVR Studio Version 4.17, Build 666

 Date: 26th December 2010
 
 Application example: Robot control over serial port

 Concepts covered:  serial communication

 Serial Port used: UART1

 There are two components to the motion control:
 1. Direction control using pins PORTA0 to PORTA3
 2. Velocity control by PWM on pins PL3 and PL4 using OC5A and OC5B.

 In this experiment for the simplicity PL3 and PL4 are kept at logic 1.
 
 Pins for PWM are kept at logic 1.
  
 Connection Details:  	
 						
  Motion control:		L-1---->PA0;		L-2---->PA1;
   						R-1---->PA2;		R-2---->PA3;
   						PL3 (OC5A) ----> Logic 1; 	PL4 (OC5B) ----> Logic 1; 


  Serial Communication:	PORTD 2 --> RXD1 UART1 receive for RS232 serial communication
						PORTD 3 --> TXD1 UART1 transmit for RS232 serial communication

						PORTH 0 --> RXD2 UART 2 receive for USB - RS232 communication
						PORTH 1 --> TXD2 UART 2 transmit for USB - RS232 communication

						PORTE 0 --> RXD0 UART0 receive for ZigBee wireless communication
						PORTE 1 --> TXD0 UART0 transmit for ZigBee wireless communication

						PORTJ 0 --> RXD3 UART3 receive available on microcontroller expansion socket
						PORTJ 1 --> TXD3 UART3 transmit available on microcontroller expansion socket

Serial communication baud rate: 9600bps
To control robot use number pad of the keyboard which is located on the right hand side of the keyboard.
Make sure that NUM lock is on.

Commands:
			Keyboard Key	HEX value	Action
				8				0x38	Forward
				2				0x32	Backward
				4				0x34	Left
				6				0x36	Right
				5				0x35	Stop
				7				0x37	Buzzer on
				9				0x39	Buzzer off

 Note: 
 
 1. Make sure that in the configuration options following settings are 
 	done for proper operation of the code

 	Microcontroller: atmega2560
 	Frequency: 14745600
 	Optimization: -O0 (For more information read section: Selecting proper optimization 
 					options below figure 2.22 in the Software Manual)

 2. Difference between the codes for RS232 serial, USB and wireless communication is only in the serial port number.
 	Rest of the things are the same. 

 3. For USB communication check the Jumper 1 position on the ATMEGA2560 microcontroller adaptor board

 4. Auxiliary power can supply current up to 1 Ampere while Battery can supply current up to 
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
 DDRC = DDRC | 0x08;		//Setting PORTC 3 as outpt
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

//Function To Initialize UART1
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
void send ( int n)
{ 
  i = 0;
  while (n) 
 { // loop till there's nothing left
    a[i++] = n % 10; // assign the last digit
    n /= 10; // "right shift" the number
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
	
