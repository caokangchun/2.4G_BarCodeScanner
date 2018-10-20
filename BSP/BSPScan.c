
#include "BSPScan.h"
#include "string.h"
#include "BSPKey.h"


#include "config.h"
extern u32 scan_delay;


const char command_request_match[]="REQUEST_MATCH";
const char command_request_match_force[]="�XMPD";
const char command_wire_wireless[]="�XMJK2";


const char command_single[]="�XMMS1";
const char command_continual[]="�XMMS5";
const char command_beep_en[]="�XMSY1";
const char command_beep_dis[]="�XMSY0";

/*����ʱ��*/
const char command_SLP_HF_MIN[]="�XMS30S";
const char command_SLP_1MIN[]="�XMS01M";
const char command_SLP_5MIN[]="�XMS05M";
const char command_SLP_10MIN[]="�XMS10M";
const char command_SLP_20MIN[]="�XMS20M";
const char command_SHUTDOWN[]="�XMGJ";


/*STX,ETX*/
const char command_STX_EN[]="�XMSTX1";
const char command_STX_DIS[]="�XMSTX0";
const char command_ETX_EN[]="�XMETX1";
const char command_ETX_DIS[]="�XMETX0";






const char command_PREFIX_EN[]="�XMTJZ2";
const char command_PREFIX_DIS[]="�XMTJZ0";
const char command_PREFIX_MODIFY[]="�XMTJZ1";



const char command_CODEID_EN[]="�XMIDZ1";
const char command_CODEID_DIS[]="�XMIDZ0";



const char command_CODE_TYPE_DISP_EN[]="�XMMX1";
const char command_CODE_TYPE_DISP_DIS[]="�XMMX0";




const char command_SUFFIX_CR[]="�XMHZ1";
const char command_SUFFIX_LF[]="�XMHZ2";
const char command_SUFFIX_CR_LF[]="�XMHZ3";
const char command_SUFFIX_HT[]="�XMHZ4";
const char command_SUFFIX_SPACE[]="�XMHZ5";
const char command_SUFFIX_EMPTY[]="�XMHZ0";


const char command_SUFFIX_EN[]="�XMTJY2";
const char command_SUFFIX_DIS[]="�XMTJY0";
const char command_SUFFIX_MODIFY[]="�XMTJY1";

/*ͷ����*/
const char command_HEAD_HIDE_EN[]="�XMYCZ2";
const char command_HEAD_HIDE_DIS[]="�XMYCZ0";
const char command_HEAD_HIDE_LEN[]="�XMYCZ1";
/*β����*/
const char command_TAIL_HIDE_EN[]="�XMYCY2";
const char command_TAIL_HIDE_DIS[]="�XMYCY0";
const char command_TAIL_HIDE_LEN[]="�XMYCY1";

/*�ַ�������*/
const char command_STRING_HIDE_EN[]="�XMSC2";
const char command_STRING_HIDE_DIS[]="�XMSC0";
const char command_STRING_HIDE_SET[]="�XMSC1";


/*�ַ�ת��*/
const char command_CHAR_NO_ACTION[]="�XMZMHF";
const char command_UPPERCASE_CONVERSION[]="�XMZMDX";
const char command_LOWERCASE_CONVERSION[]="�XMZMXX";
const char command_CHARACTER_CONVERSION[]="�XMZMFZ";


/*�ַ������*/
const char command_INSERT1[]="�XMCR1";
const char command_INSERT2[]="�XMCR2";
const char command_INSERT3[]="�XMCR3";
const char command_INSERT4[]="�XMCR4";
const char command_INSERT5[]="�XMCR5";
const char command_INSERT6[]="�XMCR6";
const char command_INSERT7[]="�XMCR7";
const char command_INSERT8[]="�XMCR8";
const char command_INSERT_SET[]="�XMCR9";
const char command_INSERT_DIS[]="�XMCR0";









/*������*/
const char command_BAUD_1200[]="�XMBTL1";
const char command_BAUD_4800[]="�XMBTL2";
const char command_BAUD_9600[]="�XMBTL3";
const char command_BAUD_14400[]="�XMBTL4";
const char command_BAUD_19200[]="�XMBTL5";
const char command_BAUD_38400[]="�XMBTL6";
const char command_BAUD_57600[]="�XMBTL7";
const char command_BAUD_115200[]="�XMBTL8";



/*EAN13*/
const char command_EAN13_EN[]="�XMd01";
const char command_EAN13_DIS[]="�XMd00";

/*EAN8*/
const char command_EAN8_EN[]="�XMg01";
const char command_EAN8_DIS[]="�XMg00";


/*UPCA*/
const char command_UPCA_EN[]="�XMc01";
const char command_UPCA_DIS[]="�XMc00";


/*UPCE0*/
const char command_UPCE0_EN[]="�XMc001";
const char command_UPCE0_DIS[]="�XMc000";


/*UPCE1*/
const char command_UPCE1_EN[]="�XMc101";
const char command_UPCE1_DIS[]="�XMc100";


/*CODE128*/
const char command_CODE128_EN[]="�XMj01";
const char command_CODE128_DIS[]="�XMj00";
const char command_CODE128_LEAST0[]="�XMj02";	//���0
const char command_CODE128_LEAST4[]="�XMj03";	//���4
const char command_CODE128_MOST32[]="�XMj04";	//�32
const char command_CODE128_MOST255[]="�XMj05";	//�255

/*CODE39*/
const char command_CODE39_EN[]="�XMb01";
const char command_CODE39_DIS[]="�XMb00";
const char command_CODE39_LEAST0[]="�XMb02";	//���0
const char command_CODE39_LEAST4[]="�XMb03";	//���4
const char command_CODE39_MOST32[]="�XMb04";	//�32
const char command_CODE39_MOST255[]="�XMb05";	//�255

/*CODE93*/
const char command_CODE93_EN[]="�XMy01";
const char command_CODE93_DIS[]="�XMy00";
const char command_CODE93_LEAST0[]="�XMy02";	//���0
const char command_CODE93_LEAST4[]="�XMy03";	//���4
const char command_CODE93_MOST32[]="�XMy04";	//�32
const char command_CODE93_MOST255[]="�XMy05";	//�255


/*CODEBAR*/
const char command_CODEBAR_EN[]="�XMa01";
const char command_CODEBAR_DIS[]="�XMa00";
const char command_CODEBAR_LEAST0[]="�XMa02";	//���0
const char command_CODEBAR_LEAST4[]="�XMa03";	//���4
const char command_CODEBAR_MOST32[]="�XMa04";	//�32
const char command_CODEBAR_MOST255[]="�XMa05";	//�255
const char command_CODEBAR_SEND_NON[]="�XMa06";			//������ֹ��
const char command_CODEBAR_SEND_SRT_END[]="�XMa07";		//����ֹ��

/*QR*/
const char command_QR_EN[]="�XMQ01";
const char command_QR_DIS[]="�XMQ00";

/*INTERLEAVED 25*/
const char command_INTERLEAVED25_EN[]="�XMe01";
const char command_INTERLEAVED25_DIS[]="�XMe00";
const char command_INTERLEAVED25_LEAST0[]="�XMe02";	//���0
const char command_INTERLEAVED25_LEAST4[]="�XMe03";	//���4
const char command_INTERLEAVED25_MOST32[]="�XMe04";	//�32
const char command_INTERLEAVED25_MOST255[]="�XMe05";	//�255

/*INDUSTRIAL 25*/
const char command_INDUSTRIAL25_EN[]="�XMi01";
const char command_INDUSTRIAL25_DIS[]="�XMi00";
const char command_INDUSTRIAL25_LEAST0[]="�XMi02";	//���0
const char command_INDUSTRIAL25_LEAST4[]="�XMi03";	//���4
const char command_INDUSTRIAL25_MOST32[]="�XMi04";	//�32
const char command_INDUSTRIAL25_MOST255[]="�XMi05";	//�255

