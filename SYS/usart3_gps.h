#ifndef __USART_GPS_H_
#define __USART_GPS_H_

#include"sys.h"

#define USART3_GPIO_PORT GPIOB
#define USART3_TX GPIO_Pin_10
#define USART3_RX GPIO_Pin_11

//��ʼ��USART3��GPSģ��Ӵ˴��ڣ�
void USART3_Init(void);
//����3�������� ����Usart3Buf��
void USART3_IRQHandler(void);


#endif
