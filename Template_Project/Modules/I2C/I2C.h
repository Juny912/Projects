#ifndef _I2C_H
#define _I2C_H

#include "stm32f10x.h"
#include "delay.h"

void IIC_Init(void);

void IIC_Start(void);

void IIC_Stop(void);

void IIC_Write(u8 data);

void IIC_Read(u8 *Data, u8 ack);

void IIC_Acknowledge(void);

void IIC_NAcknowledge(void);

u8 IIC_WaitACK(void);


#endif
