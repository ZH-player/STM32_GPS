#include"delay.h"

static uint8_t fac_us=0;  //us延时倍乘数
static uint16_t fac_ms=0; //ms延时倍乘数


void Delay_Init()
{
	((SysTick_Type*)(0xE000e010))->CTRL &=~(1<<2);//1、确定时钟源（系统时钟的8分频）
	fac_us=SystemCoreClock/8000000;  //设置为系统时钟的1/8
    fac_ms=(u16)fac_us*1000;
}

void Delay_ms(u16 nms)
{
	u32 temp;
	((SysTick_Type*)(0xE000e010))->LOAD = (u32)nms*fac_ms;//2、数值重装
	((SysTick_Type*)(0xE000e010))->VAL = 0x00;//3、清空计数器
	((SysTick_Type*)(0xE000e010))->CTRL |= (1<<0);//4、启动计数器
	do{
		temp=((SysTick_Type*)(0xE000e010))->CTRL;
	}while((temp&(1<<0))&&!(temp&(1<<16)));//5、查询是否到0
	((SysTick_Type*)(0xE000e010))->CTRL &=~(1<<0);//6、关闭计数器
	((SysTick_Type*)(0xE000e010))->VAL = 0x00;//7、清空计数器（可省略）
}
