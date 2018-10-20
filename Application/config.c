#include "config.h"
#include "BSPEeprom.h"
#include "BSPWireless.h"
#include "BSPScan.h"
#include "BSPPlay.h"
#include "BSPUsb.h"

#ifdef DEBUG_SUCCESS_ERROR
extern uint32_t send_success,send_error;
#endif

tDevConfig DevConfig;

//��ʾ���û���CODEID
uint8_t codeID_user[20]={'d','d','c','c','c','j','b','i','a','e','D','v','H','m','R','R','R','Q','u','r'};

static Prefix PrefixShadow;
static Suffix SuffixShadow;
static Hide   HideShadow;

static void ReadPrefixToTx(void);





/*********************************************
brief: 		��������
parameter��	void
return��	void	
*********************************************/
void FactorySet(void)
{
	uint32_t val=0;
	
	BSPEepromRead(FLASH_FIRST_USE,&val,1);					//��һ����
	if(val == 0xffffffff)
	{
		BSPWirelessFactoryReset();							//���߲�����������
		
		SaveWireWireless(WIRELESS_MODE);					//����ģʽ
		
		#ifdef DEBUG_SUCCESS_ERROR
		SaveSuccess(0);										//SUCCESS
		SaveErr(0);											//ERROR
		#endif
		
		SaveSingleContinual(SINGLE);						//Single
		
		SaveCountryKeyboard(KEYBOARD_AMERICAN);				//��ʽ����
		
		SaveBeepOption(BEEP_ENABLE);						//��������

		SaveSleepTime(MINUTE_HALF);							//���������

		BaudSet(115200);
		
		/*���������ݽṹ*/
		
		SaveSTXETX(0);										//STX_ETX_DISABLE
		
		SaveCodeNameDisp(CODE_TYPE_DISP_DIS);				//������
		
		SaveCodeIDCharacter();								//CODE_ID�ַ���ʾ
		
		SaveCodeID(CODE_ID_DISABLE);						//CODE_ID_DISABLE
	
			
		PrefixInit();										//����Ĭ��prefix		
		PrefixSave(&DevConfig.prefix);		
		

		SuffixInit();
		SuffixSave(&DevConfig.suffix);

		HideInit();
		SaveHideSet(&DevConfig.hide);
			
		SaveConversion(NO_CONVERSION);

		SaveEndMark(END_CR);
	
	}
}




/*********************************************
brief: 		��ȡ����
parameter��	void
return��	void	
*********************************************/
void ReadConfig(void)
{
	DevConfig.wire_wireless = ReadWireWirelessMode();	//��ȡ����/���� ģʽ
	DevConfig.single_continual = ReadSingleContinual();
	DevConfig.country_keyboard = ReadCountryKeyboard();
	DevConfig.beep_en_dis = ReadBeepOption();
	DevConfig.sleep_time = ReadSleepTime();
	ReadPrefix(&DevConfig.prefix);
	DevConfig.code_ID = ReadCodeID();
	ReadSuffix(&DevConfig.suffix);
	ReadHide(&DevConfig.hide);
	DevConfig.character = (CharConversion)ReadConversion();
	DevConfig.baud_rate = ReadBaudRate();
	DevConfig.STX_ETX = ReadSTXETX();
	DevConfig.end_mark = (EndMark)ReadEndMark();	//��ȡ������
	ReadCodeIDCharacter();
	DevConfig.code_disp = ReadCodeNameDisp();
	
	/*�������ݸ�ʽ*/
	WriteStx();
	WritePrefix();//ǰ׺д��TxBuffer	
	
	#ifdef DEBUG_SUCCESS_ERROR
			/*���ͽ��ղ�����*/
	BSPEepromRead(FLASH_SEND_SUCCESS,&send_success,1);	//��һ����//�����ͳɹ�ʧ�ܴ���
	BSPEepromRead(FLASH_SEND_ERROR,&send_error,1);		//��һ����//�����ͳɹ�ʧ�ܴ���
	#endif	
}






void ConfigComplete(void)
{
	uint32_t val=0;
	
	BSPEepromWrite(FLASH_FIRST_USE,&val,1);	//д0�����Ѿ����ù�
}


/*********************************************
brief: 		���� ����/����
parameter��	
	WIRE_MODE			����
	WIRELESS_MODE		����
return��	void	
*********************************************/
void SaveWireWireless(uint8_t mode)
{
	uint32_t val=mode;	
	BSPEepromWrite(FLASH_WIRE_OR_WIRELESS,&val,1);	//дһ����
}



