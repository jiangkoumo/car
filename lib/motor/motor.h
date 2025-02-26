#ifndef __MOTOR_H
#define __MOTOR_H

#include <stdint.h>

void Motor_Init(void);
void Motor_LeftSetSpeed(int16_t Speed);
void Motor_RightSetSpeed(int16_t Speed);

#endif // __MOTOR_H