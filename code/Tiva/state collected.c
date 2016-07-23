/*

All the sensor values are being sent back to the GUI and the state collection is achieved by this code.

Now only start_collection(); now needs to be added to collect the data.

*/



#include "state_collect.h"
#include "motor.c"
#include "sensor.c"
#include "xbee.c"
#include "buzzer.c"
#include "driverlib\lcd1.h"




int main(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	_start_collection();

	motor_pin_cofig();
	uart_pin_config();
	buzzer_pin_config();
	while(1)
	{
		//buzzer_on();
		lcd_init();
		lcd_cursor(1,2);
		lcd_string("Forward");
		stop();
		SysCtlDelay(4000000);
		fwd();
		SysCtlDelay(40000000);
		buzzer_off();
		lcd_command(0x01);
		lcd_cursor(1,2);
		lcd_string("Backward");
		stop();
		SysCtlDelay(4000000);
		bwd();
		SysCtlDelay(40000000);
		//buzzer_on();
		lcd_command(0x01);
		lcd_cursor(1,2);
		lcd_string("Left");
		stop();
		SysCtlDelay(4000000);
		left();
		SysCtlDelay(40000000);
		buzzer_off();
		lcd_command(0x01);
		lcd_cursor(1,2);
		lcd_string("Right");
		stop();
		SysCtlDelay(4000000);
		right();
		SysCtlDelay(40000000);
	}
}

