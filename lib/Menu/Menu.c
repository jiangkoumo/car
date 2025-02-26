#include "stm32f10x.h"              
#include "Keys.h"
#include "OLED.h"
#include "Menu.h"
#include "Motor.h"

uint8_t Key_Num;

int Menu1(void)
{
	KEY_Init();
	
	uint8_t flag = 1;
	
	OLED_ShowString(0,0,"循迹                  ",OLED_8X16);
	OLED_ShowString(0,16,"电机控制             ",OLED_8X16);
	OLED_Update();
	
	while(1)
	{
		Key_Num = KEY_Scan();
		if(Key_Num == KEY3_PRESS)
		{
			flag--;
			if(flag == 0){flag = 6;}
		}
		if(Key_Num == KEY2_PRESS)
		{
			flag++;
			if(flag == 7){flag = 1;}
		}
		if(Key_Num == KEY1_PRESS)
		{
			OLED_Clear();
			OLED_Update();
			return flag;
		}	
		
		
		switch(flag)
		{
			case 1:
			{
				OLED_ShowString(0,0,"循迹                  ",OLED_8X16);
				OLED_ShowString(0,16,"电机控制             ",OLED_8X16);
				OLED_ReverseArea(0,0,128,16);
				OLED_Update();
				break;
			}
			case 2:
			{
				OLED_ShowString(0,0,"循迹                  ",OLED_8X16);
				OLED_ShowString(0,16,"电机控制             ",OLED_8X16);
				OLED_ReverseArea(0,16,128,16);
				OLED_Update();
				break;
			}
		}
	}
}

	
	
	
int Menu2_Motor(void)
{
	int menu3_Motor = 0;
	uint8_t moflag = 1;
	OLED_ShowString(0,0,"<-               ",OLED_8X16);
	OLED_ShowString(0,16,"停止                  ",OLED_8X16);
	OLED_ShowString(0,32,"前进                  ",OLED_8X16);
	OLED_ShowString(0,48,"后退                  ",OLED_8X16);
	OLED_Update();
	while(1)
	{
		Key_Num = KEY_Scan();
		if(Key_Num == KEY3_PRESS)
		{
			moflag--;
			if(moflag == 0){moflag = 6;}
		}
		if(Key_Num == KEY2_PRESS)
		{
			moflag++;
			if(moflag == 7){moflag = 1;}
		}
		if(Key_Num == KEY1_PRESS)
		{
			OLED_Clear();
			OLED_Update();
			menu3_Motor = moflag;
		}
		if(menu3_Motor == 1){return 0;}
		if(menu3_Motor == 2){Motor_stop();}
		if(menu3_Motor == 3){Motor_go();}
		if(menu3_Motor == 4){Motor_back();}
		if(menu3_Motor == 5){Turn_left();}
		if(menu3_Motor == 6){Turn_right();}
		
		switch(moflag)
		{
			
			case 1:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"停止                  ",OLED_8X16);
				OLED_ShowString(0,32,"前进                  ",OLED_8X16);
				OLED_ShowString(0,48,"后退                  ",OLED_8X16);
				OLED_ReverseArea(0,0,128,16);
				OLED_Update();
				break;
			}
			
			case 2:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"停止                  ",OLED_8X16);
				OLED_ShowString(0,32,"前进                  ",OLED_8X16);
				OLED_ShowString(0,48,"后退                  ",OLED_8X16);
				OLED_ReverseArea(0,16,128,16);
				OLED_Update();
				break;
			}
			
			case 3:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"停止                  ",OLED_8X16);
				OLED_ShowString(0,32,"前进                  ",OLED_8X16);
				OLED_ShowString(0,48,"后退                  ",OLED_8X16);
				OLED_ReverseArea(0,32,128,16);
				OLED_Update();
				break;
			}
			
			case 4:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"停止                  ",OLED_8X16);
				OLED_ShowString(0,32,"前进                  ",OLED_8X16);
				OLED_ShowString(0,48,"后退                  ",OLED_8X16);
				OLED_ReverseArea(0,48,128,16);
				OLED_Update();
				break;
			}
			
			case 5:
			{
				OLED_ShowString(0,0,"左转                  ",OLED_8X16);
				OLED_ShowString(0,16,"右转                  ",OLED_8X16);
				OLED_ShowString(0,32,"                    ",OLED_8X16);
				OLED_ShowString(0,48,"                    ",OLED_8X16);
				OLED_ReverseArea(0,0,128,16);
				OLED_Update();
				break;
			}
			
			case 6:
			{
				OLED_ShowString(0,0,"左转                  ",OLED_8X16);
				OLED_ShowString(0,16,"右转                  ",OLED_8X16);
				OLED_ShowString(0,32,"                    ",OLED_8X16);
				OLED_ShowString(0,48,"                    ",OLED_8X16);
				OLED_ReverseArea(0,16,128,16);
				OLED_Update();
				break;				
			}
		}
	}
}
