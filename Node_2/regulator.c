#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "motor_driver.h"

void PID(int r, int y,double Kp,double Ki, int* error_sum, double T,int8_t difficulty){
	r = map(r,0,255,-100,100);
	int satp = 100;
	int satm = -satp;
	int e = r - y;
	*error_sum += e*T;
	int u_p = Kp*e;
	int u_i = 0;
	//int u_i = Ki*(*error_sum);
	int u = u_p+u_i;
	if (u > satp){
		u = satp;
		
		}else if (u < satm){
		u = satm;
	}
	motor_openloop(u,1500-200*difficulty);
	printf("r: %d, y: %d, u_p: %d, u_i: %d\n", r, y, u_p, u_i);
}