/*********************************************
brief: 		��ȡ ����/����ģʽ
parameter��	void
return��	
	WIRE_MODE			����
	WIRELESS_MODE		����
*********************************************/
uint8_t ReadWireWirelessMode(void)
{
	uint32_t val;
	BSPEepromRead(FLASH_WIRE_OR_WIRELESS,&val,1);	//��һ����
	return (uint8_t)val;
}


/*********************************************
brief: 		���� ����/����ģʽ
parameter��	
	SINGLE 		
	CONTINUAL 	
return��	void
*********************************************/
void SaveSingleContinual(uint8_t sin_con)	
{
	uint32_t val = sin_con;
	BSPEepromWrite(FLASH_SINGLE_CONTINUAL,&val,1);	//дһ����	
}


/*********************************************
brief: 		��ȡ ����/����ģʽ
parameter��	void	
return��	
	SINGLE 		
	CONTINUAL 
*********************************************/
uint8_t ReadSingleContinual(void)
{
	uint32_t single_continual;
	BSPEepromRead(FLASH_SINGLE_CONTINUAL,&single_continual,1);	//��һ����
	
	return (uint8_t)single_continual;
}



/*********************************************
brief: 		���� ��������
parameter��	
	KEYBOARD_AMERICAN			//��ʽ����-����
	KEYBOARD_GERMANY			//����
	KEYBOARD_FRANCE_CAN			//������ô�
	KEYBOARD_FRANCE				//���������
	KEYBOARD_ITALIAN			//�����142
	KEYBOARD_PORTUGUESE			//������
	KEYBOARD_PORTUGUESE_BR		//�������������
	KEYBOARD_SPANISH			//��������������
return��	void
*********************************************/
void SaveCountryKeyboard(uint8_t country)	
{
	uint32_t val = country;
	BSPEepromWrite(FLASH_COUNTRY,&val,1);	//дһ����	
}



/*********************************************
brief: 		��ȡ ��������
parameter��	void
return��	
	KEYBOARD_AMERICAN			//��ʽ����-����
	KEYBOARD_GERMANY			//����
	KEYBOARD_FRANCE_CAN			//������ô�
	KEYBOARD_FRANCE				//���������
	KEYBOARD_ITALIAN			//�����142
	KEYBOARD_PORTUGUESE			//������
	KEYBOARD_PORTUGUESE_BR		//�������������
	KEYBOARD_SPANISH			//��������������
*********************************************/
uint8_t ReadCountryKeyboard()
{
	uint32_t country;
	BSPEepromRead(FLASH_COUNTRY,&country,1);	//��һ����
	
	return (uint8_t)country;
}



/*********************************************
brief: 		���� �������Ƿ�ʹ��
parameter��		
	BEEP_ENABLE	
	BEEP_DISABLE
return��	void
*********************************************/
void SaveBeepOption(uint8_t beep)	
{
	uint32_t val = beep;
	BSPEepromWrite(FLASH_BEEP,&val,1);	//дһ����	
}



/*********************************************
brief: 		��ȡ �������Ƿ�ʹ��
parameter��	void
return��	
	BEEP_ENABLE	
	BEEP_DISABLE
*********************************************/
uint8_t ReadBeepOption(void)
{
	uint32_t beep;
	BSPEepromRead(FLASH_BEEP,&beep,1);	//��һ����
	
	return (uint8_t)beep;
}



/*********************************************
brief: 		���� ˯��ʱ��
parameter��	
	MINUTE_HALF 
	MINUTE_1 	 
	MINUTE_5 	
	MINUTE_10 	
	MINUTE_20 
return��	void	
*********************************************/
void SaveSleepTime(uint32_t time)
{
	uint32_t val = time;
	BSPEepromWrite(FLASH_SLEEP,&val,1);	//дһ����		
}


/*********************************************
brief: 		��ȡ ˯��ʱ��
parameter��	void
return��	
	MINUTE_HALF 
	MINUTE_1 	 
	MINUTE_5 	
	MINUTE_10 	
	MINUTE_20 	
*********************************************/
uint32_t ReadSleepTime(void)
{
	uint32_t sleep_time;
	BSPEepromRead(FLASH_SLEEP,&sleep_time,1);	//��һ����
	
	return sleep_time;
}




/*********************************************
brief: 		���ò�����
parameter��	uint32_t
return��	void
*********************************************/
void BaudSet(uint32_t baud)
{
	DevConfig.baud_rate = baud;
	BSPEepromWrite(FLASH_BAUD,&baud,1);					
}


/*********************************************
brief: 		��ȡ������
parameter��	void
return��	uint32_t
*********************************************/
uint32_t ReadBaudRate(void)
{
	uint32_t baud;
	BSPEepromRead(FLASH_BAUD,&baud,1); 
	
	return baud;
}




