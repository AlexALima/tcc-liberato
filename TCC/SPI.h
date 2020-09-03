/* 
 * File:   SPI.h
 * Author: Alexandre
 *
 * Created on 28 de Setembro de 2013, 18:02
 */

#ifndef SPI_H
#define	SPI_H

void spi_init(void){

	// Initialise I2C MSSP
	// Master 100KHz
	TRISC3=1;           	// set SCL and SDA pins as inputs
	TRISC4=1;

	SSPCON = 0b00110001;
	SSPCON2 = 0x00;

	SSPSTAT = 0b11000000; 	// Slew rate disabled

}

void spi_write(unsigned char dado){
    while(BF);
    SSPBUF=dado;
}

unsigned char spi_read(unsigned char dado){
    while(BF);
    SSPBUF=dado;
    while(BF) BF=0;
    return(SSPBUF);
}

