#include "stm32f10x.h"
#include "Track.h"
#include "Motor.h"
#include "Sensor.h"
#include "Delay.h"

// 默认速度设置
#define DEFAULT_SPEED 50      // 默认前进速度
#define TURN_SPEED_LOW 30     // 轻微转弯速度
#define TURN_SPEED_MID 40     // 中等转弯速度
#define TURN_SPEED_HIGH 50    // 急转弯速度

// 全局变量
static uint16_t g_defaultSpeed = DEFAULT_SPEED;  // 可调整的默认速度

/**
  * @brief  初始化寻迹模块
  * @param  无
  * @retval 无
  */
void Track_Init(void)
{
    // 初始化传感器
    Sensor_Init();
    
    // 初始化电机
    Motor_Init();    
    // 默认先停车
    Motor_LeftSetSpeed(0);
    Motor_RightSetSpeed(0);
}

/**
  * @brief  设置默认速度
  * @param  speed: 默认速度值(0-100)
  * @retval 无
  */
void Track_SetDefaultSpeed(uint16_t speed)
{
    if(speed <= 100)
    {
        g_defaultSpeed = speed;
    }
}

/**
  * @brief  获取所有传感器状态
  * @param  无
  * @retval 传感器状态码(6位二进制，每位代表一个传感器)
  *         0表示未检测到线(白色)，1表示检测到线(黑色)
  */
uint8_t Track_GetSensorState(void)
{
    uint8_t state = 0;
    
    // 读取所有传感器状态并合并为一个6位数
    // 注意：GPIO_ReadInputDataBit的返回值可能需要取反，取决于传感器类型
    // 假设0表示检测到黑线，1表示检测到白色
    // 这里取反使得：1表示检测到黑线，0表示检测到白色
    state |= (!Read_Senors_0()) << 0;
    state |= (!Read_Senors_1()) << 1;
    state |= (!Read_Senors_2()) << 2;
    state |= (!Read_Senors_3()) << 3;
    state |= (!Read_Senors_4()) << 4;
    state |= (!Read_Senors_5()) << 5;
    
    return state;
}

/**
  * @brief  执行寻迹控制
  * @param  无
  * @retval 无
  */
void Track_Run(void)
{
    uint8_t sensorState = Track_GetSensorState();
    
    // 根据传感器状态调整电机速度
    // 0b000000 表示所有传感器都没有检测到线
    // 0b111111 表示所有传感器都检测到线
    
    // 理想情况下，中间的传感器(2和3)位于线上
    // 判断小车位置并调整速度
    
    // 没有检测到线或全部检测到线，保持上一个状态
    if(sensorState == 0x00 || sensorState == 0x3F)
    {
        // 可以选择停止或保持上一次的动作
        // 这里选择停止
        Motor_LeftSetSpeed(0);
        Motor_RightSetSpeed(0);
        return;
    }
    
    // 直线前进 - 中间两个传感器检测到线
    if(sensorState == 0x0C) // 0b001100
    {
        Motor_LeftSetSpeed(g_defaultSpeed);
        Motor_RightSetSpeed(g_defaultSpeed);
    }
    // 轻微左偏 - 传感器2和传感器1检测到线
    else if(sensorState == 0x06) // 0b000110
    {
        Motor_LeftSetSpeed(g_defaultSpeed - TURN_SPEED_LOW);
        Motor_RightSetSpeed(g_defaultSpeed);
    }
    // 轻微右偏 - 传感器3和传感器4检测到线
    else if(sensorState == 0x18) // 0b011000
    {
        Motor_LeftSetSpeed(g_defaultSpeed);
        Motor_RightSetSpeed(g_defaultSpeed - TURN_SPEED_LOW);
    }
    // 左偏 - 只有传感器1检测到线
    else if(sensorState == 0x02) // 0b000010
    {
        Motor_LeftSetSpeed(g_defaultSpeed - TURN_SPEED_MID);
        Motor_RightSetSpeed(g_defaultSpeed);
    }
    // 右偏 - 只有传感器4检测到线
    else if(sensorState == 0x10) // 0b010000
    {
        Motor_LeftSetSpeed(g_defaultSpeed);
        Motor_RightSetSpeed(g_defaultSpeed - TURN_SPEED_MID);
    }
    // 急左偏 - 只有传感器0检测到线
    else if(sensorState == 0x01) // 0b000001
    {
        Motor_LeftSetSpeed(g_defaultSpeed - TURN_SPEED_HIGH);
        Motor_RightSetSpeed(g_defaultSpeed + TURN_SPEED_LOW);
    }
    // 急右偏 - 只有传感器5检测到线
    else if(sensorState == 0x20) // 0b100000
    {
        Motor_LeftSetSpeed(g_defaultSpeed + TURN_SPEED_LOW);
        Motor_RightSetSpeed(g_defaultSpeed - TURN_SPEED_HIGH);
    }
    // 其他情况 - 根据左右传感器数量决定转向
    else
    {
        // 计算左侧和右侧传感器的数量
        uint8_t leftCount = 0, rightCount = 0;
        
        // 计算左侧传感器检测到线的数量(传感器0、1、2)
        if(sensorState & 0x01) leftCount++;
        if(sensorState & 0x02) leftCount++;
        if(sensorState & 0x04) leftCount++;
        
        // 计算右侧传感器检测到线的数量(传感器3、4、5)
        if(sensorState & 0x08) rightCount++;
        if(sensorState & 0x10) rightCount++;
        if(sensorState & 0x20) rightCount++;
        
        // 根据左右传感器数量决定转向
        if(leftCount > rightCount)
        {
            // 向左转 - 左侧传感器检测到的线更多
            Motor_LeftSetSpeed(g_defaultSpeed - TURN_SPEED_MID);
            Motor_RightSetSpeed(g_defaultSpeed);
        }
        else if(rightCount > leftCount)
        {
            // 向右转 - 右侧传感器检测到的线更多
            Motor_LeftSetSpeed(g_defaultSpeed);
            Motor_RightSetSpeed(g_defaultSpeed - TURN_SPEED_MID);
        }
        else
        {
            // 左右平衡，直行
            Motor_LeftSetSpeed(g_defaultSpeed);
            Motor_RightSetSpeed(g_defaultSpeed);
        }
    }
    
    // 添加一个短暂延时，避免过于频繁的调整
    delay_ms(10);
}
