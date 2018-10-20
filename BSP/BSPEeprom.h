#ifndef __BSPEEPROM_H__
#define __BSPEEPROM_H__

#include "gd32f1x0.h"
#include "config.h"





#define FLASH_SRC_DEST										0x0800fc00
#define FLASH_WIRE_OR_WIRELESS								0x0800fc04

#define FLASH_LAST_SPID										0x0800fc08	//������ʾ�ɹ���s_pid�������´ο���ʱ��ͨ���հ������ݰ�ʹ���ն��ϴ�
#define FLASH_LAST_PID										0x0800fc0c	//�����ϴιػ�ǰ���͵�PID�����ⷢ�°�ʱ��pid���


	
#define FLASH_FIRST_USE										0x0800fc10

#define FLASH_SEND_SUCCESS									0x0800fc14
#define FLASH_SEND_ERROR									0x0800fc18


#define FLASH_SINGLE_CONTINUAL								0x0800fc1c

#define FLASH_COUNTRY										0x0800fc20

#define FLASH_BEEP											0x0800fc24

#define FLASH_SLEEP											0x0800fc28

#define FLASH_PREFIX										0x0800fc2c
	
#define FLASH_CODE_ID										0x0800fc40

#define FLASH_SUFFIX										0x0800fc44

#define FLASH_HIDE											0x0800fc58	//24Byte

#define FLASH_CHARACTER										0x0800fc70	

#define FLASH_BAUD											0x0800fc74	

#define FLASH_STX_ETX										0x0800fc78

#define FLASH_END_MARK										0x0800fc7c

#define FLASH_USER_CODEID									0x0800fc80

#define FLASH_CODE_DISP										0x0800fc94

#define FLASH_INSERT_ARRAY									0x0800fc98






//extern void BSPEepromWrite(uint32_t addr, uint32_t *buf, uint8_t len);

//extern void BSPEepromRead(uint32_t addr, uint32_t *buf, uint16_t len);

extern void BSPEepromWrite(uint32_t addr, uint32_t *buf, uint16_t len);

extern void BSPEepromRead(uint32_t addr, uint32_t *buf, uint16_t len);

#endif

