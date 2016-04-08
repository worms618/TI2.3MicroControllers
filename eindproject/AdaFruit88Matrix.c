/*
 * AdaFruit88Matrix.c
 *
 * Created: 7-4-2016 12:27:31
 *  Author: remco
 */ 

 #define F_CPU 8000000UL
 #include <util/delay.h>
 #include <avr/interrupt.h>
 
 #include "board.h"
 #include "ledmatrix.h"
 #include "Lcd.h"
 
 #define Bit(x) (1 << x) //bit position
 #define PlayState 0
 #define WinState 1
 #define LossState 2

 //play state, playing the game, win state have won the game.
 int state;

 //position of player on highest row, cap[0,7]
 int player = 0;

 //bullet fired -1 => not fired, >= 0 fired
 //bullet way 6, 1 position a top player, 0 line of targets, cap [6,0] 
 int bulletFired = -1,bulletWay = 6,bulletCount = 20;
 
 //lowest row, target row, cap[0,7]
 int target[] = {1,1,1,1,1,1,1,1};

 //keep score
 int score = 0;

 void initAdaFruit88Matrix(void);

 void initPlayState(void);
 void playState(void);
 void winState(void);
 void lossState(void);

 void resetPlayer(void);
 void updatePlayerPosition(void);

 void resetBulletPosition(void);
 void resetBulletCount(void);
 void updateBullet(void);

 void resetTargetLine(void);
 void fillTargetLineToBoard(void);

 void setLcd(void);
 void writeScoreToLcd(void);
 void writeResultToLcd(void);
 void writeRemainingBullets(void);
 void writeMessageToLcd(char *str);
  
 /******************************************************************/
void wait( int ms )
/* 
short:			Busy wait number of millisecs
inputs:			int ms (Number of millisecs to busy wait)
outputs:	
notes:			Busy wait, not very accurate. Make sure (external)
				clock value is set. This is used by _delay_ms inside
				util/delay.h
Version :    	DMK, Initial code
*******************************************************************/
{
	for (int i=0; i<ms; i++)
	{
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

//interrupt 7, button PE7
ISR(INT7_vect)
{
	if (bulletFired == -1 && bulletCount > 0)
	{
		bulletFired = player;
		bulletCount --;		
	}
	//bullet = player;
	//PORTA ^= Bit(2);
}

//init board for game
void initAdaFruit88Matrix()
{
	twi_init();
	initBoard();
	init_Lcd_4_bit_mode();
	
	DDRA = 0b11111111;

	EICRB |= 0b11000000; //set INT7 faling edge
	EIMSK |= 0b10000000; //enable INT&
	//enable global interrupt system
	sei();
}



 int main(void)
 {
	initAdaFruit88Matrix();	
	initPlayState();//init state
	wait(500);

	while(1)
	{	
		clearBoard();		//always clear board
		switch(state)		//check state
		{
		case PlayState:
			playState();
			break;
		case WinState:
			winState();
			break;
		case LossState:
			lossState();
			break;
		}
		//PORTA = bulletCount;		
		write_board_data();	//write data to adafruit matrix 		
		wait(150);			
	}
	 return 1;
 }

 //init of play state
 void initPlayState(void)
 {
	 state = PlayState;
	 //reset player, bullets and target
	 resetPlayer();
	 resetBulletPosition();
	 resetBulletCount();
	 resetTargetLine();
	 //set score
	 writeScoreToLcd();
 }

 //state of playing, logic for players, bullets and targets.
 void playState(void)
 {
	int win = 1;//win condition met	

	updatePlayerPosition();
	updateBullet();
	

	//check win condition
	for(int i = 0; i < 8; i++)
	{
		if(target[i] == 1)//if there is a target 1, so not hit, you did not won yet
		{
			win = 0;			
		}			
	}
	//draw player
	setLed(7,player,Led_On);
	//draw bullet
	setLed(bulletWay,bulletFired,Led_On);
	//draw targets
	fillTargetLineToBoard();

	
	if(win == 1)
	{
		state = WinState;			
	}
	else if(bulletCount <= 0)
	{
		state = LossState;
	}	
 }

 void winState(void)
 {
	//show happy smiley
	show_happySmiley();
	writeMessageToLcd("Win!");
	wait(5000);//wait 5 seconds	
	initPlayState();
 }

  void lossState(void)
  {
	//show sad smiley
	show_sadSmiley();
	writeResultToLcd();
	wait(5000);
	score = 0;
	initPlayState();
  }

 void resetPlayer(void)
 {
	player = 0;
 }

 void updatePlayerPosition(void)
 {
	 //player logic
	 player++;	//set player 1 to the right
	 player %= 8; //player cap [0,7], when on position 8 back to position 0
 }

 void resetBulletPosition(void)
 {
	bulletWay = 6;//reset bulletway
	bulletFired = -1;//reset bullet fired	
 }

 void resetBulletCount(void)
 {
	bulletCount = 20;
 }

 void updateBullet(void)
 {	
	if(bulletFired >= 0)//is bullet on his way?
	{		
		bulletWay--; //set bullet 1 up
		if(bulletWay < 0)// reached line of target, bullet cap [6,0]
		{
			if(target[bulletFired] == 1)
			{
				score++;
				writeScoreToLcd();					
			}
			else
			{
				writeRemainingBullets();	
			}			
			target[bulletFired] = 0;//bullet hit target, target is destroyed
			
			resetBulletPosition();
		}
	}
 }
 
 void resetTargetLine(void)
 {
	//filling target line
	for(int i = 0; i < 8; i++)
	{
		target[i] = 1;
	}
	fillTargetLineToBoard();
 }

 void fillTargetLineToBoard(void)
 {
	 for(int i = 0; i < 8; i++)
	 {
		 if(target[i] == 1)
			setLed(0,i,Led_On);
	 }
 }

 void setLcd(void)
 {
	set_cursor(0);
	display_clear();
 }

 void writeScoreToLcd(void)
 {
	setLcd();
	display_text_int("Score: ",score);
 }

 void writeResultToLcd(void)
 {
	setLcd();
	display_text_int("Result: ",score);
 }
	
void writeRemainingBullets(void)
{
	setLcd();
	display_text_int("Bullets: ",bulletCount);
}

void writeMessageToLcd(char *str)
{
	setLcd();
	display_text(str);
}

