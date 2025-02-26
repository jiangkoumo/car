#ifndef __TRACK_H__
#define __TRACK_H__


// 寻迹模块初始化
void Track_Init(void);

// 执行寻迹
void Track_Run(void);

// 传感器状态检测
uint8_t Track_GetSensorState(void);

// 设置默认速度
void Track_SetDefaultSpeed(uint16_t speed);

#endif // __TRACK_H__