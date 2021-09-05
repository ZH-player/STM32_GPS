#ifndef __USART1_H__
#define __USART1_H__

#include"sys.h"

extern char USART_ReceiveString[50];								//接收PC端发送过来的字符
extern int Receive_Flag;


#define USART1_GPIO_PORT GPIOA
#define USART1_TX GPIO_Pin_9
#define USART1_RX GPIO_Pin_10



void USART1_Init(void);
void USART_SendByte(USART_TypeDef* USARTx, uint16_t Data);
void USART_SendString(USART_TypeDef* USARTx,char *str);

uint16_t USART_ReceiveByte(USART_TypeDef* USARTx);
void USART1_IRQHandler(USART_TypeDef* USARTx);


#endif
