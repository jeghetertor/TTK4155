#include <stdio.h>
#include <stdlib.h>
#ifndef Menu_H_
#include "Menu.h"
#endif


menu_ptr add_menu_element(menu_ptr parent, char * text){
	menu_ptr new_element = malloc(sizeof(menu_element));
	new_element->text = text;
	new_element->parent = parent;
	int num_child = 0;
	while(parent->children[num_child] != NULL){
		num_child++;
	}
	parent->num_children++;
	parent->children[num_child] = new_element;
	return new_element;
}

void Menu_function(void){
    menu_ptr init_menu = malloc(sizeof(menu_element));
    menu_ptr menu_play = add_menu_element(init_menu, "Play game");
    menu_ptr menu_game1 = add_menu_element(menu_play, "Game 1");
    menu_ptr menu_game2 = add_menu_element(menu_play, "Game 2");
	
	while(1){
		oled_home();
		for(int i=0; i<8; i++){
			if(menu_play->children[i] != NULL){
				oled_pos(i,20);
				printf("Name: %s\n", menu_play->children[i]->text);
				oled_print("Name: %s\n", menu_play->children[i]->text);
			}
		}
		
		

		//menu_element * current  = &menu_play;


	}

}