/*********************************************
brief: 		���� STX ETX
parameter��	uint8_t ����λ
return��	void
*********************************************/
void SaveSTXETX(uint8_t option)
{
	uint32_t val = option;	
	BSPEepromWrite(FLASH_STX_ETX,&val,1);	//дһ����			
}


/*********************************************
brief: 		��ȡ STX ETX
parameter��	void
return��	uint8_t ����λ
*********************************************/
uint8_t ReadSTXETX(void)
{
	uint32_t stx_etx;
	BSPEepromRead(FLASH_STX_ETX,&stx_etx,1); //��һ����
	
	return (uint8_t)stx_etx;	
}





/*********************************************
brief: 		���� �Ƿ���ʾ����������
parameter��	uint8_t ����λ
return��	void
*********************************************/
void SaveCodeNameDisp(uint32_t option)
{
	uint32_t val = option;
	BSPEepromWrite(FLASH_CODE_DISP,&val,1);	//дһ����	
}



/*********************************************
brief: 		��ȡ �Ƿ���ʾ����������
parameter��	
	
return��	uint8_t ����λ
*********************************************/
CodeNameDisplay ReadCodeNameDisp(void)
{
	uint32_t code_dis;
	BSPEepromRead(FLASH_CODE_DISP,&code_dis,1);	//��һ����
	
	return (CodeNameDisplay)code_dis;	
}




/*********************************************
brief: 		���� CODEID �ַ�
parameter��	void
return��	void
*********************************************/
void SaveCodeIDCharacter(void)
{
	uint32_t *ptr = (uint32_t*)codeID_user;
	BSPEepromWrite(FLASH_USER_CODEID,ptr,sizeof(codeID_user)/sizeof(int));	//дһ����		
}


/*********************************************
brief: 		��ȡ CODEID �ַ�
parameter��	void
return��	void
*********************************************/
void ReadCodeIDCharacter()
{
	BSPEepromRead(FLASH_USER_CODEID,(uint32_t*)codeID_user,sizeof(codeID_user)/sizeof(int)); //��һ����
}


/*********************************************
brief: 		���� CODEID �Ƿ�ʹ��
parameter��	
	CODE_ID_ENABLE 
	CODE_ID_DISABLE
return��	void
*********************************************/
void SaveCodeID(uint32_t option)
{
	uint32_t val = option;
	BSPEepromWrite(FLASH_CODE_ID,&val,1);	//дһ����		
}

/*********************************************
brief: 		��ȡ CODEID �Ƿ�ʹ��
parameter��	void
return��	
	CODE_ID_ENABLE 
	CODE_ID_DISABLE
*********************************************/
uint8_t ReadCodeID(void)
{
	uint32_t code_id;
	BSPEepromRead(FLASH_CODE_ID,&code_id,1);	//��һ����
	
	return (uint8_t)code_id;	
}




/*********************************************
brief: 		ǰ׺��ʼ��
parameter��	void
return��	void
*********************************************/
void PrefixInit(void)
{
	DevConfig.prefix.en_dis = PREFIX_DISABLE;
	DevConfig.prefix.len = 0;

	DevConfig.prefix.Tx_prefix_len = GetPrefixLen(DevConfig.prefix);		
}



/*********************************************
brief: 		����ǰ׺
parameter��	Prefix*  ǰ׺ָ��
return��	void
*********************************************/
void PrefixSave(Prefix* prefix)
{
	uint32_t unit[5]={0,0,0,0,0};
	uint32_t addr = FLASH_PREFIX;
	uint8_t size = sizeof(Prefix);
	uint8_t i;
	uint8_t *ptr = (uint8_t*)prefix;
	uint8_t count = size/4 + ((size%4)?1:0);	//����sizeof��С����Ҫ���ٸ��� ������
	
	for(i=0;i<count*4;i++)
	{
		unit[i/4] |= ptr[i];
		if(0 != (i+1)%4 )
			unit[i/4] <<= 8;
		else
		{
			addr+=4;
		}	
	}
	BSPEepromWrite(FLASH_PREFIX,unit,count);	//дһ����
}



/*********************************************
brief: 		��ȡǰ׺
parameter��	Prefix*  ǰ׺ָ��
return��	void
*********************************************/
void ReadPrefix(Prefix* prefix)
{
	uint32_t val;
	signed char i;
	uint8_t *ptr = (uint8_t*)prefix;	
	uint32_t addr = FLASH_PREFIX;
	uint8_t count = sizeof(Prefix)/4 + ((sizeof(Prefix)%4)?1:0);	//����sizeof��С����Ҫ���ٸ��� ������
	
	
	for(;count>0;count--)
	{
		BSPEepromRead(addr,&val,1);	//����һ����
		for(i=3;i>=0;i--)			//�ֲ���ֽ�
		{
			ptr[i] = (uint8_t)val;
			val >>=8;
		}	
		addr +=4;
		ptr +=4;
	}
}



