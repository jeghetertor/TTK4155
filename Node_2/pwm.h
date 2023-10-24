#ifndef PWM_H_
#define PWM_H_

void pwm_init();
void servo_set_position(int pos);
long map(long x, long in_min, long in_max, long out_min, long out_max);

#endif