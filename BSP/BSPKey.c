#include "BSPKey.h"
#include "BSPScan.h"
#include "BSPWireless.h"
#include "config.h"

/*****************************************
�����ػ����̰�����ɨ��
���أ�
2	�̰�
3	�޶���
*****************************************/
uint8_t BSPKeyScanInContinual()
{			
	if(0 == key_press())	//����
	{
		delay_1ms(15);					//���ȥ��
		
		if(0 == key_press())		//ȷ���Ƿ���
		{
			while(0 == key_press());
			
			delay_1ms(15);					//����ȥ��
			
			return PRESS;		//�̰�
		}
	}
	return NO_PRESS;		//�޶���
}





/*********************************************
brief:		����ɨ��
parameter:	void
return:		
	PRESS
	NO_PRESS
*********************************************/
extern uint32_t time;
uint8_t BSPKeyScan()
{			
	if(0 == key_press())	//����
	{
		delay_1ms(15);					//���ȥ��
		
		if(0 == key_press())			//ȷ���Ƿ���
		{
			time=0;	
			return PRESS;				//�̰�
		}
	}
	return NO_PRESS;					//�޶���
}




/*********************************************
brief:		������ʼ��
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
	if(DevConfig.single_continual == SINGLE)			//����singleģʽ
	{
		while(0 == key_press())
		{
			BSPWirelessRoutine();
		}	
	}
}

