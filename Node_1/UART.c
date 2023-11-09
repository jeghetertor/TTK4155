#define F_CPU 4915200UL
#define FOSC 4915200UL
#define BAUD 9600UL
//#define MYUBRR (FOSC/(16UL*BAUD))-1
#define MYUBRR (FOSC/(16UL*BAUD))-1
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
//int USART_recieve(FILE *);
//void USART_Init(long);
//int USART_Transmit(unsigned char, FILE *);

int UART_transmit(unsigned char data, FILE * file)
{
	while( !(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

int UART_recieve(FILE * file)
{
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0;
}
void UART_init(long ubrr){
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
	fdevopen(UART_transmit, UART_recieve);
}