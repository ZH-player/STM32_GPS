#ifndef __GPS_H_
#define __GPS_H_

#include"sys.h"
#include<string.h>
#include <stdlib.h >




//存放未解析的基础GPS数据
typedef struct basicData
{
	char GNRMC[200];//最小定位信息
	char GNVTG[200];//地面速度信息
}basicData;

//存放定位信息（由GNRMC解析出来）
typedef struct position
{
	char *UTCtime;
	int  status;//定位状态，A=有效定位，V=无效定位
	char *N_S;//南北
	char *latitude;//纬度
	char *E_W;//东西
	char *longitude;//经度
//	int  speedKnot;// 地面速率（000.0~999.9节，Knot)
//	int  direction;//地面航向（000.0~359.9度，以真北为参考基准)
	char *UTCday;
	char *mode;//模式指示（A=自主定位，D=差分，E=估算，N=数据无效）
	int  isParsePosition;
}position;



//将GPS数据分类
int GPSDataClass(char *str);
//解析GPS数据
void parseGPSData(int DataNum);
//打印GPS数据代码到串口调试助手上
void printfPosiInfo(void);


#endif
