/*
 * B1_D.c
 *
 * Created: 16-3-2016 09:04:37
 * Author : remco
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <ctype.h>
#include "uart0.h"

char character;

// wait(): busy waiting for 'ms' millisecond
// Used library: util/delay.h
void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );						// library function (max 30 ms at 8MHz)
	}
}

int main(void)
{
    DDRB = 0xFF;							// set PORTB for output, for testing
    DDRA = 0xFF;							// set PORTA for output, for testing
    usart0_init();							// initialize USART0
    usart0_start();							// uart0: start send & receive
    
    while (1)
    {
	    wait(50);							// every 50 ms (busy waiting)
	    PORTB ^= BIT(7);					// toggle bit 7 for testing

	    character = uart0_receiveChar();	// read char
	    PORTA = character;					// show read character, for testing

	    uart0_sendChar(toupper(character));			// send back
    }
}

