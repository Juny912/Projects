#ifndef __HC05_H
#define __HC05_H

#include "stm32f10x.h"

#define HC05_AT_IN
#define HC05_AT_OUT



void Hc05_Init(void);

void Hc05_SetAtMode(void);

u8 Hc05_GetState(void);

u8 Hc05_GetRole(void);

void Hc05_SendCmd(char *CmdData);
    


#endif

