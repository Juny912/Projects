
#ifndef _LED_H
#define _LED_H

#include "stm32f10x.h"



typedef enum 
{
    LED0 = 0,
    LED1,
}Led_enuNum;


void Led_vidInit(void);
void Led_vidSetHigh(Led_enuNum LedNum);
void Led_vidSetLow(Led_enuNum LedNum);




#endif

