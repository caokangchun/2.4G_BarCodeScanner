#ifndef __LT8920_H__
#define __LT8920_H__

#include "gd32f1x0.h"





typedef enum
{
	CONNECT_NORMAL=0,
	CONNECT_FORCE
}ConnectMode;




/*********************************************
brief:		用于定时器中，或用作回调函数，产生随机数
parameter:	void
return:		void
*********************************************/
extern void LT8920IncreaseRandom(void);

/*********************************************
brief:		SPI引脚初始化，增加ISR到定时器中断函数
parameter:	void
return:		void
*********************************************/
extern void LT8920Init(void);


/*********************************************
brief:		lt8920参数设置
parameter:	void
return:		void
*********************************************/
extern void LT8920Config(void);


/*********************************************
brief:		lt8920复位
parameter:	void
return:		
	TRUE
	FALSE
*********************************************/
extern bool LT8920Reset(void);


/*********************************************
brief:		lt8920关闭
parameter:	void
return:		
	TRUE
	FALSE
*********************************************/
extern bool LT8920Close(void);

/*********************************************
brief:		lt8920唤醒
parameter:	void
return:		
	TRUE
	FALSE
*********************************************/
extern bool LT8920Open(void);


/*********************************************
brief:		lt8920建立连接
parameter:	
	CONNECT_NORMAL		初次连接
	CONNECT_FORCE		非初次连接
return:		
	TRUE
	FALSE
*********************************************/
extern bool LT8920CreateConnect(ConnectMode mode);

extern bool LT8920Send(const uint8_t* Data, uint16_t Len);

extern void LT8920DummySend(void);

extern void LT8920FactoryReset(void);

extern void LT8920SavePidSpid(void);

extern void LT8920SaveSrcDest(void);

#endif

