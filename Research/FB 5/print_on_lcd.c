
  /*****************************************************************
 * Printed the stored data on lcd after run of the bot is complete.
 * Using timer5 interrupt to collect the state after every 1 seconds. 
 *
 ********************************************************************/ 
//header files
#define __OPTIMIZE__ -O0
#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
#include <math.h>
#include <stdlib.h>
//global variables
unsigned char ADC_Value;
float battvolt;
int val,proxy1,proxy2;
volatile int count=0;
int counter,battvoltage,proximity1,proximity2,sharp1;

//function to configure lcd port
void lcd_port_config (void)
{
	cli();
	DDRC = DDRC | 0xF7;		 //all the LCD pin's direction set as output
	PORTC = PORTC & 0x80;	 // all the LCD pins are set to logic 0 except PORTC 7(safety)
	sei();
}
void buzzer_pin_config(void)
{
	DDRC = DDRC | 0x08;
	PORTC = PORTC & 0xF7;// safety
}
void buzzer_on(void)
{
	PORTC = PORTC | 0x08;
}
void buzzer_off(void)
{
	PORTC = PORTC & 0xF7;
}
void adc_config(void)
{
	DDRF  =  0x00;
	PORTF = 0x00;
	DDRK  = 0x00;
	PORTK = 0x00;
}
//function to initialize adc
void adc_init(void)
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;		//MUX5 = 0
	ADMUX = 0x20;		//Vref =5V external(setting REFS1 &REFSO AS 0) --- ADLAR=1(left adjusted) --- MUX4:0 = 0000
	ACSR = 0x80;    //to disable analog comparator
	ADCSRA = 0x86;		//ADEN=1 ---  --- ADPS2:0 = 1 1 0
}
//function to initialize ports
void port_init (void)
{
	lcd_port_config();
	buzzer_pin_config();
	adc_config();
}

//This Function accepts the Channel Number and returns the corresponding digital Value
int getdata(int ch)
{ 
	int a;      //it stores the digital value of ADCH register
    if(ch>7)
	{
		ADCSRB = 0x08;//mux5 is set
	}		
	ch = ch & 0x07;     //store only 3 LSB bits
	ADMUX = 0x20 | ch; //Select the ADC channel with left adjust select
	ADCSRA = ADCSRA | 0x40;//  set start conversion bit
	while((ADCSRA&0x10)==0);	//Wait for ADC conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10; //clear ADIF (ADC Interrupt Flag) by writing 1 to it
	ADCSRB = 0x00;
	return a;
}
// This Function prints the digital Value Of Corresponding Channel No. at required Row
// and Column Location.
void print_sensor(char row, char coloumn,unsigned char channel)
{
	ADC_Value = getdata(channel);
	lcd_print(row, coloumn, ADC_Value, 3);
}
// This Function calculates the actual distance in millimeters(mm) from the input
// analog value of Sharp Sensor.
int Sharp(int adc_reading)
{
	float distance;
	int distanceInt;
	distance = (int)(10.00*(2799.6*(1.00/(pow(adc_reading,1.1546)))));
	distanceInt = (int)distance;
	if(distanceInt>800)
	{
		distanceInt=800;
	}
	return distanceInt;
}

//TIMER5 initialize - prescaler:256
// WGM: 1110) fast pwm, TOP=0xe100
// desired value: 1Hz
// actual value:  1.000Hz (0.0%)
void timer5_init(void)
{
	TCCR5A = 0b00000010;  //fast pwm
	TCCR5B = 0b00011100; //fast pwm with 256 as prescaler and ICR5 as top value
	TCCR5C = 0x00;
	ICR5H = 0xe1;
	ICR5L = 0x00;  //icr=57600

}
//This ISR can be used to schedule events like refreshing ADC data, LCD data
// Interrupt is called after ever 1 second when Timer5 has overflowed.
ISR(TIMER5_OVF_vect)
{
	
	//TIMER5 has overflowed
	count++;
	lcd_print(1,14,count,3);                        //prints the count value
	battvolt =(((getdata(0)*100)*0.07902) + 0.7);	//Prints Battery Voltage Status
	lcd_print(1,1,battvolt,4);                     //4 is the no. of digits
	
	proxy1=getdata(4);
	proxy2=getdata(5);
	val= Sharp(getdata(9));	     //Stores Distance calculated in a variable "value".
	lcd_print(2,6,val,3); 		//Prints Value Of Distance in MM measured by Sharp IR range Sensor1.
	enqueue();
	
}

void init_devices (void)
{
	cli();          //Clears the global interrupts
	port_init();
	adc_init();
	lcd_init();
	timer5_init();
	TIMSK5 = 0x01; //timer5 overflow interrupt enable.
	sei();         //Enables the global interrupts
}
// formation of node 
struct node
{
	int battvoltage;  //variable for storing values of battery voltage.
	int proximity1;  //variable for storing values of proximity sensor1.
	int proximity2;  //variable for storing values of proximity sensor2.
	int sharp1;      //variable for storing values of sharp sensor1.
	

	struct node *next;
	struct node *prev;
	};
	struct node* root= NULL;

	
	void enqueue()//adding of new node at start
	{
		struct node *newnode;
		newnode =(struct node*)malloc(sizeof(struct node));
		newnode->battvoltage=battvolt;
		newnode->proximity1=proxy1;
		newnode->proximity2=proxy2;
		newnode->sharp1=val;
		newnode->next=NULL;
		newnode->prev=NULL;
		
		if(root==NULL)
		{
			root = newnode;
		}
		else
		{
			root->prev = newnode;
			newnode->next = root;
			root = newnode; 
		}
		counter++;
	}
	void read()   //function to read stored value in queue
	{
		struct node *temp;
		temp = root;
		while(temp!= NULL)
		{
			//printing of stored value on lcd after run is complete
			lcd_print(1,6,counter,2);
			lcd_print(2,2,temp->battvoltage,3);
			lcd_print(2,6,temp->proximity1,3);
			lcd_print(2,10,temp->sharp1,3);
			_delay_ms(2000);
			lcd_wr_command(0x01);//clear the screen
			_delay_ms(500);
			temp=temp->next;
			counter--;
		}
		
	}
	
//main function
int main(void)
{   
	init_devices();
	while(1)
	{
		if(count==10)
		{
			TIMSK5 = TIMSK5 & 0xFE; //stop timer5 overflow interrupt
			read();
			_delay_ms(2000);
		break;
		
		}
		
	}
   
}
