int WC;
int WR;
int WL;
int SHARP;
uint32_t ui32ADC0Value[1];

void getdata()
{
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



void sensor_pin_config()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1);
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

}