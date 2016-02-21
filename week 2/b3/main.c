 /*
 * B3
 *
 * Created: 16-2-2016 12:33:04
 * Author : remco
 */ 

#define F_CPU 8000000UL
#define Bit(x) (1<<x)
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void wait(int ms)
{
	for(int i = 0; i < ms; i++)
	{
		_delay_ms(1); //max 30 ms at 8MHz
	}
}

/*Lookup table voor de hexadecimale waardes*/
unsigned char Numbers[16]=
{
	//DPgfedcba
	0b00111111, // 0
	0b00000110, // 1
	0b01011011, // 2
	0b01001111, // 3
	0b01100110, // 4
	0b01101101, // 5
	0b01111101, // 6
	0b00000111, // 7
	0b01111111, // 8
	0b01101111, // 9 
	0b01110111,	// A
	0b01111100, // B
	0b00111001,	// C
	0b01011110,	// D
	0b01111001, // E
	0b01110001,	// F	
};

/*functie om een ingevulde decimal te laten verschijn op een 7 segment display*/
void display(int digit)
{
	if(digit > 15 || digit < 0)
	{
		digit = 14;
	}	
	PORTC = Numbers[digit];
} 

void init(void)
{
	DDRC = 0b11111111;	//PORTD (7:0) output	

	//init interrupt hardware
	EICRA |= 0b00001111; //set INT1 rising edge and INT0 rising edge
	EIMSK |= 0b00000011; //enable INT1 & INT 0

	//enable global interrupt system
	//SREG = 0x80; //Kan ook via wrapper sei();
	sei();
}

int vect0 = 0; //bijhouden of INT0 een interrupt request heeft gedaan
int vect1 = 0; //bijhouden of INT1 een interrupt request heeft gedaan

ISR(INT0_vect)
{
	vect0 = 1;	
}

ISR(INT1_vect)
{
	vect1 = 1;	
}


/*
* A better way is to have a periodic interrupt, like every 1 ms (1 kHz rate). 
* That's a long time on most processors, so the fraction of time spent in the interrupt will be small. 
* Simply sample the button state every interrupt. 
* Declare a new button state if you have seen the new state 50 ms in a row. 
* 50 ms is longer than most buttons bounce, but is still short enough so that humans won't notice or care about the lag.
* Bron:http://electronics.stackexchange.com/questions/81787/correct-usage-of-a-pin-change-interrupt
* Dus wij hebben een delay van 200 ms gepakt, zodat je nog makkelijker de 2 knoppen "tegelijk" kan drukken.
*/
int main(void)
{
	init();

	int count = 0;

    while (1) 
    {		
		wait(200);	
		if(vect0 == 1 && vect1 == 1)
		{
			count = 0;
			vect0 = 0;
			vect1 = 0;
		}
		else if(vect0 == 1)
		{
			count++;
			vect0 = 0;
		}
		else if(vect1 == 1)
		{			
			count--;		
			vect1 = 0;
		}
		display(count);		
    }
}

