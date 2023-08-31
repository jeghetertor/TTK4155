/*
 * TESTAGAIN.c
 *
 * Created: 29.08.2023 12:31:01
 * Author : torhstau
 */ 
#define F_CPU 4915200UL
#define FOSC 4915200UL
#define BAUD 9600UL
//#define MYUBRR (FOSC/(16UL*BAUD))-1
#define MYUBRR (FOSC/(16UL*BAUD))-1
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
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
int main(void)
{
	//UCSR0C
	//UBRRH0
	UART_init(MYUBRR);
    /* Replace with your application code */
    while (1) 
    {
		//L = USART_recieve();
		//USART_Transmit(L);
		_delay_ms(500);
		printf("Lab1 er ferdig?\n");
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
