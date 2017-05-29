//功能描述：GY-30光强传感器的操作
#include<stc15.h>
#include<IIC_Analog.h>
#include<GY-30.h>
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：GY30_PowerOn
*函数功能：设置GY-30为上电状态
*////////////////////////////////////////////////////////////////////////////////////
void GY30_PowerOn()
{
	EA = 0;
	IIC_Start();
	IIC_Write(GY30_ADDR);
	IIC_Write(0x01);
	IIC_Stop();
	EA = 1;
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：GY30_PowerDown
*函数功能：设置GY-30为掉电状态
*////////////////////////////////////////////////////////////////////////////////////
void GY30_PowerDown()
{
	EA = 0;
	IIC_Start();
	IIC_Write(GY30_ADDR);
	IIC_Write(0x00);
	IIC_Stop();
	EA = 1;
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：GY30_Delay
*函数功能：延时函数，等待GY-30传感器完成一次测量（请不要直接调用）
*参数列表：
*   mode
*       参数类型：unsigned char型数据
*       参数描述：测量模式（1：快速模式，耗时16ms精度4 lx。2：高精度模式，耗时120ms精度1 lx。3：高精度模式2，耗时120ms精度0.5 lx，测量范围减半）
*////////////////////////////////////////////////////////////////////////////////////
void GY30_Delay(unsigned char mode)
{
	unsigned char i, j, k = XTAL;

	if(mode == 1)
	{
		i = 16;
		j = 141;
	}
	else
	{
		i = 117;
		j = 184;
	}
	do
	{
		do{
			while (--k);
		}while (--j);
	} while (--i);
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：GY30_CaculateResult
*函数功能：把GY-30传感器的返回值转换为float型结果（单位为lx）（请不要直接调用）
*参数列表：
*   temp1
*       参数类型：unsigned char型数据
*       参数描述：GY-30传感器返回测量结果的高8位
*   temp2
*       参数类型：unsigned char型数据
*       参数描述：GY-30传感器返回测量结果的低8位
*   mode
*       参数类型：unsigned char型数据
*       参数描述：测量模式（1：快速模式，耗时16ms精度4 lx。2：高精度模式，耗时120ms精度1 lx。3：高精度模式2，耗时120ms精度0.5 lx，测量范围减半）
*返回值：一个float型数，测量结果
*////////////////////////////////////////////////////////////////////////////////////
float GY30_CaculateResult(unsigned char temp1, unsigned char temp2, unsigned char mode)
{
	float temp;
	switch(mode)
	{
		case 1:
		case 2:
		{
			temp = ((long)temp1) << 8 | temp2;
			break;
		}
		case 3:
		{
			temp = ((long)temp1) << 8 | temp2;
			temp /= 2;
			break;
		}
		default : return -1;
	}	
	return temp;
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：GY30_Start
*函数功能：启动GY-30测量
*参数列表：
*   mode
*       参数类型：unsigned char型数据
*       参数描述：测量模式（1：快速模式，耗时16ms精度4 lx。2：高精度模式，耗时120ms精度1 lx。3：高精度模式2，耗时120ms精度0.5 lx，测量范围减半）
*////////////////////////////////////////////////////////////////////////////////////
void GY30_Start(unsigned char mode)
{
	EA = 0;
	IIC_Start();
	IIC_Write(GY30_ADDR);
	if(mode == 1)
		IIC_Write(0x13);
	else if(mode == 3)
		IIC_Write(0x11);
	else
		IIC_Write(0x10);
	IIC_Stop();
	EA = 1;
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：GY30_ReadOnce
*函数功能：获取GY-30测量结果
*   mode
*       参数类型：unsigned char型数据
*       参数描述：测量模式（1：快速模式，耗时16ms精度4 lx。2：高精度模式，耗时120ms精度1 lx。3：高精度模式2，耗时120ms精度0.5 lx，测量范围减半）
*返回值：一个float型数，测量结果
*////////////////////////////////////////////////////////////////////////////////////
float GY30_GetResult(unsigned char mode)
{
	int temp1, temp2;
	EA = 0;
	IIC_Start();
	IIC_Write(GY30_ADDR | 0x01);
	temp1 = IIC_Read(0);
	temp2 = IIC_Read(1);
	IIC_Stop();
	EA = 1;
	return GY30_CaculateResult(temp1, temp2, mode);
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：GY30_ReadOnce
*函数功能：启动一次GY-30测量，并返回测量结果（阻塞线程）
*参数列表：
*   mode
*       参数类型：unsigned char型数据
*       参数描述：测量模式（1：快速模式，耗时16ms精度4 lx。2：高精度模式，耗时120ms精度1 lx。3：高精度模式2，耗时120ms精度0.5 lx，测量范围减半）
*返回值：一个float型数，测量结果
*////////////////////////////////////////////////////////////////////////////////////
float GY30_ReadOnce(unsigned char mode)
{
	EA = 0;
	IIC_Start();
	IIC_Write(GY30_ADDR);
	if(mode == 1)
		IIC_Write(0x23);
	else if(mode == 3)
		IIC_Write(0x21);
	else
		IIC_Write(0x20);
	IIC_Stop();
	EA = 1;
	GY30_Delay(mode);
	return GY30_GetResult(mode);
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：GY30_Start_Smart
*函数功能：启动GY-30智能读取模式
*////////////////////////////////////////////////////////////////////////////////////
int GY30_SmartReadMode = 0;	//智能读取模式记录
void GY30_Start_Smart()
{
	GY30_SmartReadMode = 2;
	GY30_Start(2);
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：GY30_GetResult_Smart
*函数功能：以智能模式获取GY-30测量结果
*返回值：一个float型数，测量结果
*////////////////////////////////////////////////////////////////////////////////////
float GY30_GetResult_Smart()
{
	float result;
	result = GY30_GetResult(GY30_SmartReadMode);
	if(result > 30000 && GY30_SmartReadMode == 3)
	{
		GY30_SmartReadMode = 2;
		GY30_Start(2);
	}
	else if(result < 10000 && GY30_SmartReadMode == 2)
	{
		GY30_SmartReadMode = 3;
		GY30_Start(3);
	}
	return result;
}