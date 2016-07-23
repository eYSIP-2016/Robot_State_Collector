/*

This code was used to test the X-BEE module on TIVA board. The motors were also used in conjuction with the X-BEE.

The X-BEE just sends "fast" or "slow" according to the speed of the motors.


PORTS used are : PORT C4 for RX
		 PORT C5 for TX

PORTS used for motor are : PORT E4 and E5 for EN. 
		 	   PORT A6 and A7 for right wheel
		 	   PORT A2 and A3 for left Wheel
		
*/




#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "inc/hw_uart.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/uart.h"
#include "driverlib/commonheader.h"

int temp=0;


void
UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    //
    // Loop while there are more characters to send.
    //
    while(ui32Count--)
    {
        //
        // Write the next character to the UART.
        //
        UARTCharPutNonBlocking(UART4_BASE, *pui8Buffer++);
    }
}

int main(void)
{

	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // Enable the GPIO A ports
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); // Enable the GPIO E ports
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);

	GPIOPinConfigure(GPIO_PE4_M0PWM4);
	GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_4);

	GPIOPinConfigure(GPIO_PC4_U4RX);
	GPIOPinConfigure(GPIO_PC5_U4TX );

	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);

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





	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7); 

	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7,68); 


	UARTConfigSetExpClk(UART4_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	UARTEnable(UART4_BASE);

	UARTCharPut(UART4_BASE,'a');
	while(1)
	{
		SysCtlDelay(4000000*10);
		temp = 1;
		UARTSend((uint8_t *)"Fast",4);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_2) / 1.25);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_2) / 1.25);
		SysCtlDelay(4000000*10);
		temp = 0;
		UARTSend((uint8_t *)"Slow",4);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_2) / 3);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_2) / 3);

	}
}
