#ifndef __KEYS_H
#define __KEYS_H

#include "stm32f10x.h"

// 定义按键返回值
typedef enum 
{
    KEY_NONE = 0,
    KEY1_PRESS,
    KEY2_PRESS,
    KEY3_PRESS
} KeyStatus;

void KEY_Init(void);
KeyStatus KEY_Scan(void);

#endif