/*MATRIX25*/
const char command_MATRIX25_EN[]="�XMv01";
const char command_MATRIX25_DIS[]="�XMv00";
const char command_MATRIX25_LEAST0[]="�XMv02";	//���0
const char command_MATRIX25_LEAST4[]="�XMv03";	//���4
const char command_MATRIX25_MOST32[]="�XMv04";	//�32
const char command_MATRIX25_MOST255[]="�XMv05";	//�255

/*CODE11*/
const char command_CODE11_EN[]="�XMz01";
const char command_CODE11_DIS[]="�XMz00";
const char command_CODE11_LEAST0[]="�XMz02";	//���0
const char command_CODE11_LEAST4[]="�XMz03";	//���4
const char command_CODE11_MOST32[]="�XMz04";	//�32
const char command_CODE11_MOST255[]="�XMz05";	//�255

/*MSI*/
const char command_MSI_EN[]="�XMm01";
const char command_MSI_DIS[]="�XMm00";
const char command_MSI_LEAST0[]="�XMm02";	//���0
const char command_MSI_LEAST4[]="�XMm03";	//���4
const char command_MSI_MOST32[]="�XMm04";	//�32
const char command_MSI_MOST255[]="�XMm05";	//�255

/*RSS*/
const char command_RSS14_EN[]="�XMR1401";	//RSS-14
const char command_RSS14_DIS[]="�XMR1400";
const char command_RSS_LMT_EN[]="�XMRL01";		//RSS�޶�ʽ
const char command_RSS_LMT_DIS[]="�XMRL00";
const char command_RSS_EXT_EN[]="�XMRE01";		//RSS��չʽ
const char command_RSS_EXT_DIS[]="�XMRE00";
const char command_RSS_LEAST0[]="�XMR02";	//���0
const char command_RSS_LEAST4[]="�XMR03";	//���4
const char command_RSS_MOST32[]="�XMR04";	//�32
const char command_RSS_MOST255[]="�XMR05";	//�255



/*DM*/
const char command_DM_EN[]="�XMu01";
const char command_DM_DIS[]="�XMu00";

/*PDF417*/
const char command_PDF417_EN[]="�XMr01";
const char command_PDF417_DIS[]="�XMr00";


	












const char command_America[]="�XMMG";
const char command_Germany[]="�XMDG";
const char command_France_CAN[]="�XMFJ";
const char command_France[]="�XMFG";
const char command_Itay[]="�XMYD";
const char command_Portuguese[]="�XMPT";
const char command_Portuguese_BR[]="�XMPB";
const char command_Spanish[]="�XMXG";





static uint8_t scan_complete=0;
uint8_t get_QRmodule_status=0;

Prefix prefix_shadow;
Suffix suffix_shadow;
Hide   hide_shadow;





uint8_t TxBuffer[TXBUFFER_SIZE];
uint8_t QR_sta[10];
uint8_t QR_len=0;

uint8_t ch;

extern void BSPPlaySuccess(int n);



extern uint8_t BSPScanGetComplete(void)
{
	return scan_complete;
}

extern void BSPScanSetComplete(uint8_t val)
{
	scan_complete = val;
}


/*****************************************
brief:		��ȡ���ͻ�����ָ��
parameter:	void
return:		uint8_t*
*****************************************/
extern uint8_t* GetTxBuf(void)
{
	return TxBuffer;
}




void BSPScannerFirstSet(void)
{
	uint32_t addr;
	
	BSPEepromRead(FLASH_FIRST_USE,&addr,1);	//��һ����
	if(addr == 0xffffffff)
	{
		noID_noSuffix_uartProtocal();	//codeID��ֹ����Э��������޺�׺���	Ĭ�ϲ�����115200���Ѿ����ú���
	
		utf8_input_output();	//utf-8���룬�����ʽ

		codeID_character_set();	
		
		ConfigComplete();
	}

}



void BSPScanInit()
{
	
	rcu_periph_clock_enable(RCU_GPIOB);
	gpio_mode_set(GPIOB,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_PIN_1| GPIO_PIN_4);
	gpio_output_options_set(GPIOB,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_1| GPIO_PIN_4);
	
	scan_idle();

	uart_init();
}


void uart_init()
{


	
	rcu_periph_clock_enable(RCU_USART0);

	
	rcu_periph_clock_enable(RCU_GPIOA);
	
	gpio_mode_set(GPIOA,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_9);
	gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_9);

	gpio_mode_set(GPIOA,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_10);	
	gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_10);	

		gpio_af_set(GPIOA,GPIO_AF_1,GPIO_PIN_9);
		gpio_af_set(GPIOA,GPIO_AF_1,GPIO_PIN_10);
		


	usart_deinit(USART0);

	usart_baudrate_set(USART0, DevConfig.baud_rate);
	usart_word_length_set(USART0,USART_WL_8BIT);
	usart_stop_bit_set(USART0, USART_STB_1BIT);
	usart_parity_config(USART0,USART_PM_NONE);
	/*Ӳ����disable*/
	usart_hardware_flow_rts_config(USART0,USART_RTS_DISABLE);
	usart_hardware_flow_cts_config(USART0,USART_CTS_DISABLE);
	/*ʹ�ܷ��ͽ���*/
	usart_receive_config(USART0,USART_RECEIVE_ENABLE);
	usart_transmit_config(USART0,USART_TRANSMIT_ENABLE);
	

	NVIC_Configuration();	//�ж����ȼ�
	
	usart_overrun_disable(USART0);
	
	usart_interrupt_enable(USART0,USART_INT_RBNE);
	
	usart_enable(USART0);
}



void NVIC_Configuration(void)
{
	nvic_irq_enable(USART0_IRQn, 1, 1);
}







static void sending_in_scanning()
{
	uint16_t count=0;
	static uint8_t pause = 0;
	
	scan_work();		//�͵�ƽ
	
	if(DevConfig.single_continual == CONTINUAL)		
	{
		if(PRESS == BSPKeyScanInContinual())	
		{
			pause ^= 0x01;		//��ͣ��ʼ
		}				
		
		if(pause == 0)
		{
			while((!scan_complete))		//������������ɨ��ʱ,��Ҫ���Ϳհ�
			{
				delay_1ms(1);
				if(count++ >1000)		//200
				{
					scan_idle();		//��ʱ�п���ɨ�����
//					delay_1ms(300);		//��ʱûɨ����ʱ��Ҫ��ʱ
					break;
				}

				
				
				if(PRESS == BSPKeyScanInContinual())	
				{
					pause ^= 0x01;		//��ͣ��ʼ
					
					break;
				}				
			}	
		}
		else
		{
			scan_idle();
		}

	}
	else
	{
		while((!scan_complete) && (0==key_press()))		//������������ɨ��ʱ,��Ҫ���Ϳհ�
		{
			BSPWatchDogReset();
			delay_1ms(5);
			if(count++ >1000)		
			{
				break;
			}
		}
		scan_idle();		//�ߵ�ƽ	
	}	
		
}



uint8_t scan_start()
{	
	sending_in_scanning();
	

	return scan_complete;
}


void BSPScan()
{ 
	if(DevConfig.single_continual == SINGLE)		//�ֶ�ģʽ
	{
		//������

		if(!scan_complete)		//û����
		{
			if(PRESS == BSPKeyScan())
			{
				scan_start();		
			}			
		}
	}
	else		//����ģʽ
	{
		//������

		if(!scan_complete)		//û����
		{	
			scan_start();					
		}		
	}
}






//uint8_t parsing()
//{

