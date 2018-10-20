#ifndef __LT8920_H__
#define __LT8920_H__

#include "gd32f1x0.h"





typedef enum
{
	CONNECT_NORMAL=0,
	CONNECT_FORCE
}ConnectMode;




/*********************************************
brief:		���ڶ�ʱ���У��������ص����������������
parameter:	void
return:		void
*********************************************/
extern void LT8920IncreaseRandom(void);

/*********************************************
brief:		SPI���ų�ʼ��������ISR����ʱ���жϺ���
parameter:	void
return:		void
*********************************************/
extern void LT8920Init(void);


/*********************************************
brief:		lt8920��������
parameter:	void
return:		void
*********************************************/
extern void LT8920Config(void);


/*********************************************
brief:		lt8920��λ
parameter:	void
return:		
	TRUE
	FALSE
*********************************************/
extern bool LT8920Reset(void);


/*********************************************
brief:		lt8920�ر�
parameter:	void
return:		
	TRUE
	FALSE
*********************************************/
extern bool LT8920Close(void);

/*********************************************
brief:		lt8920����
parameter:	void
return:		
	TRUE
	FALSE
*********************************************/
extern bool LT8920Open(void);


/*********************************************
brief:		lt8920��������
parameter:	
	CONNECT_NORMAL		��������
	CONNECT_FORCE		�ǳ�������
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

