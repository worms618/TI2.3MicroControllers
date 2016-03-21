/*
 * Project name		: Demo5_1 : UART - send / receive characters with waiting
 * Author			: Avans-TI, JW
 * Revision History	: 20110227: - initial release;
 * Description		: This program receive a character, and sends it back
 * Test configuration:
     MCU:             ATmega128
     Dev.Board:       BIGAVR6
     Oscillator:      External Clock 08.0000 MHz
     Ext. Modules:    -
     SW:              AVR-GCC
 * NOTES:
     - Turn ON switch 12, PEO/PE1 tot RX/TX
*/

# define F_CPU 8000000UL
#include <util/delay.h>
#include "uart0.h"
#include "Lcd.h"

// wait(): busy waiting for 'ms' millisecond
// Used library: util/delay.h
void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );						// library function (max 30 ms at 8MHz)
	}
}


// send/receive uart - dB-meter
int main( void )
{
	char buffer[16];						// declare string buffer 	

	init_Lcd_4_bit_mode();					// initialize LCD-display
	usart0_init();							// initialize USART0
	usart0_start();
		
	display_text("Hallo");
	set_cursor(0);
	while (1)
	{
		wait(150);							// every 50 ms (busy waiting)
		//PORTB ^= BIT(7);					// toggle bit 7 for testing

		uart0_receiveString( buffer );		// receive string from uart
		
		// write string to LCD display
		display_text(buffer);
		set_cursor(0);
	}
}
