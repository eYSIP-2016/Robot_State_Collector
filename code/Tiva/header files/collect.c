
//interrupt handler or ISR for the Timer.

void Timer0IntHandler(void)
{
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);   // Clear the timer interrupt
	getdata();		// Updates the values of all the sensors on the bot.
	send(WL);  		// Sends the reading of LEFT White line sensor. 
	send(WC);		// Sends the reading of CENTER White line sensor.
	send(WR);		// Sends the reading of RIGHT White line sensor.
	send(SHARP);		// Sends the reading of SHARP sensor.
}




/*
* Function Name: start_collection()
* Input: none
* Output: none
* Description: Starts the timer whose interrupt is used for sending the data through UART.
* Example Call: start_collection();
*/

void start_collection()
{
		
		SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);                   // Enable Timer 0 Clock
		TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);       		// Configured Timer Operation as Periodic
		TimerLoadSet(TIMER0_BASE, TIMER_A, (SysCtlClockGet() / 1)/ 2);  // Sets the Period of the timer.
		IntEnable(INT_TIMER0A);                          		// Enables the specific vector associated with Timer0A
		TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);		// Enables a specific event within the timer to generate an interrupt.
		IntMasterEnable();                      		        // Master interrupt enable
		TimerEnable(TIMER0_BASE, TIMER_A);				// Enable the TIMER_A of TIMER0 module.
		sensor_pin_config();
}

