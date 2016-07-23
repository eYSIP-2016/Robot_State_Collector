
/*
*Project name:          Robot State Collector
*Description:           This program is about to collect the state(like different sensor values etc.)of Fire Bird V robot 
                        repeatedly after every 0.5 sec using timer4 interrupt and sending the values using XBee.
*Author's name:         Amanpreet Singh,Amit Raushan 
*Mentor's name:         Shubham Gupta
*Filename:              state_collect.h
*Functions:             _adc_pinconfig (),_adc_init(),_conv_adc(unsigned char),converttomm_41sk(unsigned int)._timer4_init(),
                        send ( int),ISR(TIMER4_OVF_vect),_uart0_init(),_port_init(),_start_collection()

*Global variables:       count,sp1,sp2,sp3,sp4,sp5,lwl,cwl,rwl,IR1,IR2,IR3,IR4,IR5
 ************************************************************************************/   
/*********************************************************************************      
ADC pin Connections used for state collection of different sensors:
ADC CH. 	PORT	Sensor
1		PF1	White line sensor 3
2		PF2	White line sensor 2
3		PF3	White line sensor 1
4		PF4	IR Proximity analog sensor 1
5		PF5	IR Proximity analog sensor 2
6		PF6	IR Proximity analog sensor 3
7		PF7	IR Proximity analog sensor 4
8		PK0	IR Proximity analog sensor 5
9		PK1	Sharp IR range sensor 1
10		PK2	Sharp IR range sensor 2
11		PK3	Sharp IR range sensor 3
12		PK4	Sharp IR range sensor 4
13		PK5	Sharp IR range sensor 5


 State collection sequence:
 **************************************************************************************
 * COUNT VALUE  LEFT WL SENSOR  CENTER WL SENSOR  RIGHT WL SENSOR	 SP1  SP2  SP3  SP4 SP5
 IR1  IR2  IR3  IR4  IR5
 
 
 note: SP->Sharp IR sensor
       IR->IR proximity analog sensor
 ***************************************************************************************
  Note:
  
  1. Don't forget to include "state_collect.h" in User's program and also it is necessary to write the function
  "_start_collection()" in user's main program and this function should be called at last position before infinite 'while loop'. 


  2.Here, distance calculation (calculated in converttomm_41sk() function)is for Sharp GP2D12 (10cm-80cm) IR Range sensor.
  If your sharp sensor is of different configuration then use your required conversion formula to calculate the distance. 
  
  

  *********************************************************************************/


#ifndef STATE_COLLECT_H   /* Include guard */
#define STATE_COLLECT_H


#include "common_header.h"//include necessary header files

/*global variables*/

char a[3];          //array having size 3
int count = 0;
unsigned char sp1,sp2,sp3,sp4,sp5;    //variables which contains sharp sensor values
unsigned char lwl = 0;           //left white line sensor 
unsigned char cwl = 0;           //center white line sensor 
unsigned char rwl = 0;           //right white line sensor 
unsigned char IR1,IR2,IR3,IR4,IR5 = 0;//proximity sensors 


/*
*
* Function Name:	_adc_pinconfig
* Input:			NONE
* Output:           PORTF & PORTK pins are set as input.         
* Logic:			1 denotes to the output pin and 0 denotes to the input pin
* Example Call:		_adc_pinconfig()
*
*/
void _adc_pinconfig (void)
{
	DDRF = DDRF & 0x01;  //set PORTF direction as input
	PORTF = 0x00; //set PORTF pins floating
	DDRK = DDRK & 0xC0;  //set PORTK direction as input
	PORTK = 0x00; //set PORTK pins floating
}
/*
* Function Name:	_adc_init
* Input:			NONE
* Output:           Initialization of ADC channel
* Conversion time:  56uS
* Logic:			1 denotes to the setting of the pin
* Example Call:		_adc_init()
*
*/
void _adc_init(void)
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;		//MUX5 = 0
	ADMUX = 0x20;		//Vref=5V external --- ADLAR=1(result is left adjusted) --- MUX4:0 = 0000
	ACSR = 0x80;        //Analog comparator disable to reduce power consumption
	ADCSRA = 0x86;		//ADEN=1(ADC enable)  --- ADPS2:0 = 1 1 0
}