//		if(1 == parse_cmd(command_request_match))
//		{
//			return _REQUEST_MATCH;
//		}
//			
//		
//		
//		if(1 == parse_cmd(command_request_match_force))
//		{
//			return _REQUEST_MATCH_FORCE;
//		}
//			
//		
//		
//		if(1 == parse_cmd(command_wire_wireless))
//		{
//			return _WIRE_WIRELESS;
//		}

//	
//		if(1 == parse_cmd(command_single))
//		{
//			return _SINGLE;
//		}
//		
//		if(1 == parse_cmd(command_continual))
//		{
//			return _CONTINUAL;
//		}	

//		if(1 == parse_cmd(command_beep_en))
//		{
//			return _BEEP_EN;
//		}
//		
//		if(1 == parse_cmd(command_beep_dis))
//		{
//			return _BEEP_DIS;
//		}		

//		if(1 == parse_cmd(command_SLP_HF_MIN))
//		{
//			return _SLEEP_HF_MIN;
//		}
//		
//		if(1 == parse_cmd(command_SLP_1MIN))
//		{
//			return _SLEEP_1MIN;
//		}

//		if(1 == parse_cmd(command_SLP_5MIN))
//		{
//			return _SLEEP_5MIN;
//		}	

//		if(1 == parse_cmd(command_SLP_10MIN))
//		{
//			return _SLEEP_10MIN;
//		}	

//		if(1 == parse_cmd(command_SLP_20MIN))
//		{
//			return _SLEEP_20MIN;
//		}	

//		if(1 == parse_cmd(command_SHUTDOWN))
//		{
//			return _SHUTDOWN;
//		}

//		if(1 == parse_cmd(command_STX_EN))
//		{
//			return _STX_EN;
//		}

//		if(1 == parse_cmd(command_STX_DIS))
//		{
//			return _STX_DIS;
//		}


//		if(1 == parse_cmd(command_ETX_EN))
//		{
//			return _ETX_EN;
//		}

//		
//		if(1 == parse_cmd(command_ETX_DIS))
//		{
//			return _ETX_DIS;
//		}



//		

//		if(1 == parse_cmd(command_PREFIX_EN))
//		{
//			return _PREFIX_EN;
//		}

//		if(1 == parse_cmd(command_PREFIX_DIS))
//		{
//			return _PREFIX_DIS;
//		}


//		if(1 == parse_cmd(command_PREFIX_MODIFY))
//		{
//			return _PREFIX_MODIFY;
//		}	

//		if(1 == parsing_prefix_MDYCHAR())
//		{
//			return _PREFIX_MDYCHAR;
//		}

//		if(1 == parse_cmd(command_CODEID_EN))
//		{
//			return _CODEID_EN;
//		}

//		if(1 == parse_cmd(command_CODEID_DIS))
//		{
//			return _CODEID_DIS;
//		}



//		if(1 == parse_cmd(command_CODE_TYPE_DISP_EN))
//		{
//			return _CODE_TYPE_DISP_EN;
//		}

//		if(1 == parse_cmd(command_CODE_TYPE_DISP_DIS))
//		{
//			return _CODE_TYPE_DISP_DIS;
//		}
//		






//		if(1 == parse_cmd(command_SUFFIX_CR))
//		{
//			return _END_CR;
//		}

//		if(1 == parse_cmd(command_SUFFIX_LF))
//		{
//			return _END_LF;
//		}
//		
//		if(1 == parse_cmd(command_SUFFIX_CR_LF))
//		{
//			return _END_CR_LF;
//		}

//		if(1 == parse_cmd(command_SUFFIX_HT))
//		{
//			return _END_HT;
//		}		

//		if(1 == parse_cmd(command_SUFFIX_SPACE))
//		{
//			return _END_SPACE;
//		}

//		if(1 == parse_cmd(command_SUFFIX_EMPTY))
//		{
//			return _END_EMPTY;
//		}

//		

//		


//		if(1 == parse_cmd(command_SUFFIX_EN))
//		{
//			return _SUFFIX_EN;
//		}

//		if(1 == parse_cmd(command_SUFFIX_DIS))
//		{
//			return _SUFFIX_DIS;
//		}	

//		if(1 == parse_cmd(command_SUFFIX_MODIFY))
//		{
//			return _SUFFIX_MODIFY;
//		}	

//		if(1 == parsing_suffix_MDYCHAR())
//		{
//			return _SUFFIX_MDYCHAR;
//		}		


//		if(1 == parse_cmd(command_HEAD_HIDE_EN))
//		{
//			return _HEAD_HIDE_EN;
//		}

//		if(1 == parse_cmd(command_HEAD_HIDE_DIS))
//		{
//			return _HEAD_HIDE_DIS;
//		}	

//		if(1 == parse_cmd(command_HEAD_HIDE_LEN))	//����ͷ����
//		{
//			return _HEAD_HIDE_LEN_MODIFY;
//		}	

//		if(1 == parsing_head_len_MDYCHAR())//��������
//		{
//			return _HEAD_HIDE_LEN_MOYCHAR;
//		}	

//		

//		if(1 == parse_cmd(command_TAIL_HIDE_EN))
//		{
//			return _TAIL_HIDE_EN;
//		}

//		if(1 == parse_cmd(command_TAIL_HIDE_DIS))
//		{
//			return _TAIL_HIDE_DIS;
//		}	

//		if(1 == parse_cmd(command_TAIL_HIDE_LEN))	//����ͷ����
//		{
//			return _TAIL_HIDE_LEN_MODIFY;
//		}	

//		if(1 == parsing_tail_len_MDYCHAR())//��������
//		{
//			return _TAIL_HIDE_LEN_MOYCHAR;
//		}	

///*�����ַ���*/

//		if(1 == parse_cmd(command_STRING_HIDE_EN))
//		{
//			return _STRING_HIDE_EN;
//		}

//		if(1 == parse_cmd(command_STRING_HIDE_DIS))
//		{
//			return _STRING_HIDE_DIS;
//		}	

//		if(1 == parse_cmd(command_STRING_HIDE_SET))	//����ͷ����
//		{
//			return _STRING_HIDE_MODIFY;
//		}	

//		if(1 == parsing_string_MDYCHAR())//��������
//		{
//			return _STRING_HIDE_MOYCHAR;
//		}	



//		if(1 == parse_cmd(command_CHAR_NO_ACTION))	//�޶���
//		{
//			return _NO_CONVERSION;
//		}	


//		if(1 == parse_cmd(command_UPPERCASE_CONVERSION))	//��дת��
//		{
//			return _UPPERCASE_CONVERSION;
//		}	

//		if(1 == parse_cmd(command_LOWERCASE_CONVERSION))	//Сдת��
//		{
//			return _LOWERCASE_CONVERSION;
//		}		

//		if(1 == parse_cmd(command_CHARACTER_CONVERSION))	//��Сдת��
//		{
//			return _CHARACTER_CONVERSION;
//		}



///****************************/
//		if(1 == parse_cmd(command_INSERT1))					//������1
//		{
//			grp_index = 0;
//			if(0==insert_character_set_flag)	//����λ��pos
//			{
//				return _INSERT1_POS;
//			}
//			else	//���ò����ַ�
//			{
//				return _INSERT1_CHAR;
//			}

//		}


//		if(1 == parse_cmd(command_INSERT2))					////������2
//		{
//			grp_index = 1;
//			if(0==insert_character_set_flag)	//����λ��pos
//			{
//				return _INSERT2_POS;
//			}
//			else	//���ò����ַ�
//			{
//				return _INSERT2_CHAR;
//			}		
//		}


//		if(1 == parse_cmd(command_INSERT3))	
//		{
//			grp_index = 2;
//			if(0==insert_character_set_flag)	//����λ��pos
//			{
//				return _INSERT3_POS;
//			}
//			else	//���ò����ַ�
//			{
//				return _INSERT3_CHAR;
//			}

