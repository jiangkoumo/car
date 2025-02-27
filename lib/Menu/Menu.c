/**
 * @file Menu.c
 * @brief 智能小车菜单系统实现
 * @details 实现了主菜单和电机控制子菜单的显示与交互功能
 */
#include "stm32f10x.h"              
#include "Keys.h"
#include "OLED.h"
#include "Menu.h"
#include "Motor.h"

uint8_t Key_Num; // 按键编号变量，用于存储当前按下的按键

/**
 * @brief 主菜单函数
 * @return 返回用户选择的菜单项编号
 * @details 显示主菜单并处理用户按键输入，返回选择的功能
 */
int Menu1(void)
{
	KEY_Init(); // 初始化按键模块
	
	uint8_t flag = 1; // 菜单选择标志，初始位置为第一项
	
	// 显示主菜单选项
	OLED_ShowString(0,0,"循迹                  ",OLED_8X16);
	OLED_ShowString(0,16,"电机控制             ",OLED_8X16);
	OLED_Update(); // 更新OLED显示
	
	while(1)
	{
		Key_Num = KEY_Scan(); // 扫描按键输入
		if(Key_Num == KEY3_PRESS) // 如果按下KEY3(向上)
		{
			flag--; // 菜单选择上移
			if(flag == 0){flag = 6;} // 菜单循环选择
		}
		if(Key_Num == KEY2_PRESS) // 如果按下KEY2(向下)
		{
			flag++; // 菜单选择下移
			if(flag == 7){flag = 1;} // 菜单循环选择
		}
		if(Key_Num == KEY1_PRESS) // 如果按下KEY1(确认)
		{
			OLED_Clear(); // 清除OLED显示
			OLED_Update(); // 更新OLED显示
			return flag; // 返回选择的菜单项
		}	
		
		// 根据菜单选择标志显示高亮菜单项
		switch(flag)
		{
			case 1: // 选择"循迹"菜单项
			{
				OLED_ShowString(0,0,"循迹                  ",OLED_8X16);
				OLED_ShowString(0,16,"电机控制             ",OLED_8X16);
				OLED_ReverseArea(0,0,128,16); // 反转显示区域，高亮显示
				OLED_Update(); // 更新OLED显示
				break;
			}
			case 2: // 选择"电机控制"菜单项
			{
				OLED_ShowString(0,0,"循迹                  ",OLED_8X16);
				OLED_ShowString(0,16,"电机控制             ",OLED_8X16);
				OLED_ReverseArea(0,16,128,16); // 反转显示区域，高亮显示
				OLED_Update(); // 更新OLED显示
				break;
			}
		}
	}
}

/**
 * @brief 电机控制子菜单函数
 * @return 返回子菜单选择结果
 * @details 显示电机控制菜单，并根据用户选择执行相应的电机控制命令
 */
int Menu2_Motor(void)
{
	int menu3_Motor = 0; // 菜单选择结果
	uint8_t moflag = 1; // 菜单选择标志，初始位置为返回选项
	
	// 显示电机控制菜单选项
	OLED_ShowString(0,0,"<-               ",OLED_8X16); // 返回选项
	OLED_ShowString(0,16,"停止                  ",OLED_8X16);
	OLED_ShowString(0,32,"前进                  ",OLED_8X16);
	OLED_ShowString(0,48,"后退                  ",OLED_8X16);
	OLED_Update(); // 更新OLED显示
	
	while(1)
	{
		Key_Num = KEY_Scan(); // 扫描按键输入
		if(Key_Num == KEY3_PRESS) // 如果按下KEY3(向上)
		{
			moflag--; // 菜单选择上移
			if(moflag == 0){moflag = 6;} // 菜单循环选择
		}
		if(Key_Num == KEY2_PRESS) // 如果按下KEY2(向下)
		{
			moflag++; // 菜单选择下移
			if(moflag == 7){moflag = 1;} // 菜单循环选择
		}
		if(Key_Num == KEY1_PRESS) // 如果按下KEY1(确认)
		{
			OLED_Clear(); // 清除OLED显示
			OLED_Update(); // 更新OLED显示
			menu3_Motor = moflag; // 保存菜单选择结果
		}
		
		// 根据菜单选择执行相应的电机控制命令
		if(menu3_Motor == 1){return 0;} // 返回上一级菜单
		if(menu3_Motor == 2){Motor_stop();} // 执行电机停止命令
		if(menu3_Motor == 3){Motor_go();} // 执行电机前进命令
		if(menu3_Motor == 4){Motor_back();} // 执行电机后退命令
		if(menu3_Motor == 5){Turn_left();} // 执行左转命令
		if(menu3_Motor == 6){Turn_right();} // 执行右转命令
		
		// 根据菜单选择标志显示高亮菜单项
		switch(moflag)
		{
			case 1: // 选择返回选项
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"停止                  ",OLED_8X16);
				OLED_ShowString(0,32,"前进                  ",OLED_8X16);
				OLED_ShowString(0,48,"后退                  ",OLED_8X16);
				OLED_ReverseArea(0,0,128,16); // 反转显示区域，高亮显示
				OLED_Update(); // 更新OLED显示
				break;
			}
			
			case 2: // 选择停止选项
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"停止                  ",OLED_8X16);
				OLED_ShowString(0,32,"前进                  ",OLED_8X16);
				OLED_ShowString(0,48,"后退                  ",OLED_8X16);
				OLED_ReverseArea(0,16,128,16); // 反转显示区域，高亮显示
				OLED_Update(); // 更新OLED显示
				break;
			}
			
			case 3: // 选择前进选项
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"停止                  ",OLED_8X16);
				OLED_ShowString(0,32,"前进                  ",OLED_8X16);
				OLED_ShowString(0,48,"后退                  ",OLED_8X16);
				OLED_ReverseArea(0,32,128,16); // 反转显示区域，高亮显示
				OLED_Update(); // 更新OLED显示
				break;
			}
			
			case 4: // 选择后退选项
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"停止                  ",OLED_8X16);
				OLED_ShowString(0,32,"前进                  ",OLED_8X16);
				OLED_ShowString(0,48,"后退                  ",OLED_8X16);
				OLED_ReverseArea(0,48,128,16); // 反转显示区域，高亮显示
				OLED_Update(); // 更新OLED显示
				break;
			}
			
			case 5: // 选择左转选项
			{
				OLED_ShowString(0,0,"左转                  ",OLED_8X16);
				OLED_ShowString(0,16,"右转                  ",OLED_8X16);
				OLED_ShowString(0,32,"                    ",OLED_8X16);
				OLED_ShowString(0,48,"                    ",OLED_8X16);
				OLED_ReverseArea(0,0,128,16); // 反转显示区域，高亮显示
				OLED_Update(); // 更新OLED显示
				break;
			}
			
			case 6: // 选择右转选项
			{
				OLED_ShowString(0,0,"左转                  ",OLED_8X16);
				OLED_ShowString(0,16,"右转                  ",OLED_8X16);
				OLED_ShowString(0,32,"                    ",OLED_8X16);
				OLED_ShowString(0,48,"                    ",OLED_8X16);
				OLED_ReverseArea(0,16,128,16); // 反转显示区域，高亮显示
				OLED_Update(); // 更新OLED显示
				break;				
			}
		}
	}
}
