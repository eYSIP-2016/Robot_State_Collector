#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"

int main(void)
{

	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // Enable the GPIO A ports


	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_7); // Set pin 7 as the output port

	while(1)
	{
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_7,0);
		SysCtlDelay(4000000);
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_7,128); // Give '1' to pin 7
		SysCtlDelay(4000000);
	}
}
