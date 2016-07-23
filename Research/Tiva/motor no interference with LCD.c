/*

The Ports used in LCD were same as those used by us for motor so we had to reshuffle the pins we were using.


PORTS used for LCD are : PORT B 0-7 for Data as LCD is configured in 8 bit mode. 
		 	 PORT A4 for RS
		 	 PORT A5 for RW
		 	 PORT C6 for EN

PORTS used for motor are : PORT E4 and E5 for EN. 
		 	   PORT A6 and A7 for right wheel
		 	   PORT A2 and A3 for left Wheel

*/




#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"



int main(void)
{

	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // Enable the GPIO A ports
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); // Enable the GPIO E ports
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

	GPIOPinConfigure(GPIO_PE4_M0PWM4);
	GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_4);

	GPIOPinConfigure(GPIO_PE5_M0PWM5);
	GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_5);

	PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, 6400000);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_2) / 1.25);
	PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);
	PWMGenEnable(PWM0_BASE, PWM_GEN_2);

	PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, 6400000);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_2) / 1.25);
	PWMOutputState(PWM0_BASE, PWM_OUT_5_BIT, true);
	PWMGenEnable(PWM0_BASE, PWM_GEN_2);


		
		
		
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7); // Set pin 7 as the output port

	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7,68); // Give '1' to pin 7
	while(1)
	{
		SysCtlDelay(4000000*10);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_2) / 1.25);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_2) / 1.25);
		SysCtlDelay(4000000*10);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_2) / 3);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_2) / 3);

	}
}
