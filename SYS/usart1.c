/****************
1������ʱ�ӡ�GPIOAʱ��ʹ��
2��GPIOA�˿�ģʽ����
3�����ڲ�����ʼ��
4�������жϲ���ʼ��NVIC
5��ʹ�ܴ���
6����д�жϴ�����
****************/
#include"usart1.h"
#include <string.h>




void USART1_Init(void)
{
	//1������ʱ�ӡ�GPIOAʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);

	
	//2��GPIOA�˿�ģʽ����
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = USART1_TX;//PA9
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART1_GPIO_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = USART1_RX;//PA10
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART1_GPIO_PORT,&GPIO_InitStruct);
	
	
	//3�����ڲ�����ʼ��
	USART_InitTypeDef USART1_InitStruct;
	USART1_InitStruct.USART_BaudRate = 9600;
	USART1_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART1_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART1_InitStruct.USART_Parity = USART_Parity_No;
	USART1_InitStruct.USART_StopBits = USART_StopBits_1;
	USART1_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART1_InitStruct);
	
	
	//4�������жϲ���ʼ��NVIC
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	
	//5��ʹ�ܴ���
	USART_Cmd(USART1, ENABLE);
}
//






char USART_ReceiveString[50];													//����PC�˷��͹������ַ�
int Receive_Flag = 0;															//������Ϣ��־λ
int Receive_sum = 0;
//6����д�жϴ�����
//USART_ReceiveString
void USART_IRQHandler(USART_TypeDef* USARTx)
{
	if(USART_GetITStatus(USARTx,USART_IT_RXNE) == SET)
	{
		if(Receive_sum > 49)													//�����ܴ��50���ֽڵ�����				
		{
			USART_ReceiveString[49] = '\0';										//�����ֽڳ���50λʱ�������һλ����Ϊ\0	
			Receive_Flag = 1;													//���ձ�־λ��1��ֹͣ��������
			Receive_sum = 0;													//�����±���0
		}
		
		if(Receive_Flag == 0)													//���ձ�־λ����0����ʼ��������
		{
			USART_ReceiveString[Receive_sum] = USART_ReceiveData(USARTx);		//ͨ��USART1���ڽ����ַ�
			Receive_sum++;														//�����±�++
		}
		if(Receive_sum >= 2)													//�����±����2
		{
			if(USART_ReceiveString[Receive_sum-2] == '\r' && USART_ReceiveString[Receive_sum-1] == '\n' )
			{
				USART_ReceiveString[Receive_sum-1] = '\0';						
				USART_ReceiveString[Receive_sum-2] = '\0';
				Receive_Flag = 1;												//���ձ�־λ��1��ֹͣ��������
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
























