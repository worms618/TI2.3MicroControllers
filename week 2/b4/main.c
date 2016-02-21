/*
 * B4.c
 *
 * Created: 21-2-2016 19:32:44
 * Author : remco
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

typedef struct
{
	unsigned int adres;
	unsigned int delay;
}Pattern_Data;

Pattern_Data pattern[4][9] =
{
	{
		{0b00000001,100},
		{0b00000010,100},
		{0b00000100,100},
		{0b00001000,100},
		{0b00010000,100},
		{0b00100000,100},
		{0x00,0x00}
	},
	{
		{0b00000001,100},
		{0b00100010,100},
		{0b01000000,100},
		{0b00010100,100},
		{0b00001000,100},		
		{0b00010100,100},
		{0b01000000,100},
		{0b00100010,100},		
		{0x00,0x00}
	},
	{
		{0b00000001,100},
		{0b00100010,100},
		{0b01000000,100},
		{0b00010100,100},
		{0b00001000,100},
		{0x00,0x00}
	},
	{
		 {0b00000001,75},
		 {0b00000010,75},
		 {0b01000000,75},
		 {0b00010000,75},
		 {0b00001000,75},
		 {0b00000100,75},
		 {0b01000000,75},
		 {0b00100000,75},
		 {0x00,0x00}
	}
};

void wait(int ms)
{
	for(int i = 0; i < ms; i++)
	{
		_delay_ms(1); //max 30 ms at 8MHz
	}
}

void init(void)
{
	DDRC = 0b11111111; //PORTC (7:0) output

	//init interrupt hardware
	EICRA |= 0b00000011; //set INT0 rising edge
	EIMSK |= 0b00000001; //enable INT 0

	//enable global interrupt system
	//SREG = 0x80; //Kan ook via wrapper sei();
	sei();
}

int indexPattern = 0; //hou bij met welk patroon je bezig bent
int change = 0;	//is er verandering geweest van patroon?

ISR(INT0_vect)
{
	if(indexPattern == 3)
	{
		indexPattern = 0;
	}
	else
	{
		indexPattern++;
	}	
	change = 1;
}

int main(void)
{
    init();	

    while (1) 
    {		
		int counter = 0;
		while((pattern[indexPattern][counter].delay != 0)) //als er geen delay is, is het patroon afgelopen
		{
			if (change == 1)	//is er een nieuwe patroon ingesteld? zo ja stop met de huidige en ga door met de nieuwe
			{
				change = 0;
				break;
			}
			PORTC = pattern[indexPattern][counter].adres;
			wait(pattern[indexPattern][counter].delay);			
			counter++;
		}
    }
}

