#ifndef __BSPSCAN_H__
#define __BSPSCAN_H__


#include "gd32f1x0_libopt.h"
#include "systick.h"

#include "data_type.h"
#include "data_format.h"


#include "BSPSystem.h"

#define SINGLE 			0
#define CONTINUAL 	1


#define HEAD1 	0x7e00		
#define HEAD2 	0x0200

#define READ 			0x07
#define WRITE			0x08
#define WRITE_FLASH		0x09


#define STX		0x02
#define ETX		0x03



enum
{
	keyboard_AMERICAN = 0,		//美式键盘-中文
	keyboard_GERMANY,			//德语
	keyboard_FRANCE_CAN,		//法语（加拿大）
	keyboard_FRANCE,			//法语（法国）
	keyboard_ITALIAN,			//意大利142
	keyboard_PORTUGUESE,		//葡萄牙
	keyboard_PORTUGUESE_BR,		//葡萄牙语（巴西）
	keyboard_SPANISH,			//西班牙国际排序
};


#define ADDR_EAN13				0x002e
#define ADDR_EAN8				0x002f
#define ADDR_UPCA				0x0030
#define ADDR_UPCE0				0x0031
#define ADDR_UPCE1				0x0032
#define ADDR_CODE128_ENDIS		0x0033
#define ADDR_CODE128_LEAST		0x0034
#define ADDR_CODE128_MOST		0x0035

#define ADDR_CODE39_ENDIS		0x0036
#define ADDR_CODE39_LEAST		0x0037
#define ADDR_CODE39_MOST		0x0038

#define ADDR_CODE93_ENDIS		0x0039
#define ADDR_CODE93_LEAST		0x003a
#define ADDR_CODE93_MOST		0x003b

#define ADDR_CODEBAR_ENDIS		0x003c
#define ADDR_CODEBAR_LEAST		0x003d
#define ADDR_CODEBAR_MOST		0x003e

#define ADDR_QR_ENDIS			0x003f

#define ADDR_INTERLEAVED25_ENDIS			0x0040
#define ADDR_INTERLEAVED25_LEAST			0x0041
#define ADDR_INTERLEAVED25_MOST				0x0042

#define ADDR_INDUSTRIAL25_ENDIS				0x0043
#define ADDR_INDUSTRIAL25_LEAST				0x0044
#define ADDR_INDUSTRIAL25_MOST				0x0045

#define ADDR_MATRIX25_ENDIS					0x0046
#define ADDR_MATRIX25_LEAST					0x0047
#define ADDR_MATRIX25_MOST					0x0048

#define ADDR_CODE11_ENDIS					0x0049
#define ADDR_CODE11_LEAST					0x004a
#define ADDR_CODE11_MOST					0x004b

#define ADDR_MSI_ENDIS						0x004c
#define ADDR_MSI_LEAST						0x004d
#define ADDR_MSI_MOST						0x004e

#define ADDR_RSS14_ENDIS					0x004f
#define ADDR_RSS_LMT_ENDIS					0x0050
#define ADDR_RSS_EXT_ENDIS					0x0051
#define ADDR_RSS_LEAST						0x0052
#define ADDR_RSS_MOST						0x0053

#define ADDR_DM_ENDIS						0x0054
#define ADDR_PDF417_ENDIS					0x0055











///*数据解析结果*/
//enum
//{

///*匹配*/
//_REQUEST_MATCH=1,
//_REQUEST_MATCH_FORCE,

///*有线-无线*/	
//_WIRE,
//_WIRELESS,
//_WIRE_WIRELESS,

///*单次-连续*/	
//_SINGLE,
//_CONTINUAL,

///*蜂鸣器*/
//_BEEP_EN,
//_BEEP_DIS,

///*休眠时间*/
//_SLEEP_HF_MIN,
//_SLEEP_1MIN,
//_SLEEP_5MIN,
//_SLEEP_10MIN,
//_SLEEP_20MIN,
//_SHUTDOWN,


