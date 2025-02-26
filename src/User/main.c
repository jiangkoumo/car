#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "Keys.h"
#include "Track.h"
#include "Menu.h"
#include "OLED.h" 
#include "Sensor.h"

int main(void)
{
	LED_Init();
	KEY_Init();
	OLED_Init();

	int menu2;
	uint8_t Sen[6]; 


	// 初始化寻迹模块
	Track_Init();
    
	// 设置默认速度
	Track_SetDefaultSpeed(400); // 设置为中等速度


	while (1)
	{
		menu2 = Menu1();
		if(menu2 == 1)
		{
			while(1)
			{
				OLED_ShowString(0, 0, "AD0:", OLED_8X16);
				OLED_ShowString(0, 16, "AD1:", OLED_8X16);
				OLED_ShowString(0, 32, "AD2:", OLED_8X16);
				OLED_ShowString(0, 48, "AD3:", OLED_8X16);
				OLED_ShowString(66, 0, "AD4:", OLED_8X16);
				OLED_ShowString(66, 16, "AD5:", OLED_8X16);
				
				Sen[0] = !Read_Senors_0();
				Sen[1] = !Read_Senors_1();
				Sen[2] = !Read_Senors_2();
				Sen[3] = !Read_Senors_3();
				Sen[4] = !Read_Senors_4();
				Sen[5] = !Read_Senors_5();
				
				OLED_ShowNum(33, 0, Sen[0], 1, OLED_8X16);			
				OLED_ShowNum(33, 16, Sen[1], 1, OLED_8X16);
				OLED_ShowNum(33, 32, Sen[2], 1, OLED_8X16);
				OLED_ShowNum(33, 48, Sen[3], 1, OLED_8X16);
				OLED_ShowNum(96, 0, Sen[4], 1, OLED_8X16);
				OLED_ShowNum(96, 16, Sen[5], 1, OLED_8X16);
				OLED_Update();
				
				Track_Run();
			}
		}

		if(menu2 == 2)
		{
			Menu2_Motor();
		}
	}
	
}