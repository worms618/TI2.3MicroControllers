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

typedef enum{D7,D6,D5}BEvent;
typedef enum{START_state,STATE_1,STATE_2,STATE_3,END_State}BOARD_STATE;	


typedef struct
{
	void (*init)(void);
	void (*body)(void);
	void (*exit)(void);
}State_Struct;

State_Struct Start =	{NULL,start,NULL};
State_Struct State1 =	{NULL,s1,	NULL};
State_Struct State2 =	{NULL,s2,	NULL};
State_Struct State3 =	{NULL,s3,	NULL};
State_Struct End =		{NULL,end,	NULL};

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

BOARD_STATE state = START_state;

void handleEvent(BEvent bevent)
{
	switch(state)
	{
	case START_state:
		switch(bevent)
		{
			case D5:
			closeState(START_state);
			state = STATE_1;			
			break;
			case D6:
			closeState(START_state);
			state = STATE_2;
			break;
			case D7:
			break;
		}
	break;
	case STATE_1:
	switch(bevent)
	{
		case D5:
		closeState(STATE_1);
		state = STATE_2;
		break;
		case D6:		
		break;
		case D7:
		closeState(STATE_1);
		state = START_state;
		break;
	}
	break;
	}
	handleState(state);
}

void closeState(State_Struct state)
{
	if(state.exit != NULL)
	{
		state.exit();
	}
}

void handleState(State_Struct state)
{
	if(state.init != NULL)
	{
		state.init();
	}
	if(state.body != NULL)
	{
		state.body();
	}
}

int main(void)
{
	DDRD = 0b00011111;

	start();	

	while(1)
	{
		//start();
		//wait(500);
		//s1();
		//wait(500);
		//s2();
		//wait(500);
		//s3();
		//wait(500);
		//end();
		//wait(500);
	}

	return 1;
}


