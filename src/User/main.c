#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "keys.h"

int main(void)
{
	LED_Init();
	KEY_Init();
	uint8_t key = 0;

	while (1)
	{
		key = Key_GetNum();
		if (key == 1)
		{
			delay_ms(20);
			LED1_Turn();
			LED2_Turn();
		}
	}
	
}