/*
* Function Name:	_conv_adc
* Input:			Integer value of ADC channel
* Output:           Digital value of ADC channel entered as input in this function
* Logic:			Loads the Analog Value. Using ADCSRA register it starts converting analog to digital value which gets stored in ADCH register.
                    ADCH register will be the output of the function
* Example Call:		_conv_adc(9);
*
*/
unsigned char _conv_adc(unsigned char ch)
{
	unsigned char a;     // digital value which stores the digital value of ADCH register
	if(ch>7)
	{
		ADCSRB = 0x08;
	}
	ch = ch & 0x07;			  //Store only 3 LSB bits
	ADMUX= 0x20 | ch;		 //Select the ADC channel with left adjust select
	ADCSRA = ADCSRA | 0x40;	 //Set start conversion bit
	while((ADCSRA&0x10)==0); //Wait for ADC conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10;   //clear ADIF (ADC Interrupt Flag) by writing 1 to it
	ADCSRB = 0x00;
	return a;
}
/*
* Function Name:	 converttomm_41sk
* Input:			Digital value of sharp sensor values
* Output:           Distance in mm
* Example Call:		converttomm_41sk(_conv_adc(9));
*
*/
unsigned int converttomm_41sk(unsigned int a)//conversion of sharp sensor values.
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
/*
* Function Name:	_timer4_init
* Input:			NONE
* Output:           Initialization of timer/counter with prescaler :256 in fast PWM mode.
                    TOP value is set to 7080(hex) which will generate an interrupt after every 0.5 seconds.
* Logic:			Using the value of Special Function Register, the function configures the timer Registers
* Example Call:		NONE
*
*/
void _timer4_init(void)
{
	TCCR4B = 0x00; //stop
	OCR4AH = 0x00; //Output Compare Register (OCR)- Not used
	OCR4AL = 0x00; //Output compare Register (OCR)- Not used
	OCR4BH = 0x00; //Output compare Register (OCR)- Not used
	OCR4BL = 0x00; //Output compare Register (OCR)- Not used
	OCR4CH = 0x00; //Output compare Register (OCR)- Not used
	OCR4CL = 0x00; //Output compare Register (OCR)- Not used
	ICR4   = 0x7080; //Input Capture Register (ICR)  
	TCCR4C = 0x00;
	TCCR4A = 0b00000010;// Normal port operation........WGM41:40=1 0
	TCCR4B = 0b00011100; //start Timer.......WGM43:42=1 1........CS42:40=1 0 0(clkI/O/256 (From prescaler)) 
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
	
	UDR0 = a[2] + 48;   //sending the first digit
	while(!(UCSR0A & (1<<UDRE0)))   
	{
		//waiting for transmission to complete
	}
	UDR0 = a[1] + 48; //sending the second digit
	while(!(UCSR0A & (1<<UDRE0)))
	{
		//waiting for transmission to complete
	}
	UDR0 = a[0] + 48; //sending the third digit
	while(!(UCSR0A & (1<<UDRE0)))
	{
		//waiting for transmission to complete
	}
	UDR0 = 32;      //to give space between two integer
	while(!(UCSR0A & (1<<UDRE0)))
	{
		//waiting for transmission to complete
	}
}
/*
* Function Name:	getdigital
* Input:			NONE
* Output:           calls _conv_adc() and stores the digital values in different variables.
* Logic:			NONE
* Example Call:		getdigital();
*
*/

