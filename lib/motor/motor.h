#ifndef __MOTOR_H
#define __MOTOR_H

#include <stdint.h>

void Motor_Init(void);
void Motor_LeftSetSpeed(int16_t Speed);
void Motor_RightSetSpeed(int16_t Speed);
void Motor_stop(void);
void Motor_go(void);
void Motor_back(void);
void Turn_left(void);
void Turn_right(void);

#endif // __MOTOR_H