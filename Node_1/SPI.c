#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#ifndef SPI_H_
#include "SPI.h"
#endif

#define DDR_SPI DDRB
#define DD_MOSI PB5
#define DD_SCK PB7
#define DD_SS PB4


void SPI_init(void)
{
	/* Set MOSI and SCK output, all others input */
	DDR_SPI |= (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	
	
	ss_disable();
	
}

void ss_enable(){
	// Setting the cs bit low
	PORTB &= ~(1<<DD_SS);
}

void ss_disable(){
	// Setting the cs bit high
	PORTB |= (1<<DD_SS);
}

void SPI_send(uint8_t cData)
{
	/* Start transmission */
	//printf("cData: %0x  ", cData);
	SPDR = cData;

	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
	;
}

uint8_t SPI_read(void)
{
	SPI_send(0x00);
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)))
	;
	/* Return data register */
	return SPDR;
}

void test_SPI(){
	uint8_t char_result;
	uint8_t cData = 0x13;
	
	ss_enable();
	SPI_send(0x02); // INSTRUCTION - WRITE
	SPI_send(0xA7); // ADRESS TO BE WRITTEN TO
	SPI_send(0x0E); // DATA WRITTEN
	
	ss_disable(); // INITIATE NEW INSTRUCTION
	ss_enable();
	
	SPI_send(0x03);	// INSTRUCTION - READ
	SPI_send(0xA7);	// ADRESS TO READ
	char_result = SPI_read(); // READ RESULT
	
	ss_disable();
}