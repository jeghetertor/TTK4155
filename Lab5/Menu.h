#define max_children 8

typedef struct Menu_element{
	struct Menu_element * parent;
	struct Menu_element * children[max_children];
	char * text;
	uint8_t num_children;
} menu_element, *menu_ptr;

menu_ptr add_menu_element(menu_ptr parent, char * text);
void Menu_function(void);
