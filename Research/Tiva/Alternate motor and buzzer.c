/*

This code was used to test the buzzer and motor simultaneously . The buzzer was connected to the PORT A5 on the TIVA board and the motors were connected to the ports 
mentioned in the "PWM Motor" file. 

Alternatively the buzzer and the motor were made to work.

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
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

	GPIOPinConfigure(GPIO_PB6_M0PWM0);
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);

	GPIOPinConfigure(GPIO_PB7_M0PWM1);
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);

	PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 6400000);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0) / 1.25);
	PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
	PWMGenEnable(PWM0_BASE, PWM_GEN_0);

	PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, 6400000);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_1) / 1.25);
	PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);
	PWMGenEnable(PWM0_BASE, PWM_GEN_1);


	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6|GPIO_PIN_7); // Set pin 7 as the output port
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1|GPIO_PIN_2);
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5);


	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_6|GPIO_PIN_7,64); // Give '1' to pin 7
	GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_1|GPIO_PIN_2,4);
	while(1)
	{
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_6|GPIO_PIN_7,64); // Give '1' to pin 7
		GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_1|GPIO_PIN_2,4);
		SysCtlDelay(4000000*10);
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_6|GPIO_PIN_7,0); // Give '1' to pin 7
	    GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_1|GPIO_PIN_2,0);
	    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_5,32);
	    SysCtlDelay(400000);
	    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_5,0);
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_6|GPIO_PIN_7,128); // Give '1' to pin 7
		GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_1|GPIO_PIN_2,2);
		SysCtlDelay(4000000*10);
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_6|GPIO_PIN_7,0); // Give '1' to pin 7
		GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_1|GPIO_PIN_2,0);
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_5,32);
		SysCtlDelay(400000);
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_5,0);

	}
}