void getdigital(void)
{
	 lwl= _conv_adc(3);  	//Getting data of Left WL Sensor
	 cwl = _conv_adc(2);	//Getting data of Center WL Sensor
	 rwl = _conv_adc(1);	//Getting data of Right WL Sensor
	 sp1 = _conv_adc(9);     //Getting data of sharp sensor1.
	 sp2 = _conv_adc(10);    //Getting data of sharp sensor2.
	 sp3 = _conv_adc(11);    //Getting data of sharp sensor3.
	 sp4 = _conv_adc(12);    //Getting data of sharp sensor4.
         sp5 = _conv_adc(13);    //Getting data of sharp sensor5.
	 IR1 = _conv_adc(4);     //Getting value of IR proximity sensor1.
	 IR2 = _conv_adc(5);     //Getting value of IR proximity sensor2.
	 IR3 = _conv_adc(6);     //Getting value of IR proximity sensor3.
	 IR4 = _conv_adc(7);     //Getting value of IR proximity sensor4.
         IR5 = _conv_adc(8);     //Getting value of IR proximity sensor5.

	 
	
}

/*
* Interrupt service routine Name:  ISR(TIMER4_OVF_vect)
* Input:			    NONE
* Output:                          send the digital value of sensors & wheel velocity after every 0.5 seconds. 
* Logic:			   Timer 4 is configured as such that it gets overflow in  every 0.5 seconds so 
                                   that the sensor readings & wheel velocity are updated and sent continuously.
* Example Call:		           NONE
*/
ISR(TIMER4_OVF_vect)
{
	     cli();                 //Clears the global interrupt
		getdigital();           //stores digital value of sensors in different variables.
		//sensor_data_interpretation();
		send(count);
		send(lwl);                  //send the value of left white line sensor
		send(cwl);                  //send the value of center white line sensor
		send(rwl);                  //send the value of right white line sensor
		send(converttomm_41sk(sp1));//send the value of sharp sensor 1
		send(converttomm_41sk(sp2));//send the value of sharp sensor 2
		send(converttomm_41sk(sp3));//send the value of sharp sensor 3 
		send(converttomm_41sk(sp4));//send the value of sharp sensor 4
                send(converttomm_41sk(sp5));//send the value of sharp sensor 5
		send(IR1);                  //send the value of proximity sensor1
		send(IR2);                  //send the value of proximity sensor2
                send(IR3);                  //send the value of proximity sensor3
		send(IR4);                  //send the value of proximity sensor4
                send(IR5);                  //send the value of proximity sensor5
		count++;                      //increase the value of count
		sei();                        //enables the global interrupt      
}
/*
* Function Name:                   _uart0_init
* Input:			               NONE
* Output:                          Initialize UART0 for wireless serial communication
* Example Call:		               _uart0_init();
*/

void _uart0_init(void)
{
	UCSR0B = 0x00; //disable while setting baud rate
	UCSR0A = 0x00;
	UCSR0C = 0x06;//asynchronous usart..........character size:8-bit
	//baud rate setting;9600
	UBRR0L = 0x5F; //set baud rate lo
	UBRR0H = 0x00; //set baud rate hi
	UCSR0B = 0x98;
}


/*
* Function Name:                   _port_init
* Input:			               NONE
* Output:                          calls _adc_pinconfig(),left_encoder_pin_config (),right_encoder_pin_config ();
* Logic :                          NONE
* Example Call:		               _port_init();
*/
void _port_init()
{

	_adc_pinconfig();
}	
	
/*
	* Function Name:     _start_collection
	* Input:			 NONE
	* Output:            calls different function to initialize the state collection program.
	* Logic :            NONE
	* Example Call:		 _start_collection();
	*/
void _start_collection()
{
	cli();         //Clears the global interrupt
	_port_init();  //Initializes all the ports
	_timer4_init();
	_adc_init();
	_uart0_init(); //Initialize UART0 for wireless serial communication
	TIMSK4 = 0x01; //Timer/Counter 4 Overflow interrupt is enabled
	sei();         // Enables the global interrupt
}
#endif /*STATE_COLLECT_H */
