#include "driverlib/uart.h"
#include "inc/hw_uart.h"

void
UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    //
    // Loop while there are more characters to send.
    //
    while(ui32Count--)
    {
        //
        // Write the next character to the UART.
        //
        UARTCharPutNonBlocking(UART4_BASE, *pui8Buffer++);
    }
}


void uart_pin_config()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	
	GPIOPinConfigure(GPIO_PC4_U4RX);
	GPIOPinConfigure(GPIO_PC5_U4TX );

	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);

	UARTConfigSetExpClk(UART4_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	UARTEnable(UART4_BASE);
}