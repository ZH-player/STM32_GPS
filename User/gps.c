#include"gps.h"


struct basicData GPSData;
struct position posiInfo;






//解析GPS数据代码
//传入值：收到的GPS数据
//传出值：GPS数据分类信息：1->GNRMC,2->GNVTG
int GPSDataClass(char *str)
{
	if(str[0]=='$' && (str[2]=='N' && str[4]=='M'))
	{
		return 1;
	}
//	else if(str[0]=='$' && (str[2]=='N' && str[4]=='T'))
//	{
//		return 2;
//	}
	else
	{
		return 0;
	}
}




//解析GPS数据代码
//传入值：GPS数据类别码1->GNRMC，2->GNVTG
 void parseGPSData(int DataNum)
 {
//	printf("%s\r\n",GPSData.GNRMC);//探针
	char *subString;
	char *subStringNext;
	int i = 0;
	if(DataNum == 1)
	{
		
		
		if(GPSData.GNRMC[0] == '$' )//确认收到GNRMC
		{
			printf("*** parseGPSData ***\r\n");
			
			
			for(i = 0; i <= 10; i++)
			{
				if(i == 0)
				{
					if((subString = strstr(GPSData.GNRMC, ",")) == NULL)
					{
						printf("解析错误");
					}
				}
				else
				{
					subString++;//到达解析数据中逗号的下一位
					if((subStringNext = strstr(subString, ",")) != NULL)
					{
						//printf("Pin1");
						char usefullBuffer[2];
						switch(i)
						{
							case 1: 
								//利用subStringNext和subString的首地址相减来确定指针开辟空间的大小。
								posiInfo.UTCtime = (char *)malloc((subStringNext - subString)*sizeof(char));
								memcpy(posiInfo.UTCtime, subString, subStringNext - subString);
								posiInfo.BeiJingtime = (char *)malloc(8*sizeof(char));
								UTCtoBeiJing();
								break;
							case 2: memcpy(usefullBuffer, subString, subStringNext - subString); break;
							case 3: posiInfo.latitude = (char *)malloc((subStringNext - subString)*sizeof(char));
								memcpy(posiInfo.latitude, subString, subStringNext - subString); 
								break;
							case 4: posiInfo.N_S = (char *)malloc((subStringNext - subString)*sizeof(char));
								memcpy(posiInfo.N_S, subString, subStringNext - subString); 
								break;
							case 5: posiInfo.longitude = (char *)malloc((subStringNext - subString)*sizeof(char));
								memcpy(posiInfo.longitude, subString, subStringNext - subString); 
								break;
							case 6: posiInfo.E_W = (char *)malloc((subStringNext - subString)*sizeof(char));
								memcpy(posiInfo.E_W, subString, subStringNext - subString); 
								break;
							case 7:break;
							case 8:break;
							case 9:posiInfo.UTCday = (char *)malloc((subStringNext - subString)*sizeof(char));
								memcpy(posiInfo.UTCday, subString, subStringNext - subString); 
								posiInfo.mode = (char *)malloc(1*sizeof(char));
								memcpy(posiInfo.mode,subStringNext+3,1);
								posiInfo.Today = (char *)malloc(10*sizeof(char));
								UTCtoToday();
								break;
							
							default: break;
						}
						
//						printf("%s\r\n",posiInfo.E_W);//探针
						subString = subStringNext;
						posiInfo.isParsePosition = 1;
						if(usefullBuffer[0] == 'A')
							posiInfo.status = 1;
						else if(usefullBuffer[0] == 'V')
							posiInfo.status = 0;
					}
					else
					{
						posiInfo.isParsePosition = 0;
						printf("解析错误2");
					}
				}
			}
		}
	}
}


//格林尼治转换成北京时间
//posiInfo.UTCtime -> posiInfo.BeiJingtime
 void UTCtoBeiJing(void)
 {
	 int x;
	 
	//转换北京时间
	x = (posiInfo.UTCtime[0] - '0')*10 + posiInfo.UTCtime[1]-'0' + 8;
	x = x % 24;
	posiInfo.BeiJingtime[0] = x/10 + '0';
	posiInfo.BeiJingtime[1] = x%10 + '0';
	posiInfo.BeiJingtime[2] = ':';
	posiInfo.BeiJingtime[3] = posiInfo.UTCtime[2];
	posiInfo.BeiJingtime[4] = posiInfo.UTCtime[3];
	posiInfo.BeiJingtime[5] = ':';
	posiInfo.BeiJingtime[6] = posiInfo.UTCtime[4];
	posiInfo.BeiJingtime[7] = posiInfo.UTCtime[5];
 }

 
 //将接收的UTCday转换成常见时间格式Today
 void UTCtoToday(void)
 {
	posiInfo.Today[0] = '2';
	posiInfo.Today[1] = '0';
	posiInfo.Today[2] = posiInfo.UTCday[4];
	posiInfo.Today[3] = posiInfo.UTCday[5];
	posiInfo.Today[4] = '.';	 
	posiInfo.Today[5] = posiInfo.UTCday[2];
	posiInfo.Today[6] = posiInfo.UTCday[3];
	posiInfo.Today[7] = '.';
	posiInfo.Today[8] = posiInfo.UTCday[0];
	posiInfo.Today[9] = posiInfo.UTCday[1];
 }


 //打印GPS数据代码到串口
 void printfPosiInfo(void)
 {
	if(posiInfo.isParsePosition == 1)
	{
//		printf("%s\r\n",GPSData.GNRMC);//探针
		printf("Greenwich mean time is %c%c:%c%c:%c%c\r\n",posiInfo.UTCtime[0],posiInfo.UTCtime[1],posiInfo.UTCtime[2],posiInfo.UTCtime[3],posiInfo.UTCtime[4],posiInfo.UTCtime[5]);
		
		if(posiInfo.status == 1)
		{
			posiInfo.status = 0;
			printf("Today:20%c%c.%c%c.%c%c\r\n",posiInfo.UTCday[4],posiInfo.UTCday[5],posiInfo.UTCday[2],posiInfo.UTCday[3],posiInfo.UTCday[0],posiInfo.UTCday[1]);
			printf("BeiJing time is:%c%c:%c%c:%c%c\r\n",posiInfo.BeiJingtime[0],posiInfo.BeiJingtime[1],posiInfo.BeiJingtime[3],posiInfo.BeiJingtime[4],posiInfo.BeiJingtime[6],posiInfo.BeiJingtime[7]);
			printf("latitude:%s %s\r\n",posiInfo.latitude,posiInfo.N_S);
			printf("longitude:%s %s\r\n",posiInfo.longitude,posiInfo.E_W);
			printf("Mode:%s\r\n\r\n",posiInfo.mode);
		}
		
	}
	else
	{
		printf("GPS DATA is not usefull!\r\n");
	}
 }
 
 //清空posiInfo中数据
 void freeInfo(void)
 {
	free(posiInfo.BeiJingtime);
	free(posiInfo.UTCtime);
	free(posiInfo.N_S);
	free(posiInfo.latitude);
	free(posiInfo.E_W);
	free(posiInfo.longitude);
	free(posiInfo.UTCday);
	free(posiInfo.Today);
	free(posiInfo.mode);
 }
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
