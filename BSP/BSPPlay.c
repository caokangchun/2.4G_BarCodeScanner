#include "BSPPlay.h"
#include "config.h"
#include "BSPWireless.h"


tPlayList PlayList;

void BSPPlaySuccess()
{
	play_list_write(500, 25);
}


void BSPPlayPowerOn(void)
{
	play_list_write(1000, 50);
	play_list_write(500, 50);
}




#ifdef DEBUG_SUCCESS_ERROR
extern uint32_t send_success,send_error;
#endif

void BSPPlayPowerOff()
{
#ifdef DEBUG_SUCCESS_ERROR	
	BSPEepromWrite(FLASH_SEND_SUCCESS,&send_success,1);	
	BSPEepromWrite(FLASH_SEND_ERROR,&send_error,1);		
#endif	
	
	BSPWirelessSaveConfig();	
	
	
	play_list_write(1000, 1000);
	delay_1ms(1000);

}




void BSPPlayFail()	
{
	play_list_write(460, 70);
	play_list_write(600, 90);
	play_list_write(740, 148);
}



static void beep_init()
{
	rcu_periph_clock_enable(RCU_GPIOB);
	
	
	gpio_mode_set(GPIOB,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_3);
	gpio_output_options_set(GPIOB,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_3);
	

	gpio_af_set(GPIOB,GPIO_AF_2,GPIO_PIN_3);

	
}

void time1_init_pwm()
{
	timer_parameter_struct TIMER_TimeBaseStructure;
	timer_oc_parameter_struct  TIMER_OCInitStructure;
	
	rcu_periph_clock_enable(RCU_TIMER1);
//	RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_TIMER2,ENABLE);
	
	beep_init();
	
		/* TIMER2  configuration */
	timer_deinit(TIMER1);
//	TIMER_DeInit(TIMER2);
	
	TIMER_TimeBaseStructure.prescaler     = 71;
	TIMER_TimeBaseStructure.counterdirection   = TIMER_COUNTER_UP;
	TIMER_TimeBaseStructure.period        = 1999;	
	TIMER_TimeBaseStructure.clockdivision = TIMER_CKDIV_DIV1;
	timer_init(TIMER1,&TIMER_TimeBaseStructure);
	

	/* CH2,CH3 and CH4 Configuration in PWM mode */
	TIMER_OCInitStructure.ocpolarity  = TIMER_OC_POLARITY_HIGH;
	TIMER_OCInitStructure.outputstate = TIMER_CCX_ENABLE;

	timer_channel_output_config(TIMER1, TIMER_CH_1, &TIMER_OCInitStructure);
	timer_channel_output_mode_config(TIMER1, TIMER_CH_1, TIMER_OC_MODE_PWM0);
//	TIMER_OC2_Init(TIMER2, &TIMER_OCInitStructure);

	timer_channel_output_shadow_config(TIMER1, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);
//	TIMER_OC2_Preload(TIMER2,TIMER_OC_PRELOAD_DISABLE);	
	
		/* Auto-reload preload enable */
	timer_auto_reload_shadow_enable(TIMER1);
//	TIMER_CARLPreloadConfig(TIMER2,ENABLE);
	
	/* TIMER enable counter*/
	timer_enable(TIMER1);
//	TIMER_Enable( TIMER2, ENABLE );

}




void time0_init()
{
//	NVIC_InitPara NVIC_InitStruct;
	timer_parameter_struct TIMER_TimeBaseStructure;
	
	rcu_periph_clock_enable(RCU_TIMER0);
//	RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_TIMER1,ENABLE);
	
	
	timer_deinit(TIMER0);
//	TIMER_DeInit(TIMER1);
	
	
	TIMER_TimeBaseStructure.prescaler     = 71;
	TIMER_TimeBaseStructure.counterdirection   = TIMER_COUNTER_UP;
	TIMER_TimeBaseStructure.period        = 999;	//1ms
	TIMER_TimeBaseStructure.clockdivision = TIMER_CKDIV_DIV1;
	timer_init(TIMER0,&TIMER_TimeBaseStructure);
	
	
	nvic_irq_enable(TIMER0_BRK_UP_TRG_COM_IRQn, 1, 1);
	
//	NVIC_InitStruct.NVIC_IRQ = TIMER1_BRK_UP_TRG_COM_IRQn;
//	NVIC_InitStruct.NVIC_IRQPreemptPriority=1;
//	NVIC_InitStruct.NVIC_IRQSubPriority = 1;
//	NVIC_InitStruct.NVIC_IRQEnable = ENABLE;
//	NVIC_Init(&NVIC_InitStruct);
	
		/* Auto-reload preload enable */
	timer_auto_reload_shadow_enable(TIMER0);
//	TIMER_CARLPreloadConfig(TIMER1,ENABLE);
	
	timer_interrupt_enable(TIMER0,TIMER_INT_UP);
//	TIMER_INTConfig(TIMER1 , TIMER_INT_UPDATE, ENABLE);	
	
	timer_enable(TIMER0);
//	TIMER_Enable( TIMER1, ENABLE ); 
		
}



void PlayListInit()
{
	PlayList.write=0;
	PlayList.read=0;
	
	time1_init_pwm();
	time0_init();	
}


void play_list_write(uint16_t pitch, uint32_t time)
{
	if(BEEP_ENABLE == DevConfig.beep_en_dis)
	{
		PlayList.sound[PlayList.write].pitch = pitch;	//写音�?
		PlayList.sound[PlayList.write].time = time;		//写时�?
		
		PlayList.write = (PlayList.write+1)%4;	
	}
}


void read_play_list(uint32_t *time, uint16_t *pitch)
{
	//判断是否为空
	if(PlayList.read == PlayList.write)
	{
		*pitch = 0;		//停止播放
		*time = 0;		
		return;  //empty
	}
	else
	{
//		if(0==time)		//上一音色是否播放�?
		{
			*pitch = PlayList.sound[PlayList.read].pitch;
			*time = PlayList.sound[PlayList.read].time;		
			PlayList.read = (PlayList.read+1)%4;		
		}
	}
}


void BSPPlay()
{
	static uint32_t time=0;
	static uint16_t pitch;
	
	if(0 == time--)
	{
		read_play_list(&time, &pitch);
		TIMER_CAR(TIMER1) = pitch;
		TIMER_CH1CV(TIMER1) = pitch/2;
	}
}





