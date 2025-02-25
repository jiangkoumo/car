#include "stm32f10x.h"

/**
 * @brief 定时器2初始化函数
 * @details 配置定时器2为基本定时器模式，周期为1秒
 *          设置中断优先级并启用更新中断
 */
void Timer_Iint(void)
{
    // 使能定时器2的时钟
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);

    // 定义并初始化定时器基础结构体
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructInit;
    TIM_TimeBaseStructInit.TIM_ClockDivision = TIM_CKD_DIV1;      // 时钟分频因子，不分频
    TIM_TimeBaseStructInit.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数模式
    TIM_TimeBaseStructInit.TIM_Period = 10000 - 1;                // 自动重装载值，计数10000次
    TIM_TimeBaseStructInit.TIM_Prescaler = 7200 - 1;              // 预分频值，72MHz时钟/7200=10kHz
    TIM_TimeBaseStructInit.TIM_RepetitionCounter = 0;             // 重复计数器值，不使用重复计数
    // 应用配置到定时器2
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructInit);

    // 使能定时器更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    // 配置中断优先级分组为2
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // 配置NVIC以处理定时器2中断
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;                     // 定时器2中断通道
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;                     // 使能中断通道
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;           // 设置抢占优先级为2
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;                  // 设置子优先级为1
    NVIC_Init(&NVIC_InitStruct);

    // 启动定时器
    TIM_Cmd(TIM2, ENABLE);
}