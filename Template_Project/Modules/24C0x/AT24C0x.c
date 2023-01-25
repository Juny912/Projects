#include "AT24C0x.h"

#include "sys.h"

void AT24C0X_Init(void)
{
    IIC_Init();
}



void AT24C0X_WriteOneByte(u8 WriteAddr, u8 DataToWrite)
{
    IIC_Start();
    IIC_Write(DEVICE_ADDR_W);
    IIC_WaitACK();
    IIC_Write(WriteAddr);
    IIC_WaitACK();
    IIC_Write(DataToWrite);
    IIC_WaitACK();
    IIC_Stop();
    delay_ms(10);
}



u8 AT24C0X_ReadOneByte(u8 ReadAddr)
{
    u8 ReadData;
    IIC_Start();
    IIC_Write(DEVICE_ADDR_W);
    IIC_WaitACK();    
    IIC_Write(ReadAddr);
    IIC_WaitACK();
    IIC_Start();
    IIC_Write(DEVICE_ADDR_R);
    IIC_WaitACK();
    IIC_Read(&ReadData, 0);
    IIC_Stop();

    return ReadData;
}

void AT24C0X_Write(u8 WriteAddr,u8 *DataToWrite,u8 Size)
{
    u8 u8Len = 0;

    for(; u8Len < Size; u8Len++)
    {
        AT24C0X_WriteOneByte(WriteAddr+u8Len, *(DataToWrite+u8Len));
    }
}



void AT24C0X_Read(u8 ReadAddr, u8 *ReadBuff,u8 Size)
{
    u8 u8Len = 0;

    for(; u8Len < Size; u8Len++)
    {
        *(ReadBuff+u8Len) = AT24C0X_ReadOneByte(ReadAddr+u8Len);
    }
}

