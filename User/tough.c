#include"tough.h"

u8 flag_KeyPA15_PE14 = 0;

void Tough_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//ø™∆Ù ±÷”
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//≈‰÷√’ÎΩ≈
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}


void Key_PA15_Scan(void)
{
	if(PAin(15)==0)
		{
			Delay_ms(500);
			if(PAin(15)==0)
			{
				flag_KeyPA15_PE14 = 1;
			}
			//while(!PAin(15));
		}
}
