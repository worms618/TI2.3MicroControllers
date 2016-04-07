/*
 * Board.c
 *
 * Created: 7-4-2016 12:16:43
 *  Author: remco
 */ 

 #include "ledmatrix.h"
 #include "board.h"

 void write_board_data(void); 

int board[8][8];

 void initBoard(void)
 {
	clearBoard();
 }

 void clearBoard(void)
 {
	for(int row = 0; row < 8; row++)
	{
		for(int column = 0; column < 8; column++)
		{
			board[row][column] = 0;
		}
	}	
 }

 void setLed(int row,int column,int status)
 {
	
	if((row >= 0 && row < 8) && (column >= 0 && column < 8))
	{
		board[row][column] = status;
	}		
 }

 void show_smiley(void)
 {
	int smiley[8][8] =  
	{
		{0,0,1,1,1,1,0,0},
		{0,1,0,0,0,0,1,0},
		{1,0,1,0,0,1,0,1},
		{1,0,0,0,0,0,0,1},
		{1,0,1,0,0,1,0,1},
		{1,0,0,1,1,0,0,1},
		{0,1,0,0,0,0,1,0},
		{0,0,1,1,1,1,0,0}
	};

	for(int row = 0; row < 8; row++)
	{
		for(int column = 0; column < 8; column++)
		{
			board[row][column] = smiley[row][column];
		}
	}
	write_board_data();
 }

 void write_board_data(void)
 {
	char columnValue = 0;
	for(int row = 0; row < 8; row++)
	{
		columnValue = 0;
		for(int colomn = 0; colomn < 8; colomn++)
		{			
			if(board[row][colomn] == 1)
			{
				columnValue |= getColumnAdress(colomn);
			}			
		}
		write_data(getRowAdress(row),columnValue);
	}
 }

