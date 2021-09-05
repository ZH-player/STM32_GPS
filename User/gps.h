#ifndef __GPS_H_
#define __GPS_H_

#include"sys.h"
#include<string.h>
#include <stdlib.h >




//���δ�����Ļ���GPS����
typedef struct basicData
{
	char GNRMC[200];//��С��λ��Ϣ
	char GNVTG[200];//�����ٶ���Ϣ
}basicData;

//��Ŷ�λ��Ϣ����GNRMC����������
typedef struct position
{
	char *UTCtime;
	int  status;//��λ״̬��A=��Ч��λ��V=��Ч��λ
	char *N_S;//�ϱ�
	char *latitude;//γ��
	char *E_W;//����
	char *longitude;//����
//	int  speedKnot;// �������ʣ�000.0~999.9�ڣ�Knot)
//	int  direction;//���溽��000.0~359.9�ȣ����汱Ϊ�ο���׼)
	char *UTCday;
	char *mode;//ģʽָʾ��A=������λ��D=��֣�E=���㣬N=������Ч��
	int  isParsePosition;
}position;



//��GPS���ݷ���
int GPSDataClass(char *str);
//����GPS����
void parseGPSData(int DataNum);
//��ӡGPS���ݴ��뵽���ڵ���������
void printfPosiInfo(void);


#endif
