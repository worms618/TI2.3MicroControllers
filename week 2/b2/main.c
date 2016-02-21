/*
 * B2
 *
 * Created: 16-2-2016 11:54:27
 * Author : remco
 */ 

#define F_CPU 8000000UL
#define Bit(x)(1<<x)
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

int portCValue = 0; //counter on which bit you are on

void SetPortC(void)
{
	if (portCValue == 7) //max of 7 bits
	{
		portCValue = 0;
	}
	else
	{
		portCValue++;
	}	

	PORTC = Bit(portCValue);
}

/*function for INT0 interrupt request*/
ISR(INT0_vect)
{
	SetPortC();
}

/*function for INT1 interrupt request*/
ISR(INT1_vect)
{
	SetPortC();
}

void init()
{
	DDRC = 0b11111111;	//PORTD (7:0) output

	//init interrupt hardware 
	EICRA |= 0b00001110; //set INT1 rising edge and INT0 falling edge
	EIMSK |= 0b00000011; //enable INT1 & INT 2

	//enable global interrupt system
	//SREG = 0x80 //Kan ook via wrapper sei();
	sei();
}

int main(void)
{
    init();
	PORTC = Bit(portCValue);

    while (1) 
    {
    }
}

