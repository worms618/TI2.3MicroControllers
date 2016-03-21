/*
 * uart0.c
 *
 * Created: 16-3-2016 09:06:18
 *  Author: remco
 */ 

 #include "uart0.h"

 // initialize USART0 - receive/transmit
 void usart0_init( void ) 
 {
	int	ubrr = MYUBRR;
	UBRR0H = ubrr>>8;						// baudrate register, hoge byte
	UBRR0L = ubrr;							// baudrate register, lage byte
	UCSR0C = 0b00000110;					// asynchroon, 8 data - 1 stop - no parity
	UCSR0B = 0b00000000;					// receiver & transmitter enable
 } 

 // UART0 receiver & transmitter enable
 void usart0_start( void ) 
 {
	UCSR0B |= BIT(RXEN)|BIT(TXEN);			// set bit RXEN = Receiver enable and TXEN = Transmitter enable
 } 

 // UART0: receive ch
 char uart0_receiveChar()
 {
	while (!(UCSR0A & BIT(RXC0))) ;			// if RX0 is set: unread data present in buffer
	return UDR0;							// read ch
 } 

 // UART0: send ch
 int uart0_sendChar( char ch )
 {
	while (!(UCSR0A & BIT(UDRE0))) ;		// wait until UDRE0 is set: transmit buffer is ready
	UDR0 = ch;								// send ch
	return 0;								// OK
 }