//		}

//		if(1 == parse_cmd(command_INSERT4))	
//		{
//			grp_index = 3;
//			if(0==insert_character_set_flag)	//����λ��pos
//			{
//				return _INSERT4_POS;
//			}
//			else	//���ò����ַ�
//			{
//				return _INSERT4_CHAR;
//			}

//		}


//		if(1 == parse_cmd(command_INSERT5))	
//		{
//			grp_index = 4;
//			if(0==insert_character_set_flag)	//����λ��pos
//			{
//				return _INSERT5_POS;
//			}
//			else	//���ò����ַ�
//			{
//				return _INSERT5_CHAR;
//			}

//		}


//		if(1 == parse_cmd(command_INSERT6))
//		{
//			grp_index = 5;
//			if(0==insert_character_set_flag)	//����λ��pos
//			{
//				return _INSERT6_POS;
//			}
//			else	//���ò����ַ�
//			{
//				return _INSERT6_CHAR;
//			}

//		}


//		if(1 == parse_cmd(command_INSERT7))	
//		{
//			grp_index = 6;
//			if(0==insert_character_set_flag)	//����λ��pos
//			{
//				return _INSERT7_POS;
//			}
//			else	//���ò����ַ�
//			{
//				return _INSERT7_CHAR;
//			}

//		}


//		if(1 == parse_cmd(command_INSERT8))	
//		{
//			grp_index = 7;
//			if(0==insert_character_set_flag)	//����λ��pos
//			{
//				return _INSERT8_POS;
//			}
//			else	//���ò����ַ�
//			{
//				return _INSERT8_CHAR;
//			}

//		}


//		if(1 == parse_cmd(command_INSERT_SET))	
//		{
//			return _INSERT_SET;
//		}

//		if(1 == parse_cmd(command_INSERT_DIS))	
//		{
//			return _INSERT_DIS;
//		}


//		if(1 == parsing_insert_string_MDYCHAR())//��������
//		{
//			return _INSERT_STRING_MDYCHAR;
//		}	

//		if(1 == parsing_insert_pos_MDY())//��������
//		{
//			return _INSERT_POS_MDY;
//		}
























//		

///***������****/
//		if(1 == parse_cmd(command_BAUD_1200))	//��Сдת��
//		{
//			return _1200;
//		}

//		if(1 == parse_cmd(command_BAUD_4800))	//��Сдת��
//		{
//			return _4800;
//		}

//		if(1 == parse_cmd(command_BAUD_9600))	//��Сдת��
//		{
//			return _9600;
//		}

//		if(1 == parse_cmd(command_BAUD_14400))	//��Сдת��
//		{
//			return _14400;
//		}

//		if(1 == parse_cmd(command_BAUD_19200))	//��Сдת��
//		{
//			return _19200;
//		}
//		
//		if(1 == parse_cmd(command_BAUD_38400))	//��Сдת��
//		{
//			return _38400;
//		}

//		if(1 == parse_cmd(command_BAUD_57600))	//��Сдת��
//		{
//			return _57600;
//		}

//		if(1 == parse_cmd(command_BAUD_115200))	//��Сдת��
//		{
//			return _115200;
//		}


///*EAN13*/
//		if(1 == parse_cmd(command_EAN13_EN))	
//		{
//			return _EAN13_EN;
//		}

//		if(1 == parse_cmd(command_EAN13_DIS))	
//		{
//			return _EAN13_DIS;
//		}		

///*EAN8*/
//		if(1 == parse_cmd(command_EAN8_EN))	
//		{
//			return _EAN8_EN;
//		}

//		if(1 == parse_cmd(command_EAN8_DIS))	
//		{
//			return _EAN8_DIS;
//		}		

///*UPCA*/
//		if(1 == parse_cmd(command_UPCA_EN))	
//		{
//			return _UPCA_EN;
//		}

//		if(1 == parse_cmd(command_UPCA_DIS))	
//		{
//			return _UPCA_DIS;
//		}	

///*UPCE0*/
//		if(1 == parse_cmd(command_UPCE0_EN)) 
//		{
//			return _UPCE0_EN;
//		}

//		if(1 == parse_cmd(command_UPCE0_DIS))	
//		{
//			return _UPCE0_DIS;
//		}	


///*UPCE1*/
//		if(1 == parse_cmd(command_UPCE1_EN)) 
//		{
//			return _UPCE1_EN;
//		}

//		if(1 == parse_cmd(command_UPCE1_DIS))	
//		{
//			return _UPCE1_DIS;
//		}	

///*CODE128*/
//		if(1 == parse_cmd(command_CODE128_EN)) 
//		{
//			return _CODE128_EN;
//		}

//		if(1 == parse_cmd(command_CODE128_DIS))	
//		{
//			return _CODE128_DIS;
//		}	

//		if(1 == parse_cmd(command_CODE128_LEAST0)) 
//		{
//			return _CODE128_LEAST0;
//		}

//		if(1 == parse_cmd(command_CODE128_LEAST4))	
//		{
//			return _CODE128_LEAST4;
//		}	

//		if(1 == parse_cmd(command_CODE128_MOST32)) 
//		{
//			return _CODE128_MOST32;
//		}

//		if(1 == parse_cmd(command_CODE128_MOST255))	
//		{
//			return _CODE128_MOST255;
//		}	

///*CODE39*/
//		if(1 == parse_cmd(command_CODE39_EN)) 
//		{
//			return _CODE39_EN;
//		}

//		if(1 == parse_cmd(command_CODE39_DIS)) 
//		{
//			return _CODE39_DIS;
//		}	

//		if(1 == parse_cmd(command_CODE39_LEAST0)) 
//		{
//			return _CODE39_LEAST0;
//		}

//		if(1 == parse_cmd(command_CODE39_LEAST4))	
//		{
//			return _CODE39_LEAST4;
//		}	

//		if(1 == parse_cmd(command_CODE39_MOST32)) 
//		{
//			return _CODE39_MOST32;
//		}

//		if(1 == parse_cmd(command_CODE39_MOST255)) 
//		{
//			return _CODE39_MOST255;
//		}	

///*CODE93*/

//		if(1 == parse_cmd(command_CODE93_EN)) 
//		{
//			return _CODE93_EN;
//		}

//		if(1 == parse_cmd(command_CODE93_DIS)) 
//		{
//			return _CODE93_DIS;
//		}	

//		if(1 == parse_cmd(command_CODE93_LEAST0)) 
//		{
//			return _CODE93_LEAST0;
//		}

//		if(1 == parse_cmd(command_CODE93_LEAST4))	
//		{
//			return _CODE93_LEAST4;
//		}	

//		if(1 == parse_cmd(command_CODE93_MOST32)) 
//		{
//			return _CODE93_MOST32;
//		}

//		if(1 == parse_cmd(command_CODE93_MOST255)) 
//		{
//			return _CODE93_MOST255;
//		}	

///*CODEBAR*/
//		if(1 == parse_cmd(command_CODEBAR_EN)) 
//		{
//			return _CODEBAR_EN;
//		}

//		if(1 == parse_cmd(command_CODEBAR_DIS)) 
//		{
//			return _CODEBAR_DIS;
//		}	

//		if(1 == parse_cmd(command_CODEBAR_LEAST0)) 
//		{
//			return _CODEBAR_LEAST0;
//		}

//		if(1 == parse_cmd(command_CODEBAR_LEAST4))	
//		{
//			return _CODEBAR_LEAST4;
//		}	

//		if(1 == parse_cmd(command_CODEBAR_MOST32)) 
//		{
//			return _CODEBAR_MOST32;
//		}

