/*
 * B2C.c
 *
 * Created: 1-3-2016 12:14:04
 * Author : remco
 */ 
 #define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}

void init(void)
{
	DDRB = 0xFF; //PORTB (0:7) output
	OCR1A = 0;	//Red default off
	OCR1B = 0;  //Green default off
	OCR1C = 0;	//Blue default off
	TCCR1A = 0b10101001;		// compare output OC1A,OC1B,OC1C
	TCCR1B = 0b00001011;		// fast PWM 8 bit, prescaler=64, RUN
}

void setRed( unsigned char red )
{
	OCR1A = red;
}

void setGreen( unsigned char green)
{
	OCR1B = green;
}

void setBlue( unsigned char blue)
{
	OCR1C = blue;
}

int main(void)
{
	init();
    int red = 0,green = 0,blue = 255,delta = 5; //255 % delta == 0

	

    while (1) 
    {		
		if(red < 255 && green == 0 && blue > 0)
		{
			red += delta;
			green = 0;
			blue -= delta;
		}
		else if(red > 0 && green < 255 && blue == 0)
		{
			red -= delta;
			green += delta;
			blue = 0;
		}
		else if(red == 0 && green > 0 && blue < 255)
		{
			red = 0;
			green -= delta;
			blue += delta;
		}
		
		setRed(red);
		setGreen(green);
		setBlue(blue);
		wait(50);
    }
}

