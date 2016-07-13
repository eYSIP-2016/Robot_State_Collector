
/*
* Function Name: motor_pin_config()
* Input: none
* Output: none
* Description: Configurs pins for motor and enables the GPIO A  and GPIO E port to which the motor driver IC is connected and the PWM module.
* Example Call: motor_pin_config();
*/

void motor_pin_cofig()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); // Enable the GPIO E ports
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // Enable the GPIO A ports
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);  // Enable the PWM0 module


	GPIOPinConfigure(GPIO_PE4_M0PWM4);    			// Sets the mode of GPIO_E PORT_4 as PWM output
	GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_4);		// Sets the usage of PORT_4 of GPIO_E as PWM type.

	GPIOPinConfigure(GPIO_PE5_M0PWM5);			// Sets the mode of GPIO_E PORT_5 as PWM output
	GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_5);		// Sets the usage of PORT_5 of GPIO_E as PWM type.

	PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);	// Configures the PWM generator in DOWN mode and without aany sync.  note PWM_GEN_2 is used as PORT_4 of GPIO_E is connected to the 3rd generator of PWM 0 module.  
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, 6400000);						// Set the period for the PWM.
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_2) / 1.25);   // Set the width of the PWM.
	PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);						// Enables PWM output at PORT_4.
	PWMGenEnable(PWM0_BASE, PWM_GEN_2);							// Starts the genertaion of PWM.

	PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);	// Configures the PWM generator in DOWN mode and without aany sync.  note PWM_GEN_2 is used as PORT_5 of GPIO_E is connected to the 3rd generator of PWM 0 module.
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, 6400000);						// Set the period for the PWM.
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_2) / 1.25);	// Set the width of the PWM.
	PWMOutputState(PWM0_BASE, PWM_OUT_5_BIT, true);						// Enables PWM output at PORT_5.
	PWMGenEnable(PWM0_BASE, PWM_GEN_2);							// Starts the genertaion of PWM.

	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7);	// Set the pin with buzzer is connected to as an output pin

}


/*
* Function Name: stop()
* Input: none
* Output: none
* Description: Stops the bot.
* Example Call: stop();
*/

void stop()
{
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7,0);   // Write 0 to all pins

}



	
/*
* Function Name: fwd()
* Input: none
* Output: none
* Description: Makes the bot go forward.
* Example Call: fwd();
*/

void fwd()
{
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7,68);	// Write 1 to GPIO_PIN_6 and GPIO_PIN_2.

}



	
/*
* Function Name: bwd()
* Input: none
* Output: none
* Description: Makes the bot go backward.
* Example Call: bwd();
*/

void bwd()
{
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7,136); // Write 1 to GPIO_PIN_7 and GPIO_PIN_3.

}	



/*
* Function Name: right()
* Input: none
* Output: none
* Description: Makes the bot turn right.
* Example Call: right();
*/

void right()
{
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7,72); // Write 1 to GPIO_PIN_6 and GPIO_PIN_3.

}




/*
* Function Name: left()
* Input: none
* Output: none
* Description: Makes the bot turn left.
* Example Call: left();
*/	

void left()
{
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7,132); // Write 1 to GPIO_PIN_7 and GPIO_PIN_2.

}		