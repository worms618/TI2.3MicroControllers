/*
 * CodingB2
 * Assignment: alternately turn on Led PORTD.7 and Led PORTD.6, every 500 ms.
 * Created: 2-2-2016 12:34:43
 * Author : Jannick van Ballegooijen, 
			Tim van Lieshout, 
			Remco Vorthoren
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>


void wait(int ms)
{
	for(int i = 0; i < ms; i++)
	{
		_delay_ms(1);// max 30 ms by 8 MHz
	}
}
int main(void)
{
    DDRD = 0b11111111; //all pins PORTD are set to output
	
    while (1) 
    {
		PORTD = 0b10000000; // set PortD.7 high
		wait(500);			// wait 500 ms
		PORTD = 0b01000000; // set PortD.7 high 
		wait(500);			// wait 500 ms
    }
	return 1;
}

