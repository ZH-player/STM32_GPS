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
						printf("��������2");
					}
				}
			}
		}
	}
}


////����GPS����
////����ֵ��GPS���ݷ���Ľ����1->GNRMC��2->GNVTG
// void parseGPSData(int DataNum)
// {
//	 int i=0;//��¼���ڵĶ���λ��
//	 int j=0;//��¼ǰһ������λ��
//	 int commaNum = 0;
//	 if(DataNum == 1)//������ΪGNRMCʱ
//	 {
////		posiInfo.isParsePosition = 0;
//		for(i=0;GPSData.GNRMC[i]!='*';i++)
//		{
//			if(GPSData.GNRMC[i]==',')//ÿ������һ����,��ʱ������ǰ�����Ϣ
//			{
//				commaNum++;
//				switch(commaNum)
//				{
//					case 1:break;
//					case 2:posiInfo.UTCtime = (char *)malloc((i-j)*sizeof(char));
//						   memcpy(posiInfo.UTCtime,GPSData.GNRMC+j,i-j-1);break;
//					case 3:if(GPSData.GNRMC[i-1]=='A')
//						   {
//								posiInfo.status = 1;
//						   }else
//						   {
//								posiInfo.status = 0;
//						   }break;
//					case 4:posiInfo.N_S = (char *)malloc((i-j)*sizeof(char));
//						   memcpy(posiInfo.N_S,GPSData.GNRMC+j,i-j-1);break;
//					case 5:posiInfo.latitude = (char *)malloc((i-j)*sizeof(char));
//						   memcpy(posiInfo.latitude,GPSData.GNRMC+j,i-j-1);break;
//					case 6:posiInfo.E_W = (char *)malloc((i-j)*sizeof(char));
//						   memcpy(posiInfo.E_W,GPSData.GNRMC+j,i-j-1);break;
//					case 7:posiInfo.longitude = (char *)malloc((i-j)*sizeof(char));
//						   memcpy(posiInfo.longitude,GPSData.GNRMC+j,i-j-1);break;
//					case 8:break;// ��������
//					case 9:break;//���溽��
//					case 10:posiInfo.UTCday = (char *)malloc((i-j)*sizeof(char));
//							memcpy(posiInfo.UTCday,GPSData.GNRMC+j,i-j-1);break;
//					case 11:posiInfo.mode = (char *)malloc((i-j)*sizeof(char));
//							memcpy(posiInfo.mode,GPSData.GNRMC+j,i-j-1);break;
//					default: break;
//				}
//				j = i;
//			posiInfo.isParsePosition = 1;
//			}
//			
//		}
//	 }
// }


 //打印GPS数据代码到串口
 void printfPosiInfo(void)
 {
	if(posiInfo.isParsePosition == 1)
	{
//		printf("%s\r\n",GPSData.GNRMC);//探针
		printf("Greenwich mean time is %c%c:%c%c:%c%c\r\n",posiInfo.UTCtime[0],posiInfo.UTCtime[1],posiInfo.UTCtime[2],posiInfo.UTCtime[3],posiInfo.UTCtime[4],posiInfo.UTCtime[5]);
		free(posiInfo.UTCtime);
		if(posiInfo.status == 1)
		{
			posiInfo.status = 0;
			printf("Today:20%c%c.%c%c.%c%c\r\n",posiInfo.UTCday[4],posiInfo.UTCday[5],posiInfo.UTCday[2],posiInfo.UTCday[3],posiInfo.UTCday[0],posiInfo.UTCday[1]);
			free(posiInfo.UTCday);
			printf("latitude:%s %s\r\n",posiInfo.latitude,posiInfo.N_S);
			free(posiInfo.latitude);
			free(posiInfo.N_S);
			printf("longitude:%s %s\r\n",posiInfo.longitude,posiInfo.E_W);
			free(posiInfo.longitude);
			free(posiInfo.E_W);
			printf("Mode:%s\r\n\r\n",posiInfo.mode);
			free(posiInfo.mode);
		}
		
	}
	else
	{
		printf("GPS DATA is not usefull!\r\n");
	}
 }
 
 
 
 
 
