#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "BSPWatchDog.h"

#include "data_format.h"

#include "BSPEeprom.h"

#include "debug.h"


/*有线无线*/
#define WIRE_MODE			0
#define WIRELESS_MODE		1


/*休眠时间*/
#define MINUTE_HALF 30000000 
#define MINUTE_1 	60000000
#define MINUTE_5 	300000000
#define MINUTE_10 	600000000
#define MINUTE_20 	1200000000


#define SINGLE 		0
#define CONTINUAL 	1


#define BEEP_ENABLE		0
#define BEEP_DISABLE	1


#define CODE_ID_ENABLE 	1
#define CODE_ID_DISABLE	0

enum
{
	KEYBOARD_AMERICAN = 0,		//美式键盘-中文
	KEYBOARD_GERMANY,			//德语
	KEYBOARD_FRANCE_CAN,		//法语（加拿大）
	KEYBOARD_FRANCE,			//法语（法国）
	KEYBOARD_ITALIAN,			//意大利142
	KEYBOARD_PORTUGUESE,		//葡萄牙
	KEYBOARD_PORTUGUESE_BR,		//葡萄牙语（巴西）
	KEYBOARD_SPANISH,			//西班牙国际排序
};



/*设备配置*/
typedef struct 
{
//	uint16_t src_addr;					//源地址
//	uint16_t dest_addr;					//目标地址
	uint8_t wire_wireless;			//有线无线
//	uint8_t pid;								//当前pid
//	uint8_t s_pid;							//已发送pid
	uint8_t single_continual;		//单次模式，连续模式
	uint8_t country_keyboard;		//国家键盘
	uint8_t beep_en_dis;				//蜂鸣器使能或禁止
	uint32_t sleep_time;
	Prefix prefix;					//前缀
	uint8_t code_ID;						//二维码类型
	Suffix suffix;					//后缀
	Hide hide;						//隐藏设置
	CharConversion character;
	uint32_t baud_rate;
	uint8_t STX_ETX;						//bits[1:0] 分别代表STX  ,ETX
	EndMark end_mark;				//结束符 
	CodeNameDisplay code_disp;
}tDevConfig;






#define STX_LEN				1
#define CODEID_NAME_LEN		13
#define CODEID_LEN			1
#define PREFIX_LEN 			DevConfig.prefix.Tx_prefix_len
#define ETX_LEN				1
#define END_MARK_LEN 		((DevConfig.end_mark == END_CR_LF)?2:1)

#define CODE_ID				DevConfig.code_ID
#define TXBUF_SUFFIX		DevConfig.suffix.TX_suffix_len


#define STX_IDX				0
#define CODE_TYPE_IDX		STX_LEN
#define CODEID_IDX			(STX_LEN+CODEID_NAME_LEN)
#define PREFIX_IDX			(CODEID_IDX+CODEID_LEN)
#define DATA_IDX			(PREFIX_IDX+PREFIX_LEN)



#define STX_CODETYPE_CODEID_SUFFIX	(STX_LEN+CODEID_NAME_LEN+CODEID_LEN+PREFIX_LEN+TXBUF_SUFFIX+ETX_LEN+END_MARK_LEN)


#define TX_DATA_LEN  				(data_tail+1-DATA_IDX)



extern uint8_t codeID_user[20];


extern tDevConfig DevConfig;


extern tDevConfig* GetDevConfig(void);



/*********************************************
brief: 		出厂设置
parameter：	void
return：	void	
*********************************************/
extern void FactorySet(void);


/*********************************************
brief: 		读取设置
parameter：	void
return：	void
*********************************************/
extern void ReadConfig(void);




extern void ConfigComplete(void);


/*********************************************
brief: 		保存 有线/无线
parameter：	
	WIRE_MODE			有线
	WIRELESS_MODE		无线
return：	void	
*********************************************/
extern void SaveWireWireless(uint8_t mode);


/*********************************************
brief: 		读取 有线/无线模式
parameter：	void
return：	
	WIRE_MODE			有线
	WIRELESS_MODE		无线
*********************************************/
extern uint8_t ReadWireWirelessMode(void);


/*********************************************
brief: 		保存 单次/连续模式
parameter：	
	SINGLE 		
	CONTINUAL 	
return：	void
*********************************************/
extern void SaveSingleContinual(uint8_t sin_con);



/*********************************************
brief: 		读取 单次/连续模式
parameter：	void	
return：	
	SINGLE 		
	CONTINUAL 
*********************************************/
extern uint8_t ReadSingleContinual(void);



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
extern void SaveCountryKeyboard(uint8_t country);



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
extern uint8_t ReadCountryKeyboard(void);



/*********************************************
brief: 		保存 蜂鸣器是否使能
parameter：		
	BEEP_ENABLE	
	BEEP_DISABLE
return：	void
*********************************************/
extern void SaveBeepOption(uint8_t beep);


/*********************************************
brief: 		读取 蜂鸣器是否使能
parameter：	void
return：	
	BEEP_ENABLE	
	BEEP_DISABLE
*********************************************/
extern uint8_t ReadBeepOption(void);


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
extern void SaveSleepTime(uint32_t time);


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
extern uint32_t ReadSleepTime(void);



