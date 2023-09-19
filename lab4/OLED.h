#ifndef OLED_H_
#define OLED_H_
//#include "fonts.h"
void oled_init(void);
void oled_reset(void);
//void oled_write_cmd(uint8_t cmd);
//void oled_write_data(uint8_t data);
void oled_pos(uint8_t row, uint8_t col);
void oled_print_arrow(uint8_t row, uint8_t col);
void oled_goto_line(uint8_t line);
void oled_goto_column(uint8_t col);
void oled_reset(void);
void oled_print(char*);
void oled_page_addressing(void);
#endif 