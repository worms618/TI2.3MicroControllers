
/*
 * Lcd.h
 * LCD c module voor een lcd op een atmega128 chip
 * Created: 23-2-2016 11:46:51
 *  Author : Jannick van Ballegooijen, 
 *			Tim van Lieshout, 
 *			Remco Vorthoren
 */ 


void init_Lcd_4_bit_mode(void);
void display_text(char *str);
void display_text_int(char *str,int number);
void display_int(int number);
void display_clear(void);
void set_cursor(int position);

