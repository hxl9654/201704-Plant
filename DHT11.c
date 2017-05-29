//DHT11������ʪ��
#include<stc15.h>
#include<intrins.h>
#include<config.h>
#define _6nop(); {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}
sbit DHT11_Port = DEFDHT11_Port;
/*///////////////////////////////////////////////////////////////////////////////////
*��������DelayX10us
*�������ܣ���ʱ10*t΢��벻Ҫֱ�ӵ��ã�
*�����б�
*   t
*       �������ͣ�unsigned char������
*       ����������Ҫ��ʱ��ʱ��
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
*��������Delay20ms
*�������ܣ���ʱ20ms
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
*��������DHT11_Read
*�������ܣ���DHT11��ȡ��ʪ������
*�����б�
*   *_Wet
*       �������ͣ�unsigned char��ָ��
*       �����������洢ʪ�����ݵı���
*����ֵ��һ��unsigned char�������¶�
*////////////////////////////////////////////////////////////////////////////////////
unsigned char DHT11_Read(unsigned char *_Wet)
{
	unsigned char mask, Temp = 0, Wet = 0;
	DHT11_Port = 0;
	Delay20ms();
	EA = 0;                               //�����жϣ���ֹʱ�򱻸���
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
