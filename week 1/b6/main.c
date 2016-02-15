/*
 * CodingB6
 * assignment:
 * Created: 8-2-2016 19:03:48
 * Author : Jannick van Ballegooijen, 
 *			Tim van Lieshout, 
 *			Remco Vorthoren
 */ 
  
#define F_CPU 8000000UL
#define Frequency(x) (1000/x); //xHz, x times per second
#define Bit(x) (1 << x); //bit position
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

void wait(int ms)
{
	for(int i = 0; i < ms; i++)
	{
		_delay_ms(1);//max 30 ms by 8 MHz
	}
}

int main(void)
{
	DDRD = Bit(7);					//set bit seven to output
	bool pushed = false;			//is PORTC.0 been pushed
	int freq1 = Frequency(1);		//frequency 1Hz definition
	int freq4 = Frequency(4);		//frequency 4Hz definition
	int cFreq= freq1;				//current used frequency
	unsigned long Bit7 = Bit(7);	//bit number 7 definition
	unsigned long Bit0 = Bit(0);	//bit number 0 definition
	unsigned long Clear = 0x00;		//clear bit code


    while (1) 
    {
		if(PINC & Bit0)
		{
			if (!pushed)
			{
				pushed = true;
				if(cFreq == freq1)
				{
					cFreq = freq4;
				}
				else if(cFreq == freq4)
				{
					cFreq = freq1;
				}
			}								
		}
		else if(pushed)
		{
			pushed = false;
		}

		PORTD = Bit7;
		wait(cFreq);
		PORTD = Clear;
		wait(cFreq);
    }
	return 1;
}

