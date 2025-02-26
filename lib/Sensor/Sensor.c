#include "stm32f10x.h"

void Sensor_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//从左到右依次为0到5,读取传感器状态，返回1表示遇到白线，返回0表示遇到黑线
uint8_t Read_Senors_0()
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);
}

uint8_t Read_Senors_1()
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
}

uint8_t Read_Senors_2()
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5);
}

uint8_t Read_Senors_3()
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
}

uint8_t Read_Senors_4()
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
}

uint8_t Read_Senors_5()
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2);
}
