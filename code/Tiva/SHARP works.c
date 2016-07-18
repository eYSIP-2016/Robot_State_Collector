/*

This code was used to test the SHARP sensor. The buzzer was connected to the PORT E3 on the TIVA board. This was the first time we were using a pin as an ADC.

Simple code which was used to check the values incoming from the the sensor using the debugger of CCS. 

NOTE: As TIVA board is a 12 bit micro-controller and ATMEGA 2560 is an 8 bit micro-controller the formula used for converting the digital values to distance in mm 
      would not work in both of the above mentioned micro-controllers.

*/




#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_adc.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"

int Temp;

int main(void)
{
	uint32_t ui32ADC0Value[1];
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE,3,0,ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 3);
	while(1)
	{
		ADCIntClear(ADC0_BASE, 3);
		ADCProcessorTrigger(ADC0_BASE, 3);
		while(!ADCIntStatus(ADC0_BASE, 3, false))
		{
		}
		ADCSequenceDataGet(ADC0_BASE, 3, ui32ADC0Value);
		Temp = ui32ADC0Value[0] ;

	}
}
