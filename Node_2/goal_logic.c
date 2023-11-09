#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "goal_logic.h"
#include "Structs.h"


void goal_scored(Goal *goal, int IR_val){
	if(goal->debounce > 50){
		if((IR_val < 2300)&&(goal->scored_flag == 0)){
			goal->scored_flag = 1;
			goal->current_goal++;
			goal->debounce = 0;
		}
	}
	if((IR_val > 2300) && goal->scored_flag == 1){
		goal->scored_flag = 0;
	}
	goal->debounce++;
}

void goal_reset(int *goal){
	goal = 0;
}