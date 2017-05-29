#ifndef _DHT11_H_
#define _DHT11_H_
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：DHT11_Read
*函数功能：从DHT11读取温湿度数据
*参数列表：
*   *_Wet
*       参数类型：unsigned char型指针
*       参数描述：存储湿度数据的变量
*返回值：一个unsigned char型数，温度
*////////////////////////////////////////////////////////////////////////////////////
unsigned char DHT11_Read(unsigned char *Wet);

#endif