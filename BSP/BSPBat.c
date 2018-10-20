#include "BSPBat.h"

#include "BSPPlay.h"

static void ad0_rcc_config(void);


static void ADC0_GPIO_Configuration(void);

static void BatteryStable(void);

static uint16_t ReadBattery(void);
	
void BSPBatCheckInit()
{
	ad0_rcc_config();	//����ʱ��
	
	ADC0_GPIO_Configuration();	//AD��������
	
	adc_channel_length_config(ADC_REGULAR_CHANNEL,1);
	//ɨ��ģʽĬ�Ͽ���
	adc_special_function_config(ADC_CONTINUOUS_MODE, ENABLE);	//����ģʽ
	adc_external_trigger_source_config(ADC_REGULAR_CHANNEL,ADC_EXTTRIG_REGULAR_SWRCST);	//���ʹ��
	adc_external_trigger_config(ADC_REGULAR_CHANNEL,ENABLE);
	adc_data_alignment_config(ADC_DATAALIGN_RIGHT);			//�����Ҷ��룬��λ����
	adc_enable();
	
	adc_regular_channel_config(0,ADC_CHANNEL_0,ADC_SAMPLETIME_239POINT5);
	
	adc_calibration_enable();

	BatteryStable();
}


void BatteryStable(void)
{
	uint8_t i;
	
	for(i=0;i<80;i++)	//ad���
	{
		ReadBattery();
	}
}



uint16_t ReadBattery()
{
	static uint16_t val=0;
	
	adc_software_trigger_enable(ADC_REGULAR_CHANNEL);

	while(!adc_flag_get(ADC_FLAG_EOC));

	val = val*(DIVIDE-1)/DIVIDE + adc_regular_data_read()/DIVIDE;	//16bit ADֵ

	return 65*val/4096;
}



//*******************************************************************
//���ã���ѹ���
//��������
//���أ���
//*******************************************************************
void BSPBatLowPowerCheck(void)
{
	uint16_t bat;
	//��ص�ѹ���
	bat = ReadBattery();
	if(bat<35)	//3.5v*10
	{
		BSPPlayPowerOff();
		BSPSystemPowerOff();		//�ص�Դ	
	}
}




static void ad0_rcc_config()
{
	/* ADCCLK = PCLK2/6 */
	rcu_adc_clock_config(RCU_ADCCK_APB2_DIV8);
//	RCC_ADCCLKConfig(RCC_ADCCLK_APB2_DIV8); 

	/* Enable GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOA);
//	RCC_AHBPeriphClock_Enable(RCC_AHBPERIPH_GPIOA, ENABLE);
	
	/* Enable ADC clock */
	rcu_periph_clock_enable(RCU_ADC);
//	RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_ADC1, ENABLE);
}

						
static void ADC0_GPIO_Configuration(void)
{
	gpio_mode_set(GPIOA,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,GPIO_PIN_0);
}