//		if(1 == parse_cmd(command_CODEBAR_MOST255)) 
//		{
//			return _CODEBAR_MOST255;
//		}
//		
//		if(1 == parse_cmd(command_CODEBAR_SEND_NON)) 
//		{
//			return _CODEBAR_SEND_NON;
//		}

//		if(1 == parse_cmd(command_CODEBAR_SEND_SRT_END)) 
//		{
//			return _CODEBAR_SEND_SRT_END;
//		}		


///*QR*/
//		if(1 == parse_cmd(command_QR_EN)) 
//		{
//			return _QR_EN;
//		}

//		if(1 == parse_cmd(command_QR_DIS)) 
//		{
//			return _QR_DIS;
//		}	


///*INTERLEAVED 25*/
//		if(1 == parse_cmd(command_INTERLEAVED25_EN)) 
//		{
//			return _INTERLEAVED25_EN;
//		}

//		if(1 == parse_cmd(command_INTERLEAVED25_DIS)) 
//		{
//			return _INTERLEAVED25_DIS;
//		}	

//		if(1 == parse_cmd(command_INTERLEAVED25_LEAST0)) 
//		{
//			return _INTERLEAVED25_LEAST0;
//		}

//		if(1 == parse_cmd(command_INTERLEAVED25_LEAST4))	
//		{
//			return _INTERLEAVED25_LEAST4;
//		}	

//		if(1 == parse_cmd(command_INTERLEAVED25_MOST32)) 
//		{
//			return _INTERLEAVED25_MOST32;
//		}

//		if(1 == parse_cmd(command_INTERLEAVED25_MOST255)) 
//		{
//			return _INTERLEAVED25_MOST255;
//		}	

///*INDUSTRIAL 25*/
//		if(1 == parse_cmd(command_INDUSTRIAL25_EN)) 
//		{
//			return _INDUSTRIAL25_EN;
//		}

//		if(1 == parse_cmd(command_INDUSTRIAL25_DIS)) 
//		{
//			return _INDUSTRIAL25_DIS;
//		}	

//		if(1 == parse_cmd(command_INDUSTRIAL25_LEAST0)) 
//		{
//			return _INDUSTRIAL25_LEAST0;
//		}

//		if(1 == parse_cmd(command_INDUSTRIAL25_LEAST4))	
//		{
//			return _INDUSTRIAL25_LEAST4;
//		}	

//		if(1 == parse_cmd(command_INDUSTRIAL25_MOST32)) 
//		{
//			return _INDUSTRIAL25_MOST32;
//		}

//		if(1 == parse_cmd(command_INDUSTRIAL25_MOST255)) 
//		{
//			return _INDUSTRIAL25_MOST255;
//		}	

///*MATRIX25*/
//		if(1 == parse_cmd(command_MATRIX25_EN)) 
//		{
//			return _MATRIX25_EN;
//		}

//		if(1 == parse_cmd(command_MATRIX25_DIS)) 
//		{
//			return _MATRIX25_DIS;
//		}	

//		if(1 == parse_cmd(command_MATRIX25_LEAST0)) 
//		{
//			return _MATRIX25_LEAST0;
//		}

//		if(1 == parse_cmd(command_MATRIX25_LEAST4)) 
//		{
//			return _MATRIX25_LEAST4;
//		}	

//		if(1 == parse_cmd(command_MATRIX25_MOST32)) 
//		{
//			return _MATRIX25_MOST32;
//		}

//		if(1 == parse_cmd(command_MATRIX25_MOST255)) 
//		{
//			return _MATRIX25_MOST255;
//		}	


///*CODE11*/
//		if(1 == parse_cmd(command_CODE11_EN)) 
//		{
//			return _CODE11_EN;
//		}

//		if(1 == parse_cmd(command_CODE11_DIS)) 
//		{
//			return _CODE11_DIS;
//		}	

//		if(1 == parse_cmd(command_CODE11_LEAST0)) 
//		{
//			return _CODE11_LEAST0;
//		}

//		if(1 == parse_cmd(command_CODE11_LEAST4)) 
//		{
//			return _CODE11_LEAST4;
//		}	

//		if(1 == parse_cmd(command_CODE11_MOST32)) 
//		{
//			return _CODE11_MOST32;
//		}

//		if(1 == parse_cmd(command_CODE11_MOST255)) 
//		{
//			return _CODE11_MOST255;
//		}	


///*MSI*/
//		if(1 == parse_cmd(command_MSI_EN)) 
//		{
//			return _MSI_EN;
//		}

//		if(1 == parse_cmd(command_MSI_DIS)) 
//		{
//			return _MSI_DIS;
//		}	

//		if(1 == parse_cmd(command_MSI_LEAST0)) 
//		{
//			return _MSI_LEAST0;
//		}

//		if(1 == parse_cmd(command_MSI_LEAST4)) 
//		{
//			return _MSI_LEAST4;
//		}	

//		if(1 == parse_cmd(command_MSI_MOST32)) 
//		{
//			return _MSI_MOST32;
//		}

//		if(1 == parse_cmd(command_MSI_MOST255)) 
//		{
//			return _MSI_MOST255;
//		}	


///*RSS*/
//		if(1 == parse_cmd(command_RSS14_EN)) 
//		{
//			return _RSS14_EN;
//		}

//		if(1 == parse_cmd(command_RSS14_DIS)) 
//		{
//			return _RSS14_DIS;
//		}	

//		if(1 == parse_cmd(command_RSS_LMT_EN)) 
//		{
//			return _RSS_LMT_EN;
//		}

//		if(1 == parse_cmd(command_RSS_LMT_DIS)) 
//		{
//			return _RSS_LMT_DIS;
//		}	

//		if(1 == parse_cmd(command_RSS_EXT_EN)) 
//		{
//			return _RSS_EXT_EN;
//		}

//		if(1 == parse_cmd(command_RSS_EXT_DIS)) 
//		{
//			return _RSS_EXT_DIS;
//		}			

//		if(1 == parse_cmd(command_RSS_LEAST0)) 
//		{
//			return _RSS_LEAST0;
//		}

//		if(1 == parse_cmd(command_RSS_LEAST4)) 
//		{
//			return _RSS_LEAST4;
//		}	

//		if(1 == parse_cmd(command_RSS_MOST32)) 
//		{
//			return _RSS_MOST32;
//		}

//		if(1 == parse_cmd(command_RSS_MOST255)) 
//		{
//			return _RSS_MOST255;
//		}

///*DM*/
//		if(1 == parse_cmd(command_DM_EN)) 
//		{
//			return _DM_EN;
//		}

//		if(1 == parse_cmd(command_DM_DIS)) 
//		{
//			return _DM_DIS;
//		}	


///*PDF417*/
//		if(1 == parse_cmd(command_PDF417_EN)) 
//		{
//			return _PDF417_EN;
//		}

//		if(1 == parse_cmd(command_PDF417_DIS)) 
//		{
//			return _PDF417_DIS;
//		}	

////const char command_PDF417_EN[]="�XMr01";
////const char command_PDF417_DIS[]="�XMr00";		



//		
//		
//		if(1 == parse_cmd(command_America))
//		{
//			return _AMERICA;
//		}		


//		if(1 == parse_cmd(command_Germany))
//		{
//			return _GERMANY;
//		}	

//		
//		if(1 == parse_cmd(command_France_CAN))
//		{
//			return _FRANCE_CAN;
//		}			
//		
//		
//		if(1 == parse_cmd(command_France))
//		{
//			return _FRANCE;
//		}			
//		

//		if(1 == parse_cmd(command_Itay))
//		{
//			return _ITAY;
//		}

//		if(1 == parse_cmd(command_Portuguese))		//������
//		{
//			return _PORTUGUESE;
//		}

//		if(1 == parse_cmd(command_Portuguese_BR))				//����������
//		{
//			return _PORTUGUESE_BR;
//		}

