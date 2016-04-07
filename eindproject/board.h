#ifndef _board_H
#define _board_H

/*
 * board.h
 *
 * Created: 7-4-2016 12:25:41
 *  Author: remco
 */ 
 
 #define Led_On 1
 #define Led_Off 0

 void initBoard(void);
 void clearBoard(void);
 
 void setLed(int,int,int);
 void write_board_data(void);
 void show_smiley(void);
 #endif
