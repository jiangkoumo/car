#include "stm32f10x.h"
#include "Delay.h"

/**
  * @brief  LED初始化
  */
void LED_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /* 使能GPIO时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    /* 配置LED1引脚为推挽输出 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* 关闭LED1 */
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
    
    /* 配置LED2引脚为推挽输出 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* 关闭LED2 */
    GPIO_SetBits(GPIOA, GPIO_Pin_10);

}

void LED1_ON(void)
{
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void LED1_OFF(void)
{
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void LED1_Turn(void)
{
    if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) == Bit_SET) 
    {
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    } 
    else 
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_13);
    }
}

void LED2_ON(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_10);
}

void LED2_OFF(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_10);
}

void LED2_Turn(void)
{
    if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_10) == Bit_SET) 
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_10);
    } 
    else 
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_10);
    }
}

void LED_Flash_ON(void)
{
    while(1)
    {
        LED1_ON();
        LED2_ON();
        delay_ms(500);
        LED1_OFF();
        LED2_OFF();
        delay_ms(500);
    }
}

void LED_Flash_OFF(void)
{
    LED1_OFF();
    LED2_OFF();
}