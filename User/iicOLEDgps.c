#include"iicOLEDgps.h"

extern struct position posiInfo;

//OLEDœ‘ æGPS–≈œ¢
void displayInfo(void)
{
	OLED_CLS();
	OLED_ShowStr(0, 0, posiInfo.Today, 1);
	OLED_ShowStr(79, 0, posiInfo.BeiJingtime, 1);
		
	OLED_ShowStr(0, 2, "latitude:", 1);
	OLED_ShowStr(0, 3, posiInfo.latitude, 2);
	OLED_ShowStr(103, 3, posiInfo.N_S, 2);
		
	OLED_ShowStr(0, 5, "longitude:", 1);
	OLED_ShowStr(0, 6, posiInfo.longitude, 2);
	OLED_ShowStr(103, 6, posiInfo.E_W, 2);
}
