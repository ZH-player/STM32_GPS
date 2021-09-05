#include"delay.h"

static uint8_t fac_us=0;  //us��ʱ������
static uint16_t fac_ms=0; //ms��ʱ������


void Delay_Init()
{
	((SysTick_Type*)(0xE000e010))->CTRL &=~(1<<2);//1��ȷ��ʱ��Դ��ϵͳʱ�ӵ�8��Ƶ��
	fac_us=SystemCoreClock/8000000;  //����Ϊϵͳʱ�ӵ�1/8
    fac_ms=(u16)fac_us*1000;
}

void Delay_ms(u16 nms)
{
	u32 temp;
	((SysTick_Type*)(0xE000e010))->LOAD = (u32)nms*fac_ms;//2����ֵ��װ
	((SysTick_Type*)(0xE000e010))->VAL = 0x00;//3����ռ�����
	((SysTick_Type*)(0xE000e010))->CTRL |= (1<<0);//4������������
	do{
		temp=((SysTick_Type*)(0xE000e010))->CTRL;
	}while((temp&(1<<0))&&!(temp&(1<<16)));//5����ѯ�Ƿ�0
	((SysTick_Type*)(0xE000e010))->CTRL &=~(1<<0);//6���رռ�����
	((SysTick_Type*)(0xE000e010))->VAL = 0x00;//7����ռ���������ʡ�ԣ�
}
