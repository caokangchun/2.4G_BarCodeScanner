#include "BSPEeprom.h"


#define GD32_FLASH_BASE 									0x08000000
#define FLASH_EEPROM										0x0800fc00
#define GD32_F150G8_FLASH_PAGE								64				//64K 

#define PAGE_SIZE											1024


static uint32_t page[PAGE_SIZE/4];	//����ҳ����

static void FlashWriteNoCheck(uint32_t addr,uint32_t* buf, uint16_t len);


/*flash��������32λ����*/
void BSPEepromWrite(uint32_t addr, uint32_t *buf, uint16_t len)
{
	uint16_t i;
	
	uint8_t equal_num=0;
	uint8_t erase_flag=0;
	
	uint8_t page_pos=0;					//ҳλ��
	uint16_t page_off=0;				//ҳ��ƫ��
	uint16_t remain_in_page=0;			//ҳ��ʣ��
	uint32_t add_off= addr - GD32_FLASH_BASE;		//���flash��ƫ�Ƶ�ַ
	
	
	page_pos = add_off/PAGE_SIZE;			//����ҳƫ��
	page_off = add_off%PAGE_SIZE/4;		//����ҳ��ƫ�Ƶ�ַ
	remain_in_page = PAGE_SIZE/4 - page_off;	//ʣ����������  n*16bits
	
	//flash��Χ��ĵ�ַ
	if((GD32_FLASH_BASE > addr) || (addr >= GD32_FLASH_BASE +	PAGE_SIZE*GD32_F150G8_FLASH_PAGE))		return;
	//ʣ��ռ�̫Сʱ
	if(remain_in_page<len)	return;
	
	
	fmc_unlock();								//����
	
	BSPEepromRead(GD32_FLASH_BASE+page_pos*PAGE_SIZE, page, PAGE_SIZE/4);		//����ҳ����	,�ֶ�ȡ
	
	
	//����Ƿ���Ҫ����
	for(i=0;i<len;i++)
	{
//		if(page[page_off+i] != 0xffffffff) 
//			erase_flag=1;	//��Ҫ����
		
		
				if(page[page_off+i] != 0xffffffff) 
				{
					//�ѱ�д
					//�鿴�Ƿ��뽫Ҫд�������һ��
					if(page[page_off+i] != buf[i])
					{
						erase_flag=1;		
						break;
					}
					else	//���
					{
						equal_num++;
					}
				}
				
	}

	if(erase_flag == 1)	//����
	{
		fmc_page_erase(GD32_FLASH_BASE+page_pos*PAGE_SIZE);
		erase_flag=0;	//�������
		
		//buf��ֵ��page
		for(i=0;i<len;i++)
		{
			page[page_off+i] = buf[i];
		}
		
		//pageд��flash
		FlashWriteNoCheck(GD32_FLASH_BASE+ page_pos*PAGE_SIZE,page, PAGE_SIZE/4);
	}
	else	//������
	{
		if(len!=equal_num)		//��������flash�����ʱ������д
			FlashWriteNoCheck(addr,buf,len);		//��ָ����ַд��
	}
	
	equal_num=0;
	
	fmc_lock();	//д������
}






void BSPEepromRead(uint32_t addr, uint32_t *buf, uint16_t len)
{
	uint16_t i=0;
	uint32_t *pr = (uint32_t*)addr;
	
	for(i=0;i<len;i++)
	{
		buf[i] = pr[i];
	}
}



void FlashWriteNoCheck(uint32_t addr,uint32_t* buf, uint16_t len)
{
	uint16_t i;
	
	for(i=0;i<len;i++)
	{
		fmc_word_program(addr+i*4, buf[i]);
	}
}




