/*
 * B3.c
 *http://www.atmel.com/images/atmel-8151-8-bit-avr-atmega128a_datasheet.pdf
 * Created: 5-3-2016 13:18:12
 * Author : remco
 */ 

 #define F_CPU 8000000UL
 #define BIT(x)	(1 << (x))
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
 #include "Lcd.h"

 void B3_1(void);
 void B3_2(void);
 void B3_3(void);

 void wait( int ms )
 {
	 for (int tms=0; tms<ms; tms++)
	 {
		 _delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	 }
 }

void initADC1(void)
{
	//AREF = VCC, Left adjust result, channel1 at pin PF1
	ADMUX = 0b01100001;
	//ADC Enable,Start, Free running select, prescaler 64	
	ADCSRA = 0b11100110;		
}

void initADC2(void)
{
	// AREF=2,56 V, result left adjusted, channel1 at pin PF1
	ADMUX = 0b11100001;	
	// ADC-enable, no interrupt, no free running, division by 64
	ADCSRA = 0b10000110;		
}

void initADC3(void)
{
	// AREF=2,56 V, result left adjusted, channel1 at pin PF1
	ADMUX = 0b11100001;		
	// ADC-enable, no interrupt, start, free running, division by 128	
	ADCSRA = 0b11100110;		
}

int main(void)
{
	//B3_1();
    //B3_2();
	B3_3();
    while (1) 
    {
    }
}

void B3_1(void)
{
	DDRF = 0x00; //PORTF (0:7) input
	DDRA = 0xFF; //PORTA (0:7) output
	DDRB = 0xFF; //PORTB (0:7) output
	initADC1();
	//init_Lcd_4_bit_mode();
	while(1)
	{
		PORTB = ADCL;	//ADC0 ... ADC1
		PORTA = ADCH;	//ADC2 ... ADC9
		//PORTB ^= 0b00000001;
		//PORTA ^= 0b00000001;
		//display_text_int("Test: ",10);
		//display_text_int("ADCL: ",ADCL);
		//display_text_int("ADCH: ",ADCH);
		//set_cursor(0);
		wait(200);
	}
}

void B3_2(void)
{
	DDRF = 0x00; //PORTF (0:7) input
	DDRA = 0xFF; //PORTA (0:7) output	
	initADC2();						// initialize ADC

	while (1)
	{
		ADCSRA |= BIT(6);				// Start ADC
		while ( ADCSRA & BIT(6) ) ;		// Wait for completion
		PORTA = ADCH;					// Show MSB (bit 9:2) of ADC
		wait(500);						// every 50 ms (busy waiting)
	}
}

void B3_3(void)
{
	PORTF = 0x00;
	DDRA = 0xFF; //PORTA (0:7) output	
	initADC3();
	init_Lcd_4_bit_mode();
	while(1)
	{
		PORTA = ADCH;		
		display_text_int("Temp: ",ADCH);		
		set_cursor(0);
		wait(250);
	}
}

