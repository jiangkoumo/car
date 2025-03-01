/**
 * @file Track.c
 * @brief 小车寻迹模块实现
 * @details 基于多传感器状态的小车寻迹算法
 * @encoding UTF-8
 */
#include "stm32f10x.h"
#include "Track.h"
#include "Motor.h"
#include "Sensor.h"
#include "Delay.h"

// 默认速度设置
#define DEFAULT_SPEED 850      // 默认前进速度
#define TURN_SPEED_LOW 320     // 轻微转弯速度
#define TURN_SPEED_MID 400    // 中等转弯速度
#define TURN_SPEED_HIGH 620   // 急转弯速度 

#define STOP_DELAY_MS 500     //第二次全黑延时时间


// 全局变量
static uint16_t g_defaultSpeed = DEFAULT_SPEED;  // 可调整的默认速度
static uint8_t full_black_CNT = 0;			//全黑区域计数器
static uint8_t is_second_black = 0;			//第二次全黑标志位
static uint32_t stop_timer = 0;				//停止计数器

static uint8_t lastSensorState = 0;
static uint8_t stableStateCounter = 0;

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
  * @param  speed: 默认速度值(0-1000)
  * @retval 无
  */
void Track_SetDefaultSpeed(uint16_t speed)
{
    if(speed <= 1000)
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

uint8_t Track_GetStableSensorState(void)
{
    uint8_t currentState = Track_GetSensorState();
    
    if(currentState == lastSensorState)
    {
        if(++stableStateCounter >= 2) // 需要连续2次相同状态才确认
        {
            return currentState;
        }
    }
    else
    {
        lastSensorState = currentState;
        stableStateCounter = 0;
    }
    
    return lastSensorState; // 返回上一次的状态
}

/**
 * @brief 执行寻迹控制函数
 * @param 无
 * @retval 无
 * @note 根据传感器状态调整电机速度实现寻迹功能
 */
void Track_Run(void)
{
    // 获取当前传感器状态
    uint8_t sensorState = Track_GetSensorState();
    
    // 处理全黑状态 (0x3F = 0b111111, 所有传感器都检测到黑线)
    if(sensorState == 0x3F)
    {
        if(full_black_CNT == 0)
        {
            // 第一次检测到全黑区域，保持直行
            Motor_LeftSetSpeed(DEFAULT_SPEED);
            Motor_RightSetSpeed(DEFAULT_SPEED);
            full_black_CNT = 1;  // 标记已经检测到第一次全黑
        }
        else if(full_black_CNT == 1 && !is_second_black)
        {
            // 检测到第二次全黑区域，启动停车流程
            is_second_black = 1;  // 设置第二次全黑标志
            stop_timer = 0;       // 重置停车计时器
        }
        else
        {
            // 处理其他全黑情况
            if(full_black_CNT == 1 && !is_second_black)
            {
                full_black_CNT = 0;  // 重置第一次全黑检测标志
            }
        }
        
        // 如果已经检测到第二次全黑区域，执行停车流程
        if(is_second_black)
        {
            if(++stop_timer > 500) // 500ms延时后停车
            {
                Motor_LeftSetSpeed(0);  // 左轮停止
                Motor_RightSetSpeed(0); // 右轮停止
                is_second_black = 0;    // 重置第二次全黑标志
                full_black_CNT = 0;     // 重置第一次全黑标志
                return;                 // 完全停止后退出
            }
            else
            {
                return;  // 延时期间继续直行
            }
        }
    }
    
    // 处理中间两个传感器检测到线的情况(直线前进)
    if(sensorState == 0x0C) // 0b001100
    {
        Motor_LeftSetSpeed(g_defaultSpeed);   // 左轮正常速度
        Motor_RightSetSpeed(g_defaultSpeed);  // 右轮正常速度
    }
    
    // 处理轻微左偏情况 - 传感器1和2检测到线
    else if(sensorState == 0x06) // 0b000110
    {
        Motor_LeftSetSpeed(g_defaultSpeed - TURN_SPEED_LOW); // 左轮减速
        Motor_RightSetSpeed(g_defaultSpeed);                 // 右轮正常速度
    }
    
    // 处理轻微右偏情况 - 传感器3和4检测到线
    else if(sensorState == 0x18) // 0b011000
    {
        Motor_LeftSetSpeed(g_defaultSpeed);                  // 左轮正常速度
        Motor_RightSetSpeed(g_defaultSpeed - TURN_SPEED_LOW); // 右轮减速
    }
    
    // 处理中度左偏情况 - 只有传感器1检测到线
    else if(sensorState == 0x02) // 0b000010
    {
        Motor_LeftSetSpeed(g_defaultSpeed - TURN_SPEED_MID); // 左轮中度减速
        Motor_RightSetSpeed(g_defaultSpeed);                 // 右轮正常速度
    }
    
    // 处理中度右偏情况 - 只有传感器4检测到线
    else if(sensorState == 0x10) // 0b010000
    {
        Motor_LeftSetSpeed(g_defaultSpeed);                  // 左轮正常速度
        Motor_RightSetSpeed(g_defaultSpeed - TURN_SPEED_MID); // 右轮中度减速
    }
    
    // 处理急左转情况 - 只有最左侧传感器0检测到线
    else if(sensorState == 0x01) // 0b000001
    {
        Motor_LeftSetSpeed(g_defaultSpeed - TURN_SPEED_HIGH);  // 左轮大幅减速
        Motor_RightSetSpeed(g_defaultSpeed + TURN_SPEED_HIGH); // 右轮大幅加速
    }
    
    // 处理急右转情况 - 只有最右侧传感器5检测到线
    else if(sensorState == 0x20) // 0b100000
    {
        Motor_LeftSetSpeed(g_defaultSpeed + TURN_SPEED_HIGH);  // 左轮大幅加速
        Motor_RightSetSpeed(g_defaultSpeed - TURN_SPEED_HIGH); // 右轮大幅减速
    }
    
    // 处理全白情况 - 所有传感器都未检测到线
    else if(sensorState == 0x00) // 0b000000
    {
        Motor_RightSetSpeed(g_defaultSpeed - TURN_SPEED_LOW);  // 右轮轻微减速
        // Motor_back();  // 后退
        // Motor_stop();  // 停止
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
            int diff = leftCount - rightCount;
            if(diff >= 2) {
                // 差距大，大幅度转向
                Motor_LeftSetSpeed(g_defaultSpeed - TURN_SPEED_HIGH);
                Motor_RightSetSpeed(g_defaultSpeed + TURN_SPEED_MID);
            } else {
                // 差距小，轻微转向
                Motor_LeftSetSpeed(g_defaultSpeed - TURN_SPEED_MID);
                Motor_RightSetSpeed(g_defaultSpeed);
            }
        }
        else if(rightCount > leftCount)
        {
            // 向右转 - 右侧传感器检测到的线更多
            int diff = rightCount - leftCount;
            if(diff >= 2) {
                // 差距大，大幅度转向
                Motor_LeftSetSpeed(g_defaultSpeed + TURN_SPEED_MID);
                Motor_RightSetSpeed(g_defaultSpeed - TURN_SPEED_HIGH);
            } else {
                // 差距小，轻微转向
                Motor_LeftSetSpeed(g_defaultSpeed);
                Motor_RightSetSpeed(g_defaultSpeed - TURN_SPEED_MID);
            }
        }
        else
        {
            // 左右平衡，直行
            Motor_LeftSetSpeed(g_defaultSpeed);
            Motor_RightSetSpeed(g_defaultSpeed);
        }
    }
    
    // 添加一个短暂延时，避免过于频繁的调整
    delay_ms(20);
}

// 添加加速缓冲函数
// void Motor_SmoothAccelerate(uint16_t targetSpeed)
// {
//     uint16_t currentSpeed = 0;
//     while(currentSpeed < targetSpeed)
//     {
//         currentSpeed += 50;  // 每次增加50的速度
//         if(currentSpeed > targetSpeed) currentSpeed = targetSpeed;
        
//         Motor_LeftSetSpeed(currentSpeed);
//         Motor_RightSetSpeed(currentSpeed);
//         delay_ms(20);  // 每次增速后短暂等待
//     }
// }
