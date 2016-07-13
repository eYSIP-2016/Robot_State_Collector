
//interrupt handler or ISR for the Timer.
void Timer0IntHandler(void)
{
	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	getdata();
	send(WL);
	send(WC);
	send(WR);
	send(SHARP);
}




void start_collection()
{
		
		SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);     //Enable Timer 0 Clock
		TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);  // Configured Timer Operation as Periodic
		//subtract one from the timer period since the interrupt fires at the zero count.
		TimerLoadSet(TIMER0_BASE, TIMER_A, (SysCtlClockGet() / 1)/ 2);
		IntEnable(INT_TIMER0A);                          //enables the specific vector associated with Timer0A
		TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);// enables a specific event within the timer to generate an interrupt.
		IntMasterEnable();                             //master interrupt enable
		TimerEnable(TIMER0_BASE, TIMER_A);//start timer0A

}

