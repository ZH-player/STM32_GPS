//解析GPS数据
//传入值：GPS数据分类的结果，1->GNRMC、2->GNVTG
 void parseGPSData(int DataNum)
 {
	 int i=0;//记录现在的逗号位置
	 int j=0;//记录前一个逗号位置
	 int commaNum = 0;
	 if(DataNum == 1)//当数据为GNRMC时
	 {
//		posiInfo.isParsePosition = 0;
		for(i=0;GPSData.GNRMC[i]!='*';i++)
		{
			if(GPSData.GNRMC[i]==',')//每当遇到一个“,”时解析它前面的信息
			{
				commaNum++;
				switch(commaNum)
				{
					case 1:break;
					case 2:posiInfo.UTCtime = (char *)malloc((i-j)*sizeof(char));
						   memcpy(posiInfo.UTCtime,GPSData.GNRMC+j,i-j-1);break;
					case 3:if(GPSData.GNRMC[i-1]=='A')
						   {
								posiInfo.status = 1;
						   }else
						   {
								posiInfo.status = 0;
						   }break;
					case 4:posiInfo.N_S = (char *)malloc((i-j)*sizeof(char));
						   memcpy(posiInfo.N_S,GPSData.GNRMC+j,i-j-1);break;
					case 5:posiInfo.latitude = (char *)malloc((i-j)*sizeof(char));
						   memcpy(posiInfo.latitude,GPSData.GNRMC+j,i-j-1);break;
					case 6:posiInfo.E_W = (char *)malloc((i-j)*sizeof(char));
						   memcpy(posiInfo.E_W,GPSData.GNRMC+j,i-j-1);break;
					case 7:posiInfo.longitude = (char *)malloc((i-j)*sizeof(char));
						   memcpy(posiInfo.longitude,GPSData.GNRMC+j,i-j-1);break;
					case 8:break;// 地面速率
					case 9:break;//地面航向
					case 10:posiInfo.UTCday = (char *)malloc((i-j)*sizeof(char));
							memcpy(posiInfo.UTCday,GPSData.GNRMC+j,i-j-1);break;
					case 11:posiInfo.mode = (char *)malloc((i-j)*sizeof(char));
							memcpy(posiInfo.mode,GPSData.GNRMC+j,i-j-1);break;
					default: break;
				}
				j = i;
			posiInfo.isParsePosition = 1;
			}
			
		}
	 }
 }