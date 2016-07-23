/*

Timer is added and random data is being sent back to test the code.

Task remaining is to the send the sesor readings.

*/


#include "driverlib/commonheader.h"
#include "motor.c"
#include "sensor.c"
#include "xbee.c"
#include "buzzer.c"
#include "driverlib\lcd1.h"



//*****************************************************************************
//interrupt handler or ISR for the Timer.
void Timer0IntHandler(void)
{
	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	getdata();
	UARTSend((uint8_t *)"Forward ",8);
	while(UARTBusy(UART4_BASE));
	UARTSend((uint8_t *)"Right ",6);
	while(UARTBusy(UART4_BASE));
	UARTSend((uint8_t *)"Back ",5);
	while(UARTBusy(UART4_BASE));
	UARTSend((uint8_t *)"Left ",5);
	while(UARTBusy(UART4_BASE));


}



int main(void)
{
	uint32_t ui32Period;
	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//enable GPIO Peripherals
	//configure GPIO pin1,pin2,pin3 as output
	//GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);     //Enable Timer 0 Clock
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);  // Configured Timer Operation as Periodic
	ui32Period = (SysCtlClockGet() / 1)/ 2;           //To toggle  GPIO at 1Hz and a 50% duty cycle
	//subtract one from the timer period since the interrupt fires at the zero count.
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period );
	IntEnable(INT_TIMER0A);                          //enables the specific vector associated with Timer0A
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);// enables a specific event within the timer to generate an interrupt.
	IntMasterEnable();                             //master interrupt enable
	TimerEnable(TIMER0_BASE, TIMER_A);//start timer0A


	motor_pin_cofig();
	sensor_pin_config();
	uart_pin_config();
	buzzer_pin_config();
	while(1)
	{
		//buzzer_on();
		lcd_init();
		lcd_cursor(1,2);
		lcd_string("Forward");
		stop();
		SysCtlDelay(4000000);
		fwd();
		SysCtlDelay(40000000);
		buzzer_off();
		lcd_command(0x01);
		lcd_cursor(1,2);
		lcd_string("Backward");
		stop();
		SysCtlDelay(4000000);
		bwd();
		SysCtlDelay(40000000);
		//buzzer_on();
		lcd_command(0x01);
		lcd_cursor(1,2);
		lcd_string("Left");
		stop();
		SysCtlDelay(4000000);
		left();
		SysCtlDelay(40000000);
		buzzer_off();
		lcd_command(0x01);
		lcd_cursor(1,2);
		lcd_string("Right");
		stop();
		SysCtlDelay(4000000);
		right();
		SysCtlDelay(40000000);
	}
}

