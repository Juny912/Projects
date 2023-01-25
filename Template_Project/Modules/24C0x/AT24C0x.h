#ifndef _24C0X_H
#define _24C0X_H

#include "stm32f10x.h"
#include "I2C.h"


#define DEVICE_ADDR_W 0xA0
#define DEVICE_ADDR_R 0xA1

#define EEPROM_ADDR 0x0



void AT24C0X_Init(void);

void AT24C0X_WriteOneByte(u8 WriteAddr, u8 DataToWrite);

u8 AT24C0X_ReadOneByte(u8 ReadAddr);

void AT24C0X_Write(u8 WriteAddr,u8 *DataToWrite,u8 Size);

void AT24C0X_Read(u8 ReadAddr, u8 *ReadBuff,u8 Size);


#endif