/*********************************************
brief: 		ǰ׺��ֵ
parameter��	void
return��	void
*********************************************/
void PrefixAssignment(void)
{
	uint8_t i;
	
	DevConfig.prefix.len = PrefixShadow.len;
	for(i=0;i<PrefixShadow.len;i++)
	{
		DevConfig.prefix.buf[i] = PrefixShadow.buf[i];
	}
}



/*********************************************
brief: 		��ȡǰ׺����
parameter��	Prefix
return��	uint8_t
*********************************************/
uint8_t GetPrefixLen(Prefix prefix)			
{
	if(prefix.en_dis == PREFIX_ENABLE)	
	{
		ReadPrefixToTx();
		return prefix.len;	
	}
	else
	{
		return 0;						
	}
}



/*********************************************
brief: 		��ǰ׺װ�ص�TxBuffer
parameter��	void
return��	void
*********************************************/
static void ReadPrefixToTx(void)
{
	uint8_t i;
	for(i=0;i<DevConfig.prefix.len;i++)
	{
		TxBuffer[i+PREFIX_IDX] = DevConfig.prefix.buf[i];	
	}
}








/*********************************************
brief: 		��׺��ʼ��
parameter��	void
return��	void
*********************************************/
void SuffixInit(void)
{
	DevConfig.suffix.en_dis = SUFFIX_DISABLE;
	DevConfig.suffix.len = 0;	
	DevConfig.suffix.TX_suffix_len = 0;	
}


/*********************************************
brief: 		�����׺
parameter��	Suffix* ָ��
return��	void
*********************************************/
void SuffixSave(Suffix* suffix)
{
	uint32_t unit[5]={0,0,0,0,0};
	uint32_t addr = FLASH_SUFFIX;
	uint8_t size = sizeof(Suffix);
	uint8_t i;
	uint8_t *ptr = (uint8_t*)suffix;
	uint8_t count = size/4 + ((size%4)?1:0);	//����sizeof��С����Ҫ���ٸ��� ������
	
	for(i=0;i<count*4;i++)
	{
		unit[i/4] |= ptr[i];
		if(0 != (i+1)%4 )
			unit[i/4] <<= 8;
		else
		{
			addr+=4;
		}	
	}
	BSPEepromWrite(FLASH_SUFFIX,unit,count);	//дһ����	
}



/*********************************************
brief: 		��ȡ��׺
parameter��	Suffix* ָ��
return��	void
*********************************************/
void ReadSuffix(Suffix* suffix)
{
	uint32_t val;
	signed char i;
	uint8_t *ptr = (uint8_t*)suffix;	
	uint32_t addr = FLASH_SUFFIX;
	uint8_t count = sizeof(Suffix)/4 + ((sizeof(Suffix)%4)?1:0);	//����sizeof��С����Ҫ���ٸ��� ������
	
	
	for(;count>0;count--)
	{
		BSPEepromRead(addr,&val,1);	//����һ����
		for(i=3;i>=0;i--)			//�ֲ���ֽ�
		{
			ptr[i] = (uint8_t)val;
			val >>=8;
		}	
		addr +=4;
		ptr +=4;
	}
}



/*********************************************
brief: 		��׺��ֵ
parameter��	void
return��	void
*********************************************/
void SuffixAssignment(void)
{
	uint8_t i;
	
	DevConfig.suffix.len = SuffixShadow.len;
	for(i=0;i<SuffixShadow.len;i++)
	{
		DevConfig.suffix.buf[i] = SuffixShadow.buf[i];
	}
}




/*********************************************
brief: 		�ַ������س�ʼ��
parameter��	void
return��	void
*********************************************/
void HideInit(void)
{
	uint8_t i;
	
	DevConfig.hide.head_hide_enDis = HIDE_DISABLE;
	DevConfig.hide.head_hide_len = 0;

	DevConfig.hide.tail_hide_enDis = HIDE_DISABLE;
	DevConfig.hide.tail_hide_len = 0;	

	DevConfig.hide.string_hide_enDis = HIDE_DISABLE;
	DevConfig.hide.string_len = 0;
	for(i=0;i<15;i++)
	{
		DevConfig.hide.string_hide[i]=0;
	}
}


