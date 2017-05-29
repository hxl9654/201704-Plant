#ifndef _CONFIG_H_
#define _CONFIG_H_
#include<stc15.h>

#define LCD1602_DB P0
#define DEFLCD1602_RS P1^2
#define DEFLCD1602_RW P1^1
#define DEFLCD1602_E P1^0

#define DEFDHT11_Port P2^6

#define DEFGY30_SDA P2^5
#define DEFGY30_SCL P2^4

#define XTAL 11.0592

#define DS1302_CE_SET P2^1
#define DS1302_IO_SET P2^2
#define DS1302_CK_SET P2^3

sbit LED = P2^7;
sbit Water = P4^5;

#endif