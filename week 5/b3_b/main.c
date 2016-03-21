/*
* Project name : Demo5_7a : spi - 7 segments display
* Author : Avans-TI, WvdE, JW
* Revision History : 20110228: - initial release; 20120307: - this version
* Description : This program sends data to a 4-digit display with spi
* Test configuration:
MCU: ATmega128
Dev.Board: BIGAVR6
Oscillator: External Clock 08.0000 MHz
Ext. Modules: Serial 7-seg display
SW: AVR-GCC
* NOTES : Turn ON switch 15, PB1/PB2/PB3 to MISO/MOSI/SCK
*/
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#define BIT(x) ( 1<<x )
#define DDR_SPI DDRB // spi Data direction register
#define PORT_SPI PORTB // spi Output register
#define SPI_SCK 1 // PB1: spi Pin System Clock
#define SPI_MOSI 2 // PB2: spi Pin MOSI
#define SPI_MISO 3 // PB3: spi Pin MISO
#define SPI_SS 0 // PB0: spi Pin Slave Select

void spi_writeWord(unsigned char adress, unsigned char data);
void writeLedDisplay(int value);

// wait(): busy waiting for 'ms' millisecond - used library: util/delay.h
void wait(int ms)
{
	for (int i=0; i<ms; i++)
	_delay_ms(1);
}
void spi_masterInit(void)
{
	DDR_SPI = 0xff; // All pins output: MOSI, SCK, SS, SS_display
	DDR_SPI &= ~BIT(SPI_MISO); // except: MISO input
	PORT_SPI |= BIT(SPI_SS); // SS_ADC == 1: deselect slave
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1); // or: SPCR = 0b11010010;
	// Enable spi, MasterMode, Clock rate fck/64
	// bitrate=125kHz, Mode = 0: CPOL=0, CPPH=0
}
// Write a byte from master to slave
void spi_write( unsigned char data )
{
	SPDR = data; // Load byte --> starts transmission
	while( !(SPSR & BIT(SPIF)) ); // Wait for transmission complete
}
// Write a byte from master to slave and read a byte from slave - not used here
char spi_writeRead( unsigned char data )
{
	SPDR = data; // Load byte --> starts transmission
	while( !(SPSR & BIT(SPIF)) ); // Wait for transmission complete
	data = SPDR; // New received data (eventually, MISO) in SPDR
	return data; // Return received byte
}
// Select device on pinnumer PORTB
void spi_slaveSelect(unsigned char chipNumber)
{
	PORTB &= ~BIT(chipNumber);
}
// Deselect device on pinnumer PORTB
void spi_slaveDeSelect(unsigned char chipNumber)
{
	PORTB |= BIT(chipNumber);
}

// Write a word = address byte + data byte from master to slave
void spi_writeWord(unsigned char adress, unsigned char data)
{
	spi_slaveSelect(0); // Select display chip (MAX7219)
	spi_write(adress); // Adress
	spi_write(data); // Data
	spi_slaveDeSelect(0); // Deselect display chip
}

void writeLedDisplay(int value)
{
	int isNegatief = value < 0; //is de value negatief? 1 = true, 0 = false
	int i = 0;	//hou bij hoeveel getallen je hebt
	int imax;	//maximaal aantal getallen die je kan wegschrijven

	if(isNegatief == 1)
	{
		imax = 3;		//bij negatief getal, max 3 getallen, vanwege -  voor getal
		value *= -1;	//maak value positief, zodat je naar digit kan krijgen
	}
	else
	{
		imax = 4;		//bij positief getal max 4 getallen
	}
	//maximaal aantal nummers displayen, dus maximaal aantal keer loopen
	for(; i < imax; i++)
	{
		//laatste getalnaar digits schrijven
		spi_writeWord(i+1,value%10);		
		//1 na laatste getal, wordt laatste getal,
		//en laatste getal valt weg
		value /= 10;	
		if(value == 0)
		{
			break;
		}	
	}

	if(isNegatief == 1)
	{
		spi_writeWord(i+2,0x0A); //zet streepje op digit voor laatste getal
	}
	
}

//Microcontrollers – TI2.3 - Opdrachten track 5 - versie 14-03-2016 Pagina 10 van 11
// Initialize the driver chip (type MAX 7219)
void displayDriverInit()
{
	spi_slaveSelect(0); // Select display chip (MAX7219)
	spi_write(0x09); // Register 09: Decode Mode
	spi_write(0xFF); // -> 1's = BCD mode for all digits	 
	spi_slaveDeSelect(0); // Deselect display chip
	spi_slaveSelect(0); // Select dispaly chip
	spi_write(0x0A); // Register 0A: Intensity
	spi_write(0x04); // -> Level 4 (in range [1..F])
	spi_slaveDeSelect(0); // Deselect display chip
	spi_slaveSelect(0); // Select display chip
	spi_write(0x0B); // Register 0B: Scan-limit
	spi_write(0x03); // -> 1 = Display digits 0..3
	spi_slaveDeSelect(0); // Deselect display chip
	spi_slaveSelect(0); // Select display chip
	spi_write(0x0C); // Register 0B: Shutdown register
	spi_write(0x01); // -> 1 = Normal operation
	spi_slaveDeSelect(0); // Deselect display chip
}
// Set display on ('normal operation')
void displayOn()
{
	spi_slaveSelect(0); // Select display chip
	spi_write(0x0C); // Register 0B: Shutdown register
	spi_write(0x01); // -> 1 = Normal operation
	spi_slaveDeSelect(0); // Deselect display chip
}
// Set display off ('shut down')
void displayOff()
{
	spi_slaveSelect(0); // Select display chip
	spi_write(0x0C); // Register 0B: Shutdown register
	spi_write(0x00); // -> 1 = Normal operation
	spi_slaveDeSelect(0); // Deselect display chip
}
int main()
{
	DDRB=0x01; // Set PB0 pin as output for display select
	spi_masterInit(); // Initialize spi module
	displayDriverInit(); // Initialize display chip
	// clear display (all zero's)
	for (char i =1; i<=4; i++)
	{
		//spi_slaveSelect(0); // Select display chip
		//spi_write(i); // digit adress: (digit place)
		//spi_write(0); // digit value: 0
		//spi_slaveDeSelect(0); // Deselect display chip
		spi_writeWord(i,0x0F);
	}
	wait(1000);
	// write 4-digit data
	//for (char i =1; i<=4; i++)
	//{
		////spi_slaveSelect(0); // Select display chip
		////spi_write(i); // digit adress: (digit place)
		////spi_write(i); // digit value: i (= digit place)
		////spi_slaveDeSelect(0); // Deselect display chip
		//spi_writeWord(i,i);
		//wait(1000);
	//}
	//wait(1000);

	writeLedDisplay(5);
	wait(1000);
	writeLedDisplay(52);
	wait(1000);
	writeLedDisplay(528);
	wait(1000);
	writeLedDisplay(5281);	
	wait(1000);
	for (char i =1; i<=4; i++)
	{
		spi_writeWord(i,0x0F);
	}
	writeLedDisplay(-5);
	wait(1000);
	writeLedDisplay(-52);
	wait(1000);
	writeLedDisplay(-528);
	
	return (1);
}