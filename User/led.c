#include"led.h"


u32 cnt=1000;
extern u8 flag_KeyPA15_PE14;

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//ø™∆Ù ±÷”
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	//≈‰÷√’ÎΩ≈
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStruct);
}

void Led_High(void)
{
	GPIO_ResetBits(GPIOE,GPIO_Pin_14);
}

void Led_Low(void)
{
		GPIO_SetBits(GPIOE,GPIO_Pin_14);
}


void Led_Run(void)
{
	static u8 i=0;
	switch(i)
	{
		case 0:
			if(cnt==0)
			{
				Led_High();
				i=1;
				cnt=1000;
			}break;
		case 1:
			if(cnt==0)
			{
				Led_Low();
				i=0;
				cnt=1000;
			}break;
	}
}



void Led_Change(void)
{
	static u8 i=0;
	if(flag_KeyPA15_PE14 == 1)
	{
		flag_KeyPA15_PE14 = 0;
		switch(i)
		{
			case 0:PEout(14) = 0; i = 1; break;
			case 1:PEout(14) = 1; i = 0 ;break;
		}
		while(!PAin(15));
	}
}

