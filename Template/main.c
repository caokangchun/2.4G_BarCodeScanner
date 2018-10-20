

/* Includes ------------------------------------------------------------------*/
#include "gd32f1x0.h"
#include "gd32f1x0_libopt.h"
#include "systick.h"
#include <stdio.h>
#include <stdlib.h>//������ͷ�ļ�
#include "lt8920.h"



#include "BSPScan.h"
#include "BSPKey.h"

#include "usbd_std.h"
#include "hid_core.h"




#include "BSPBat.h"
#include "data_type.h"
#include "data_format.h"
#include "BSPUsb.h"
#include "BSPWireless.h"
#include "BSPSystem.h"
#include "BSPLed.h"


#include "config.h"
#include "dataParse.h"



extern uint32_t time;

uint32_t wire_wireless=0;


#ifdef DEBUG_SUCCESS_ERROR
uint32_t send_success,send_error;
#endif

uint16_t ad=0;
uint8_t BAT=0;

uint8_t channel_jump[7];
uint8_t index1 = 0;

uint8_t prefix_mdy_flag=0;
uint8_t suffix_mdy_flag=0;
uint8_t head_hide_mdy_flag=0;
uint8_t tail_hide_mdy_flag=0;
uint8_t string_hide_mdy_flag=0;


uint8_t insert_character_set_flag=0,insert_pos_flag=0;
uint8_t grp_index=0;



extern uint8_t get_len_flag;
extern uint8_t get_codeID;




