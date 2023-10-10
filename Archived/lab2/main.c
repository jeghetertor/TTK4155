#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART.h"

#define F_CPU 4915200UL
#define FOSC 4915200UL
#define BAUD 9600UL
#define MYUBRR (FOSC/(16UL*BAUD))-1

void SRAM_test(void)
{
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\n");
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			write_errors++;
		}
	}
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);
	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			retrieval_errors++;
		}
		printf("Cycle [%4d]\n", i);
	}
	printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}
void latch_test(void){
	uint8_t byte = 0x1800;
	DDRA = byte;
	DDRE = 0b10;
	PORTE = 0b10;
	PORTA = byte;
	int i = 0;
	while(1){
		PORTA = byte;
		_delay_ms(5000);
		PORTA = 0b0;
		_delay_ms(5000);
		PORTA = byte;
		i++;
		if(i == 5){
			PORTE = 0b0;
		}
	}
}
int main(void){
	MCUCR |= (1 << 7); //Enabling XMEM
	SFIOR = (1 << XMM2); //mask pc4-pc7
	UART_init(MYUBRR);
	//uint16_t test = 0x1800
	uint16_t adr = 0xFFF;
	
	volatile char *ext_ram = (char *) 0x00;
	//uint16_t ram;
	/*while (1)
	{
		ext_ram[adr] = 0x00;
		_delay_ms(100);
	}
	*/
	//ext_ram[2047] = 0x7FF;
	
	_delay_ms(500);
	printf("D funker aa importere!\n");
	//byte |= (1 << 1:)
	//latch_test(); 
	//printf("Init test\n");
	SRAM_test();	

	return 0;
}