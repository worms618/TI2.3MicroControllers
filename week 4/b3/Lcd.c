
/*
 * Lcd.c
 * LCD c module voor een lcd op een atmega128 chip
 * Created: 23-2-2016 11:46:37
 *  Author : Jannick van Ballegooijen, 
 *			Tim van Lieshout, 
 *			Remco Vorthoren
 *
 * Commands codes van de site http://www.dinceraydin.com/lcd/commands.htm
 */ 

 /*defines*/
#define F_CPU 8000000UL
#define Lcd_E 3
#define Lcd_RS 2

/*includes*/
#include "Lcd.h" 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

/*Prototypes*/
void toggle_Lcd_E(void);
void enable_Lcd_Rs(void);
void unable_Lcd_Rs(void);
void lcd_command(unsigned char cmd);
void lcd_write_data( unsigned char data );

 void toggle_Lcd_E(void)
 {
	PORTC |= (1<<Lcd_E);	//E hoog
	_delay_ms(1);			//wacht even, belangrijk niet te vergeten
	PORTC &= ~(1<<Lcd_E);	//E laag	
	_delay_ms(1);
 }

 void enable_Lcd_Rs(void)
 {
	PORTC |= (1<<Lcd_RS); //RS hoog
 }

 /*Zet RS laag*/
 void unable_Lcd_Rs(void)
 {
	PORTC &= ~(1<<Lcd_RS); //RS laag
 }

 void lcd_write_data( unsigned char data )
 {
	//First nibble
	 PORTC = data;
	 enable_Lcd_Rs();
	 toggle_Lcd_E();

	 //Second nibble
	 PORTC = (data<<4);
	 enable_Lcd_Rs();
	 toggle_Lcd_E();
 }

 void lcd_command(unsigned char cmd)
 {
	//upper nibble
	PORTC = cmd;
	unable_Lcd_Rs();
	toggle_Lcd_E();

	//lower nibble
	PORTC = (cmd<<4);
	unable_Lcd_Rs();
	toggle_Lcd_E();	
 }

  void init_Lcd_4_bit_mode(void)
  {
	  DDRC = 0b11111111;	//PORTC (7:0) output
	  PORTC = 0x00;		//Maak alle pins van portc laag, ook e en rs

	  //PORTC = 0x28;		//Function set (4-bit interface, 2 lines, 5*8 Pixels)
	  PORTC = 0x20;		//Function set (4-bit interface, 1 lines, 5*8 Pixels)
	  toggle_Lcd_E();
	  
	  //lcd_command(0x0E);	//Zet display aan en cursor aan en maak cursor niet knipperent	(0x0E -> 0b00001110)
	  lcd_command(0x0C);	//Zet display aan en cursor uit en maak cursor niet knipperent		(0x0F -> 0b00001100)
	  lcd_command(0x06);	//Entry mode 06, van links naar rechts schrijven. cursor schuift mee
	  display_clear();
  }
 
 void display_text(char *str)
 {
	 for(;*str; str++)
	 {
		 lcd_write_data(*str);
	 }
 }

 void display_int(int number)
 {
	char buffer[999];
	itoa(number,buffer,10);
	display_text(buffer);
 }
 
 void display_text_int(char *str,int number)
 {
	display_text(str);
	display_int(number);
 }

 void display_clear()
 {
	lcd_command(0x01);	//clear display
 }

  void set_cursor(int position)
  {	  
	/*werkt alleen als je 2 lines hebt. line 1 adressen: 0...15, line 2 adressen: 64...79*/
	  //if(position > 15)
	  //{
		//position -= 15;
		//position += 64;
	  //}
	  position += 128;
	  lcd_command(position);
  }