///*STX*/
//_STX_EN,
//_STX_DIS,

///*ETX*/
//_ETX_EN,
//_ETX_DIS,


///*前缀*/
//_PREFIX_EN,
//_PREFIX_DIS,
//_PREFIX_MODIFY,
//_PREFIX_MDYCHAR,

///*CodeID*/
//_CODEID_EN,
//_CODEID_DIS,


///*Code Type disp*/
//_CODE_TYPE_DISP_EN,
//_CODE_TYPE_DISP_DIS,



///*SUFFIX*/
//_SUFFIX_EN,
//_SUFFIX_DIS,
//_SUFFIX_MODIFY,
//_SUFFIX_MDYCHAR,



///*结束符*/
//_END_CR,		//回车
//_END_LF,		//换行
//_END_CR_LF,	//回车加换行
//_END_HT,		//\t制表符
//_END_SPACE,	//空格
//_END_EMPTY,	//无



///*隐藏数据头长度*/
//_HEAD_HIDE_EN,
//_HEAD_HIDE_DIS,
//_HEAD_HIDE_LEN_MODIFY,
//_HEAD_HIDE_LEN_MOYCHAR,

///*隐藏数据尾长度*/
//_TAIL_HIDE_EN,
//_TAIL_HIDE_DIS,
//_TAIL_HIDE_LEN_MODIFY,
//_TAIL_HIDE_LEN_MOYCHAR,

///*隐藏字符串*/
//_STRING_HIDE_EN,
//_STRING_HIDE_DIS,
//_STRING_HIDE_MODIFY,
//_STRING_HIDE_MOYCHAR,

///*字符转换*/
//_NO_CONVERSION,
//_UPPERCASE_CONVERSION,
//_LOWERCASE_CONVERSION,
//_CHARACTER_CONVERSION,


///**/
//_INSERT1_CHAR,
//_INSERT1_POS,

//_INSERT2_CHAR,
//_INSERT2_POS,

//_INSERT3_CHAR,
//_INSERT3_POS,

//_INSERT4_CHAR,
//_INSERT4_POS,

//_INSERT5_CHAR,
//_INSERT5_POS,

//_INSERT6_CHAR,
//_INSERT6_POS,

//_INSERT7_CHAR,
//_INSERT7_POS,

//_INSERT8_CHAR,
//_INSERT8_POS,


//_INSERT_SET,
//_INSERT_DIS,
//_INSERT_STRING_MDYCHAR,
//_INSERT_POS_MDY,


///*波特率设置*/
//_1200,
//_4800,
//_9600,
//_14400,
//_19200,
//_38400,
//_57600,
//_115200,

///*条码类型设置*/
//_EAN13_EN,
//_EAN13_DIS,

//_EAN8_EN,
//_EAN8_DIS,

//_UPCA_EN,
//_UPCA_DIS,

//_UPCE0_EN,
//_UPCE0_DIS,

//_UPCE1_EN,
//_UPCE1_DIS,

//_CODE128_EN,
//_CODE128_DIS,
//_CODE128_LEAST0,
//_CODE128_LEAST4,
//_CODE128_MOST32,
//_CODE128_MOST255,

//_CODE39_EN,
//_CODE39_DIS,
//_CODE39_LEAST0,
//_CODE39_LEAST4,
//_CODE39_MOST32,
//_CODE39_MOST255,

//_CODE93_EN,
//_CODE93_DIS,
//_CODE93_LEAST0,
//_CODE93_LEAST4,
//_CODE93_MOST32,
//_CODE93_MOST255,

//_CODEBAR_EN,
//_CODEBAR_DIS,
//_CODEBAR_LEAST0,
//_CODEBAR_LEAST4,
//_CODEBAR_MOST32,
//_CODEBAR_MOST255,
//_CODEBAR_SEND_NON,
//_CODEBAR_SEND_SRT_END,

//_QR_EN,
//_QR_DIS,

