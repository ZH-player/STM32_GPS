#ifndef __USART_GPS_H_
#define __USART_GPS_H_

#include"sys.h"

#define USART3_GPIO_PORT GPIOB
#define USART3_TX GPIO_Pin_10
#define USART3_RX GPIO_Pin_11

//初始化USART3（GPS模块接此串口）
void USART3_Init(void);
//串口3接收数据 存入Usart3Buf中
void USART3_IRQHandler(void);


#endif
