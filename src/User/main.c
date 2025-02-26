#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "Keys.h"
#include "Track.h"

int main(void)
{
	LED_Init();
	KEY_Init();
	// 初始化寻迹模块
	Track_Init();
    
	// 设置默认速度(可选)
	Track_SetDefaultSpeed(40); // 设置为中等速度
	

	while (1)
	{
		if (KEY_Scan() == KEY1_PRESS)
		{
			LED1_Turn();
			LED2_Turn();
		}
		// 执行寻迹
		Track_Run();

	}
	
}