/*
 * CodingB7a
 * Assignment: our version of the finite state machine
 * Created: 15-2-2016 11:42:27
 * Author : Jannick van Ballegooijen,
 *			Tim van Lieshout,
 *			Remco Vorthoren
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

void wait(int ms)
{
	for(int i = 0; i < ms; i++)
	{
		_delay_ms(1);//max 30 ms at 8 MHz
	}
}

//prototypes
void s1(void);
void s2(void);
void s3(void);
void start(void);
void end(void);

typedef enum{D7,D6,D5,NoEvent}BEvent;	//button events, indicates which button you have pressed, or not even 1
typedef enum{START_state,STATE_1,STATE_2,STATE_3,END_State}BOARD_STATE;	//state of the board

//shows in which states the board is
void s1(void)
{
	PORTD = 0b00000010;
}

void s2(void)
{
	PORTD = 0b00000100;
}

void s3(void)
{
	PORTD = 0b00001000;
}

void start(void)
{
	PORTD = 0b00000001;
}

void end(void)
{
	PORTD = 0b00010000;
}

BEvent bevent = NoEvent;		//last given event
BOARD_STATE state = START_state;//current state
int newEvent = 0;	//shows there has been a new trigger: 1 = true, 0 = false


//sets the last button event
BEvent getButton()
{
	if((PIND & 0b10000000))
	{
		if(bevent != D7)
		{
			newEvent = 1;		//new event had triggerd
			return D7;
		}		
		else
		{
			newEvent = 0;
		}
	}
	else if((PIND & 0b01000000))
	{
		if(bevent != D6)
		{
			newEvent = 1;
			return D6;
		}	
		else
		{
			newEvent = 0;
		}	
	}
	else if((PIND & 0b00100000))
	{
		if(bevent != D5)
		{
			newEvent = 1;
			return D5;
		}	
		else
		{
			newEvent = 0;
		}	
	}
	else
	{
		newEvent = 0;
		return NoEvent;
	}	
}

void handleState()
{
	if(newEvent == 0)		//if there was no new event, the state does not have to update
	{
		return;
	}

	switch(state)
	{
	case START_state:
		switch(bevent)
		{
			case D5:
			state = STATE_2;
			s2();
			break;
			case D6:
			state = STATE_1;
			s1();
			break;
			case D7:
			break;
			case NoEvent:
			break;
		}
	break;

	case STATE_1:
	switch(bevent)
	{
		case D5:
		state = STATE_2;
		s2();
		break;
		case D6:
		break;
		case D7:
		state = START_state;
		start();
		break;
		case NoEvent:
		break;
	}
	break;

	case STATE_2:
		switch(bevent)
		{
			case D5:
			state = STATE_3;
			s3();
			break;
			case D6:
			state = STATE_1;
			s1();
			break;
			case D7:
			state = START_state;
			start();
			break;
			case NoEvent:
			break;
		}
	break;

	case STATE_3:
		switch(bevent)
		{
			case D5:			
			case D6:
			state = END_State;
			end();
			break;
			case D7:
			state = START_state;
			start();
			break;
			case NoEvent:
		break;
		}
		break;

	case END_State:
		switch(bevent)
		{
			case D5:
			break;
			case D6:
			break;
			case D7:
			state = START_state;
			start();
			break;
			case NoEvent:
			break;
		}
	break;
	}
}

int main(void)
{
	DDRD = 0b00011111;	// Pins PORTD<7:5> input, PORTD<4:0> output
	start();	//installation function

	while(1)
	{		
		bevent = getButton();
		handleState();		
	}

	return 1;
}


