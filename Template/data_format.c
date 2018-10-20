#include "data_format.h"
#include "config.h"
#include "BSPScan.h"



StringInsert array_insert[INSERT_GRP_SIZE];		//8个插入组







extern u16 data_tail;
void data_hide(void)			// CODEID+PREFIX+DATA+SUFFIX
{
	u8 i;
	s16 data_index;
	
	if(DevConfig.hide.head_hide_enDis == HIDE_ENABLE)
	{
		//根据长度将DATA清零
		for(i=0;i<DevConfig.hide.head_hide_len;i++)
		{
			TxBuffer[DATA_IDX+i] = 0;		//隐藏头
			if(DATA_IDX+i >= data_tail)	//若超过了数据总数，就返回
			{
				return;	
			}
		}
	}


	if(DevConfig.hide.tail_hide_enDis == HIDE_ENABLE)
	{
		//根据长度将DATA清零
		for(i=0;i<DevConfig.hide.tail_hide_len;i++)
		{
			TxBuffer[data_tail-i] = 0;	//隐藏尾
			if(data_tail-i <= DATA_IDX)
			{
				return;
			}
		}
	}


	while((DevConfig.hide.string_hide_enDis == HIDE_ENABLE) && (DevConfig.hide.string_len > 0))		//当使能隐藏字符串，且字符长度大于0时
	{
		data_index=KMP((char*)&TxBuffer[DATA_IDX],TX_DATA_LEN, (char*)&DevConfig.hide.string_hide[0], DevConfig.hide.string_len);	//寻找index
		if(data_index > -1)
		{
			for(i=0;i<DevConfig.hide.string_len;i++)
			{
				TxBuffer[DATA_IDX + data_index + i] = 0;	//隐藏
			}
			
		}
		else
		{
			return;
		}
				
	}

	



}








static void cal_next(char *str, int *next, int len)
{
		int q,k;
    next[0] = -1;//next[0]初始化为-1，-1表示不存在相同的最大前缀和最大后缀
    k = -1;//k初始化为-1
    for (q = 1; q <= len-1; q++)
    {
        while (k > -1 && str[k + 1] != str[q])//如果下一个不同，那么k就变成next[k]，注意next[k]是小于k的，无论k取任何值。
        {
            k = next[k];//往前回溯
        }
        if (str[k + 1] == str[q])//如果相同，k++
        {
            k = k + 1;
        }
        next[q] = k;//这个是把算的k的值（就是相同的最大前缀和最大后缀长）赋给next[q]
    }
}

static int g_next[15];
int KMP(char *str, int slen, char *ptr, int plen)
{
	int k,i;
    int *next = g_next;
    cal_next(ptr, next, plen);//计算next数组
    k = -1;
    for (i = 0; i < slen; i++)
    {
        while (k >-1&& ptr[k + 1] != str[i])//ptr和str不匹配，且k>-1（表示ptr和str有部分匹配）
            k = next[k];//往前回溯
        if (ptr[k + 1] == str[i])
            k = k + 1;
        if (k == plen-1)//说明k移动到ptr的最末端
        {
            //cout << "在位置" << i-plen+1<< endl;
            //k = -1;//重新初始化，寻找下一个
            //i = i - plen + 1;//i定位到该位置，外层for循环i++可以继续找下一个（这里默认存在两个匹配字符串可以部分重叠），感谢评论中同学指出错误。
            return i-plen+1;//返回相应的位置
        }
    }

    return -1;  
}




void character_conversion(void)
{
	u16 i;
	u8 ch;
	switch(DevConfig.character)
	{
	case UPPERCASE_CONVERSION:
		for(i=0;i<TX_DATA_LEN;i++)
		{
			ch = TxBuffer[DATA_IDX+i];
			if((ch>='a') && (ch<='z'))
			{
				ch-=32;		//小写转大写
				TxBuffer[DATA_IDX+i] = ch;
			}
		}
	break;

	case LOWERCASE_CONVERSION:
		for(i=0;i<TX_DATA_LEN;i++)
		{
			ch = TxBuffer[DATA_IDX+i];
			if((ch>='A') && (ch<='Z'))
			{
				ch+=32;		//大写转小写
				TxBuffer[DATA_IDX+i] = ch;
			}
		}	
	break;

	case CHARACTER_CONVERSION:
		for(i=0;i<TX_DATA_LEN;i++)
		{
			ch = TxBuffer[DATA_IDX+i];
			
			if((ch>='a') && (ch<='z'))
			{
				ch-=32; 	//小写转大写
				TxBuffer[DATA_IDX+i] = ch;
			}
			
			else if((ch>='A') && (ch<='Z'))
			{
				ch+=32;		//大写转小写
				TxBuffer[DATA_IDX+i] = ch;
			}			
		}		
	break;	

	default: break;
	}
}


