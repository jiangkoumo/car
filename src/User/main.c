#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "Keys.h"

int main(void)
{
	LED_Init();
	KEY_Init();

	while (1)
	{
		if (KEY_Scan() == KEY1_PRESS)
		{
			LED1_Turn();
			LED2_Turn();
		}
	}
	
}