/*********************************************
brief: 		设置波特率
parameter：	uint32_t
return：	void
*********************************************/
extern void BaudSet(uint32_t baud);


/*********************************************
brief: 		读取波特率
parameter：	void
return：	uint32_t
*********************************************/
extern uint32_t ReadBaudRate(void);



/*********************************************
brief: 		保存 STX ETX
parameter：	uint8_t 低两位
return：	void
*********************************************/
extern void SaveSTXETX(uint8_t option);


/*********************************************
brief: 		读取 STX ETX
parameter：	void
return：	uint8_t 低两位
*********************************************/
extern uint8_t ReadSTXETX(void);


/*********************************************
brief: 		保存 是否显示条码类型名
parameter：	uint8_t 低两位
return：	void
*********************************************/
extern void SaveCodeNameDisp(uint32_t option);


/*********************************************
brief: 		读取 是否显示条码类型名
parameter：	
	
return：	uint8_t 低两位
*********************************************/
extern CodeNameDisplay ReadCodeNameDisp(void);



/*********************************************
brief: 		保存 CODEID 字符
parameter：	void
return：	void
*********************************************/
extern void SaveCodeIDCharacter(void);


/*********************************************
brief: 		读取 CODEID 字符
parameter：	void
return：	void
*********************************************/
extern void ReadCodeIDCharacter(void);

/*********************************************
brief: 		保存 CODEID 是否使能
parameter：	
	CODE_ID_ENABLE 
	CODE_ID_DISABLE
return：	void
*********************************************/
extern void SaveCodeID(uint32_t option);


/*********************************************
brief: 		读取 CODEID 是否使能
parameter：	void
return：	
	CODE_ID_ENABLE 
	CODE_ID_DISABLE
*********************************************/
extern uint8_t ReadCodeID(void);


/*********************************************
brief: 		前缀初始化
parameter：	void
return：	void
*********************************************/
extern void PrefixInit(void);


/*********************************************
brief: 		保存前缀
parameter：	Prefix*  前缀指针
return：	void
*********************************************/
extern void PrefixSave(Prefix* prefix);


/*********************************************
brief: 		读取前缀
parameter：	Prefix*  前缀指针
return：	void
*********************************************/
extern void ReadPrefix(Prefix* prefix);


/*********************************************
brief: 		前缀赋值
parameter：	void
return：	void
*********************************************/
extern void PrefixAssignment(void);


/*********************************************
brief: 		获取前缀长度
parameter：	Prefix
return：	uint8_t
*********************************************/
extern uint8_t GetPrefixLen(Prefix prefix);



/*********************************************
brief: 		后缀初始化
parameter：	void
return：	void
*********************************************/
extern void SuffixInit(void);


/*********************************************
brief: 		保存后缀
parameter：	Suffix* 指针
return：	void
*********************************************/
extern void SuffixSave(Suffix* suffix);

/*********************************************
brief: 		读取后缀
parameter：	Suffix* 指针
return：	void
*********************************************/
extern void ReadSuffix(Suffix* suffix);

/*********************************************
brief: 		后缀赋值
parameter：	void
return：	void
*********************************************/
extern void SuffixAssignment(void);



/*********************************************
brief: 		字符串隐藏初始化
parameter：	void
return：	void
*********************************************/
extern void HideInit(void);

/*********************************************
brief: 		保存隐藏设置
parameter：	Hide* 指针
return：	void
*********************************************/
extern void SaveHideSet(Hide* p_hide);

/*********************************************
brief: 		读取隐藏设置
parameter：	Hide* 指针
return：	void
*********************************************/
extern void ReadHide(Hide* p_hide);


/*********************************************
brief: 		隐藏设置赋值
parameter：	void
return：	void
*********************************************/
extern void StringAssignment(void);


/*********************************************
brief: 		保存转换设置
parameter：	
	NO_CONVERSION,
	UPPERCASE_CONVERSION
	LOWERCASE_CONVERSION
	CHARACTER_CONVERSION
return：	void
*********************************************/
extern void SaveConversion(CharConversion val);

/*********************************************
brief: 		读取转换设置
parameter：	void
return：	
	NO_CONVERSION,
	UPPERCASE_CONVERSION
	LOWERCASE_CONVERSION
	CHARACTER_CONVERSION
*********************************************/
extern uint8_t ReadConversion(void);




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
extern void SaveEndMark(EndMark option);


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
extern uint8_t ReadEndMark(void);



/*********************************************
brief: 	  	写入STX到TxBuffer
parameter：	void
return：	void
*********************************************/
extern void WriteStx(void);

/*********************************************
brief: 	  	写入ETX到TxBuffer
parameter：	void
return：	void
*********************************************/
extern void WriteEtx(void);


/*********************************************
brief: 	  	写入前缀到TxBuffer
parameter：	void
return：	void
*********************************************/
extern void WritePrefix(void);


extern void SendData(void);


#ifdef DEBUG_SUCCESS_ERROR
extern void SaveSuccess(uint32_t suc);
extern void SaveErr(uint32_t err);
#endif

#endif




