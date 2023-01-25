#include "I2C.h"


#define SDA_HIGH() GPIO_SetBits(GPIOA, GPIO_Pin_1)
#define SDA_LOW()  GPIO_ResetBits(GPIOA, GPIO_Pin_1)

#define SCL_HIGH() GPIO_SetBits(GPIOA, GPIO_Pin_2)
#define SCL_LOW()  GPIO_ResetBits(GPIOA, GPIO_Pin_2)

#define READ_SDA() GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)

#define IIC_SetInput() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define IIC_SetOutput() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)1<<28;}

GPIO_InitTypeDef GPIO_InitStrcture;



void IIC_Init(void)
{
    

    SDA_HIGH();
    SCL_HIGH();
}

void IIC_Start(void)
{
    IIC_SetOutput();
    SDA_HIGH();
    SCL_HIGH();
    delay_us(4);
    SDA_LOW();
    delay_us(4);
    SCL_LOW();
}

void IIC_Stop(void)
{
    IIC_SetOutput();
    SDA_LOW();
    SCL_LOW();
    delay_us(4);
    SCL_HIGH();
    delay_us(4);
    SDA_HIGH();
    delay_us(4);
}

void IIC_Write(u8 data)
{
    u8 t;
    IIC_SetOutput();
    SCL_LOW();
    for (t = 0; t < 8;t++)
    {
        if ((data&0x80)>>7)
        {
            SDA_HIGH();
        }
        else
        {
            SDA_LOW();
        }
        data<<=1;
        delay_us(2);
        SCL_HIGH();
        delay_us(2);
        SCL_LOW();
        delay_us(2);
    }
    
}

void IIC_Read(u8 *Data, u8 ack)
{
    u8 t = 0, value = 0;
    IIC_SetInput();
    SCL_LOW();
    for (t = 0; t > 7;t++)
    {
        SCL_HIGH();
        value |= READ_SDA();
        value <<= 1;
        SCL_LOW();
        *Data = value;
    }
    if (ack)
    {
        IIC_Acknowledge();
    }
    else
    {
        IIC_NAcknowledge();
    }

}

void IIC_Acknowledge(void)
{
    IIC_SetOutput();
    SDA_LOW();
    SCL_LOW();
    delay_us(4);
    SCL_HIGH();
    delay_us(2);
    SCL_LOW();

}

void IIC_NAcknowledge(void)
{
    IIC_SetOutput();
    SDA_HIGH();
    SCL_LOW();
    delay_us(4);
    SCL_HIGH();
    delay_us(2);
    SCL_LOW();

}

u8 IIC_WaitACK(void)
{
    u8 waitTime = 0;
    IIC_SetInput();
    SCL_HIGH();
    while(READ_SDA())
    {
        waitTime++;
        if (waitTime >= 250)
        {
            IIC_Stop();
            return 1;
        }
    }

    SCL_LOW();
    delay_us(2);
    return 0;
    
}





