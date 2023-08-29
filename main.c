/*
 * TESTAGAIN.c
 *
 * Created: 29.08.2023 12:31:01
 * Author : torhstau
 */ 
#define F_CPU 4915200
#define FOSC 4915200
#define BAUD 9600
#define MYUBRR (FOSC/(16UL*BAUD))-1
#include <avr/io.h>
#include <util/delay.h>
//#include <stdlib.h>
#include <stdio.h>
unsigned char USART_recieve();
void USART_Init(long);
void USART_Transmit(unsigned char);

void uart_Init(unsigned int ubrr) {
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
}

void uart_Transmit( unsigned char data ) {
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}
unsigned char uart_Receive( void ){
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}


int main(void)
{
	//UCSR0C
	//UBRRH0
	unsigned char L;
	unsigned char rxVar;
	uart_Init(MYUBRR);
    /* Replace with your application code */
    while (1) 
    {
		L = "j";
		uart_Transmit(L);
		//_delay_ms(100);
		//rxVar = USART_recieve();
		//_delay_ms(10);
		//L = "b";
		//USART_Transmit(L);
		//rxVar = USART_recieve();
		/*
		DDRB = 0xff;
		PORTB = 0x0;
		_delay_ms(100);
		PORTB = 0xff;
		_delay_ms(100);
		*/

    }	
}
void USART_Init(long ubrr){
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
}

void USART_Transmit(unsigned char data)
{
	while( !(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

unsigned char USART_recieve(void)
{
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