//_INTERLEAVED25_EN,
//_INTERLEAVED25_DIS,
//_INTERLEAVED25_LEAST0,
//_INTERLEAVED25_LEAST4,
//_INTERLEAVED25_MOST32,
//_INTERLEAVED25_MOST255,

//_INDUSTRIAL25_EN,
//_INDUSTRIAL25_DIS,
//_INDUSTRIAL25_LEAST0,
//_INDUSTRIAL25_LEAST4,
//_INDUSTRIAL25_MOST32,
//_INDUSTRIAL25_MOST255,

//_MATRIX25_EN,
//_MATRIX25_DIS,
//_MATRIX25_LEAST0,
//_MATRIX25_LEAST4,
//_MATRIX25_MOST32,
//_MATRIX25_MOST255,

//_CODE11_EN,
//_CODE11_DIS,
//_CODE11_LEAST0,
//_CODE11_LEAST4,
//_CODE11_MOST32,
//_CODE11_MOST255,

//_MSI_EN,
//_MSI_DIS,
//_MSI_LEAST0,
//_MSI_LEAST4,
//_MSI_MOST32,
//_MSI_MOST255,

//_RSS14_EN,
//_RSS14_DIS,
//_RSS_LMT_EN,
//_RSS_LMT_DIS,
//_RSS_EXT_EN,
//_RSS_EXT_DIS,
//_RSS_LEAST0,
//_RSS_LEAST4,
//_RSS_MOST32,
//_RSS_MOST255,

//_DM_EN,
//_DM_DIS,

//_PDF417_EN,
//_PDF417_DIS,








///*keyboard*/	
//	_AMERICA,
//	_GERMANY,
//	_FRANCE_CAN,		//法语加拿大
//	_FRANCE,			//法语法国
//	_ITAY,				//意大利语
//	_PORTUGUESE,		//葡萄牙语言
//	_PORTUGUESE_BR,		//巴西-葡萄牙语
//	_SPANISH,			//西班牙
///*数据传送*/
//	_DATA
//};

extern uint8_t get_QRmodule_status;
extern uint8_t QR_len;

extern u32 scan_time;

#define TXBUFFER_SIZE	700

#define scan_idle()		gpio_bit_set(GPIOB,GPIO_PIN_1);		\
											scan_time=0


#define scan_work()		while(scan_time<120);							\
									if(scan_delay>10)		gpio_bit_reset(GPIOB,GPIO_PIN_1)

									


												
extern uint8_t* GetTxBuf(void);												
extern void BSPScanInit(void) __attribute__((section ("BSPScanInit")));
extern void BSPScannerFirstSet(void);
									
extern uint8_t BSPScanGetComplete(void);
extern void BSPScanSetComplete(uint8_t val);									
									
void uart_init(void) __attribute__((section ("uart_init")));
void NVIC_Configuration(void) __attribute__((section ("NVIC_Configuration")));
static void sending_in_scanning(void);
uint8_t scan_start(void);
extern void BSPScan(void);

uint8_t parsing(void);
static uint8_t parsing_request_match(void);
static uint8_t parsing_request_match_force(void);
static uint8_t parsing_wire(void);
static uint8_t parsing_wireless(void);


static uint8_t parsing_single(void);
static uint8_t parsing_continual(void);

static uint8_t parsing_beep_en(void);
static uint8_t parsing_beep_dis(void);


static uint8_t parsing_sleep_hf_min(void);
static uint8_t parsing_sleep_1min(void);
static uint8_t parsing_sleep_5min(void);
static uint8_t parsing_sleep_10min(void);
static uint8_t parsing_sleep_20min(void);
static uint8_t parsing_shutdown(void);

static uint8_t parsing_prefix_en(void);
static uint8_t parsing_prefix_dis(void);
static uint8_t parsing_prefix_modify(void);
static uint8_t parsing_prefix_MDYCHAR(void) __attribute__((section ("parsing_prefix_MDYCHAR")));


static uint8_t parsing_CodeId_en(void);
static uint8_t parsing_CodeId_dis(void);



