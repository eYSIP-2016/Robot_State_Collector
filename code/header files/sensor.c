int WC;					// Global variable for storing data for the Center White Line sensor.
int WR;					// Global variable for storing data for the Right White Line sensor.
int WL;					// Global variable for storing data for the Left White Line sensor.
int SHARP;				// Global variable for storing data for the Sharp sensor.
uint32_t ui32ADC0Value[1];		// Global variable used as a buffer used in A_to_D conversion.



/*
* Function Name: getdata()
* Input: none
* Output: none
* Description: Configurs pins for UART and enables the GPIO C port to which the X-BEE is connected.
* Example Call: getdata();
*/

void getdata()
{
	

	// Set of code to get the value for Left White Line sensor.
	
	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);				
	ADCSequenceStepConfigure(ADC0_BASE,3,0,ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 3);
	ADCIntClear(ADC0_BASE, 3);
	ADCProcessorTrigger(ADC0_BASE, 3);
	while(!ADCIntStatus(ADC0_BASE, 3, false))
	{
	}
	ADCSequenceDataGet(ADC0_BASE, 3, ui32ADC0Value);
	WL = ui32ADC0Value[0] ;


	// Set of code to get the value for Center White Line sensor.

	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE,3,0,ADC_CTL_CH1|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 3);
	ADCIntClear(ADC0_BASE, 3);
	ADCProcessorTrigger(ADC0_BASE, 3);
	while(!ADCIntStatus(ADC0_BASE, 3, false))
	{
	}
	ADCSequenceDataGet(ADC0_BASE, 3, ui32ADC0Value);
	WC = ui32ADC0Value[0] ;


	// Set of code to get the value for Right White Line sensor.


	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE,3,0,ADC_CTL_CH2|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 3);
	ADCIntClear(ADC0_BASE, 3);
	ADCProcessorTrigger(ADC0_BASE, 3);
	while(!ADCIntStatus(ADC0_BASE, 3, false))
	{
	}
	ADCSequenceDataGet(ADC0_BASE, 3, ui32ADC0Value);
	WR = ui32ADC0Value[0] ;


	// Set of code to get the value for Sharp sensor.

	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE,3,0,ADC_CTL_CH3|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 3);
	ADCIntClear(ADC0_BASE, 3);
	ADCProcessorTrigger(ADC0_BASE, 3);
	while(!ADCIntStatus(ADC0_BASE, 3, false))
	{
	}
	ADCSequenceDataGet(ADC0_BASE, 3, ui32ADC0Value);
	SHARP = ui32ADC0Value[0] ;


}



/*
* Function Name: sensor_pin_config()
* Input: none
* Output: none
* Description: Configurs pins for SHARP and 3 White line sensors and enables the GPIO E port to which the sensors are connected.
* Example Call: sensor_pin_config();
*/

void sensor_pin_config()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);		// Enable the GPIO E ports 
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);		// Sets the usage of PORT_3 of GPIO_E as ADC type.
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);		// Sets the usage of PORT_2 of GPIO_E as ADC type.
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1);		// Sets the usage of PORT_1 of GPIO_E as ADC type.
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0);		// Sets the usage of PORT_0 of GPIO_E as ADC type.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);		// Enable the ADC 0 ports 

}