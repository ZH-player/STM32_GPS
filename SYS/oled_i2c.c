#include"oled_i2c.h"
#include"oled_font.h"

void I2C_Configuration(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//配置GPIO引脚
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//I2C必须开漏输出
	GPIO_InitStructure.GPIO_Pin = OLED_I2C_SCL|OLED_I2C_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(OLED_I2C_GPIO_PORT,&GPIO_InitStructure);
	
	//配置I2C
	I2C_DeInit(I2C1);//使用I2C1
	I2C_InitTypeDef I2C_InitStruct;
	I2C_InitStruct.I2C_ClockSpeed = 400000;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x30;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1, &I2C_InitStruct);
	I2C_Cmd(I2C1, ENABLE);
}

//向寄存器地址写一个byte的数据
void I2C_WriteByte(uint8_t addr,uint8_t data)
{
	//1.开始
	//2.设备地址/写
	//3.数据地址
	//4.写一字节数据
	//5.停止
	//I2C指南：https://www.cnblogs.com/yeshenmeng/p/9766054.html
	
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	//1.开始
	I2C_GenerateSTART(I2C1, ENABLE);//开启I2C1
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/
	//2.设备地址/写
	I2C_Send7bitAddress(I2C1, OLED_ADDRESS, I2C_Direction_Transmitter);//器件地址 -- 默认0x78
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	//3.数据地址
	I2C_SendData(I2C1, addr);//寄存器地址
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	//4.写一字节数据
	I2C_SendData(I2C1, data);//发送数据
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	//5.停止
	I2C_GenerateSTOP(I2C1, ENABLE);//关闭I2C1总线
}

//单字节读
//void I2C_ReadByte(uint8_t addr,uint8_t data)

//I2C写命令
void WriteCmd(unsigned char I2C_Command)
{
	I2C_WriteByte(0x00, I2C_Command);
}

//I2C写数据
void WriteDat(unsigned char I2C_Data)
{
	I2C_WriteByte(0x40, I2C_Data);
}


void OLED_Init(void)
{
	//OLED命令指南：https://www.cnblogs.com/yilangUAV/p/12727606.html
	//SSD1306 datasheet:https://html.alldatasheetcn.com/html-pdf/1179026/ETC2/SSD1306/111/1/SSD1306.html
	
	Delay_ms(100); //!!!!!!!!必要!!!!!!!!
	
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //亮度调节 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}

//设置起始点坐标
void OLED_SetPos(unsigned char x, unsigned char y) 
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

//全屏填充
void OLED_Fill(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				WriteDat(fill_Data);
			}
	}
}

//清屏
void OLED_CLS(void)
{
	OLED_Fill(0x00);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none
// Description    : 将OLED从休眠中唤醒
//--------------------------------------------------------------
void OLED_ON(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X14);  //开启电荷泵
	WriteCmd(0XAF);  //OLED唤醒
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : 让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X10);  //关闭电荷泵
	WriteCmd(0XAE);  //OLED休眠
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); ch[] -- 要显示的字符串; TextSize -- 字符大小(1:6*8 ; 2:8*16)
// Description    : 显示codetab.h中的ASCII字符,有6*8和8*16可选择
//--------------------------------------------------------------
void OLED_ShowStr(unsigned char x, unsigned char y, char ch[], unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}

////--------------------------------------------------------------
//// Prototype      : void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
//// Calls          : 
//// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); N:汉字在codetab.h中的索引
//// Description    : 显示codetab.h中的汉字,16*16点阵
////--------------------------------------------------------------
//void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
//{
//	unsigned char wm=0;
//	unsigned int  adder=32*N;
//	OLED_SetPos(x , y);
//	for(wm = 0;wm < 16;wm++)
//	{
//		WriteDat(F16x16[adder]);
//		adder += 1;
//	}
//	OLED_SetPos(x,y + 1);
//	for(wm = 0;wm < 16;wm++)
//	{
//		WriteDat(F16x16[adder]);
//		adder += 1;
//	}
//}

//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
// Calls          : 
// Parameters     : x0,y0 -- 起始点坐标(x0:0~127, y0:0~7); x1,y1 -- 起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
// Description    : 显示BMP位图
//--------------------------------------------------------------
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			WriteDat(BMP[j++]);
		}
	}
}

