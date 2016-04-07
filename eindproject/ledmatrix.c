/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** ledmatrix.c
**
** Beschrijving:	Simple HT16K33 Ledmatix demo.
** Target:			AVR mcu
** Build:			avr-gcc -std=c99 -Wall -O3 -mmcu=atmega128 -D F_CPU=8000000UL -c ledmatrix.c
**					avr-gcc -g -mmcu=atmega128 -o ledmatrix.elf ledmatrix.o
**					avr-objcopy -O ihex ledmatrix.elf ledmatrix.hex 
**					or type 'make'
** Author: 			dkroeske@gmail.com
** -------------------------------------------------------------------------*/

#include "ledmatrix.h"

/*prototypes*/
void twi_start(void);
void twi_stop(void);
void twi_tx(unsigned char data);
char adressOfrow[]= {0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E};
char adressOfColum[]={0x80,0x01,0x02,0x04,0x08,0x10,0x20,0x40};

char getRowAdress(int row)
{
	return adressOfrow[row];
}

char getColumnAdress(int column)
{
	return adressOfColum[column];
}

/******************************************************************/
void twi_init(void)
/* 
short:			Init AVR TWI interface and set bitrate
inputs:			
outputs:	
notes:			TWI clock is set to 100 kHz
Version :    	DMK, Initial code
*******************************************************************/
{
	TWSR = 0;
	TWBR = 32;	 // TWI clock set to 100kHz, prescaler = 0
	// Init HT16K22. Page 32 datasheet
	twi_start();
	twi_tx(0xE0);	// Display I2C addres + R/W bit
	twi_tx(0x21);	// Internal osc on (page 10 HT16K33)
	twi_stop();
	
	twi_start();
	twi_tx(0xE0);	// Display I2C address + R/W bit
	twi_tx(0xA0);	// HT16K33 pins all output
	twi_stop();
	
	twi_start();
	twi_tx(0xE0);	// Display I2C address + R/W bit
	twi_tx(0xE3);	// Display Dimming 4/16 duty cycle
	twi_stop();
	
	twi_start();
	twi_tx(0xE0);	// Display I2C address + R/W bit
	twi_tx(0x81);	// Display OFF - Blink On
	twi_stop();
	
}

/******************************************************************/
void twi_start(void)
/* 
short:			Generate TWI start condition
inputs:		
outputs:	
notes:			
Version :    	DMK, Initial code
*******************************************************************/
{
	TWCR = (0x80 | 0x20 | 0x04);
	while( 0x00 == (TWCR & 0x80) );
}

/******************************************************************/
void twi_stop(void)
/* 
short:			Generate TWI stop condition
inputs:		
outputs:	
notes:			
Version :    	DMK, Initial code
*******************************************************************/
{
	TWCR = (0x80 | 0x10 | 0x04);
}

/******************************************************************/
void twi_tx(unsigned char data)
/* 
short:			transmit 8 bits data
inputs:		
outputs:	
notes:			
Version :    	DMK, Initial code
*******************************************************************/
{
	TWDR = data;
	TWCR = (0x80 | 0x04);
	while( 0 == (TWCR & 0x80) );
}

void write_data(unsigned char adress,unsigned char data)
{
	twi_start();
	twi_tx(0xE0);	// Display I2C addres + R/W bit
	twi_tx(adress);	// Address
	twi_tx(data);	// data
	twi_stop();
}

/******************************************************************/
//int main( void )
///* 
//short:			main() loop, entry point of executable
//inputs:			
//outputs:	
//notes:			Looping forever, trashing the HT16K33
//Version :    	DMK, Initial code
//*******************************************************************/
//{
	//
	//twi_init();		// Init TWI interface
//
	//// Init HT16K22. Page 32 datasheet
	//twi_start();
	//twi_tx(0xE0);	// Display I2C addres + R/W bit
	//twi_tx(0x21);	// Internal osc on (page 10 HT16K33)
	//twi_stop();
//
	//twi_start();
	//twi_tx(0xE0);	// Display I2C address + R/W bit
	//twi_tx(0xA0);	// HT16K33 pins all output
	//twi_stop();
//
	//twi_start();
	//twi_tx(0xE0);	// Display I2C address + R/W bit
	//twi_tx(0xE3);	// Display Dimming 4/16 duty cycle
	//twi_stop();
//
	//twi_start();
	//twi_tx(0xE0);	// Display I2C address + R/W bit
	//twi_tx(0x81);	// Display OFF - Blink On
	//twi_stop();
	//
	////while (1)
	////{
		//twi_start();
		//twi_tx(0xE0);	// Display I2C addres + R/W bit
		//twi_tx(0x00);	// Address
		//twi_tx(0xFF);	// data
		//twi_stop();
////
		////wait(500);	
//
		////twi_start();
		////twi_tx(0xE0);	// Display I2C addres + R/W bit
		////twi_tx(0x00);	// Address
		////twi_tx(0x04);	// data
		////twi_stop();	
//
		////wait(500);
	////}
//
	//return 1;
//}
