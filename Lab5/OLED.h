#ifndef OLED_H_
#define OLED_H_
//#include "fonts.h"

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
struct Arrow_state move_arrow(uint8_t i, uint8_t joy_direction, uint8_t max);



#endif