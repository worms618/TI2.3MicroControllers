#ifndef _ledmatrix_H
#define _ledmatrix_H

/*
 * ledmatrix.h
 *
 * Created: 7-4-2016 12:21:39
 *  Author: remco
 */ 


 #include <avr/io.h>

 void twi_init(void);
 void write_data(unsigned char,unsigned char);
 char getRowAdress(int);
 char getColumnAdress(int);

 #endif
