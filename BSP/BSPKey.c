#include "BSPKey.h"
#include "BSPScan.h"
#include "BSPWireless.h"
#include "config.h"

/*****************************************
长按关机，短按启动扫描
返回：
2	短按
3	无动作
*****************************************/
uint8_t BSPKeyScanInContinual()
{			
	if(0 == key_press())	//按下
	{
		delay_1ms(15);					//软件去抖
		
		if(0 == key_press())		//确认是否按下
		{
			while(0 == key_press());
			
			delay_1ms(15);					//弹起去抖
			
			return PRESS;		//短按
		}
	}
	return NO_PRESS;		//无动作
}





/*********************************************
brief:		按键扫描
parameter:	void
return:		
	PRESS
	NO_PRESS
*********************************************/
extern uint32_t time;
uint8_t BSPKeyScan()
{			
	if(0 == key_press())	//按下
	{
		delay_1ms(15);					//软件去抖
		
		if(0 == key_press())			//确认是否按下
		{
			time=0;	
			return PRESS;				//短按
		}
	}
	return NO_PRESS;					//无动作
}




/*********************************************
brief:		按键初始化
parameter:	void
return:		void
*********************************************/
void BSPKeyInit()
{
	rcu_periph_clock_enable(RCU_GPIOB);
	gpio_mode_set(GPIOB,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,GPIO_PIN_5);
}




void BSPKeyWaitRelease(void)
{
	if(DevConfig.single_continual == SINGLE)			//若是single模式
	{
		while(0 == key_press())
		{
			BSPWirelessRoutine();
		}	
	}
}

