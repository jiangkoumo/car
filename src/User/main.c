/**
 * @file main.c
 * @brief 智能小车主程序
 * @details 实现了小车的基本功能控制，包括寻迹模式和电机测试模式
 */
#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "Keys.h"
#include "Track.h"
#include "Menu.h"
#include "OLED.h" 
#include "Sensor.h"

/**
 * @brief 主函数
 * @return 正常情况下不会返回
 */
int main(void)
{
	// 初始化硬件模块
	LED_Init();    // LED指示灯初始化
	KEY_Init();    // 按键初始化
	OLED_Init();   // OLED显示屏初始化

	int menu2;     // 菜单选择结果变量
	uint8_t Sen[6]; // 存储6个传感器的状态数组


	// 初始化寻迹模块
	Track_Init();
    
	// 设置默认速度
	Track_SetDefaultSpeed(700); // 设置为中等速度


	while (1) // 主循环
	{
		menu2 = Menu1(); // 显示主菜单并获取用户选择
		if(menu2 == 1)   // 如果选择了选项1(寻迹模式)
		{
			while(1)     // 寻迹模式循环
			{
				// 在OLED上显示传感器标签
				OLED_ShowString(0, 0, "AD0:", OLED_8X16);
				OLED_ShowString(0, 16, "AD1:", OLED_8X16);
				OLED_ShowString(0, 32, "AD2:", OLED_8X16);
				OLED_ShowString(0, 48, "AD3:", OLED_8X16);
				OLED_ShowString(66, 0, "AD4:", OLED_8X16);
				OLED_ShowString(66, 16, "AD5:", OLED_8X16);
				
				// 读取各个传感器的状态
				// 对传感器数值取反(!)是为了让黑线显示为1，更直观
				Sen[0] = !Read_Senors_0(); // 读取传感器0的状态
				Sen[1] = !Read_Senors_1(); // 读取传感器1的状态
				Sen[2] = !Read_Senors_2(); // 读取传感器2的状态
				Sen[3] = !Read_Senors_3(); // 读取传感器3的状态
				Sen[4] = !Read_Senors_4(); // 读取传感器4的状态
				Sen[5] = !Read_Senors_5(); // 读取传感器5的状态
				
				// 在OLED上显示各传感器的数值
				OLED_ShowNum(33, 0, Sen[0], 1, OLED_8X16);			
				OLED_ShowNum(33, 16, Sen[1], 1, OLED_8X16);
				OLED_ShowNum(33, 32, Sen[2], 1, OLED_8X16);
				OLED_ShowNum(33, 48, Sen[3], 1, OLED_8X16);
				OLED_ShowNum(96, 0, Sen[4], 1, OLED_8X16);
				OLED_ShowNum(96, 16, Sen[5], 1, OLED_8X16);
				OLED_Update(); // 更新OLED显示内容
				
				Track_Run(); // 执行寻迹算法，控制小车运动
			}
		}

		if(menu2 == 2) // 如果选择了选项2(电机测试模式)
		{
			Menu2_Motor(); // 显示电机测试菜单并执行相应操作
		}

		if(menu2 == 3) // 如果选择了选项3(流水灯控制)
		{
			Menu2_LED(); // 显示流水灯控制菜单并执行相应操作
		}
	}
	
}