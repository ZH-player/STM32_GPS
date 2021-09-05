/****************
1、串口时钟、GPIOA时钟使能
2、GPIOA端口模式设置
3、串口参数初始化
4、开启中断并初始化NVIC
5、使能串口
6、编写中断处理函数
****************/
#include"usart1.h"
#include <string.h>




void USART1_Init(void)
{
	//1、串口时钟、GPIOA时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);

	
	//2、GPIOA端口模式设置
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = USART1_TX;//PA9
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART1_GPIO_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = USART1_RX;//PA10
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART1_GPIO_PORT,&GPIO_InitStruct);
	
	
	//3、串口参数初始化
	USART_InitTypeDef USART1_InitStruct;
	USART1_InitStruct.USART_BaudRate = 9600;
	USART1_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART1_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART1_InitStruct.USART_Parity = USART_Parity_No;
	USART1_InitStruct.USART_StopBits = USART_StopBits_1;
	USART1_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART1_InitStruct);
	
	
	//4、开启中断并初始化NVIC
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	
	//5、使能串口
	USART_Cmd(USART1, ENABLE);
}
//






char USART_ReceiveString[50];													//接收PC端发送过来的字符
int Receive_Flag = 0;															//接收消息标志位
int Receive_sum = 0;
//6、编写中断处理函数
//USART_ReceiveString
void USART_IRQHandler(USART_TypeDef* USARTx)
{
	if(USART_GetITStatus(USARTx,USART_IT_RXNE) == SET)
	{
		if(Receive_sum > 49)													//数组能存放50个字节的数据				
		{
			USART_ReceiveString[49] = '\0';										//数据字节超过50位时，将最后一位设置为\0	
			Receive_Flag = 1;													//接收标志位置1，停止接收数据
			Receive_sum = 0;													//数组下标置0
		}
		
		if(Receive_Flag == 0)													//接收标志位等于0，开始接收数据
		{
			USART_ReceiveString[Receive_sum] = USART_ReceiveData(USARTx);		//通过USART1串口接收字符
			Receive_sum++;														//数组下标++
		}
		if(Receive_sum >= 2)													//数组下标大于2
		{
			if(USART_ReceiveString[Receive_sum-2] == '\r' && USART_ReceiveString[Receive_sum-1] == '\n' )
			{
				USART_ReceiveString[Receive_sum-1] = '\0';						
				USART_ReceiveString[Receive_sum-2] = '\0';
				Receive_Flag = 1;												//接收标志位置1，停止接收数据
				Receive_sum = 0;	
			}
		}
		USART_ClearITPendingBit(USARTx,USART_IT_RXNE);
	}
}
//




//
void USART_SendByte(USART_TypeDef* USARTx, uint16_t Data)
{
	USART_SendData(USARTx, Data);
	
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);
}
//



//
void USART_SendString(USART_TypeDef* USARTx,char *str)
{
	while(*str != '\0')
	{
		USART_SendByte(USARTx, *str++);
	}
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);
}
//


//
uint16_t USART_ReceiveByte(USART_TypeDef* USARTx)
{
	while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE)==RESET);
	
	return USART_ReceiveData( USARTx);
}
//




//
#pragma import(__use_no_semihosting)             
                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;          
void _sys_exit(int x) 
{ 
	x = x; 
} 

void _ttywrch(int ch)
{
ch = ch;
}

int fputc(int ch, FILE *f)
{
		
		USART_SendData(USART1, (uint8_t) ch);			
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);			
		return (ch);
}


int fgetc(FILE *f)
{		
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
		return (int)USART_ReceiveData(USART1);
}
























