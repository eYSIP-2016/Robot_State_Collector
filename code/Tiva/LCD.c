/*
 * main.c
 *
 *  Created on: 06-Mar-2016
 *  Author: Saurav Shandilya, ERTS Lab, CSE Department IIT Bombay
 *  Description - Sample Program to interface LCD in 8-bit mode with Tiva Board.
 *  Output: Display 1st Line: ERTS Lab and counter in 2nd Line
 *  Dependencies: lcd.h and commonheader.h file. lcd.c file should be included in project folder.
 */

#include "driverlib\commonheader.h"
#include "driverlib\lcd1.h"

uint32_t i = 0;

void main(void)
{

	lcd_init();		// initialize LCD. Set required pin for interfacing LED. Configure LCD in 8-bit mode. Refer to lcd.c file for details

	lcd_cursor(1,2);
	lcd_string("ERTS Lab");

	for( i = 0; i<255; i++)
	{
		lcd_cursor(2,1);
		lcd_print(i,3);
		SysCtlDelay(6700000);		// creates ~500ms delay - TivaWare fxn
		//lcd_command(0x80);			// clear display
	}

	while(1);
}
