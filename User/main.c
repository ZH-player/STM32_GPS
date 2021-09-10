#include"sys.h"
extern struct basicData GPSData;
extern struct position posiInfo;

int main()
{
	
	USART1_Init();
	printf("Hello Wrold!\r\n");
	USART3_Init();
	GPIO_Config();
	PAout(1) = 1;
	PEout(14) = 1;
	I2C_Configuration();
	Delay_Init();
	Delay_ms(1000);
	OLED_ON();
	while(1)
	{
		USART3_IRQHandler();
		parseGPSData(1);
		
		displayInfo();//OLED显示GPS信息
		
		printfPosiInfo();
		freeInfo(); //清空posiInfo中数据
		Delay_ms(1000);
		
	}
	
	
	
}