static uint8_t parsing_suffix_en(void);
static uint8_t parsing_suffix_dis(void);
static uint8_t parsing_suffix_modify(void);
static uint8_t parsing_suffix_MDYCHAR(void) __attribute__((section ("parsing_suffix_MDYCHAR")));




static uint8_t parsing_head_hide_en(void);
static uint8_t parsing_head_hide_dis(void);
static uint8_t parsing_head_hide_len(void);
static uint8_t parsing_head_len_MDYCHAR(void) __attribute__((section ("parsing_head_len_MDYCHAR")));





static uint8_t parsing_tail_hide_en(void);
static uint8_t parsing_tail_hide_dis(void);
static uint8_t parsing_tail_hide_len(void);
static uint8_t parsing_tail_len_MDYCHAR(void) __attribute__((section ("parsing_tail_len_MDYCHAR")));



static uint8_t parsing_string_hide_en(void);
static uint8_t parsing_string_hide_dis(void);
static uint8_t parsing_string_hide(void);
static uint8_t parsing_string_MDYCHAR(void) __attribute__((section ("parsing_string_MDYCHAR")));

static uint8_t parsing_insert_string_MDYCHAR(void);
static uint8_t parsing_insert_pos_MDY(void);





static uint8_t parsing_America(void);
static uint8_t parsing_Germany(void);
static uint8_t parsing_France_CAN(void);
static uint8_t parsing_France(void);
static uint8_t parsing_Itay(void);
static uint8_t parsing_Portuguese(void);
static uint8_t parsing_Portuguese_BR(void);
static uint8_t parsing_Spanish(void);


static uint8_t parse_cmd(const char *command);



void serial_command(uint8_t rw, uint16_t addr,uint8_t len);

uint8_t write_serial_command(uint16_t addr,uint8_t len,uint8_t *data);
//uint8_t flash_write_serial_command(uint16_t addr,uint8_t len,uint8_t *data);		//v1.58
uint8_t QR_flash_save(void);	//v1.60

uint8_t read_serial_command(uint16_t addr,uint8_t len);
uint16_t crc_cal_by_bit(uint8_t *ptr, uint8_t len);
uint8_t serial_check(uint8_t *ptr, uint8_t len1);
uint8_t serial_check2(uint8_t *ptr);
void CodeID_enable(void);
void CodeID_disable(void);


void noID_noSuffix_uartProtocal(void) __attribute__((section ("noID_noSuffix_uartProtocal")));
void utf8_input_output(void) __attribute__((section ("utf8_input_output")));
void codeID_character_set(void) __attribute__((section ("codeID_character_set")));



void QR_moduler_set(uint16_t addr,uint8_t bits_set,uint8_t bits_reset);
void QR_moduler_val_assignment(uint16_t addr,uint8_t val);
void QR_moduler_test(uint16_t addr);


void usart_write_baud(u32 baud);


void write_prefix(void) __attribute__((section ("write_prefix")));
uint8_t get_prefix_len(Prefix prefix) __attribute__((section ("get_prefix_len")));
uint8_t get_suffix_len(Suffix suffix) __attribute__((section ("get_suffix_len")));


static void read_prefix_to_tx(void) __attribute__((section ("read_prefix_to_tx")));
static void read_suffix_to_tx(void);


void write_stx(void) __attribute__((section ("write_stx")));
void write_etx(void) __attribute__((section ("write_etx")));



void write_end_mark(void);



extern uint8_t prefix_mdy_flag;
extern uint8_t suffix_mdy_flag;
extern uint8_t head_hide_mdy_flag;
extern uint8_t tail_hide_mdy_flag;
extern uint8_t string_hide_mdy_flag;

extern uint8_t insert_character_set_flag,insert_pos_flag;
extern uint8_t grp_index;


extern Prefix prefix_shadow;
extern Suffix suffix_shadow;
extern Hide   hide_shadow;


extern uint8_t scan_complete;


extern uint8_t TxBuffer[];
extern uint16_t tx_len;


#endif
