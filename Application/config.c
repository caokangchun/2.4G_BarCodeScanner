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

//显示给用户的CODEID
uint8_t codeID_user[20]={'d','d','c','c','c','j','b','i','a','e','D','v','H','m','R','R','R','Q','u','r'};

static Prefix PrefixShadow;
static Suffix SuffixShadow;
static Hide   HideShadow;

static void ReadPrefixToTx(void);





/*********************************************
brief: 		出厂设置
parameter：	void
return：	void	
*********************************************/
void FactorySet(void)
{
	uint32_t val=0;
	
	BSPEepromRead(FLASH_FIRST_USE,&val,1);					//读一个字
	if(val == 0xffffffff)
	{
		BSPWirelessFactoryReset();							//无线参数出厂设置
		
		SaveWireWireless(WIRELESS_MODE);					//无线模式
		
		#ifdef DEBUG_SUCCESS_ERROR
		SaveSuccess(0);										//SUCCESS
		SaveErr(0);											//ERROR
		#endif
		
		SaveSingleContinual(SINGLE);						//Single
		
		SaveCountryKeyboard(KEYBOARD_AMERICAN);				//美式键盘
		
		SaveBeepOption(BEEP_ENABLE);						//蜂鸣器打开

		SaveSleepTime(MINUTE_HALF);							//半分钟休眠

		BaudSet(115200);
		
		/*以下是数据结构*/
		
		SaveSTXETX(0);										//STX_ETX_DISABLE
		
		SaveCodeNameDisp(CODE_TYPE_DISP_DIS);				//条码名
		
		SaveCodeIDCharacter();								//CODE_ID字符显示
		
		SaveCodeID(CODE_ID_DISABLE);						//CODE_ID_DISABLE
	
			
		PrefixInit();										//设置默认prefix		
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
brief: 		读取设置
parameter：	void
return：	void	
*********************************************/
void ReadConfig(void)
{
	DevConfig.wire_wireless = ReadWireWirelessMode();	//获取有线/无线 模式
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
	DevConfig.end_mark = (EndMark)ReadEndMark();	//获取结束符
	ReadCodeIDCharacter();
	DevConfig.code_disp = ReadCodeNameDisp();
	
	/*设置数据格式*/
	WriteStx();
	WritePrefix();//前缀写入TxBuffer	
	
	#ifdef DEBUG_SUCCESS_ERROR
			/*发送接收测试用*/
	BSPEepromRead(FLASH_SEND_SUCCESS,&send_success,1);	//读一个字//读发送成功失败次数
	BSPEepromRead(FLASH_SEND_ERROR,&send_error,1);		//读一个字//读发送成功失败次数
	#endif	
}






void ConfigComplete(void)
{
	uint32_t val=0;
	
	BSPEepromWrite(FLASH_FIRST_USE,&val,1);	//写0代表已经设置过
}


/*********************************************
brief: 		保存 有线/无线
parameter：	
	WIRE_MODE			有线
	WIRELESS_MODE		无线
return：	void	
*********************************************/
void SaveWireWireless(uint8_t mode)
{
	uint32_t val=mode;	
	BSPEepromWrite(FLASH_WIRE_OR_WIRELESS,&val,1);	//写一个字
}



/*********************************************
brief: 		读取 有线/无线模式
parameter：	void
return：	
	WIRE_MODE			有线
	WIRELESS_MODE		无线
*********************************************/
uint8_t ReadWireWirelessMode(void)
{
	uint32_t val;
	BSPEepromRead(FLASH_WIRE_OR_WIRELESS,&val,1);	//读一个字
	return (uint8_t)val;
}


/*********************************************
brief: 		保存 单次/连续模式
parameter：	
	SINGLE 		
	CONTINUAL 	
return：	void
*********************************************/
void SaveSingleContinual(uint8_t sin_con)	
{
	uint32_t val = sin_con;
	BSPEepromWrite(FLASH_SINGLE_CONTINUAL,&val,1);	//写一个字	
}


/*********************************************
brief: 		读取 单次/连续模式
parameter：	void	
return：	
	SINGLE 		
	CONTINUAL 
*********************************************/
uint8_t ReadSingleContinual(void)
{
	uint32_t single_continual;
	BSPEepromRead(FLASH_SINGLE_CONTINUAL,&single_continual,1);	//读一个字
	
	return (uint8_t)single_continual;
}



/*********************************************
brief: 		保存 键盘类型
parameter：	
	KEYBOARD_AMERICAN			//美式键盘-中文
	KEYBOARD_GERMANY			//德语
	KEYBOARD_FRANCE_CAN			//法语（加拿大）
	KEYBOARD_FRANCE				//法语（法国）
	KEYBOARD_ITALIAN			//意大利142
	KEYBOARD_PORTUGUESE			//葡萄牙
	KEYBOARD_PORTUGUESE_BR		//葡萄牙语（巴西）
	KEYBOARD_SPANISH			//西班牙国际排序
return：	void
*********************************************/
void SaveCountryKeyboard(uint8_t country)	
{
	uint32_t val = country;
	BSPEepromWrite(FLASH_COUNTRY,&val,1);	//写一个字	
}



/*********************************************
brief: 		读取 键盘类型
parameter：	void
return：	
	KEYBOARD_AMERICAN			//美式键盘-中文
	KEYBOARD_GERMANY			//德语
	KEYBOARD_FRANCE_CAN			//法语（加拿大）
	KEYBOARD_FRANCE				//法语（法国）
	KEYBOARD_ITALIAN			//意大利142
	KEYBOARD_PORTUGUESE			//葡萄牙
	KEYBOARD_PORTUGUESE_BR		//葡萄牙语（巴西）
	KEYBOARD_SPANISH			//西班牙国际排序
*********************************************/
uint8_t ReadCountryKeyboard()
{
	uint32_t country;
	BSPEepromRead(FLASH_COUNTRY,&country,1);	//读一个字
	
	return (uint8_t)country;
}



/*********************************************
brief: 		保存 蜂鸣器是否使能
parameter：		
	BEEP_ENABLE	
	BEEP_DISABLE
return：	void
*********************************************/
void SaveBeepOption(uint8_t beep)	
{
	uint32_t val = beep;
	BSPEepromWrite(FLASH_BEEP,&val,1);	//写一个字	
}



/*********************************************
brief: 		读取 蜂鸣器是否使能
parameter：	void
return：	
	BEEP_ENABLE	
	BEEP_DISABLE
*********************************************/
uint8_t ReadBeepOption(void)
{
	uint32_t beep;
	BSPEepromRead(FLASH_BEEP,&beep,1);	//读一个字
	
	return (uint8_t)beep;
}



/*********************************************
brief: 		保存 睡眠时间
parameter：	
	MINUTE_HALF 
	MINUTE_1 	 
	MINUTE_5 	
	MINUTE_10 	
	MINUTE_20 
return：	void	
*********************************************/
void SaveSleepTime(uint32_t time)
{
	uint32_t val = time;
	BSPEepromWrite(FLASH_SLEEP,&val,1);	//写一个字		
}


/*********************************************
brief: 		读取 睡眠时间
parameter：	void
return：	
	MINUTE_HALF 
	MINUTE_1 	 
	MINUTE_5 	
	MINUTE_10 	
	MINUTE_20 	
*********************************************/
uint32_t ReadSleepTime(void)
{
	uint32_t sleep_time;
	BSPEepromRead(FLASH_SLEEP,&sleep_time,1);	//读一个字
	
	return sleep_time;
}




/*********************************************
brief: 		设置波特率
parameter：	uint32_t
return：	void
*********************************************/
void BaudSet(uint32_t baud)
{
	DevConfig.baud_rate = baud;
	BSPEepromWrite(FLASH_BAUD,&baud,1);					
}


/*********************************************
brief: 		读取波特率
parameter：	void
return：	uint32_t
*********************************************/
uint32_t ReadBaudRate(void)
{
	uint32_t baud;
	BSPEepromRead(FLASH_BAUD,&baud,1); 
	
	return baud;
}




/*********************************************
brief: 		保存 STX ETX
parameter：	uint8_t 低两位
return：	void
*********************************************/
void SaveSTXETX(uint8_t option)
{
	uint32_t val = option;	
	BSPEepromWrite(FLASH_STX_ETX,&val,1);	//写一个字			
}


/*********************************************
brief: 		读取 STX ETX
parameter：	void
return：	uint8_t 低两位
*********************************************/
uint8_t ReadSTXETX(void)
{
	uint32_t stx_etx;
	BSPEepromRead(FLASH_STX_ETX,&stx_etx,1); //读一个字
	
	return (uint8_t)stx_etx;	
}





/*********************************************
brief: 		保存 是否显示条码类型名
parameter：	uint8_t 低两位
return：	void
*********************************************/
void SaveCodeNameDisp(uint32_t option)
{
	uint32_t val = option;
	BSPEepromWrite(FLASH_CODE_DISP,&val,1);	//写一个字	
}



/*********************************************
brief: 		读取 是否显示条码类型名
parameter：	
	
return：	uint8_t 低两位
*********************************************/
CodeNameDisplay ReadCodeNameDisp(void)
{
	uint32_t code_dis;
	BSPEepromRead(FLASH_CODE_DISP,&code_dis,1);	//读一个字
	
	return (CodeNameDisplay)code_dis;	
}




/*********************************************
brief: 		保存 CODEID 字符
parameter：	void
return：	void
*********************************************/
void SaveCodeIDCharacter(void)
{
	uint32_t *ptr = (uint32_t*)codeID_user;
	BSPEepromWrite(FLASH_USER_CODEID,ptr,sizeof(codeID_user)/sizeof(int));	//写一个字		
}


/*********************************************
brief: 		读取 CODEID 字符
parameter：	void
return：	void
*********************************************/
void ReadCodeIDCharacter()
{
	BSPEepromRead(FLASH_USER_CODEID,(uint32_t*)codeID_user,sizeof(codeID_user)/sizeof(int)); //读一个字
}


/*********************************************
brief: 		保存 CODEID 是否使能
parameter：	
	CODE_ID_ENABLE 
	CODE_ID_DISABLE
return：	void
*********************************************/
void SaveCodeID(uint32_t option)
{
	uint32_t val = option;
	BSPEepromWrite(FLASH_CODE_ID,&val,1);	//写一个字		
}

/*********************************************
brief: 		读取 CODEID 是否使能
parameter：	void
return：	
	CODE_ID_ENABLE 
	CODE_ID_DISABLE
*********************************************/
uint8_t ReadCodeID(void)
{
	uint32_t code_id;
	BSPEepromRead(FLASH_CODE_ID,&code_id,1);	//读一个字
	
	return (uint8_t)code_id;	
}




/*********************************************
brief: 		前缀初始化
parameter：	void
return：	void
*********************************************/
void PrefixInit(void)
{
	DevConfig.prefix.en_dis = PREFIX_DISABLE;
	DevConfig.prefix.len = 0;

	DevConfig.prefix.Tx_prefix_len = GetPrefixLen(DevConfig.prefix);		
}



/*********************************************
brief: 		保存前缀
parameter：	Prefix*  前缀指针
return：	void
*********************************************/
void PrefixSave(Prefix* prefix)
{
	uint32_t unit[5]={0,0,0,0,0};
	uint32_t addr = FLASH_PREFIX;
	uint8_t size = sizeof(Prefix);
	uint8_t i;
	uint8_t *ptr = (uint8_t*)prefix;
	uint8_t count = size/4 + ((size%4)?1:0);	//根据sizeof大小，需要多少个字 来保存
	
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
	BSPEepromWrite(FLASH_PREFIX,unit,count);	//写一个字
}



/*********************************************
brief: 		读取前缀
parameter：	Prefix*  前缀指针
return：	void
*********************************************/
void ReadPrefix(Prefix* prefix)
{
	uint32_t val;
	signed char i;
	uint8_t *ptr = (uint8_t*)prefix;	
	uint32_t addr = FLASH_PREFIX;
	uint8_t count = sizeof(Prefix)/4 + ((sizeof(Prefix)%4)?1:0);	//根据sizeof大小，需要多少个字 来保存
	
	
	for(;count>0;count--)
	{
		BSPEepromRead(addr,&val,1);	//读出一个字
		for(i=3;i>=0;i--)			//字拆分字节
		{
			ptr[i] = (uint8_t)val;
			val >>=8;
		}	
		addr +=4;
		ptr +=4;
	}
}



/*********************************************
brief: 		前缀赋值
parameter：	void
return：	void
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
brief: 		获取前缀长度
parameter：	Prefix
return：	uint8_t
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
brief: 		将前缀装载到TxBuffer
parameter：	void
return：	void
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
brief: 		后缀初始化
parameter：	void
return：	void
*********************************************/
void SuffixInit(void)
{
	DevConfig.suffix.en_dis = SUFFIX_DISABLE;
	DevConfig.suffix.len = 0;	
	DevConfig.suffix.TX_suffix_len = 0;	
}


/*********************************************
brief: 		保存后缀
parameter：	Suffix* 指针
return：	void
*********************************************/
void SuffixSave(Suffix* suffix)
{
	uint32_t unit[5]={0,0,0,0,0};
	uint32_t addr = FLASH_SUFFIX;
	uint8_t size = sizeof(Suffix);
	uint8_t i;
	uint8_t *ptr = (uint8_t*)suffix;
	uint8_t count = size/4 + ((size%4)?1:0);	//根据sizeof大小，需要多少个字 来保存
	
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
	BSPEepromWrite(FLASH_SUFFIX,unit,count);	//写一个字	
}



/*********************************************
brief: 		读取后缀
parameter：	Suffix* 指针
return：	void
*********************************************/
void ReadSuffix(Suffix* suffix)
{
	uint32_t val;
	signed char i;
	uint8_t *ptr = (uint8_t*)suffix;	
	uint32_t addr = FLASH_SUFFIX;
	uint8_t count = sizeof(Suffix)/4 + ((sizeof(Suffix)%4)?1:0);	//根据sizeof大小，需要多少个字 来保存
	
	
	for(;count>0;count--)
	{
		BSPEepromRead(addr,&val,1);	//读出一个字
		for(i=3;i>=0;i--)			//字拆分字节
		{
			ptr[i] = (uint8_t)val;
			val >>=8;
		}	
		addr +=4;
		ptr +=4;
	}
}



/*********************************************
brief: 		后缀赋值
parameter：	void
return：	void
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
brief: 		字符串隐藏初始化
parameter：	void
return：	void
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
brief: 		保存隐藏设置
parameter：	Hide* 指针
return：	void
*********************************************/
void SaveHideSet(Hide* p_hide)
{
	uint32_t unit[6]={0,0,0,0,0,0};
	uint32_t addr = FLASH_HIDE;
	uint8_t size = sizeof(Hide);
	uint8_t i;
	uint8_t *ptr = (uint8_t*)p_hide;
	uint8_t count = size/4 + ((size%4)?1:0);	//根据sizeof大小，需要多少个字 来保存
	
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
	BSPEepromWrite(FLASH_HIDE,unit,count);	//写一个字	
}


/*********************************************
brief: 		读取隐藏设置
parameter：	Hide* 指针
return：	void
*********************************************/
void ReadHide(Hide* p_hide)
{
	uint32_t val;
	signed char i;
	uint8_t *ptr = (uint8_t*)p_hide;	
	uint32_t addr = FLASH_HIDE;
	uint8_t count = sizeof(Hide)/4 + ((sizeof(Hide)%4)?1:0);	//根据sizeof大小，需要多少个字 来保存
	
	
	for(;count>0;count--)
	{
		BSPEepromRead(addr,&val,1);	//读出一个字
		for(i=3;i>=0;i--)			//字拆分字节
		{
			ptr[i] = (uint8_t)val;
			val >>=8;
		}	
		addr +=4;
		ptr +=4;
	}
}


/*********************************************
brief: 		隐藏设置赋值
parameter：	void
return：	void
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
brief: 		保存转换设置
parameter：	
	NO_CONVERSION,
	UPPERCASE_CONVERSION
	LOWERCASE_CONVERSION
	CHARACTER_CONVERSION
return：	void
*********************************************/
void SaveConversion(CharConversion val)
{
	DevConfig.character = val;
	BSPEepromWrite(FLASH_CHARACTER,(uint32_t*)&val,1);						
}


/*********************************************
brief: 		读取转换设置
parameter：	void
return：	
	NO_CONVERSION,
	UPPERCASE_CONVERSION
	LOWERCASE_CONVERSION
	CHARACTER_CONVERSION
*********************************************/
uint8_t ReadConversion()
{
	uint32_t character;
	BSPEepromRead(FLASH_CHARACTER,&character,1); //读一个字
	
	return (uint8_t)character;
}




/*********************************************
brief: 		保存结束符
parameter：
	END_CR
	END_LF
	END_CR_LF
	END_HT
	END_SPACE
	END_EMPTY
return：	void
*********************************************/
void SaveEndMark(EndMark option)
{
	uint32_t val = option;
	BSPEepromWrite(FLASH_END_MARK,&val,1);	//写一个字			
}


/*********************************************
brief: 		读取结束符
parameter：	void
return：	
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
	BSPEepromRead(FLASH_END_MARK,&end_mark,1); //读一个字
	
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
	if(DevConfig.prefix.en_dis == PREFIX_ENABLE)	//前缀使能时，添加前缀
	{
		for(i=0;i<DevConfig.prefix.len;i++)
		{
			TxBuffer[i+PREFIX_IDX] = DevConfig.prefix.buf[i];
		}
		tx_len = DATA_IDX;				//设置前缀长度
	}
	else											//前缀禁止时
	{
		tx_len = DATA_IDX;
	}
}



static void SendWireless()
{
	if(BSPWirelessWriteBytes(TxBuffer,tx_len))
	{
		BSPPlaySuccess();		//成功	
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
	if(DevConfig.wire_wireless == WIRELESS_MODE)		//无线模式
	{
		SendWireless();
	}
	else												//有线模式
	{
		SendWire();
	}
}

#ifdef DEBUG_SUCCESS_ERROR
void SaveSuccess(uint32_t suc)
{
	BSPEepromWrite(FLASH_SEND_SUCCESS,&suc,1);	//写一个字	
}



void SaveErr(uint32_t err)
{
	BSPEepromWrite(FLASH_SEND_ERROR,&err,1);	//写一个字	
}
#endif



