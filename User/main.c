#include"sys.h"
extern struct basicData GPSData;


int main()
{
	
	USART1_Init();
	printf("Hello Wrold!\r\n");
	USART3_Init();
	
	Delay_Init();
	
	while(1)
	{
		USART3_IRQHandler();
//		printf("%s\r\n",GPSData.GNRMC);
		parseGPSData(1);
		printfPosiInfo();
		Delay_ms(1000);
	}
	
	
	
}
