//DHT11测量温湿度
#include<stc15.h>
#include<intrins.h>
#include<config.h>
#define _6nop(); {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}
sbit DHT11_Port = DEFDHT11_Port;
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：DelayX10us
*函数功能：延时10*t微妙（请不要直接调用）
*参数列表：
*   t
*       参数类型：unsigned char型整数
*       参数描述：要延时的时长
*////////////////////////////////////////////////////////////////////////////////////
void DelayX10us(unsigned char t)
{
	unsigned int i;
    i = t * 2;
    while (--i)
    {
        _6nop();_6nop();_6nop();_6nop();_6nop();
        _6nop();_nop_();_nop_();_nop_();
    }
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：Delay20ms
*函数功能：延时20ms
*////////////////////////////////////////////////////////////////////////////////////
void Delay20ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 1;
	j = 216;
	k = 35;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：DHT11_Read
*函数功能：从DHT11读取温湿度数据
*参数列表：
*   *_Wet
*       参数类型：unsigned char型指针
*       参数描述：存储湿度数据的变量
*返回值：一个unsigned char型数，温度
*////////////////////////////////////////////////////////////////////////////////////
unsigned char DHT11_Read(unsigned char *_Wet)
{
	unsigned char mask, Temp = 0, Wet = 0;
	DHT11_Port = 0;
	Delay20ms();
	EA = 0;                               //禁能中断，防止时序被干扰
	DHT11_Port = 1;
	DelayX10us(4);
	while(DHT11_Port);
	while(!DHT11_Port);
	while(DHT11_Port);
	for(mask=0x80; mask != 0; mask >>= 1)   
	{		
		while(!DHT11_Port);
		DelayX10us(4);
		if(DHT11_Port)
			Wet |= mask;
		while(DHT11_Port);
	}
	for(mask=0x80; mask != 0; mask >>= 1)   
	{
		while(!DHT11_Port);
		while(DHT11_Port);	
	}
	for(mask=0x80; mask != 0; mask >>= 1)   
	{
		while(!DHT11_Port);
		DelayX10us(4);
		if(DHT11_Port)
			Temp |= mask;
		while(DHT11_Port);
	}
	*_Wet = Wet;
	EA = 1;              
	return Temp;
}