//static uint8_t string_insert_flag=0;


//static uint8_t _get_num_insert()
//{
//	uint8_t num,i;
//	for(i=0;i<8;i++)
//	{
//		if(array_insert[i].enable == 1)
//		{
//			num++;
//		}
//	}
//	return num;
//}


///*获取组插入字符串长度*/
//static uint8_t _get_str_len(uint8_t grp_index)
//{
////	uint8_t len=0;
////	uint8_t i=0;
////	while(0 != array_insert[grp_index].str[i++])
////	{
////		len++;
////		if(len >= INSERT_STRING_SIZE)
////		{
////			return len;
////		}
////	}
////	return len;
//	return array_insert[grp_index].len;
//}



//static void _insert(uint16_t pos,uint8_t grp_index)
//{
//	uint8_t i;
//	uint16_t temp = tx_len-1;		//获取数组的最后元素的下标
//	uint8_t len = _get_str_len(grp_index);	//获取字符串的长度

//	//如果数据不够长，则不插入
//	if(pos>data_tail)
//	{
//		return;
//	}
//	
//	for(;temp>=pos;temp--)	//数组后移len字节
//	{
//		TxBuffer[temp+len] = TxBuffer[temp];
//	}

//	for(i=0;i<len;i++)
//	{
//		TxBuffer[pos+i] = array_insert[grp_index].str[i];
//	}
//	tx_len += len;	//数组加长
//}


//static void _string_insert(uint8_t grp_index, uint16_t pos_insert)
//{
//	uint16_t pos = DATA_IDX;
//	uint8_t	len;
//	
//	while(pos_insert)
//	{
//		if(0 != TxBuffer[pos])	//不为0才计算
//		{
//			pos_insert--;
//		}
//		pos++;
//	}
//	//此时得到实际插入位置
//	
//	_insert(pos,grp_index);	//插入位置后的元素后移N（插入数组的长度）

//	

//	string_insert_flag |= (1<<grp_index);	//置位对应string_insert_flag

//	
//}





//static void _find_grp(uint16_t *l_pos_insert, uint8_t *grp_index)
//{
//	uint8_t i;

//	
//	for(i=0;i<8;i++)	//找到最大index
//	{
//		if(array_insert[i].enable == 1)
//		{
//			if(0 == (string_insert_flag & (1<<i)))		//当没处理过时
//			{
//				if(array_insert[i].pos_insert >= *l_pos_insert)
//				{
//					*l_pos_insert = array_insert[i].pos_insert;	//取出对应index
//					*grp_index = i;
//				}
//			}
//		}
//	}
//}






//void string_insert()
//{
//	//找到最大index数据
//	//数据后移
//	//插入数据
//	//标记
//	uint16_t l_pos_insert;
//	uint8_t i=0,grp_index=0;

//	uint8_t num = _get_num_insert();	//需要插入的数量
//	for(;num>0;num--)
//	{
//		l_pos_insert = 0;
//		_find_grp(&l_pos_insert,&grp_index);
//		_string_insert(grp_index,l_pos_insert);
//	}
//	string_insert_flag=0;
//}


//void insert_character_init()
//{
//	array_insert[grp_index].len =0; 		//字符串归零
//	array_insert[grp_index].enable =0;
//	insert_character_set_flag=1;	//仍旧为设置字符串			
//	save_String_insert_array();
//}


//void insert_pos_init()
//{
//	array_insert[grp_index].pos_insert =0;
//	array_insert[grp_index].enable =0;
//	insert_pos_flag =1; //标志要设置位置
//	save_String_insert_array();
//}



//void string_insert_init()
//{
//	uint8_t i=0,j=0;

//	for(;i<8;i++)
//	{
//		array_insert[i].enable = 0;
//		array_insert[i].len = 0;
//		array_insert[i].pos_insert = 0;
//		for(;j<INSERT_STRING_SIZE;j++)
//		{
//			array_insert[i].str[j] = 0;
//		}
//	}
//}


////判断是否可以使能
//void insert_en_dis(uint8_t grp_index)
//{
//	if(array_insert[grp_index].enable == 0)	//若还没enable
//	{
//		//判断是否可以使能
////		if(array_insert[grp_index].len>0 && array_insert[grp_index].pos_insert>0)
//		if(array_insert[grp_index].len>0)
//		{
//			array_insert[grp_index].enable = 1;
//		}
//	}
//}



//void string_insert_diasble()
//{
//	uint8_t i=0,j=0;

//	for(;i<8;i++)
//	{
//		array_insert[i].enable = 0;
//	}
//	save_String_insert_array();
//}


