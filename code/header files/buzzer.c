/*

* Function Name: buzzer_pin_config()

* Input: none

* Output: none

* Description: Configurs pins for buzzer and enables the GPIO F port to which the buzzer is connected.

* Example Call: buzzer_pin_config();

*/

void buzzer_pin_config()    
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Enable the GPIO F ports 

         //The port F pins are locked by default and the next 3 code lines are used to unlock those pins for our use
	
	 HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;
	 HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
	 HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0;

	// Set the pin with buzzer is connected to as an output pin

	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
}


/*

* Function Name: buzzer_on()

* Input: none

* Output: none

* Description: Turns on the buzzer.
* Example Call: buzzer_on();

*/

void buzzer_on()
{
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,1); // Write 1 to the pin to which the buzzer is connected.
}


/*

* Function Name: buzzer_off()

* Input: none

* Output: none

* Description: Turns off the buzzer.
* Example Call: buzzer_off();

*/

void buzzer_off()
{
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,0); // Write 0 to the pin to which the buzzer is connected.
}