//		if(1 == parse_cmd(command_Spanish))				//������
//		{
//			return _SPANISH;
//		}		
//		
//		
//		
//		
//	
//		
//		
//		
//		
//	
//		
//		
//		/*ɨ������*/	
//		return _DATA;
////	}
//}





static uint8_t parsing_prefix_MDYCHAR()
{
	if(1 == prefix_mdy_flag)
	{
		//�ж�������Ƿ��޸��ַ�
		if(2 == tx_len - STX_CODETYPE_CODEID_SUFFIX)	//�����ַ�ֻ��2byte
		{
			if(TxBuffer[DATA_IDX] == '$')
			{

				return 1;
			}
		}
	}

	return 0;

}






static uint8_t parsing_suffix_MDYCHAR()
{
	if(1 == suffix_mdy_flag)
	{
		//�ж�������Ƿ��޸��ַ�
		if(2 == tx_len - STX_CODETYPE_CODEID_SUFFIX)	//�����ַ�ֻ��2byte
		{
			if(TxBuffer[DATA_IDX] == '$')
			{
				return 1;
			}
		}
	}

	return 0;

}




static uint8_t parsing_head_len_MDYCHAR()
{
	if(1 == head_hide_mdy_flag)
	{
		//�ж�������Ƿ��޸��ַ�
		if(2 == tx_len - STX_CODETYPE_CODEID_SUFFIX)	//�����ַ�ֻ��2byte
		{
			if(TxBuffer[DATA_IDX] == '$')
			{
				return 1;
			}
		}
	}

	return 0;

}







static uint8_t parsing_tail_len_MDYCHAR()
{
	if(1 == tail_hide_mdy_flag)
	{
		//�ж�������Ƿ��޸��ַ�
		if(2 == tx_len - STX_CODETYPE_CODEID_SUFFIX)	//�����ַ�ֻ��2byte
		{
			if(TxBuffer[DATA_IDX] == '$')
			{
				return 1;
			}
		}
	}

	return 0;

}







static uint8_t parsing_string_MDYCHAR()
{
	if(1 == string_hide_mdy_flag)
	{
		//�ж�������Ƿ��޸��ַ�
		if(2 == tx_len - STX_CODETYPE_CODEID_SUFFIX)	//�����ַ�ֻ��2byte
		{
			if(TxBuffer[DATA_IDX] == '$')
			{
				return 1;
			}
		}
	}

	return 0;

}




static uint8_t parsing_insert_string_MDYCHAR()
{
	if(1 == insert_character_set_flag)
	{
		//�ж�������Ƿ��޸��ַ�
		if(2 == tx_len - STX_CODETYPE_CODEID_SUFFIX)	//�����ַ�ֻ��2byte
		{
			if(TxBuffer[DATA_IDX] == '$')
			{
				return 1;
			}
		}
	}

	return 0;

}



static uint8_t parsing_insert_pos_MDY()
{
	if(1 == insert_pos_flag)
	{
		//�ж�������Ƿ��޸��ַ�
		if(2 == tx_len - STX_CODETYPE_CODEID_SUFFIX)	//�����ַ�ֻ��2byte
		{
			if(TxBuffer[DATA_IDX] == '$')
			{
				return 1;
			}
		}
	}

	return 0;

}










//static uint8_t parse_cmd(const char *command)
//{
//	uint8_t i=0;
//	if(strlen(command) == tx_len-STX_CODETYPE_CODEID_SUFFIX)
//	{
//		for(i=0;i<tx_len-STX_CODETYPE_CODEID_SUFFIX;i++)
//		{
//			if(TxBuffer[i+DATA_IDX] != command[i])
//				return 0;
//		}
//		return 1;
//	}
//	return 0;
//}









 




//1 success
//0 fail
uint8_t read_serial_command(uint16_t addr,uint8_t length)
{
	uint8_t i;
	uint16_t crc;
	uint8_t buf[9];
	
	
	
	buf[0] = (uint8_t)(HEAD1>>8);
	buf[1] = (uint8_t)HEAD1;

	buf[2] = READ;	//��
	
	buf[3] = 1;
	
	buf[4] = (uint8_t)(addr>>8);	
	buf[5] = (uint8_t)addr;

	buf[6] = length;	

	crc = crc_cal_by_bit(&buf[2],4+length);

	buf[7] = (uint8_t)(crc>>8);
	buf[8] =(uint8_t)crc;



	get_QRmodule_status=1;
	QR_len=0;
	
	
	for(i=0;i<9;i++)
	{
		usart_data_transmit(USART0,buf[i]);
		while(usart_flag_get(USART0, USART_FLAG_TC) !=SET);
	}
	
	delay_1ms(100);	//�ȴ�����
	
	if(serial_check(QR_sta,2+length))	//��У����ȷ
	{
		return 1;
	}
	else	//����
	{
		length=0;
		return 0;
	}
	
}

uint8_t write_serial_command(uint16_t addr,uint8_t len,uint8_t *data)
{
	uint8_t i;
	uint16_t crc;
	uint8_t buf[10];
	
	
	buf[0] = (uint8_t)(HEAD1>>8);
	buf[1] = (uint8_t)HEAD1;

	buf[2] = WRITE;	//д
	
	buf[3] = len;		//д���ݳ���
	
	buf[4] = (uint8_t)(addr>>8);		//��ַ
	buf[5] = (uint8_t)addr;

	for(i=0;i<len;i++)
	{
		buf[6+i] = data[i];		//data
	}


	crc = crc_cal_by_bit(&buf[2],4+len);

	buf[6+len] = (uint8_t)(crc>>8);
	buf[6+1+len] =(uint8_t)crc;

	

	get_QRmodule_status=1;
	QR_len=0;

	

	for(i=0;i<(8+len);i++)			//8���̶�����+���ݳ���len
	{
		usart_data_transmit(USART0,buf[i]);
		while(usart_flag_get(USART0, USART_FLAG_TC) == RESET);
	}
	
	delay_1ms(100);	//�ȴ�����
	
	if(serial_check2(QR_sta))
	{
		for(i=0;i<10;i++)
		{
			QR_sta[i]=0;
		}
		return 1;
	}
	return 0;
	
}




uint8_t QR_flash_save(void)	//v1.60
{
	uint8_t i;
	uint16_t crc;
	uint8_t buf[9];//10
	
	
	buf[0] = (uint8_t)(HEAD1>>8);
	buf[1] = (uint8_t)HEAD1;

	buf[2] = WRITE_FLASH;	//д
	
	buf[3] = 1;		//д���ݳ���
	
	buf[4] = 0;		//��ַ
	buf[5] = 0;

	buf[6] = 0;

	crc = crc_cal_by_bit(&buf[2],5);

	buf[7] = (uint8_t)(crc>>8);
	buf[8] =(uint8_t)crc;

	

	get_QRmodule_status=1;
	QR_len=0;

	

	for(i=0;i<9;i++)			//8���̶�����+���ݳ���len
	{
		usart_data_transmit(USART0,buf[i]);
		while(usart_flag_get(USART0, USART_FLAG_TC) == RESET);
	}
	
	delay_1ms(200);	//�ȴ�����
	
	if(serial_check2(QR_sta))
	{
		return 1;
	}
	return 0;
	
}



uint16_t crc_cal_by_bit(uint8_t *ptr, uint8_t len)
{
	u32 crc = 0;
	uint8_t i;

	while(len-- != 0)
	{
		for(i = 0x80; i != 0; i /= 2)
		{
			crc *= 2;
			if((crc&0x10000) !=0) //��һλ CRC �� 2 ������λ�� 1������� 0x11021
				crc ^= 0x11021;
			if((*ptr&i) != 0) //�����λ�� 1����ô CRC = ��һλ�� CRC + ��λ/CRC_CCITT
				crc ^= 0x1021;
		}
		ptr++;
	}
	return crc;
}