/*********************************************
brief: 		������������
parameter��	Hide* ָ��
return��	void
*********************************************/
void SaveHideSet(Hide* p_hide)
{
	uint32_t unit[6]={0,0,0,0,0,0};
	uint32_t addr = FLASH_HIDE;
	uint8_t size = sizeof(Hide);
	uint8_t i;
	uint8_t *ptr = (uint8_t*)p_hide;
	uint8_t count = size/4 + ((size%4)?1:0);	//����sizeof��С����Ҫ���ٸ��� ������
	
	for(i=0;i<count*4;i++)
	{
		unit[i/4] |= ptr[i];
		if(0 != (i+1)%4 )
			unit[i/4] <<= 8;
		else
		{
			addr+=4;
		}	
	}
	BSPEepromWrite(FLASH_HIDE,unit,count);	//дһ����	
}


/*********************************************
brief: 		��ȡ��������
parameter��	Hide* ָ��
return��	void
*********************************************/
void ReadHide(Hide* p_hide)
{
	uint32_t val;
	signed char i;
	uint8_t *ptr = (uint8_t*)p_hide;	
	uint32_t addr = FLASH_HIDE;
	uint8_t count = sizeof(Hide)/4 + ((sizeof(Hide)%4)?1:0);	//����sizeof��С����Ҫ���ٸ��� ������
	
	
	for(;count>0;count--)
	{
		BSPEepromRead(addr,&val,1);	//����һ����
		for(i=3;i>=0;i--)			//�ֲ���ֽ�
		{
			ptr[i] = (uint8_t)val;
			val >>=8;
		}	
		addr +=4;
		ptr +=4;
	}
}


/*********************************************
brief: 		�������ø�ֵ
parameter��	void
return��	void
*********************************************/
void StringAssignment(void)
{
	uint8_t i;

	DevConfig.hide.string_len = hide_shadow.string_len;
	for(i=0;i<hide_shadow.string_len;i++)
	{
		DevConfig.hide.string_hide[i] = hide_shadow.string_hide[i];
	}

}


/*********************************************
brief: 		����ת������
parameter��	
	NO_CONVERSION,
	UPPERCASE_CONVERSION
	LOWERCASE_CONVERSION
	CHARACTER_CONVERSION
return��	void
*********************************************/
void SaveConversion(CharConversion val)
{
	DevConfig.character = val;
	BSPEepromWrite(FLASH_CHARACTER,(uint32_t*)&val,1);						
}


/*********************************************
brief: 		��ȡת������
parameter��	void
return��	
	NO_CONVERSION,
	UPPERCASE_CONVERSION
	LOWERCASE_CONVERSION
	CHARACTER_CONVERSION
*********************************************/
uint8_t ReadConversion()
{
	uint32_t character;
	BSPEepromRead(FLASH_CHARACTER,&character,1); //��һ����
	
	return (uint8_t)character;
}




/*********************************************
brief: 		���������
parameter��
	END_CR
	END_LF
	END_CR_LF
	END_HT
	END_SPACE
	END_EMPTY
return��	void
*********************************************/
void SaveEndMark(EndMark option)
{
	uint32_t val = option;
	BSPEepromWrite(FLASH_END_MARK,&val,1);	//дһ����			
}


/*********************************************
brief: 		��ȡ������
parameter��	void
return��	
	END_CR
	END_LF
	END_CR_LF
	END_HT
	END_SPACE
	END_EMPTY
*********************************************/
uint8_t ReadEndMark(void)
{
	uint32_t end_mark;
	BSPEepromRead(FLASH_END_MARK,&end_mark,1); //��һ����
	
	return (uint8_t)end_mark;	
}




void WriteStx(void)
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



void WriteEtx(void)
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



void WritePrefix(void)
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



static void SendWireless()
{
	if(BSPWirelessWriteBytes(TxBuffer,tx_len))
	{
		BSPPlaySuccess();		//�ɹ�	
		#ifdef DEBUG_SUCCESS_ERROR
			send_success++;
		#endif								
	}
	else
	{
		BSPPlayFail();
		#ifdef DEBUG_SUCCESS_ERROR
			send_error++;
		#endif								
	}	
}



void SendData()
{
	if(DevConfig.wire_wireless == WIRELESS_MODE)		//����ģʽ
	{
		SendWireless();
	}
	else												//����ģʽ
	{
		SendWire();
	}
}

#ifdef DEBUG_SUCCESS_ERROR
void SaveSuccess(uint32_t suc)
{
	BSPEepromWrite(FLASH_SEND_SUCCESS,&suc,1);	//дһ����	
}



void SaveErr(uint32_t err)
{
	BSPEepromWrite(FLASH_SEND_ERROR,&err,1);	//дһ����	
}
#endif



