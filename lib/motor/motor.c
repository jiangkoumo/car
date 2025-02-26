#include "stm32f10x.h"
#include "PWM.h"

void Motor_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);  

    //左电机
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;   
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;    
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    //右电机
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;   
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;    
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);
    GPIO_SetBits(GPIOC, GPIO_Pin_14 | GPIO_Pin_15);

    PWM_Init();
}

void Motor_LeftSetSpeed(int16_t Speed)
{
    if(Speed >= 0)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_1);
        GPIO_ResetBits(GPIOB, GPIO_Pin_0);
        PWM_SetCompare1(Speed);
    }
    else
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_0);
        GPIO_ResetBits(GPIOB, GPIO_Pin_1);
        PWM_SetCompare1(-Speed);
    }
}

void Motor_RightSetSpeed(int16_t Speed)
{
    if(Speed >= 0)
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_15);
        GPIO_ResetBits(GPIOC, GPIO_Pin_14);
        PWM_SetCompare4(Speed);
    }
    else
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_14);
        GPIO_ResetBits(GPIOC, GPIO_Pin_15);
        PWM_SetCompare4(-Speed);
    }
}