//1 success
//0 fail
uint8_t serial_check(uint8_t *ptr, uint8_t len1)
{
	uint16_t crc;
	
	uint16_t head2 = ptr[0];		//�ж��Ƿ����HEAD2
	head2 <<=8;
	head2 |= ptr[1];
	if(head2 != HEAD2)		//ͷ�����򷵻ش���
	{
		return 0;
	}
	
	if(len1>0)
	{
		crc = crc_cal_by_bit(ptr+2,len1);
		if(QR_sta[QR_len-2] == (uint8_t)(crc>>8) && QR_sta[QR_len-1] == (uint8_t)crc)
		{
			return 1;
		}
	}
	return 0;
}


uint8_t serial_check2(uint8_t *ptr)
{
	uint16_t head2 = ptr[0];		//�ж��Ƿ����HEAD2
	head2 <<=8;
	head2 |= ptr[1];
	if(head2 != HEAD2)		//ͷ�����򷵻ش���
	{
		return 0;
	}
	
	if(0 != ptr[2])
	{
		return 0;	
	}
	
	if(1 != ptr[3])
	{
		return 0;
	}
	
	if(0 != ptr[4])
	{
		return 0;
	}	
	
	if(0x33 == ptr[5] && 0x31 == ptr[6])
	{
		return 1;
	}
	
	return 0;
}





//void CodeID_enable(void)
//{
//	uint8_t temp_read;
//	while(1)
//	{
//		if(read_serial_command(0x0060,1))		//���������ȷ
//		{
//			temp_read = QR_sta[4];	//������ǰֵ
//			temp_read|=0x04;					//set codeID 
//			if(write_serial_command(0x0060,1,&temp_read))
//			{
////				if(flash_write_serial_command(0x0060,1,&temp_read))
//				if(QR_flash_save())
//				{	
//					get_QRmodule_status=0;
//					return;			
//				}
//			}
//		}
//	}
//}


//void CodeID_disable(void)
//{
//	uint8_t temp_read;
//	while(1)
//	{
//		if(read_serial_command(0x0060,1))		//���������ȷ
//		{
//			temp_read = QR_sta[4];	//ȡ����Ч���� ������ǰֵ
//			temp_read &= ~0x04;					//reset codeID 
//			
//			if(write_serial_command(0x0060,1,&temp_read))
//			{
////				if(flash_write_serial_command(0x0060,1,&temp_read))
//				if(QR_flash_save())
//				{	
//					get_QRmodule_status=0;
//					return;			
//				}
//			}	
//		}
//	}
//}

void noID_noSuffix_uartProtocal(void)
{
	uint8_t temp_read;
	while(1)
	{
		if(read_serial_command(0x0060,1))		//���������ȷ
		{
			temp_read = QR_sta[4];	//ȡ����Ч���� ������ǰֵ
			temp_read |= 0x04;		//set codeID 
			temp_read |= 0xe0;		//���ڴ�Э�����,�޺�׺
			
			if(write_serial_command(0x0060,1,&temp_read))
			{
//				if(flash_write_serial_command(0x0060,1,&temp_read))
				if(QR_flash_save())
				{	
					get_QRmodule_status=0;
					return;			
				}
			}	
		}
	}
}


void utf8_input_output(void)
{
	uint8_t temp_read;
	while(1)
	{
		if(read_serial_command(0x000d,1))		//���������ȷ
		{
			temp_read = QR_sta[4];			//ȡ����Ч���� ������ǰֵ
			temp_read |= 0x3c;					//ʹ��utf8�������
			temp_read &= ~0x03;					//�������
			if(write_serial_command(0x000d,1,&temp_read))
			{
//				if(flash_write_serial_command(0x000d,1,&temp_read))
				if(QR_flash_save())
				{	
					get_QRmodule_status=0;
					return;			
				}
			}	
		}
	}

}




void codeID_character_set(void)
{
	uint8_t i;
	uint8_t cha='a';
	
	for(i=0;i<20;i++)	
	{	
		while(1)	//�޸�codeID�ַ�
		{
			if(write_serial_command(0x0091+i,1,&cha))
			{
				cha+=1;
				break;
			}		
		}
	}
	
	while(1)
	{
		if(QR_flash_save())		//����flash
		{
			get_QRmodule_status=0;
			return; 		
		}
	}
}








/*������Ӧ�ı�־λ����λ�������Ӧλ*/
void QR_moduler_set(uint16_t addr,uint8_t bits_set,uint8_t bits_reset)
{
	uint8_t temp_read;
	while(1)
	{
		if(read_serial_command(addr,1))		//���������ȷ
		{
			temp_read = QR_sta[4];			//ȡ����Ч���� ������ǰֵ
			temp_read |= bits_set;					//ʹ��utf8�������
			temp_read &= ~bits_reset;					//�������
			if(write_serial_command(addr,1,&temp_read))
			{
//				if(flash_write_serial_command(addr,1,&temp_read))
				if(QR_flash_save())
				{	
					get_QRmodule_status=0;
					return;			
				}
			}	
		}
	}
}

/*������Ӧ�ı�־λ,ֱ�Ӹ�ֵ*/
void QR_moduler_val_assignment(uint16_t addr,uint8_t val)
{
	while(1)
	{
		if(write_serial_command(addr,1,&val))
		{
//			if(flash_write_serial_command(addr,1,&val))
			if(QR_flash_save())
			{	
				get_QRmodule_status=0;
				return;			
			}
		}	
	}
}





//void QR_moduler_test(uint16_t addr)
//{
//	uint8_t temp_read;
//	while(1)
//	{
//		if(read_serial_command(addr,1))		//���������ȷ
//		{
//			temp_read = QR_sta[4];			//ȡ����Ч���� ������ǰֵ
//			get_QRmodule_status=0;
//			return;
//		}
//	}
//}

void usart_write_baud(u32 baud)
{
	uint8_t data[2];
	uint16_t val;
	switch (baud)
	{
		case 1200:
			val=0x09c4;
		break;


		case 4800:
			val=0x0271;
		break;

		case 9600:
			val=0x0139;
		break;

		case 14400:
			val=0x00d0;
		break;		

		case 19200:
			val=0x009c;
		break;

		case 38400:
			val=0x004e;
		break;

		case 57600:
			val=0x0034;
		break;		

		case 115200:
			val=0x001a;
		break;

		default: val=0x0139;	//9600
	}
	data[0] = val;
	data[1] = val>>8;


	while(1)
	{
		if(write_serial_command(0x002A,2,&data[0]))
		{
			DevConfig.baud_rate = baud;
			uart_init();	//��д�ɹ�����Ҫ���³�ʼ�����ڣ���дflash

			while(1)
			{
				if(QR_flash_save())
				{	
					get_QRmodule_status=0;
					return;			
				}
			}
		}		
	}
}







void write_prefix(void)
{
	uint8_t i;
	if(DevConfig.prefix.en_dis == PREFIX_ENABLE)	//ǰ׺ʹ��ʱ�����ǰ׺
	{
		for(i=0;i<DevConfig.prefix.len;i++)
		{
			TxBuffer[i+PREFIX_IDX] = DevConfig.prefix.buf[i];
		}
		tx_len = DATA_IDX;				//����ǰ׺����
	}
	else											//ǰ׺��ֹʱ
	{
		tx_len = DATA_IDX;
	}
}


/*ÿ�η�����ɣ������������clear*/
uint8_t get_prefix_len(Prefix prefix)			
{
	if(prefix.en_dis == PREFIX_ENABLE)	
	{
		read_prefix_to_tx();
		return prefix.len;	
	}
	else
	{
		return 0;						
	}
}



