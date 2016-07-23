/*

This code was used to test all the components together. 
All the port info. is contained inside the header files even a separate file named pin_config will be provided.


*/

#include "driverlib/commonheader.h"
#include "motor.c"
#include "sensor.c"
#include "xbee.c"
#include "buzzer.c"
#include "driverlib\lcd1.h"

int main(void)
{
	motor_pin_cofig();
	sensor_pin_config();
	uart_pin_config();
	buzzer_pin_config();
	while(1)
	{
		buzzer_on();
		lcd_init();
		lcd_cursor(1,2);
		lcd_string("Forward");
		stop();
		fwd();
		getdata();
		UARTSend((uint8_t *)"Forward ",8);
		SysCtlDelay(4000000);
		buzzer_off();
		lcd_init();
		lcd_cursor(1,2);
		lcd_string("Backward");
		stop();
		bwd();
		getdata();
		UARTSend((uint8_t *)"Back ",5);
		SysCtlDelay(4000000);
		buzzer_on();
		lcd_init();
		lcd_cursor(1,2);
		lcd_string("Left");
		stop();
		left();
		getdata();
		UARTSend((uint8_t *)"Left ",5);
		SysCtlDelay(4000000);
		buzzer_off();
		lcd_init();
		lcd_cursor(1,2);
		lcd_string("Right");
		stop();
		right();
		getdata();
		UARTSend((uint8_t *)"Right ",6);
		SysCtlDelay(4000000);
	}
}
