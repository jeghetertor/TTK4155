#ifndef UART_H_
#define UART_H_
#include <stdio.h>
int UART_transmit(unsigned char data, FILE * file);
int UART_recieve(unsigned char data, FILE * file);
void UART_init(long ubrr);
#endif 