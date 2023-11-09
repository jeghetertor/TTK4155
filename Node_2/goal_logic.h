#ifndef GOAL_LOGIC_H_
#define GOAL_LOGIC_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "Structs.h"

void goal_scored(Goal *goal, int IR_val);
void goal_reset(int *goal);

#endif /* GOAL_LOGIC_H_ */