/*
 * CodingB5
 * Assignment: create a light pattern with a data structure
 * Created: 8-2-2016 17:44:25
 *Author : Jannick van Ballegooijen, 
 *			Tim van Lieshout, 
 *			Remco Vorthoren
 */ 

#define F_CPU 8000000UL
#define Bit(x) (1 << x); //bit position

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

typedef struct
{	unsigned int adres;
	unsigned int delay;
}Pattern_Data;

Pattern_Data pattern[]=
{	
	{0b00000001,500},{0b00000011,500},{0b00000111,500},{0b00001111,500},{0b00011111,500},{0b00111111,500},{0b01111111,500},{0b11111111,500},
	{0b11111111,200},{0b11111110,500},{0b11111100,500},{0b11111000,500},{0b11110000,500},{0b11100000,500},{0b11000000,500},{0b10000000,500},
	{0b10000000,200},{0b11000000,500},{0b11100000,500},{0b11110000,500},{0b11111000,500},{0b11111100,500},{0b11111110,500},{0b11111111,500},
	{0b11111111,200},{0b01111111,500},{0b00111111,500},{0b00011111,500},{0b00001111,500},{0b00000111,500},{0b00000011,500},{0b00000001,500},
	{0x00,1000},
	{0b10000001,500},{0b01000010,500},{0b00100100,500},{0b00011000,500},
	{0x00,1000},
	{0b10000001,500},{0b11000011,500},{0b11100111,500},{0b11111111,500},	
	{0x00,0x00}
};

void wait(int ms)
{
	for(int i = 0; i < ms; i++)
	{
		_delay_ms(1);//max 30 ms by 8 MHz
	}
}

int main(void)
{   
	DDRD = 0b11111111;	//set all PORTD to output
	

    while (1) 
    {		
		int index = 0;
		while((pattern[index].delay != 0))
		{
			
			PORTD = pattern[index].adres;
			wait(pattern[index].delay);
			index++;
		}		
	}

	return 1;
}


