/*
 * B4
 * Assignment: A light loop with the LED's of PORTD, 
 * Created: 8-2-2016 11:45:25
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
		_delay_ms(1);//max 30 ms by 8 MHz
	}
}

int main(void)
{
    DDRD = 0b11111111; //all pins PORTD set to output	

	long pinValue = 0b10000000; //set PORTD.7 to high
	int lefright = 0; //0 = direction to the right, 1 = direction to the left
	
	while(1)
	{
		PORTD = pinValue;
		wait(50);
				
		if (lefright == 0)
		{
			pinValue = pinValue >> 1;
		}
		else
		{
			pinValue = pinValue << 1;
		}

		if(pinValue == 0b10000001)		//if led 7 is high
		{
			lefright = 1;
		}
		else if(pinValue == 0b00000001)	//if led 0 is high
		{
			lefright = 0;
		}					
	}
}

