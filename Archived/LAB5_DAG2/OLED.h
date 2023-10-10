#ifndef OLED_H_
#define OLED_H_
#ifndef ADC_H_
#include "ADC.h"
#endif
struct Arrow_state{
	uint8_t forward;
	uint8_t backward;
	uint8_t i;
};

void oled_init(void);
void oled_reset(void);
void oled_write_cmd(uint8_t cmd);
void oled_write_data(uint8_t data);
void oled_pos(uint8_t row, uint8_t col);
void oled_print_arrow(uint8_t row, uint8_t col);
void oled_goto_line(uint8_t line);
void oled_goto_column(uint16_t col);
void oled_reset(void);
void oled_print_char(char);
void oled_page_addressing(void);
void oled_home(void);
void oled_print(char* string, ... );
uint8_t move_arrow(uint8_t current_line_number, Joy_Mode joy_direction, uint8_t  max_menu_elements);
void oled_setting_dark_mode();
void oled_setting_light_mode();



#endif