int main(void)
{   
	uint8_t result_parsing;

	uint8_t ch_len;

	BSPSystemInit();
	
	FactorySet();
	
	ReadConfig();
	
	PlayListInit();				//������ʾ��
	
	SysTick_Configuration();
	
	BSPBatCheckInit();		
	
	BSPWirelessInit();
	BSPWirelessConfig();		

	BSPKeyInit();				//����

	BSPUsbInit();				//������������USBʱ���Ῠ����

	BSPScanInit();				//���� ɨ������ʼ��

	BSPSystemPowerOn();				//����Դ
	
	BSPScannerFirstSet();			//����ʹ������
								
	BSPPlayPowerOn();			//��ʾ�����ɹ�
	BSPWatchDogInit();
	
	while(1)	
	{
		BSPWatchDogReset();
		
		BSPBatLowPowerCheck();				//��ص�ѹ���
		
		BSPLedIdle();						//����
	
		BSPSystemFreeTimeingShutdown();		//���м�ʱ�ػ�
		
		BSPWirelessRoutine();
		
		BSPScan();
		
		if(BSPScanGetComplete())
		{
			result_parsing = Parsing();
			prefix_mdy_flag=0;
			suffix_mdy_flag=0;
			head_hide_mdy_flag=0;
			tail_hide_mdy_flag=0;
			string_hide_mdy_flag=0;

			insert_character_set_flag=0;
			insert_pos_flag=0;
					time=0;
					switch (result_parsing)		//������������
					{
						
#ifdef DEBUG_SUCCESS_ERROR	
						case _CLEAR_RECORD:
							send_success = 0;
							send_error = 0;
							BSPEepromWrite(FLASH_SEND_SUCCESS,&send_success,1);	
							BSPEepromWrite(FLASH_SEND_ERROR,&send_error,1);		
							BSPPlaySuccess();		//�ɹ�						
						break;						
	
#endif							

						
						
						case _REQUEST_MATCH:
		
							BSPLedWork();

							if(BSPWirelessConnect(CONNECT_NORMAL))
							{
								BSPWirelessSaveConnect();		//����ƥ���ַ
								BSPPlaySuccess();		//�ɹ�
							}
							else
							{
								BSPPlayFail();
							}
							BSPLedIdle();
						
						break;
							
							
	
						case _REQUEST_MATCH_FORCE:
							BSPLedWork();

							if(BSPWirelessConnect(CONNECT_FORCE))
							{
								BSPWirelessSaveConnect();		//����ƥ���ַ
								BSPPlaySuccess();		//�ɹ�
							}
							else
							{
								BSPPlayFail();
							}
							BSPLedIdle();
						
						break;

							
							

						
						case _WIRE_WIRELESS:
							DevConfig.wire_wireless = ReadWireWirelessMode();			//������ǰ����
							DevConfig.wire_wireless = DevConfig.wire_wireless ^ 0x01;	//ȡ��

							SaveWireWireless(DevConfig.wire_wireless);				//д��flash
							BSPPlaySuccess();		//�ɹ�
						break;
						
						
						case _SINGLE:
							SaveSingleContinual(SINGLE);
							DevConfig.single_continual = ReadSingleContinual();
							BSPPlaySuccess();		//�ɹ�
							
						break;
						
			
						case _CONTINUAL:
							SaveSingleContinual(CONTINUAL);
							DevConfig.single_continual = ReadSingleContinual();
							BSPPlaySuccess();		//�ɹ�	
							
						break;	
					
						
						case _BEEP_EN:
							SaveBeepOption(BEEP_ENABLE);
							DevConfig.beep_en_dis = ReadBeepOption();
							BSPPlaySuccess();		//�ɹ�	
														
						break;
						
						
						case _BEEP_DIS:
							SaveBeepOption(BEEP_DISABLE);
							DevConfig.beep_en_dis = ReadBeepOption();
							BSPPlaySuccess();		//�ɹ�	
														
						break;



						case _SLEEP_HF_MIN:
							SaveSleepTime(MINUTE_HALF);
							DevConfig.sleep_time = ReadSleepTime();	
							BSPPlaySuccess();		//�ɹ�	
														
						break;		
						

						case _SLEEP_1MIN:
							SaveSleepTime(MINUTE_1);
							DevConfig.sleep_time = ReadSleepTime();	
							BSPPlaySuccess();		//�ɹ�	
														
						break;

						
						case _SLEEP_5MIN:
							SaveSleepTime(MINUTE_5);
							DevConfig.sleep_time = ReadSleepTime();	
							BSPPlaySuccess();		//�ɹ�	
														
						break;


						case _SLEEP_10MIN:
							SaveSleepTime(MINUTE_10);
							DevConfig.sleep_time = ReadSleepTime();	
							BSPPlaySuccess();		//�ɹ�	
														
						break;	


						case _SLEEP_20MIN:
							SaveSleepTime(MINUTE_20);
							DevConfig.sleep_time = ReadSleepTime();	
							BSPPlaySuccess();		//�ɹ�	
														
						break;	


						case _SHUTDOWN:
							BSPPlayPowerOff();
							BSPSystemPowerOff();		//�ص�Դ	
														
						break;	

						
						case _STX_EN:
							DevConfig.STX_ETX |= (0x01<<STX_MASK_BIT);
							SaveSTXETX(DevConfig.STX_ETX);
							BSPPlaySuccess();		//�ɹ�	
						break;

						case _STX_DIS:
							DevConfig.STX_ETX &= ~(0x01<<STX_MASK_BIT);
							SaveSTXETX(DevConfig.STX_ETX);
							BSPPlaySuccess();		//�ɹ�							
						break;


						case _ETX_EN:
							DevConfig.STX_ETX |= (0x01<<ETX_MASK_BIT);
							SaveSTXETX(DevConfig.STX_ETX);
							BSPPlaySuccess();		//�ɹ�	
						break;

						case _ETX_DIS:
							DevConfig.STX_ETX &= ~(0x01<<ETX_MASK_BIT);
							SaveSTXETX(DevConfig.STX_ETX);
							BSPPlaySuccess();		//�ɹ�						
						break;


						case _PREFIX_EN:
							DevConfig.prefix.en_dis = PREFIX_ENABLE;		//ʹ��
							DevConfig.prefix.Tx_prefix_len = GetPrefixLen(DevConfig.prefix);		//д��TXBUFF������ʵ��ǰ׺����						
							PrefixSave(&DevConfig.prefix);						//����flash
							BSPPlaySuccess();		//�ɹ�		
						break;		

						case _PREFIX_DIS:
							DevConfig.prefix.en_dis = PREFIX_DISABLE;	//��ֹ
							DevConfig.prefix.Tx_prefix_len = GetPrefixLen(DevConfig.prefix);		//����ʵ��ǰ׺����
							PrefixSave(&DevConfig.prefix);						//����flash
							BSPPlaySuccess();		//�ɹ�
						break;						

						case _PREFIX_MODIFY:
							prefix_mdy_flag=1;
							prefix_shadow.len=0;
							BSPPlaySuccess();		//�ɹ�	
						break;	

						case _PREFIX_MDYCHAR:
							if(prefix_shadow.len < sizeof(prefix_shadow.buf))
							{
								prefix_mdy_flag=1;
								prefix_shadow.buf[prefix_shadow.len] = TxBuffer[DATA_IDX+1];		//ȡ��һ��������prefix_shadow
								prefix_shadow.len++; 	//����++
								PrefixAssignment();	//shadow ��ֵ�� DevConfig
								DevConfig.prefix.Tx_prefix_len = GetPrefixLen(DevConfig.prefix);		//����ʵ��ǰ׺���ȣ��Լ�ǰ׺д��TxBuffer
								PrefixSave(&DevConfig.prefix);//flash����							
								BSPPlaySuccess();		//�ɹ�							
							}

						break;		




						case _CODEID_EN:
							
							DevConfig.code_ID = CODE_ID_ENABLE;
							SaveCodeID(DevConfig.code_ID);
							
							BSPPlaySuccess();		//�ɹ�		
						break;	

						case _CODEID_DIS:
							
							DevConfig.code_ID = CODE_ID_DISABLE;
							SaveCodeID(DevConfig.code_ID);
								
							BSPPlaySuccess();		//�ɹ�	
								
						break;	




						case _CODE_TYPE_DISP_EN:
							DevConfig.code_disp = CODE_TYPE_DISP_EN;
							SaveCodeNameDisp(CODE_TYPE_DISP_EN);
							BSPPlaySuccess();		//�ɹ�	
						break;


						case _CODE_TYPE_DISP_DIS:
							DevConfig.code_disp = CODE_TYPE_DISP_DIS;
							SaveCodeNameDisp(CODE_TYPE_DISP_DIS);
							BSPPlaySuccess();		//�ɹ�
						break;

						


						case _END_CR:
							DevConfig.end_mark = END_CR;
							SaveEndMark(DevConfig.end_mark);
							BSPPlaySuccess();		//�ɹ�	
							
						break;
						
						case _END_LF:
							DevConfig.end_mark = END_LF;
							SaveEndMark(DevConfig.end_mark);
							BSPPlaySuccess();		//�ɹ�	

						break;

						case _END_CR_LF:
							DevConfig.end_mark = END_CR_LF;
							SaveEndMark(DevConfig.end_mark);
							BSPPlaySuccess();		//�ɹ�	

						break;

						case _END_HT:
							DevConfig.end_mark = END_HT;
							SaveEndMark(DevConfig.end_mark);
							BSPPlaySuccess();		//�ɹ�	

						break;

						case _END_SPACE:
							DevConfig.end_mark = END_SPACE;
							SaveEndMark(DevConfig.end_mark);
							BSPPlaySuccess();		//�ɹ�	

						break;

						case _END_EMPTY:
							DevConfig.end_mark = END_EMPTY;
							SaveEndMark(DevConfig.end_mark);
							BSPPlaySuccess();		//�ɹ�	

						break;


						case _SUFFIX_EN:
							DevConfig.suffix.en_dis = SUFFIX_ENABLE;		//ʹ��
							DevConfig.suffix.TX_suffix_len = get_suffix_len(DevConfig.suffix);
							SuffixSave(&DevConfig.suffix);						//����flash
							BSPPlaySuccess();		//�ɹ�		
						break;		

						case _SUFFIX_DIS:
							DevConfig.suffix.en_dis = SUFFIX_DISABLE;	//��ֹ
							DevConfig.suffix.TX_suffix_len = get_suffix_len(DevConfig.suffix);
							SuffixSave(&DevConfig.suffix);						//����flash
							BSPPlaySuccess();		//�ɹ�
						break;	

						case _SUFFIX_MODIFY:
							suffix_mdy_flag=1;
							suffix_shadow.len=0;
							BSPPlaySuccess();		//�ɹ�	
						break;	

						case _SUFFIX_MDYCHAR:
							if(suffix_shadow.len < sizeof(suffix_shadow.buf))
							{
								suffix_mdy_flag=1;		//д��һ���ַ��󣬼���ʹ��
								suffix_shadow.buf[suffix_shadow.len] = TxBuffer[DATA_IDX+1];		//ȡ��һ��������prefix_shadow
								suffix_shadow.len++; 	//����++
								SuffixAssignment();	//shadow ��ֵ�� DevConfig
								DevConfig.suffix.TX_suffix_len = get_suffix_len(DevConfig.suffix);	
								SuffixSave(&DevConfig.suffix);//flash����							
								BSPPlaySuccess();		//�ɹ�			
							}

						break;		



						case _HEAD_HIDE_EN:
							DevConfig.hide.head_hide_enDis = HIDE_ENABLE;			//ʹ��
							SaveHideSet(&DevConfig.hide);						//����flash
							BSPPlaySuccess();		//�ɹ�		
						break;		

						case _HEAD_HIDE_DIS:
							DevConfig.hide.head_hide_enDis = HIDE_DISABLE;			//ʹ��
							SaveHideSet(&DevConfig.hide);						//����flash
							BSPPlaySuccess();		//�ɹ�		

						break;	

						case _HEAD_HIDE_LEN_MODIFY:
							head_hide_mdy_flag=1;
							hide_shadow.head_hide_len=0;
							BSPPlaySuccess();		//�ɹ�
						break;	

						case _HEAD_HIDE_LEN_MOYCHAR:
							head_hide_mdy_flag=1;
							
							ch_len = TxBuffer[DATA_IDX+1];		//��ȡ�ַ�
							if(ch_len>='0' && ch_len<='9')	//�ж��ַ�
							{
								ch_len-='0';
								hide_shadow.head_hide_len *= 10;
								hide_shadow.head_hide_len += ch_len;
								if(hide_shadow.head_hide_len > 15)
								{
									hide_shadow.head_hide_len = 15;
								}
								DevConfig.hide.head_hide_len = hide_shadow.head_hide_len;	//д��ϵͳ����
								SaveHideSet(&DevConfig.hide);
								BSPPlaySuccess();		//�ɹ�	
							}

						break;	


						case _TAIL_HIDE_EN:
							DevConfig.hide.tail_hide_enDis = HIDE_ENABLE;			//ʹ��
							SaveHideSet(&DevConfig.hide);						//����flash
							BSPPlaySuccess();		//�ɹ�		
						break;		

						case _TAIL_HIDE_DIS:
							DevConfig.hide.tail_hide_enDis = HIDE_DISABLE;			//ʹ��
							SaveHideSet(&DevConfig.hide);						//����flash
							BSPPlaySuccess();		//�ɹ�		

						break;	

						case _TAIL_HIDE_LEN_MODIFY:
							tail_hide_mdy_flag=1;
							hide_shadow.tail_hide_len=0;
							BSPPlaySuccess();		//�ɹ�
						break;	

						case _TAIL_HIDE_LEN_MOYCHAR:
							tail_hide_mdy_flag=1;
							
							ch_len = TxBuffer[DATA_IDX+1];		//��ȡ�ַ�
							if(ch_len>='0' && ch_len<='9')	//�ж��ַ�
							{
								ch_len-='0';
								hide_shadow.tail_hide_len *= 10;
								hide_shadow.tail_hide_len += ch_len;
								if(hide_shadow.tail_hide_len > 15)
								{
									hide_shadow.tail_hide_len = 15;
								}
								DevConfig.hide.tail_hide_len = hide_shadow.tail_hide_len;	//д��ϵͳ����
								SaveHideSet(&DevConfig.hide);
								BSPPlaySuccess();		//�ɹ�	
							}

						break;	


/*****************************************************************************/
						
						case _STRING_HIDE_EN:
							DevConfig.hide.string_hide_enDis = HIDE_ENABLE;			//ʹ��
							SaveHideSet(&DevConfig.hide);						//����flash
							BSPPlaySuccess();		//�ɹ�		
						break;		

						case _STRING_HIDE_DIS:
							DevConfig.hide.string_hide_enDis = HIDE_DISABLE;			//ʹ��
							SaveHideSet(&DevConfig.hide);						//����flash
							BSPPlaySuccess();		//�ɹ�		

						break;	

						case _STRING_HIDE_MODIFY:
							string_hide_mdy_flag=1;
							hide_shadow.string_len=0;
							BSPPlaySuccess();		//�ɹ�
						break;	

						case _STRING_HIDE_MOYCHAR:
							string_hide_mdy_flag=1;
							
							ch_len = TxBuffer[DATA_IDX+1];		//��ȡ�ַ�	
							if(hide_shadow.string_len < 15)
							{
								hide_shadow.string_hide[hide_shadow.string_len] = ch_len;	//д��string
								hide_shadow.string_len++;
								StringAssignment();	//д�볤�ȣ��Լ��ַ���
								SaveHideSet(&DevConfig.hide);
								BSPPlaySuccess();		//�ɹ�	
							}	

						break;	



						case _NO_CONVERSION:
							SaveConversion(NO_CONVERSION);
							BSPPlaySuccess();		//�ɹ�	
						break;


						case _UPPERCASE_CONVERSION:
							SaveConversion(UPPERCASE_CONVERSION);
							BSPPlaySuccess();		//�ɹ�	
						break;

						case _LOWERCASE_CONVERSION:
							SaveConversion(LOWERCASE_CONVERSION);
							BSPPlaySuccess();		//�ɹ�	
						break;

						case _CHARACTER_CONVERSION:
							SaveConversion(CHARACTER_CONVERSION);
							BSPPlaySuccess();		//�ɹ�	
						break;						



//						case _INSERT_SET:
//							insert_character_set_flag = 1;
//							grp_index = INSERT_GRP_SIZE;	//���⻹ûɨ�������Ͳ�������
//							BSPPlaySuccess();		//�ɹ�
//						
//						break;


//						case _INSERT1_CHAR:
//							insert_character_init();		
//							BSPPlaySuccess();		//�ɹ�
//						break;

//						case _INSERT1_POS:
//							insert_pos_init();
//							BSPPlaySuccess();		//�ɹ�
//						break;


//						
//						case _INSERT2_CHAR:
//							insert_character_init();		
//							BSPPlaySuccess();		//�ɹ�
//						break;

//						case _INSERT2_POS:
//							insert_pos_init();
//							BSPPlaySuccess();		//�ɹ�
//						break;

//	
//						case _INSERT3_CHAR:
//							insert_character_init();		
//							BSPPlaySuccess();		//�ɹ�
//						break;

//						case _INSERT3_POS:
//							insert_pos_init();
//							BSPPlaySuccess();		//�ɹ�
//						break;


//						
//						case _INSERT4_CHAR:
//							insert_character_init();		
//							BSPPlaySuccess();		//�ɹ�
//						break;

//						case _INSERT4_POS:
//							insert_pos_init();
//							BSPPlaySuccess();		//�ɹ�
//						break;


//						
//						case _INSERT5_CHAR:
//							insert_character_init();		
//							BSPPlaySuccess();		//�ɹ�
//						break;

//						case _INSERT5_POS:
//							insert_pos_init();
//							BSPPlaySuccess();		//�ɹ�
//						break;


//						
//						case _INSERT6_CHAR:
//							insert_character_init();		
//							BSPPlaySuccess();		//�ɹ�
//						break;

//						case _INSERT6_POS:
//							insert_pos_init();
//							BSPPlaySuccess();		//�ɹ�
//						break;


//						
//						case _INSERT7_CHAR:
//							insert_character_init();		
//							BSPPlaySuccess();		//�ɹ�
//						break;

//						case _INSERT7_POS:
//							insert_pos_init();
//							BSPPlaySuccess();		//�ɹ�
//						break;


//						
//						case _INSERT8_CHAR:
//							insert_character_init();		
//							BSPPlaySuccess();		//�ɹ�
//						break;

//						case _INSERT8_POS:
//							insert_pos_init();
//							BSPPlaySuccess();		//�ɹ�
//						break;



//						case _INSERT_DIS:
//							string_insert_diasble();
//							BSPPlaySuccess();		//�ɹ�
//						break;

//						case _INSERT_STRING_MDYCHAR:

//							if(grp_index<INSERT_GRP_SIZE)
//							{
//								insert_character_set_flag=1;	//�Ծ�Ϊ�����ַ���
//								ch_len = TxBuffer[DATA_IDX+1];		//��ȡ�ַ�	
//								
//								if(array_insert[grp_index].len < INSERT_STRING_SIZE)
//								{
//									array_insert[grp_index].str[array_insert[grp_index].len] = ch_len;
//									array_insert[grp_index].len++;
//									insert_en_dis(grp_index);
//									save_String_insert_array();						
//								}
//								BSPPlaySuccess();		//�ɹ�

//							}
//						break;		

//						case _INSERT_POS_MDY:
//							insert_pos_flag =1;
//							ch_len = TxBuffer[DATA_IDX+1];		//��ȡ�ַ�
//							if(ch_len>='0' && ch_len<='9')		//�ж��ַ�
//							{
//								ch_len-='0';
//								array_insert[grp_index].pos_insert *=10;
//								array_insert[grp_index].pos_insert +=ch_len;
//								insert_en_dis(grp_index);
//								save_String_insert_array();
//							}	

//							BSPPlaySuccess();		//�ɹ�				
//						break;





				/*������*/
						case _1200:
							
							usart_write_baud(1200);//��ͨ������������Ķ�άģ�����
							BaudSet(1200);
						  	BSPPlaySuccess();		//�ɹ�	

						break;

						case _4800:
							usart_write_baud(4800);//��ͨ������������Ķ�άģ�����
							BaudSet(4800);
							BSPPlaySuccess();		//�ɹ�	

						break;						

						case _9600:
							
							usart_write_baud(9600);//��ͨ������������Ķ�άģ�����
							BaudSet(9600);
						  BSPPlaySuccess();		//�ɹ�	
						break;

						case _14400:
							
							usart_write_baud(14400);//��ͨ������������Ķ�άģ�����
							BaudSet(14400);
						  BSPPlaySuccess();		//�ɹ�							
						break;


						case _19200:
							
							usart_write_baud(19200);//��ͨ������������Ķ�άģ�����
							BaudSet(19200);
							BSPPlaySuccess();		//�ɹ�	

						break;

						case _38400:
							
							usart_write_baud(38400);//��ͨ������������Ķ�άģ�����
							BaudSet(38400);	
							BSPPlaySuccess();		//�ɹ�	

						break;

						case _57600:
							
							usart_write_baud(57600);//��ͨ������������Ķ�άģ�����
							BaudSet(57600);
							BSPPlaySuccess();		//�ɹ�	
						break;

						case _115200:
							
							usart_write_baud(115200);//��ͨ������������Ķ�άģ�����
							BaudSet(115200);
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _EAN13_EN:
							QR_moduler_set(ADDR_EAN13,0x01,0);	//0x002e ��λ1���������Ҫ			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _EAN13_DIS:
							QR_moduler_set(ADDR_EAN13,0,0x01);	//0x002e ��λ����Ҫ�����1					
						  	BSPPlaySuccess();		//�ɹ�						
						break;

						/*EAN*/
						case _EAN8_EN:
							QR_moduler_set(ADDR_EAN8,0x01,0);	
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _EAN8_DIS:
							QR_moduler_set(ADDR_EAN8,0,0x01);	
						  	BSPPlaySuccess();		//�ɹ�							
						break;

						/*UPCA*/
						case _UPCA_EN:
							QR_moduler_set(ADDR_UPCA,0x01,0);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _UPCA_DIS:
							QR_moduler_set(ADDR_UPCA,0,0x01);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;

						/*UPCE0*/
						case _UPCE0_EN:
							QR_moduler_set(ADDR_UPCE0,0x01,0);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _UPCE0_DIS:
							QR_moduler_set(ADDR_UPCE0,0,0x01);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	

						
						/*UPCE1*/
						case _UPCE1_EN:
							QR_moduler_set(ADDR_UPCE1,0x01,0);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _UPCE1_DIS:
							QR_moduler_set(ADDR_UPCE1,0,0x01);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	


						/*CODE128*/
						case _CODE128_EN:
							QR_moduler_set(ADDR_CODE128_ENDIS,0x01,0);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _CODE128_DIS:
							QR_moduler_set(ADDR_CODE128_ENDIS,0,0x01);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	

						case _CODE128_LEAST0:
							QR_moduler_val_assignment(ADDR_CODE128_LEAST,0);		
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _CODE128_LEAST4:
							QR_moduler_val_assignment(ADDR_CODE128_LEAST,4);				
						  	BSPPlaySuccess();		//�ɹ�						
						break;	
						
						case _CODE128_MOST32:
							QR_moduler_val_assignment(ADDR_CODE128_MOST,32);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _CODE128_MOST255:	//ʹ��ɨ��û���⣬д��־λҲû���⣬����һдflash֮�󣬶��������ݾͲ����ˣ�ò��ģ�����³�ʼ����һ�飬��Ҫ�͹�Ӧ�̹�ͨ
							QR_moduler_val_assignment(ADDR_CODE128_MOST,255);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	


						/*CODE39*/
						case _CODE39_EN:
							QR_moduler_set(ADDR_CODE39_ENDIS,0x01,0);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _CODE39_DIS:
							QR_moduler_set(ADDR_CODE39_ENDIS,0,0x01);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	

						case _CODE39_LEAST0:
							QR_moduler_val_assignment(ADDR_CODE39_LEAST,0);		
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _CODE39_LEAST4:
							QR_moduler_val_assignment(ADDR_CODE39_LEAST,4);				
						  	BSPPlaySuccess();		//�ɹ�						
						break;	
						
						case _CODE39_MOST32:
							QR_moduler_val_assignment(ADDR_CODE39_MOST,32);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _CODE39_MOST255:	//ʹ��ɨ��û���⣬д��־λҲû���⣬����һдflash֮�󣬶��������ݾͲ����ˣ�ò��ģ�����³�ʼ����һ�飬��Ҫ�͹�Ӧ�̹�ͨ
							QR_moduler_val_assignment(ADDR_CODE39_MOST,255);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	

						


						/*CODE93*/
						case _CODE93_EN:
							QR_moduler_set(ADDR_CODE93_ENDIS,0x01,0);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _CODE93_DIS:
							QR_moduler_set(ADDR_CODE93_ENDIS,0,0x01);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	

						case _CODE93_LEAST0:
							QR_moduler_val_assignment(ADDR_CODE93_LEAST,0);		
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _CODE93_LEAST4:
							QR_moduler_val_assignment(ADDR_CODE93_LEAST,4);				
						  	BSPPlaySuccess();		//�ɹ�						
						break;	
						
						case _CODE93_MOST32:
							QR_moduler_val_assignment(ADDR_CODE93_MOST,32);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _CODE93_MOST255:	//ʹ��ɨ��û���⣬д��־λҲû���⣬����һдflash֮�󣬶��������ݾͲ����ˣ�ò��ģ�����³�ʼ����һ�飬��Ҫ�͹�Ӧ�̹�ͨ
							QR_moduler_val_assignment(ADDR_CODE93_MOST,255);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;							



						/*CODEBAR*/
						case _CODEBAR_EN:
							QR_moduler_set(ADDR_CODEBAR_ENDIS,0x01,0);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _CODEBAR_DIS:
							QR_moduler_set(ADDR_CODEBAR_ENDIS,0,0x01);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	

						case _CODEBAR_LEAST0:
							QR_moduler_val_assignment(ADDR_CODEBAR_LEAST,0);		
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _CODEBAR_LEAST4:
							QR_moduler_val_assignment(ADDR_CODEBAR_LEAST,4);				
						  	BSPPlaySuccess();		//�ɹ�						
						break;	
						
						case _CODEBAR_MOST32:
							QR_moduler_val_assignment(ADDR_CODEBAR_MOST,32);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _CODEBAR_MOST255:	//ʹ��ɨ��û���⣬д��־λҲû���⣬����һдflash֮�󣬶��������ݾͲ����ˣ�ò��ģ�����³�ʼ����һ�飬��Ҫ�͹�Ӧ�̹�ͨ
							QR_moduler_val_assignment(ADDR_CODEBAR_MOST,255);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;					
						
						case _CODEBAR_SEND_SRT_END:
							QR_moduler_set(ADDR_CODEBAR_ENDIS,0x02,0);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _CODEBAR_SEND_NON:
							QR_moduler_set(ADDR_CODEBAR_ENDIS,0,0x02);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	

						/*QR*/
						case _QR_EN:
							QR_moduler_set(ADDR_QR_ENDIS,0x01,0);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _QR_DIS:
							QR_moduler_set(ADDR_QR_ENDIS,0,0x01);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;							



						/*INTERLEAVED25*/
						case _INTERLEAVED25_EN:
							QR_moduler_set(ADDR_INTERLEAVED25_ENDIS,0x01,0);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _INTERLEAVED25_DIS:
							QR_moduler_set(ADDR_INTERLEAVED25_ENDIS,0,0x01);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	

						case _INTERLEAVED25_LEAST0:
							QR_moduler_val_assignment(ADDR_INTERLEAVED25_LEAST,0);		
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _INTERLEAVED25_LEAST4:
							QR_moduler_val_assignment(ADDR_INTERLEAVED25_LEAST,4);				
						  	BSPPlaySuccess();		//�ɹ�						
						break;	
						
						case _INTERLEAVED25_MOST32:
							QR_moduler_val_assignment(ADDR_INTERLEAVED25_MOST,32);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _INTERLEAVED25_MOST255:	//ʹ��ɨ��û���⣬д��־λҲû���⣬����һдflash֮�󣬶��������ݾͲ����ˣ�ò��ģ�����³�ʼ����һ�飬��Ҫ�͹�Ӧ�̹�ͨ
							QR_moduler_val_assignment(ADDR_INTERLEAVED25_MOST,255);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;					


						/*INDUSTRIAL25*/
						case _INDUSTRIAL25_EN:
							QR_moduler_set(ADDR_INDUSTRIAL25_ENDIS,0x01,0);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _INDUSTRIAL25_DIS:
							QR_moduler_set(ADDR_INDUSTRIAL25_ENDIS,0,0x01);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	

						case _INDUSTRIAL25_LEAST0:
							QR_moduler_val_assignment(ADDR_INDUSTRIAL25_LEAST,0);		
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _INDUSTRIAL25_LEAST4:
							QR_moduler_val_assignment(ADDR_INDUSTRIAL25_LEAST,4);				
						  	BSPPlaySuccess();		//�ɹ�						
						break;	
						
						case _INDUSTRIAL25_MOST32:
							QR_moduler_val_assignment(ADDR_INDUSTRIAL25_MOST,32);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _INDUSTRIAL25_MOST255:	//ʹ��ɨ��û���⣬д��־λҲû���⣬����һдflash֮�󣬶��������ݾͲ����ˣ�ò��ģ�����³�ʼ����һ�飬��Ҫ�͹�Ӧ�̹�ͨ
							QR_moduler_val_assignment(ADDR_INDUSTRIAL25_MOST,255);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	



						/*MATRIX25*/
						case _MATRIX25_EN:
							QR_moduler_set(ADDR_MATRIX25_ENDIS,0x01,0);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _MATRIX25_DIS:
							QR_moduler_set(ADDR_MATRIX25_ENDIS,0,0x01);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	

						case _MATRIX25_LEAST0:
							QR_moduler_val_assignment(ADDR_MATRIX25_LEAST,0);		
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _MATRIX25_LEAST4:
							QR_moduler_val_assignment(ADDR_MATRIX25_LEAST,4);				
						  	BSPPlaySuccess();		//�ɹ�						
						break;	
						
						case _MATRIX25_MOST32:
							QR_moduler_val_assignment(ADDR_MATRIX25_MOST,32);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _MATRIX25_MOST255:	//ʹ��ɨ��û���⣬д��־λҲû���⣬����һдflash֮�󣬶��������ݾͲ����ˣ�ò��ģ�����³�ʼ����һ�飬��Ҫ�͹�Ӧ�̹�ͨ
							QR_moduler_val_assignment(ADDR_MATRIX25_MOST,255);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	


						/*CODE11*/
						case _CODE11_EN:
							QR_moduler_set(ADDR_CODE11_ENDIS,0x01,0);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _CODE11_DIS:
							QR_moduler_set(ADDR_CODE11_ENDIS,0,0x01);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	

						case _CODE11_LEAST0:
							QR_moduler_val_assignment(ADDR_CODE11_LEAST,0);		
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _CODE11_LEAST4:
							QR_moduler_val_assignment(ADDR_CODE11_LEAST,4);				
						  	BSPPlaySuccess();		//�ɹ�						
						break;	
						
						case _CODE11_MOST32:
							QR_moduler_val_assignment(ADDR_CODE11_MOST,32);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _CODE11_MOST255:	//ʹ��ɨ��û���⣬д��־λҲû���⣬����һдflash֮�󣬶��������ݾͲ����ˣ�ò��ģ�����³�ʼ����һ�飬��Ҫ�͹�Ӧ�̹�ͨ
							QR_moduler_val_assignment(ADDR_CODE11_MOST,255);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	


						/*MSI*/
						case _MSI_EN:
							QR_moduler_set(ADDR_MSI_ENDIS,0x01,0);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _MSI_DIS:
							QR_moduler_set(ADDR_MSI_ENDIS,0,0x01);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	

						case _MSI_LEAST0:
							QR_moduler_val_assignment(ADDR_MSI_LEAST,0);		
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _MSI_LEAST4:
							QR_moduler_val_assignment(ADDR_MSI_LEAST,4);				
						  	BSPPlaySuccess();		//�ɹ�						
						break;	
						
						case _MSI_MOST32:
							QR_moduler_val_assignment(ADDR_MSI_MOST,32);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _MSI_MOST255:	//ʹ��ɨ��û���⣬д��־λҲû���⣬����һдflash֮�󣬶��������ݾͲ����ˣ�ò��ģ�����³�ʼ����һ�飬��Ҫ�͹�Ӧ�̹�ͨ
							QR_moduler_val_assignment(ADDR_MSI_MOST,255);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	


						/*RSS*/

						case _RSS14_EN:
							QR_moduler_set(ADDR_RSS14_ENDIS,0x01,0);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _RSS14_DIS:
							QR_moduler_set(ADDR_RSS14_ENDIS,0,0x01);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	

						case _RSS_LMT_EN:
							QR_moduler_set(ADDR_RSS_LMT_ENDIS,0x01,0);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _RSS_LMT_DIS:
							QR_moduler_set(ADDR_RSS_LMT_ENDIS,0,0x01);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	

						
						case _RSS_EXT_EN:
							QR_moduler_set(ADDR_RSS_EXT_ENDIS,0x01,0);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _RSS_EXT_DIS:
							QR_moduler_set(ADDR_RSS_EXT_ENDIS,0,0x01);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	

						case _RSS_LEAST0:
							QR_moduler_val_assignment(ADDR_RSS_LEAST,0);		
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _RSS_LEAST4:
							QR_moduler_val_assignment(ADDR_RSS_LEAST,4);				
						  	BSPPlaySuccess();		//�ɹ�						
						break;	
						
						case _RSS_MOST32:
							QR_moduler_val_assignment(ADDR_RSS_MOST,32);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _RSS_MOST255:	//ʹ��ɨ��û���⣬д��־λҲû���⣬����һдflash֮�󣬶��������ݾͲ����ˣ�ò��ģ�����³�ʼ����һ�飬��Ҫ�͹�Ӧ�̹�ͨ
							QR_moduler_val_assignment(ADDR_RSS_MOST,255);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	


						/*DM*/
						case _DM_EN:
							QR_moduler_set(ADDR_DM_ENDIS,0x01,0);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _DM_DIS:
							QR_moduler_set(ADDR_DM_ENDIS,0,0x01);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	


						/*PDF417*/
						case _PDF417_EN:
							QR_moduler_set(ADDR_PDF417_ENDIS,0x01,0);			
						  	BSPPlaySuccess();		//�ɹ�							
						break;


						case _PDF417_DIS:
							QR_moduler_set(ADDR_PDF417_ENDIS,0,0x01);			
						  	BSPPlaySuccess();		//�ɹ�						
						break;	















						case _AMERICA:
							SaveCountryKeyboard(keyboard_AMERICAN);
							DevConfig.country_keyboard = ReadCountryKeyboard();
							BSPPlaySuccess();		//�ɹ�	
							
						break;		


						case _GERMANY:
							SaveCountryKeyboard(keyboard_GERMANY);
							DevConfig.country_keyboard = ReadCountryKeyboard();
							BSPPlaySuccess();		//�ɹ�	
							
						break;				


						case _FRANCE_CAN:
							SaveCountryKeyboard(keyboard_FRANCE_CAN);
							DevConfig.country_keyboard = ReadCountryKeyboard();
							BSPPlaySuccess();		//�ɹ�	
							
						break;				

						case _FRANCE:
							SaveCountryKeyboard(keyboard_FRANCE);
							DevConfig.country_keyboard = ReadCountryKeyboard();
							BSPPlaySuccess();		//�ɹ�	
							
						break;		


						case _ITAY:	//�����������
							SaveCountryKeyboard(keyboard_ITALIAN);	//�����������������
							DevConfig.country_keyboard = ReadCountryKeyboard();
							BSPPlaySuccess();		//�ɹ�	
													
						break;
				
						case _PORTUGUESE:	//������-��������
							SaveCountryKeyboard(keyboard_PORTUGUESE);	//������������������
							DevConfig.country_keyboard = ReadCountryKeyboard();
							BSPPlaySuccess();		//�ɹ�	
												
						break;


						case _PORTUGUESE_BR:	//����-��������
							SaveCountryKeyboard(keyboard_PORTUGUESE_BR);	//�����������������
							DevConfig.country_keyboard = ReadCountryKeyboard();
							BSPPlaySuccess();		//�ɹ�	
												
						break;

						
						case _SPANISH:	//��������
							SaveCountryKeyboard(keyboard_SPANISH);	//�����������������
							DevConfig.country_keyboard = ReadCountryKeyboard();
							BSPPlaySuccess();		//�ɹ�	
												
						break;						

						case _DATA:

							data_hide();	//���ݴ���-����
							character_conversion();		//�ַ�ת��
//							string_insert();	//�����ַ���
							
							SendData();
//							if(BSPWirelessWriteBytes(TxBuffer,tx_len))
//							{
//								BSPPlaySuccess();		//�ɹ�	
//								#ifdef DEBUG_SUCCESS_ERROR
//									send_success++;
//								#endif								
//							}
//							else
//							{
//								BSPPlayFail();
//								#ifdef DEBUG_SUCCESS_ERROR
//									send_error++;
//								#endif								
//							}
						break;
						
						
						default: break;
					}

					
					tx_len = DATA_IDX;	
					BSPScanSetComplete(0);		//������0
					get_len_flag=0;		
					get_codeID = 0;
					
					BSPKeyWaitRelease();		//�ȴ��ͷŰ���
				
						
		}
	}
}























