#ifndef ADC_H_
#define ADC_H_


struct ADC {
	int l_slider;
	int r_slider;
	int x_axis;
	int y_axis;
	int8_t x_axis_percent;
	int8_t y_axis_percent;
	uint8_t state_x;
	uint8_t state_y;
};

struct Offset_const{
	int offset_x;
	int offset_y;
};
void ADC_init(void);
struct ADC  ADC_read(void);
struct Offset_const ADC_calibration(void);
struct ADC ADC_output(struct Offset_const offset_const);
typedef enum Joy_Mode{NEUTRAL,UP,DOWN,LEFT,RIGHT} Joy_Mode;
Joy_Mode Joy_direction(struct Offset_const offset_const);
int max(int x, int y);

#endif 