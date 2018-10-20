#ifndef	__WIRELESS_H__
#define	__WIRELESS_H__


#include "lt8920.h"
#include "Common.h"
#include "BSP.h"


#define WIRELESS_DATA_LEN_MAX		700		//Byte

/*********************************************
描述: 无线出厂设置
参数：void
返回：void	
*********************************************/
extern void BSPWirelessFactoryReset(void);



/*********************************************
描述: 无线芯片引脚初始化，添加定时器回调函数
参数：void
返回：void	
*********************************************/
extern void BSPWirelessInit(void);


/*********************************************
描述: 无线芯片参数配置
参数：void
返回：void	
*********************************************/
extern void BSPWirelessConfig(void);


/*********************************************
描述: 无线设备开启
参数：void
返回：
	TRUE成功  FALSE失败	
*********************************************/
extern bool BSPWirelessOpen(void);


/*********************************************
描述: 无线设备关闭
参数：void
返回：
	TRUE成功  FALSE失败	
*********************************************/
extern bool BSPWirelessClose(void);



/*********************************************
描述: 无线设备复位
参数：void
返回：
	TRUE成功  FALSE失败	
*********************************************/
extern bool BSPWirelessReset(void);



/************************************
描述: 与接收器建立连接
参数：
	CONNECT_NORMAL	接收器初次连接
	CONNECT_FORCE	非初次连接
返回：
	TRUE	成功
	FALSE	失败	
************************************/
extern bool BSPWirelessConnect(ConnectMode mode);



extern void BSPWirelessSaveConnect(void);


//无线通讯在线状态查询
extern bool BSPWirelessOnline(void);


/*********************************************
brief:		无线通讯发送数据
parameter:	
	Data 	数据
	Len		长度
return:	
	TRUE	成功
	FALSE	失败
*********************************************/
extern bool BSPWirelessWriteBytes(const uint8_t* Data, uint16_t Count);




/*********************************************
brief:		无线通讯循环程序
parameter:	void
return:		void
*********************************************/
extern void BSPWirelessRoutine(void);

extern void BSPWirelessSaveConfig(void);



//未实现
extern bool BSPWirelessDisconnet(void);
//未实现
extern uint16_t BSPWirelessBufCount(void);
//未实现
extern void BSPWirelessBufClear(uint8_t* Data, uint16_t Count);
//未实现
extern uint8_t BSPWirelessReadByte(void);
//未实现
extern void BSPWirelessReadBytes(uint8_t* Data, uint16_t Count);
//未实现
extern bool BSPWirelessAddWriteISR(CallBackISR Program);
//未实现
extern bool BSPWirelessAddReadISR(CallBackISR Program);



#endif



