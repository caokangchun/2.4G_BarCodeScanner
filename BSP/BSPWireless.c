#include <stdlib.h>
#include "gd32f1x0.h"
#include "Common.h"

#include "BSPWireless.h"
#include "lt8920.h"



/*********************************************
����: ���߳�������
������void
���أ�void	
*********************************************/
void BSPWirelessFactoryReset(void)
{
	LT8920FactoryReset();
}	


//����ͨѶ��ʼ��
void BSPWirelessInit(void)
{
	LT8920Init();
}

//����ͨѶ������
void BSPWirelessConfig(void)
{
	LT8920Config();
}	

//����ͨѶ��
bool BSPWirelessOpen(void)
{
	if(LT8920Open())
	{
		return TRUE;	
	}
	return FALSE;
}


	
//����ͨѶ�ر�
bool BSPWirelessClose(void)
{
	if(LT8920Close())
	{
		return TRUE;	
	}
	return FALSE;	
}

//����ͨѶ��λ
bool BSPWirelessReset(void)
{
	if(LT8920Reset())
	{
		return TRUE;	
	}
	return FALSE;		
}

//����ͨѶ��������
bool BSPWirelessConnect(ConnectMode mode)
{
	return LT8920CreateConnect(mode);
}


//����ƥ����Ϣ
void BSPWirelessSaveConnect(void)
{
	LT8920SaveSrcDest();
}

//����ͨѶ�Ͽ�����
bool BSPWirelessDisconnet(void)
{
	return TRUE;
}	

//����ͨѶ����״̬��ѯ
bool BSPWirelessOnline(void)
{
	return TRUE;
}

//����ͨѶ��������
bool BSPWirelessWriteBytes(const uint8_t* Data, uint16_t Count)
{
	return LT8920Send(Data, Count);
}

//����ͨѶ��ѯ���ջ����þ���
uint16_t BSPWirelessBufCount(void)
{
	return 1;
}

//����ͨѶ��ս��ջ���������
void BSPWirelessBufClear(uint8_t* Data, uint16_t Count)
{
}

//����ͨѶ��������һ�ֽ�
uint8_t BSPWirelessReadByte(void)
{
	return 1;
}	

//����ͨѶ�������ݶ��ֽ�
void BSPWirelessReadBytes(uint8_t* Data, uint16_t Count)
{
}

//����ͨѶ��ӷ�����������жϷ���
bool BSPWirelessAddWriteISR(CallBackISR Program)
{
	return(TRUE);
}

//����ͨѶ��ӽ�����������жϷ���
extern bool BSPWirelessAddReadISR(CallBackISR Program)
{
	return(TRUE);
}

//����ͨѶѭ������
void BSPWirelessRoutine(void)
{
	LT8920DummySend();
}



void BSPWirelessSaveConfig(void)
{
	LT8920SavePidSpid();
}
