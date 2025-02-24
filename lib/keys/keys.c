#include "stm32f10x.h"
#include "keys.h"
#include "delay.h"

/**
  * @brief  按键GPIO初始化函数
  * @note   初始化PB3/PB4/PB5为上拉输入模式
  * @param  无
  * @retval 无
  */
void KEY_Init(void)
{
    /* 使能GPIO时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//使能SWD


    /* 配置K1/K2/K3为上拉输入 */
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
  * @brief  多按键扫描函数
  * @note   采用连续多次采样的方式进行按键消抖，确保按键状态稳定
  *         - 每个按键使用独立的计数器记录持续按下状态
  *         - 需要连续4次检测到按下才认为是有效按压
  *         - 采样间隔为5ms，总消抖时间约20ms
  *         - 等待按键释放后才返回按键值，避免连续触发
  * @param  无
  * @retval KeyStatus: 
  *         - KEY_NONE: 无按键按下
  *         - KEY1_PRESS: K1按键按下并释放
  *         - KEY2_PRESS: K2按键按下并释放
  *         - KEY3_PRESS: K3按键按下并释放
  */
KeyStatus KEY_Scan(void)
{
    /* 定义静态计数器，记录每个按键的持续按下次数 */
    static uint8_t key1_count = 0;
    static uint8_t key2_count = 0;
    static uint8_t key3_count = 0;
    
    /* 检测K1按键（PB3）
     * Bit_RESET表示按键按下（低电平）
     * Bit_SET表示按键释放（上拉高电平）
     */
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == Bit_RESET) 
    {
        key1_count++;    // 按下计数器增加
        if(key1_count >= 4) 
        {  // 连续4次检测到按下，认为是有效按压
            key1_count = 0;     // 复位计数器
            /* 等待按键释放，防止连续触发 */
            while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == Bit_RESET);
            return KEY1_PRESS;  // 返回K1按键按下标志
        }
    } 
    else 
    {
        key1_count = 0;  // 一旦检测到释放，立即清零计数器
    }
    
    /* 检测K2按键（PB4）
     * 检测逻辑与K1完全相同
     */
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == Bit_RESET) 
    {
        key2_count++;
        if(key2_count >= 4) 
        {
            key2_count = 0;
            while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == Bit_RESET);
            return KEY2_PRESS;
        }
    } 
    else 
    {
        key2_count = Bit_RESET;
    }
    
    /* 检测K3按键（PB5）
     * 检测逻辑与K1完全相同
     */
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == Bit_RESET) 
    {
        key3_count++;
        if(key3_count >= 4) 
        {
            key3_count = 0;
            while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == Bit_RESET);
            return KEY3_PRESS;
        }
    } 
    else 
    {
        key3_count = 0;
    }
    
    /* 采样间隔延时
     * 降低CPU占用率，并为机械按键提供足够的机械回弹时间
     */
    delay_ms(5);
    
    /* 无按键按下时返回空状态 */
    return KEY_NONE;
}

// /**
//   * @brief  获取按键值
//   * @note   采用独立按键检测方式，每次只检测一个按键
//   * @param  无
//   * @retval uint8_t: 
//   *         - 0: 无按键按下
//   *         - 1: K1按键按下
//   *         - 2: K2按键按下
//   *         - 3: K3按键按下
//   */
// uint8_t Key_GetNum(void)
// {
//     uint8_t key = 0;
//     if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == Bit_RESET)
//     {
//         delay_ms(20);
//         while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == Bit_RESET);
//         delay_ms(20);
//         key = 1;
//     }

//     if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == Bit_RESET)
//     {
//         delay_ms(20);
//         while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == Bit_RESET);
//         delay_ms(20);
//         key = 2;
//     }

//     if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == Bit_RESET)
//     {
//         delay_ms(20);
//         while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == Bit_RESET);
//         delay_ms(20);
//         key = 3;
//     }

//     return key;
// }