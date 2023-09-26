// Online C compiler to run C program online
#include <stdio.h>
#include <stdlib.h>

#define num_children 8

typedef struct Menu_element{
    struct Menu_element * parent;
    struct Menu_element * children[num_children];
    char * text;
} menu_element, *menu_ptr;

menu_ptr add_menu_element(menu_ptr parent, char * text){
    menu_ptr new_element = malloc(sizeof(menu_element));
    new_element->text = text;
    new_element->parent = parent;
    int num_child = 0;
    while(parent->children[num_child] != NULL){
        num_child++;
    }
    parent->children[num_child] = new_element;
    return new_element;
}

int main() {
    menu_ptr init_menu = malloc(sizeof(menu_element));
    menu_ptr menu_play = add_menu_element(init_menu, "Play game");
    menu_ptr menu_game1 = add_menu_element(menu_play, "Game 1");
    menu_ptr menu_game2 = add_menu_element(menu_play, "Game 2");
    
    int tall = 1;
    
    // Print display
    menu_ptr current  = (menu_ptr*)init_menu;
    
    //current = &menu_play;
    
    printf("%s\n", current->text);
    
    
    //printf("%s", current->text);
    
    
    for(int i=0; i<8; i++){
        if(menu_play->children[i] != NULL){
            printf("Name: %s\n", menu_play->children[i]->text);
        }
    }

    return 0;
}