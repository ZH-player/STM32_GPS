#ifndef __OLED_I2C_H_
#define __OLED_I2C_H_

#include"sys.h"

#define OLED_ADDRESS	0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78


#define OLED_I2C_GPIO_PORT GPIOB
#define OLED_I2C_SCL GPIO_Pin_6
#define OLED_I2C_SDA GPIO_Pin_7

//初始化I2C
void I2C_Configuration(void);
//向寄存器地址写一个byte的数据
void I2C_WriteByte(uint8_t addr,uint8_t data);
//单字节读
//void I2C_ReadByte(uint8_t addr,uint8_t data);
//I2C写命令
void WriteCmd(unsigned char I2C_Command);
//I2C写数据
void WriteDat(unsigned char I2C_Data);

/*------------------------------------------------------------------------*/
//OLED屏相关函数
//OLED命令指南：https://www.cnblogs.com/yilangUAV/p/12727606.html
//SSD1306 datasheet:https://html.alldatasheetcn.com/html-pdf/1179026/ETC2/SSD1306/111/1/SSD1306.html
//OLED屏初始化
void OLED_Init(void);
//设置起始点坐标
void OLED_SetPos(unsigned char x, unsigned char y);
//全屏填充
void OLED_Fill(unsigned char fill_Data);
//清屏
void OLED_CLS(void);
//OLED唤醒
void OLED_ON(void);
//OLED休眠
void OLED_OFF(void);
//显示oled_font.h中的ASCII字符,有6*8和8*16可选择
void OLED_ShowStr(unsigned char x, unsigned char y, char ch[], unsigned char TextSize);
////显示oled_font.h中的汉字,16*16点阵
//void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
//显示BMP位图
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);




#endif
