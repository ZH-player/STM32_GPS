#ifndef __LED_H
#define __LED_H

#include"sys.h"

extern u32 cnt;


void GPIO_Config(void);

void Led_High(void);
void Led_Low(void);

void Led_Run(void);
void Led_Change(void);

#endif
