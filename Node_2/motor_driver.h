#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_

#define DIR PIO_PD10
#define EN PIO_PD9
#define SEL PIO_PD2
#define NOT_RST PIO_PD1
#define NOT_OE PIO_PD0



#define encoder0 PIO_PC1
#define encoder1 PIO_PC2
#define encoder2 PIO_PC3
#define encoder3 PIO_PC4
#define encoder4 PIO_PC5
#define encoder5 PIO_PC6
#define encoder6 PIO_PC7
#define encoder7 PIO_PC8
#define ENCODERMASK 0x1FE
#define ENCODERMASK 0x1FE
void motor_init();
int16_t motor_read_encoder();
void motor_start();
void motor_stop();


#endif