uint8_t get_suffix_len(Suffix suffix)			
{
	if(suffix.en_dis == SUFFIX_ENABLE)	
	{
		return suffix.len;	
	}
	else
	{
		return 0;						//�����±�����
	}
}








static void read_prefix_to_tx(void)
{
	uint8_t i;
	for(i=0;i<DevConfig.prefix.len;i++)
	{
		TxBuffer[i+PREFIX_IDX] = DevConfig.prefix.buf[i];	
	}
}


static void read_suffix_to_tx(void)
{
	uint8_t i;
	
	for(i=0;i<DevConfig.suffix.TX_suffix_len;i++)
	{
		TxBuffer[tx_len++] = DevConfig.suffix.buf[i];
	}
}




void write_stx(void)
{
	if(DevConfig.STX_ETX & (0x01<<STX_MASK_BIT))
	{
		TxBuffer[STX_IDX] = STX;
	}
	else
	{
		TxBuffer[STX_IDX] = 0;
	}
}



void write_etx(void)
{
	if(DevConfig.STX_ETX & (0x01<<ETX_MASK_BIT))
	{
		TxBuffer[tx_len++] = ETX;
	}
	else
	{
		TxBuffer[tx_len++] = 0;
	}
}


static void _assignment(char *p)
{
	uint8_t i=CODE_TYPE_IDX,j=0;
	while('\0' != p[j])
	{
		TxBuffer[i] = p[j];
		i++;
		j++;
	}
	for(;i<CODEID_IDX;i++)
	{
		TxBuffer[i] = 0;
	}
}



static void zero_assignment()
{
	uint8_t i = CODE_TYPE_IDX;
	
	for(;i<CODEID_IDX;i++)
	{
		TxBuffer[i] = 0;
	}

}



void write_CODE_TYPE_ID(void)
{
	char *ptr=NULL;
	//codeTYPE
	if(CODE_TYPE_DISP_EN == DevConfig.code_disp)
	{
		switch(TxBuffer[CODEID_IDX])
		{
			case 'a':	ptr = "EAN-13";		break;

			case 'b':	ptr = "EAN-8";		break;

			case 'c':	ptr = "UPC-A";		break;
			
			case 'd':	ptr = "UPC-E0";		break;

			case 'e':	ptr = "UPC-E1";		break;

			case 'f':	ptr = "Code 128";	break;

			case 'g':	ptr = "Code 39";	break;
			
			case 'h':	ptr = "Code 93";	break;

			case 'i':	ptr = "Codebar";	break;

			case 'j':	ptr = "Interleaved25";	break;

			case 'k':	ptr = "Industrial25";break;
			
			case 'l':	ptr = "Matrix25";	break;

			case 'm':	ptr = "Code 11";	break;

			case 'n':	ptr = "MSI-Plessey";	break;

			case 'o':	ptr = "RSS-14";			break;
			
			case 'p':	ptr = "RSS Limited";	break;
			
			case 'q':	ptr = "RSS Expanded";	break;
			
			case 'r':	ptr = "QR Code";		break;
			
			case 's':	ptr = "Data Matrix";	break;
			
			case 't':	ptr = "PDF 417";		break;

			default: break;
		}
		
		_assignment(ptr);

	}
	else
	{
		zero_assignment();
	}

	

	//codeID
	if(CODE_ID_DISABLE == CODE_ID)	//��codeID��ֹ
	{
		TxBuffer[CODEID_IDX] = 0;			//����
	}
	else
	{
		TxBuffer[CODEID_IDX] = codeID_user[TxBuffer[CODEID_IDX]-'a'];
	}

}




void write_end_mark(void)
{
	switch(DevConfig.end_mark)
	{
		case END_CR:
			TxBuffer[tx_len++] = '\r';		
		break;

		case END_LF:
		    TxBuffer[tx_len++] = '\n';	
		break;

		case END_CR_LF:
			TxBuffer[tx_len++] = '\r';	
			TxBuffer[tx_len++] = '\n';	
		break;

		case END_HT:
			TxBuffer[tx_len++] = '\t';
		break;

		case END_SPACE:
			TxBuffer[tx_len++] = ' ';
		break;

		case END_EMPTY:
			TxBuffer[tx_len++] = 0;
		break;		

		default: break;
	}
}





uint16_t tx_len=0;


uint16_t uart_len=0;
uint16_t data_tail;

uint8_t get_len_flag=0;
uint8_t get_codeID = 0;


uint8_t resend=0;
static uint8_t usart_03_resend()
{
	if(0x03 == ch)
	{
		tx_len = DATA_IDX;	//��������ݲ���ʱ��ȫ��clear
		get_len_flag=0;
		uart_len=0;
		get_codeID = 0;
		scan_complete =0;	//��ȡû���
		
		TxBuffer[tx_len++] = ch;	//��03�ÿ�ʼ��
		resend++;
		return 1;
	}
	return 0;
}


void get_QR_module_data()
{
	if(0 == get_QRmodule_status)
	{
//		if(0 == scan_complete)		//��ֹ��û������ɣ����յ�������
		{
			if(tx_len<TXBUFFER_SIZE-1)
			{
				ch = usart_data_receive(USART0); 
				TxBuffer[tx_len++] = ch;				//��
				
				if(get_len_flag == 0)
				{
					if(0x03 == TxBuffer[DATA_IDX])	//	�ж��Ƿ������ݿ�ͷ
					{
						if(DATA_IDX+3 == tx_len)	//�յ�3��
						{
							uart_len = TxBuffer[DATA_IDX+1];	//��ȡ���ݳ���
							uart_len <<=8;
							uart_len |= TxBuffer[DATA_IDX+2];
							data_tail = DATA_IDX + uart_len -1 - 1;	//��¼����β���±�		 ���ҵ�codeIDʹ��ʱ����Ҫ-1
							tx_len = DATA_IDX;
							get_len_flag=1;
						}
					}
					else
					{
						tx_len = DATA_IDX;	//��������ݲ���ʱ��ȫ��clear
						get_len_flag=0;
						uart_len=0;
						get_codeID = 0;
						scan_complete =0;	//��ȡû���
					}					
				}
				else	//�Ѿ�������ݳ���
				{
					if(usart_03_resend())		//�����³���03�󣬰����ݶ��������½���
					{
						return;
					}
					
					if(--uart_len != 0)
					{
						if(0 == get_codeID) 
						{
							TxBuffer[CODEID_IDX] = ch; 					
							tx_len--;	//��ָ�뷵�����ݶ�
							get_codeID = 1;
						}
					}
					else	//���
					{
						read_suffix_to_tx();	//��Ӻ�׺
						write_etx();			//���ETX
						write_CODE_TYPE_ID();
						write_end_mark();
						scan_complete =1;	//��ȡ���
						scan_idle();		//�ߵ�ƽ 
					}
				}
			}
			else
			{
				tx_len = DATA_IDX;	//�����������ʱ��ȫ��clear
				get_len_flag=0;
				uart_len=0;
				get_codeID = 0;
				scan_complete =0;	//��ȡû���
			}
		}
	}
	else
	{
		ch = usart_data_receive(USART0); 
		QR_sta[QR_len++] = ch;
	}

}

uint8_t TA[100]={0};
uint8_t ttt=0;
void USART0_IRQHandler(void)
{
	if(SET == usart_flag_get(USART0,USART_FLAG_RBNE))	//�鿴�Ƿ�����ж�
	{
//		if(scan_complete)
//		{
//			TA[ttt++] = usart_data_receive(USART0);
//			if(ttt>100)
//			{
//				scan_complete=1;
//			}
//		}		
		scan_delay=0;
		get_QR_module_data();
//		TA[ttt++] = usart_data_receive(USART0);

	}

	//���־λ
}


