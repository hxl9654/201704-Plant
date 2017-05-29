//功能描述：STC15单片机ADC操作模块
#include<STC15.h>

/*///////////////////////////////////////////////////////////////////////////////////
*函数名：ADC_Get
*函数功能：立即从STC15单片机ADC获取一次转换结果
*参数列表：
*   channel
*       参数类型：unsigned char型数据
*       参数描述：要转换的通道
*返回值：unsigned int型数据，ADC转换的结果
*////////////////////////////////////////////////////////////////////////////////////
unsigned int ADC_Get(unsigned char channel) 
{
	unsigned int temp = 0x0000;
	bit EADCBackup;
	EADCBackup = EADC;
	EADC = 0;
	
	P1ASF = 0x01 << channel;
	ADC_CONTR = 0xE8 | channel;	
	
	while(!(ADC_CONTR & 0x10));	//等待转换完成
	ADC_CONTR &= 0xEF;
	
	temp = ADC_RES;
	temp <<= 8;
	temp |= ADC_RESL;
	EADC = EADCBackup;
	return temp;
}
