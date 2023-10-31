#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
typedef struct Joy{
	int8_t x;
	int8_t y;
	uint8_t r_slider;
} Joy;

Joy joy;

typedef struct Goal{
	int8_t old_goal;
	int8_t current_goal;
	long debounce;
	bool scored_flag;
	} Goal;