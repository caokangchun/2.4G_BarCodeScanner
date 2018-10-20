#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "BSPWatchDog.h"

#include "data_format.h"

#include "BSPEeprom.h"

#include "debug.h"


/*��������*/
#define WIRE_MODE			0
#define WIRELESS_MODE		1


/*����ʱ��*/
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
	KEYBOARD_AMERICAN = 0,		//��ʽ����-����
	KEYBOARD_GERMANY,			//����
	KEYBOARD_FRANCE_CAN,		//������ô�
	KEYBOARD_FRANCE,			//���������
	KEYBOARD_ITALIAN,			//�����142
	KEYBOARD_PORTUGUESE,		//������
	KEYBOARD_PORTUGUESE_BR,		//�������������
	KEYBOARD_SPANISH,			//��������������
};



/*�豸����*/
typedef struct 
{
//	uint16_t src_addr;					//Դ��ַ
//	uint16_t dest_addr;					//Ŀ���ַ
	uint8_t wire_wireless;			//��������
//	uint8_t pid;								//��ǰpid
//	uint8_t s_pid;							//�ѷ���pid
	uint8_t single_continual;		//����ģʽ������ģʽ
	uint8_t country_keyboard;		//���Ҽ���
	uint8_t beep_en_dis;				//������ʹ�ܻ��ֹ
	uint32_t sleep_time;
	Prefix prefix;					//ǰ׺
	uint8_t code_ID;						//��ά������
	Suffix suffix;					//��׺
	Hide hide;						//��������
	CharConversion character;
	uint32_t baud_rate;
	uint8_t STX_ETX;						//bits[1:0] �ֱ����STX  ,ETX
	EndMark end_mark;				//������ 
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
brief: 		��������
parameter��	void
return��	void	
*********************************************/
extern void FactorySet(void);


/*********************************************
brief: 		��ȡ����
parameter��	void
return��	void
*********************************************/
extern void ReadConfig(void);




extern void ConfigComplete(void);


/*********************************************
brief: 		���� ����/����
parameter��	
	WIRE_MODE			����
	WIRELESS_MODE		����
return��	void	
*********************************************/
extern void SaveWireWireless(uint8_t mode);


/*********************************************
brief: 		��ȡ ����/����ģʽ
parameter��	void
return��	
	WIRE_MODE			����
	WIRELESS_MODE		����
*********************************************/
extern uint8_t ReadWireWirelessMode(void);


/*********************************************
brief: 		���� ����/����ģʽ
parameter��	
	SINGLE 		
	CONTINUAL 	
return��	void
*********************************************/
extern void SaveSingleContinual(uint8_t sin_con);



/*********************************************
brief: 		��ȡ ����/����ģʽ
parameter��	void	
return��	
	SINGLE 		
	CONTINUAL 
*********************************************/
extern uint8_t ReadSingleContinual(void);



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
extern void SaveCountryKeyboard(uint8_t country);



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
extern uint8_t ReadCountryKeyboard(void);



/*********************************************
brief: 		���� �������Ƿ�ʹ��
parameter��		
	BEEP_ENABLE	
	BEEP_DISABLE
return��	void
*********************************************/
extern void SaveBeepOption(uint8_t beep);


/*********************************************
brief: 		��ȡ �������Ƿ�ʹ��
parameter��	void
return��	
	BEEP_ENABLE	
	BEEP_DISABLE
*********************************************/
extern uint8_t ReadBeepOption(void);


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
extern void SaveSleepTime(uint32_t time);


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
extern uint32_t ReadSleepTime(void);



/*********************************************
brief: 		���ò�����
parameter��	uint32_t
return��	void
*********************************************/
extern void BaudSet(uint32_t baud);


/*********************************************
brief: 		��ȡ������
parameter��	void
return��	uint32_t
*********************************************/
extern uint32_t ReadBaudRate(void);



/*********************************************
brief: 		���� STX ETX
parameter��	uint8_t ����λ
return��	void
*********************************************/
extern void SaveSTXETX(uint8_t option);


/*********************************************
brief: 		��ȡ STX ETX
parameter��	void
return��	uint8_t ����λ
*********************************************/
extern uint8_t ReadSTXETX(void);


/*********************************************
brief: 		���� �Ƿ���ʾ����������
parameter��	uint8_t ����λ
return��	void
*********************************************/
extern void SaveCodeNameDisp(uint32_t option);


/*********************************************
brief: 		��ȡ �Ƿ���ʾ����������
parameter��	
	
return��	uint8_t ����λ
*********************************************/
extern CodeNameDisplay ReadCodeNameDisp(void);



/*********************************************
brief: 		���� CODEID �ַ�
parameter��	void
return��	void
*********************************************/
extern void SaveCodeIDCharacter(void);


/*********************************************
brief: 		��ȡ CODEID �ַ�
parameter��	void
return��	void
*********************************************/
extern void ReadCodeIDCharacter(void);

/*********************************************
brief: 		���� CODEID �Ƿ�ʹ��
parameter��	
	CODE_ID_ENABLE 
	CODE_ID_DISABLE
return��	void
*********************************************/
extern void SaveCodeID(uint32_t option);


/*********************************************
brief: 		��ȡ CODEID �Ƿ�ʹ��
parameter��	void
return��	
	CODE_ID_ENABLE 
	CODE_ID_DISABLE
*********************************************/
extern uint8_t ReadCodeID(void);


/*********************************************
brief: 		ǰ׺��ʼ��
parameter��	void
return��	void
*********************************************/
extern void PrefixInit(void);


/*********************************************
brief: 		����ǰ׺
parameter��	Prefix*  ǰ׺ָ��
return��	void
*********************************************/
extern void PrefixSave(Prefix* prefix);


/*********************************************
brief: 		��ȡǰ׺
parameter��	Prefix*  ǰ׺ָ��
return��	void
*********************************************/
extern void ReadPrefix(Prefix* prefix);


/*********************************************
brief: 		ǰ׺��ֵ
parameter��	void
return��	void
*********************************************/
extern void PrefixAssignment(void);


/*********************************************
brief: 		��ȡǰ׺����
parameter��	Prefix
return��	uint8_t
*********************************************/
extern uint8_t GetPrefixLen(Prefix prefix);



/*********************************************
brief: 		��׺��ʼ��
parameter��	void
return��	void
*********************************************/
extern void SuffixInit(void);


/*********************************************
brief: 		�����׺
parameter��	Suffix* ָ��
return��	void
*********************************************/
extern void SuffixSave(Suffix* suffix);

/*********************************************
brief: 		��ȡ��׺
parameter��	Suffix* ָ��
return��	void
*********************************************/
extern void ReadSuffix(Suffix* suffix);

/*********************************************
brief: 		��׺��ֵ
parameter��	void
return��	void
*********************************************/
extern void SuffixAssignment(void);



/*********************************************
brief: 		�ַ������س�ʼ��
parameter��	void
return��	void
*********************************************/
extern void HideInit(void);

/*********************************************
brief: 		������������
parameter��	Hide* ָ��
return��	void
*********************************************/
extern void SaveHideSet(Hide* p_hide);

/*********************************************
brief: 		��ȡ��������
parameter��	Hide* ָ��
return��	void
*********************************************/
extern void ReadHide(Hide* p_hide);


/*********************************************
brief: 		�������ø�ֵ
parameter��	void
return��	void
*********************************************/
extern void StringAssignment(void);


/*********************************************
brief: 		����ת������
parameter��	
	NO_CONVERSION,
	UPPERCASE_CONVERSION
	LOWERCASE_CONVERSION
	CHARACTER_CONVERSION
return��	void
*********************************************/
extern void SaveConversion(CharConversion val);

/*********************************************
brief: 		��ȡת������
parameter��	void
return��	
	NO_CONVERSION,
	UPPERCASE_CONVERSION
	LOWERCASE_CONVERSION
	CHARACTER_CONVERSION
*********************************************/
extern uint8_t ReadConversion(void);




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
extern void SaveEndMark(EndMark option);


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
extern uint8_t ReadEndMark(void);



/*********************************************
brief: 	  	д��STX��TxBuffer
parameter��	void
return��	void
*********************************************/
extern void WriteStx(void);

/*********************************************
brief: 	  	д��ETX��TxBuffer
parameter��	void
return��	void
*********************************************/
extern void WriteEtx(void);


/*********************************************
brief: 	  	д��ǰ׺��TxBuffer
parameter��	void
return��	void
*********************************************/
extern void WritePrefix(void);


extern void SendData(void);


#ifdef DEBUG_SUCCESS_ERROR
extern void SaveSuccess(uint32_t suc);
extern void SaveErr(uint32_t err);
#endif

#endif




