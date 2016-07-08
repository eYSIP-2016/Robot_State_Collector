/*
 * author-amit raushan,amanpreet singh
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"//Definitions for the interrupt and register assignments
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"//includes API functions such as IntEnable and IntPrioritySet.
#include "driverlib/gpio.h"
#include "driverlib/timer.h"//includes API functions such as TimerConfigure and TimerLoadSet.
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"

volatile  uint32_t x=1;

//*****************************************************************************
//interrupt handler or ISR for the Timer.
void Timer0IntHandler(void)
{
// Clear the timer interrupt
TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
// Read the current state of the GPIO pin and
// write back the opposite state
x++;
if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3))
{
GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
}
else
{
GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 8);
}
}


int main(void) {
	
	uint32_t ui32Period;
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);//Enable Timer 0 Clock

	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);  // Configured Timer Operation as Periodic

	ui32Period = (SysCtlClockGet() / 1)/ 2;           //calculation of the number of clock cycles required for a 1Hz
	//to subtract one from the timer period since the interrupt fires at the zero count.
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);

	IntEnable(INT_TIMER0A);                          //enable timer 0A interrupt
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);// Timer 0A Interrupt when Timeout
	IntMasterEnable();                             //master interrupt enable


	TimerEnable(TIMER0_BASE, TIMER_A);//start timer0A

	while(1)
	{

	}
}

