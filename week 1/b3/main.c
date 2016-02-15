/*
 * B3
 * Assignment: Let Led PORTD.7 flash, when Button PORTC.0 is pressed.
 * Created: 2-2-2016 16:47:15
 * Author : Jannick van Ballegooijen, 
 *			Tim van Lieshout, 
 *			Remco Vorthoren
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>


void wait(int ms)
{
	for(int i = 0; i < ms; i++)
	{
		_delay_ms(1);//max 30 ms at 8 MHz
	}
}

int main(void)
{
    DDRD = 0b10000000; //PORTD.7 is set to ouput
	int onoff = 0;	//1 = led is on, 0 = led is off

	while(1)
	{
		if(PINC & 0b10000001) //check of PINC.0 is high.
		{
			if(onoff == 0)
			{
				PORTD = 0b10000000; //set PORTD.7 high
				onoff = 1;
			}
			else
			{
				PORTD = 0b00000000; //set all PORTD low
				onoff = 0;
			}
			wait(100);
		}
		else
		{
			PORTD = 0b00000000; //set all PORTD low
		}
	}

	    
    return 1;
}

