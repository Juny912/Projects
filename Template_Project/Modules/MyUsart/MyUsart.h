
#ifndef _MYUSART_H
#define _MYUSART_H

#include "stm32f10x.h"


void MyUsart_vidInit(void);


void MyUart_SendBuffer(char *buffer, u8 len);



#endif

