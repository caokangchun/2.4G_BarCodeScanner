#ifndef __DATA_FORMAT_H__
#define __DATA_FORMAT_H__

#include "gd32f1x0.h"

#define INSERT_STRING_SIZE	12
#define INSERT_GRP_SIZE		8


typedef struct 
{
	uint8_t en_dis;
	uint8_t len;
	uint8_t buf[15];
	uint8_t Tx_prefix_len;		//实际前缀长度
}Prefix;





typedef struct 
{
	uint8_t en_dis;
	uint8_t len;
	uint8_t buf[15];
	uint8_t TX_suffix_len;
}Suffix;




typedef struct 
{
	uint8_t head_hide_enDis;
	uint8_t head_hide_len;
	
	uint8_t tail_hide_enDis;
	uint8_t tail_hide_len;
	
	uint8_t string_hide_enDis;
	uint8_t string_len;
	uint8_t string_hide[15];
}Hide;



typedef enum 
{
	NO_CONVERSION,
	UPPERCASE_CONVERSION,
	LOWERCASE_CONVERSION,
	CHARACTER_CONVERSION,
}CharConversion;



typedef enum 
{
	END_CR,
	END_LF,
	END_CR_LF,
	END_HT,
	END_SPACE,
	END_EMPTY,
}EndMark;



typedef enum 
{
	CODE_TYPE_DISP_EN,
	CODE_TYPE_DISP_DIS,
}CodeNameDisplay;



typedef struct
{
	uint8_t enable;
	uint8_t len;
	uint16_t pos_insert;	//出入位置
	uint8_t str[INSERT_STRING_SIZE];		
}StringInsert;






#define STX_MASK_BIT	0
#define ETX_MASK_BIT	1

#define STX_ETX_EN		1
#define STX_ETX_DIS		0


#define PREFIX_ENABLE 	1
#define PREFIX_DISABLE	0


#define SUFFIX_ENABLE 	1
#define SUFFIX_DISABLE	0




#define CODE_ID_ENABLE 	1
#define CODE_ID_DISABLE	0


#define SUFFIX_ENABLE 	1
#define SUFFIX_DISABLE	0



#define HIDE_ENABLE 	1
#define HIDE_DISABLE 	0





void data_hide(void);




static void cal_next(char *str, int *next, int len);
int KMP(char *str, int slen, char *ptr, int plen);



void character_conversion(void);


void string_insert(void);
void insert_character_init(void);
void insert_pos_init(void);






uint8_t get_end_mark(void);
void save_end_mark(EndMark option);



void string_insert_init(void);
void insert_en_dis(uint8_t grp_index);
void string_insert_diasble(void);

#endif

