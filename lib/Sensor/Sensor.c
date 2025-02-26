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

//从左到右依次为0到5
uint8_t Sensor0_Get_State()
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);
}

uint8_t Sensor1_Get_State()
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
}

uint8_t Sensor2_Get_State()
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5);
}

uint8_t Sensor3_Get_State()
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
}

uint8_t Sensor4_Get_State()
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
}

uint8_t Sensor5_Get_State()
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2);
}
