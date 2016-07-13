#include "driverlib/uart.h"    // include the library that includes the Basic UART functions.
#include "inc/hw_uart.h"      // include the library that includes the Basic UART functions.



char a[4];  // Global variable used as a buffer to send the values via UART
int i;      // Global variable used for a loop for sending data via UART

/*
* Function Name: send ( int n)
* Input: Integer values of sensors
* Output: none
* Description: Digitises the values and sends them through UART.
* Example Call: send(WL);
*/


void send ( int n)
{
	int z = 0;    
	int c = n;

	// Ensuring that the leading digits are not a random no. i.e. if only 3 digits are to be transmitted then the 'first' digit that will be sent would be a garbage value to prevent that we set them all to 0 initially			
	a[0] = 0;
	a[1] = 0;
	a[2] = 0;
	a[3] = 0;
	
	// Code to digitise the integer
	while (c)
	{ // loop till there's nothing left
		a[z++] = (char)(c % 10); // assign the last digit and convert it to char. in this way the digit does not become '1'. 
		c /= 10; // "right shift" the number
	}

	
	for (i =3; i>=0;i--)
	{
		UARTCharPut(UART4_BASE,a[i]+48); // Sends the data through UART after converting the digit to '1'. 
		while(UARTBusy(UART4_BASE)); // Wait for the transmission to complete before sending new values.
	}
	UARTCharPut(UART4_BASE,' '); // Send a blank space '' in order to spearate the values so they are clear to the user.
}



/*
* Function Name: uart_pin_config()
* Input: none
* Output: none
* Description: Configurs pins for UART and enables the GPIO C port to which the X-BEE is connected.
* Example Call: uart_pin_config();
*/


void uart_pin_config()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);  // Enable the UART 4 ports 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);  // Enable the GPIO C ports 
	
	GPIOPinConfigure(GPIO_PC4_U4RX);     // Sets the mode of GPIO_C PORT_4 as the receiver of UART 4.
	GPIOPinConfigure(GPIO_PC5_U4TX );    // Sets the mode of GPIO_C PORT_5 as the transmitter of UART 4.

	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);  // Sets the usage of PORT_4 and PORT_5 of GPIO_C as UART type.

	

	// Sets the configurations of the UART 4 -- clock to system clock , baud rate to 115200 , data bits to 8 , stop bits to 0 and parity bits to NONE.

	UARTConfigSetExpClk(UART4_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));  

	UARTEnable(UART4_BASE); // Enables the UART for use.
}
