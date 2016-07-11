void buzzer_pin_config()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Enable the GPIO F ports

	 HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;
	 HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
	 HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0;

	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
}

void buzzer_on()
{
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,1);
}

void buzzer_off()
{
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,0);
}