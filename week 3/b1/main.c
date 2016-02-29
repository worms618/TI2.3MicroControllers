/*
 * B1
 * Assingment: test code voor de LCD c Module code.
 * Created: 23-2-2016 11:46:15
 * Author : Jannick van Ballegooijen, 
 *			Tim van Lieshout, 
 *			Remco Vorthoren
 */ 

 #define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "Lcd.h"


int main(void)
{	
	init_Lcd_4_bit_mode();
	display_text("Yedi you are2");
	set_cursor(15);
    while (1) 
    {
		
    }
}

