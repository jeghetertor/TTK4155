#ifndef MENU_H_
#define MENU_H_

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define max_children 8

typedef struct Menu_element{
	struct Menu_element * parent;
	struct Menu_element * children[max_children];
	char * text;
	uint8_t num_children;
	void (* function)();
} menu_element;

menu_element* add_menu_element(menu_element* parent, char * text, void (* function)());

void main_menu();
void print_something();
#endif