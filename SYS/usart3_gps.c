#include"usart3_gps.h"

extern struct basicData GPSData;


//初始化USART3（GPS模块接此串口）
void USART3_Init(void)
{


	//1、使能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//2、PGPIOB端口模式设置
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = USART3_TX;//PB10
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART3_GPIO_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = USART3_RX;//PAB11
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART3_GPIO_PORT,&GPIO_InitStruct);
	
	//3、串口参数初始化
	USART_InitTypeDef USART3_InitStruct;
	USART3_InitStruct.USART_BaudRate = 9600;
	USART3_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART3_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART3_InitStruct.USART_Parity = USART_Parity_No;
	USART3_InitStruct.USART_StopBits = USART_StopBits_1;
	USART3_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART3_InitStruct);
	
	//4、开启中断并初始化NVIC
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
	
	
	//5、使能串口
	USART_Cmd(USART3, ENABLE);
	
}


//串口3接收数据 存入Usart3Buf中
char Usart3Buf[200];
int Count;
void USART3_IRQHandler(void)
{
	char getByte;
	int i;
	if(SET==USART_GetITStatus(USART3,USART_IT_RXNE))
	{
		getByte = USART_ReceiveData(USART3);
		if(getByte == '$')
		{
			Count = 0;
		}
		Usart3Buf[Count++] = getByte;
		if(getByte == '\n')
		{
			i=GPSDataClass(Usart3Buf);
			switch(i)
			{
				case 0:break;
				case 1:memcpy(GPSData.GNRMC, Usart3Buf, Count);
					   break;
//				case 2:memcpy(GPSData.GNVTG, Usart3Buf, Count);
//					   break;
				default: break;
			}
			Count = 0;
			memset(Usart3Buf, 0, 200);
//			printf("%s\r\n",GPSData.GNRMC);//探针
//			printf("%s\r\n",GPSData.GNVTG);//探针
//			printf("%s\r\n",GPSData.GNVTG);//探针

			
//			parseGPSData(1);
//			printfPosiInfo();
		}
		
		if(Count >= 200)
		{
			Count = 200;
		}
		
	
	}
}
