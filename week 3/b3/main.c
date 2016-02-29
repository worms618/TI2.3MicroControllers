/*
 * B3
 * Assignment: realiseer met de AVR T/C 2 timer mode pulsen op PORTD7 van 15 ms hoog en 25 ms laag.
 * Created: 29-2-2016 10:20:19
 * Author : Jannick van Ballegooijen,
 *			Tim van Lieshout,
 *			Remco Vorthoren
 */ 

 #define Bit(x)(1<<x)
 
 #define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int ms = 0;
int onoff = 0;

void wait(int ms)
{
	for(int i = 0; i < ms; i++)
	{
		_delay_ms(1);//max 30 ms at 8 MHz
	}
}

void ToggleD7(void)
{
	PORTD ^= Bit(7);
}

void init(void)
{
	DDRD |= Bit(7);

	OCR2 = 250;
	TIMSK |= Bit(7);
	sei();
	TCCR2 = 0b00001101;
}

ISR(TIMER2_COMP_vect)
{
	ms++;
	if(onoff && ms == 15)
	{
		onoff = 0;
		ms = 0;
		ToggleD7();
	}
	else if(!onoff && ms == 25)
	{
		onoff = 1;
		ms = 0;
		ToggleD7();
	}
}

int main(void)
{
    init();
	
    while (1) 
    {
		
    }
}

