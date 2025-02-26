#include "stm32f10x.h"

/**
 * @brief PWM初始化函数
 * @details 配置TIM1的通道1(PA8)和通道4(PA11)为PWM输出模式
 *          PWM频率约为100Hz (72MHz/720/100)
 */
void PWM_Init(void)
{
    // 禁用USART1时钟，避免潜在的引脚冲突
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE);
    // 使能GPIOA、TIM1和AFIO的时钟
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO, ENABLE);
    // 复位TIM1到默认状态
    TIM_DeInit(TIM1);

    // 配置GPIO引脚为复用推挽输出，用于PWM信号输出
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;      // 复用推挽输出模式
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;  // PA8(TIM1_CH1)和PA11(TIM1_CH4)
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;    // 输出速率50MHz
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 启用TIM1的部分重映射，使PWM信号能从正确的引脚输出
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);

    // 配置TIM1使用内部时钟源
    TIM_InternalClockConfig(TIM1);

    // 配置TIM1的时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructInit;
    TIM_TimeBaseStructInit.TIM_ClockDivision = TIM_CKD_DIV1;       // 时钟分频因子，不分频
    TIM_TimeBaseStructInit.TIM_CounterMode = TIM_CounterMode_Up;   // 向上计数模式
    TIM_TimeBaseStructInit.TIM_Period = 1000 - 1;                   // 自动重装载值，决定PWM周期
    TIM_TimeBaseStructInit.TIM_Prescaler = 36 - 1;                // 预分频值，决定计数频率
    TIM_TimeBaseStructInit.TIM_RepetitionCounter = 0;              // 重复计数器值，不使用
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructInit);

    // 配置TIM1的输出通道参数
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCStructInit(&TIM_OCInitStruct);                         // 初始化结构体
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;               // PWM模式1：CNT<CCR时输出高电平
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;    // 使能输出
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;        // 输出极性为高
    TIM_OCInitStruct.TIM_Pulse = 0;                               // 初始占空比为0
    TIM_OC1Init(TIM1, &TIM_OCInitStruct);                         // 初始化通道1
    TIM_OC4Init(TIM1, &TIM_OCInitStruct);                         // 初始化通道4

    // 使能TIM1通道1和通道4的预装载寄存器
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

    // 使能TIM1计数器
    TIM_Cmd(TIM1, ENABLE);
    // 使能TIM1的PWM输出（高级定时器特有）
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

/**
 * @brief 设置TIM1通道1的PWM占空比
 * @param Compare PWM比较值，范围0-99，对应0%-99%占空比
 */
void PWM_SetCompare1(uint16_t Compare)
{
    TIM_SetCompare1(TIM1, Compare);
}

/**
 * @brief 设置TIM1通道4的PWM占空比
 * @param Compare PWM比较值，范围0-99，对应0%-99%占空比
 */
void PWM_SetCompare4(uint16_t Compare)
{
    TIM_SetCompare4(TIM1, Compare);
}