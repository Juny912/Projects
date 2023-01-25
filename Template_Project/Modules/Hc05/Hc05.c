#include "Hc05.h"
#include "string.h"
#include "MyUsart.h"

#define HC05_SET_AT     "AT\r\n"
#define HC05_SET_NAME   "AT+NAME=HC05_YJ\r\n"
#define HCO5_SET_PSWD   "AT+PSWD=8888\r\n"

#define HC05_GET_UART   "AT+UART?\r\n"
#define HC05_GET_PSWD   "AT+PSWD?\r\n"
#define HC05_GET_ROLE   "AT+ROLE?\r\n"
#define HC05_GET_NAME   "AT+NAME?\r\n"
#define HC05_GET_ROLE   "AT+ROLE?\r\n"
#define HC05_GET_VER    "AT+VERSION?\r\n"


char HC05_SendData[30] = HC05_GET_VER;


#define HC05_EN_KEY_HIGH()  GPIO_SetBits(GPIOB, GPIO_Pin_1)


void Hc05_SendCmd(char *CmdData);


void Hc05_Init(void)
{
    /*Entry AT mode*/
    HC05_EN_KEY_HIGH();

    Hc05_SendCmd(HC05_SendData);
}


void Hc05_SendCmd(char *CmdData)
{
    u8 i = 0;
    i=strlen((const char*)CmdData);
    MyUart_SendBuffer(CmdData, i);
}


