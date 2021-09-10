#ifndef __OLED_I2C_H_
#define __OLED_I2C_H_

#include"sys.h"

#define OLED_ADDRESS	0x78 //ͨ������0R����,������0x78��0x7A������ַ -- Ĭ��0x78


#define OLED_I2C_GPIO_PORT GPIOB
#define OLED_I2C_SCL GPIO_Pin_6
#define OLED_I2C_SDA GPIO_Pin_7

//��ʼ��I2C
void I2C_Configuration(void);
//��Ĵ�����ַдһ��byte������
void I2C_WriteByte(uint8_t addr,uint8_t data);
//���ֽڶ�
//void I2C_ReadByte(uint8_t addr,uint8_t data);
//I2Cд����
void WriteCmd(unsigned char I2C_Command);
//I2Cд����
void WriteDat(unsigned char I2C_Data);

/*------------------------------------------------------------------------*/
//OLED����غ���
//OLED����ָ�ϣ�https://www.cnblogs.com/yilangUAV/p/12727606.html
//SSD1306 datasheet:https://html.alldatasheetcn.com/html-pdf/1179026/ETC2/SSD1306/111/1/SSD1306.html
//OLED����ʼ��
void OLED_Init(void);
//������ʼ������
void OLED_SetPos(unsigned char x, unsigned char y);
//ȫ�����
void OLED_Fill(unsigned char fill_Data);
//����
void OLED_CLS(void);
//OLED����
void OLED_ON(void);
//OLED����
void OLED_OFF(void);
//��ʾoled_font.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
void OLED_ShowStr(unsigned char x, unsigned char y, char ch[], unsigned char TextSize);
////��ʾoled_font.h�еĺ���,16*16����
//void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
//��ʾBMPλͼ
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);




#endif
