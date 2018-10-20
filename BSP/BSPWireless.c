#include <stdlib.h>
#include "gd32f1x0.h"
#include "Common.h"

#include "BSPWireless.h"
#include "lt8920.h"



/*********************************************
描述: 无线出厂设置
参数：void
返回：void	
*********************************************/
void BSPWirelessFactoryReset(void)
{
	LT8920FactoryReset();
}	


//无线通讯初始化
void BSPWirelessInit(void)
{
	LT8920Init();
}

//无线通讯设配置
void BSPWirelessConfig(void)
{
	LT8920Config();
}	

//无线通讯打开
bool BSPWirelessOpen(void)
{
	if(LT8920Open())
	{
		return TRUE;	
	}
	return FALSE;
}


	
//无线通讯关闭
bool BSPWirelessClose(void)
{
	if(LT8920Close())
	{
		return TRUE;	
	}
	return FALSE;	
}

//无线通讯复位
bool BSPWirelessReset(void)
{
	if(LT8920Reset())
	{
		return TRUE;	
	}
	return FALSE;		
}

//无线通讯建立连接
bool BSPWirelessConnect(ConnectMode mode)
{
	return LT8920CreateConnect(mode);
}


//保存匹配信息
void BSPWirelessSaveConnect(void)
{
	LT8920SaveSrcDest();
}

//无线通讯断开连接
bool BSPWirelessDisconnet(void)
{
	return TRUE;
}	

//无线通讯在线状态查询
bool BSPWirelessOnline(void)
{
	return TRUE;
}

//无线通讯发送数据
bool BSPWirelessWriteBytes(const uint8_t* Data, uint16_t Count)
{
	return LT8920Send(Data, Count);
}

//无线通讯查询接收缓冲获得据量
uint16_t BSPWirelessBufCount(void)
{
	return 1;
}

//无线通讯清空接收缓冲区数据
void BSPWirelessBufClear(uint8_t* Data, uint16_t Count)
{
}

//无线通讯接收数据一字节
uint8_t BSPWirelessReadByte(void)
{
	return 1;
}	

//无线通讯接收数据多字节
void BSPWirelessReadBytes(uint8_t* Data, uint16_t Count)
{
}

//无线通讯添加发送数据完成中断服务
bool BSPWirelessAddWriteISR(CallBackISR Program)
{
	return(TRUE);
}

//无线通讯添加接收数据完成中断服务
extern bool BSPWirelessAddReadISR(CallBackISR Program)
{
	return(TRUE);
}

//无线通讯循环程序
void BSPWirelessRoutine(void)
{
	LT8920DummySend();
}



void BSPWirelessSaveConfig(void)
{
	LT8920SavePidSpid();
}
