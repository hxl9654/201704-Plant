#include<stc15.h>
#include<string.h>
#include<stdio.h>
#include<config.h>
#include<main.h>
#include<1602.h>
#include<ADC.h>
#include<DHT11.h>
#include<DS1302.h>
#include<GY-30.h>

void main()
{
	unsigned char i, t = 0;
	unsigned char temperature, wet, soilWet;
	struct DS1302_Time Time;
	float light;
	char strTemp[17] = {0};
	Init_IOPort();	//³õÊ¼»¯IO¿Ú
	
	LCD1602_Init();
	LCD1602_Show(0, 0, "Hello", 5);
	
	GY30_PowerDown();
	GY30_PowerOn();
	GY30_ReadOnce(0);
	GY30_Start_Smart();
	
//	DS1302_Init();
//	DS1302_SingleRead(0);
//	DS1302_SingleWrite(8, 0xA5);
//	Time.year = 17;
//	Time.month = 5;
//	Time.day = 2;
//	Time.week = 2;
//	Time.hour = 17;
//	Time.min = 47;
//	Time.sec = 0;
//	DS1302_Write(Time);
	
	LCD1602_Clear();
	
	while(1)
	{
		Delay200ms();
		
		if(t == 2)
			temperature = DHT11_Read(&wet);	
		else if(t == 4)
			soilWet = YL69_GetWet();
		else if(t >= 5)
			t = 0;
		t++;
		light = GY30_GetResult_Smart();
		DS1302_Read(&Time);

		sprintf(strTemp, "%2d:%2d:%2d L%.1f", (int)Time.hour, (int)Time.min, (int)Time.sec, light);
		for(i = 0; i < 8; i++)
			if(strTemp[i] == ' ')
				strTemp[i] = '0';
					
		LCD1602_AreaClear(0, 0, 16);
		LCD1602_Show(0, 0, strTemp, strlen(strTemp));
		
		sprintf(strTemp, "T:%d,W:%d,SW:%d", (int)temperature, (int)wet, (int)soilWet);
		LCD1602_AreaClear(0, 1, 16);
		LCD1602_Show(0, 1, strTemp, strlen(strTemp));
			
		if(light < 1000)
			LED = 0;
		else LED = 1;
		
		if(soilWet < 40)
		{
			Water = 1;
			LCD1602_Clear();
			LCD1602_Show(0, 0, "Watering...", 11);
			LCD1602_Show(0, 1, "Please Wait", 11);
			LCD1602_ShowCursor();
			while(YL69_GetWet() < 40)
				Delay200ms();
			Water = 0;
			LCD1602_CloseCursor();
			LCD1602_Clear();
		}
		else Water = 0;
	}
}

unsigned char YL69_GetWet()
{
	int temp;
	temp = ADC_Get(7);
	temp = (1280 - temp) * 5 / 128 - 50;
	return (unsigned char) temp;
}
void Delay200ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 9;
	j = 104;
	k = 139;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Init_IOPort()
{
	P0M0 = 0x00; P0M1 = 0x00;
	P1M0 = 0x00; P1M1 = 0x00;
	P2M0 = 0x00; P2M1 = 0x00;
	P3M0 = 0x00; P3M1 = 0x00;
	P4M0 = 0x20; P4M1 = 0x00;
	P5M0 = 0x00; P5M1 = 0x00;
	Water = 0;
}