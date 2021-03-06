#include <stdlib.h>
#include "Common.h"
#include "gd32f1x0.h"


#include "config.h"
#include "BSPPlay.h"
													
//*******************************************************************
//作用：系统初始化
//参数：无
//返回：无
//*******************************************************************
void BSPSystemInit(void)
{	
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2); 
}   

//*******************************************************************
//作用：总中断打开
//参数：无
//返回：无
//*******************************************************************
void BSPSystemIntOpen(void)
{
	__enable_irq();
}

//*******************************************************************
//作用：总中断关闭
//参数：无
//返回：无
//*******************************************************************
void BSPSystemIntClose(void)
{
	__disable_irq();
}

//*******************************************************************
//作用：系统复位
//参数：无
//返回：无
//*******************************************************************
void BSPSystemReset(void)
{
	fwdgt_config(100, FWDGT_PSC_DIV32);
	fwdgt_counter_reload();
	fwdgt_write_disable();
	fwdgt_enable();
	while(1);
}



//*******************************************************************
//作用：系统空闲关机
//参数：无
//返回：无
//*******************************************************************
extern uint32_t time;
void BSPSystemFreeTimeingShutdown()
{
	if(time > DevConfig.sleep_time)	//1min		1*60*1000000us
	{
		BSPPlayPowerOff();
		BSPSystemPowerOff();		//关电源	
	}
}



