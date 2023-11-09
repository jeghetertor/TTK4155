#ifndef REGULATOR_H_
#define REGULATOR_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

void PID(int r, int y,double Kp,double Ki, int* error_sum, double T, int8_t difficulty);


#endif /* REGULATOR_H_ */