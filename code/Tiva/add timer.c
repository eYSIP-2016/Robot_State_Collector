#include "driverlib/commonheader.h"
#include "motor.c"
#include "sensor.c"
#include "xbee.c"
#include "buzzer.c"
#include "driverlib\lcd1.h"

int main(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	motor_pin_cofig();
	sensor_pin_config();
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
		getdata();
		UARTSend((uint8_t *)"Forward ",8);
		SysCtlDelay(40000000);
		buzzer_off();
		lcd_command(0x01);
		lcd_cursor(1,2);
		lcd_string("Backward");
		stop();
		SysCtlDelay(4000000);
		bwd();
		getdata();
		UARTSend((uint8_t *)"Back ",5);
		SysCtlDelay(40000000);
		//buzzer_on();
		lcd_command(0x01);
		lcd_cursor(1,2);
		lcd_string("Left");
		stop();
		SysCtlDelay(4000000);
		left();
		getdata();
		UARTSend((uint8_t *)"Left ",5);
		SysCtlDelay(40000000);
		buzzer_off();
		lcd_command(0x01);
		lcd_cursor(1,2);
		lcd_string("Right");
		stop();
		SysCtlDelay(4000000);
		right();
		getdata();
		UARTSend((uint8_t *)"Right ",6);
		SysCtlDelay(40000000);
	}
}

