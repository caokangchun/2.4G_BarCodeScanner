#include "BSPEeprom.h"


#define GD32_FLASH_BASE 									0x08000000
#define FLASH_EEPROM										0x0800fc00
#define GD32_F150G8_FLASH_PAGE								64				//64K 

#define PAGE_SIZE											1024


static uint32_t page[PAGE_SIZE/4];	//保存页内容

static void FlashWriteNoCheck(uint32_t addr,uint32_t* buf, uint16_t len);


/*flash操作是以32位操作*/
void BSPEepromWrite(uint32_t addr, uint32_t *buf, uint16_t len)
{
	uint16_t i;
	
	uint8_t equal_num=0;
	uint8_t erase_flag=0;
	
	uint8_t page_pos=0;					//页位置
	uint16_t page_off=0;				//页内偏移
	uint16_t remain_in_page=0;			//页内剩余
	uint32_t add_off= addr - GD32_FLASH_BASE;		//相对flash的偏移地址
	
	
	page_pos = add_off/PAGE_SIZE;			//计算页偏移
	page_off = add_off%PAGE_SIZE/4;		//计算页内偏移地址
	remain_in_page = PAGE_SIZE/4 - page_off;	//剩余容量多少  n*16bits
	
	//flash范围外的地址
	if((GD32_FLASH_BASE > addr) || (addr >= GD32_FLASH_BASE +	PAGE_SIZE*GD32_F150G8_FLASH_PAGE))		return;
	//剩余空间太小时
	if(remain_in_page<len)	return;
	
	
	fmc_unlock();								//解锁
	
	BSPEepromRead(GD32_FLASH_BASE+page_pos*PAGE_SIZE, page, PAGE_SIZE/4);		//读出页内容	,字读取
	
	
	//检查是否需要擦除
	for(i=0;i<len;i++)
	{
//		if(page[page_off+i] != 0xffffffff) 
//			erase_flag=1;	//需要擦除
		
		
				if(page[page_off+i] != 0xffffffff) 
				{
					//已被写
					//查看是否与将要写入的内容一致
					if(page[page_off+i] != buf[i])
					{
						erase_flag=1;		
						break;
					}
					else	//相等
					{
						equal_num++;
					}
				}
				
	}

	if(erase_flag == 1)	//擦除
	{
		fmc_page_erase(GD32_FLASH_BASE+page_pos*PAGE_SIZE);
		erase_flag=0;	//擦除完成
		
		//buf赋值到page
		for(i=0;i<len;i++)
		{
			page[page_off+i] = buf[i];
		}
		
		//page写入flash
		FlashWriteNoCheck(GD32_FLASH_BASE+ page_pos*PAGE_SIZE,page, PAGE_SIZE/4);
	}
	else	//不擦除
	{
		if(len!=equal_num)		//当内容与flash不相等时，才重写
			FlashWriteNoCheck(addr,buf,len);		//从指定地址写入
	}
	
	equal_num=0;
	
	fmc_lock();	//写完上锁
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




