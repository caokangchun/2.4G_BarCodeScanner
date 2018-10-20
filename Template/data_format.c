#include "data_format.h"
#include "config.h"
#include "BSPScan.h"



StringInsert array_insert[INSERT_GRP_SIZE];		//8��������







extern u16 data_tail;
void data_hide(void)			// CODEID+PREFIX+DATA+SUFFIX
{
	u8 i;
	s16 data_index;
	
	if(DevConfig.hide.head_hide_enDis == HIDE_ENABLE)
	{
		//���ݳ��Ƚ�DATA����
		for(i=0;i<DevConfig.hide.head_hide_len;i++)
		{
			TxBuffer[DATA_IDX+i] = 0;		//����ͷ
			if(DATA_IDX+i >= data_tail)	//�������������������ͷ���
			{
				return;	
			}
		}
	}


	if(DevConfig.hide.tail_hide_enDis == HIDE_ENABLE)
	{
		//���ݳ��Ƚ�DATA����
		for(i=0;i<DevConfig.hide.tail_hide_len;i++)
		{
			TxBuffer[data_tail-i] = 0;	//����β
			if(data_tail-i <= DATA_IDX)
			{
				return;
			}
		}
	}


	while((DevConfig.hide.string_hide_enDis == HIDE_ENABLE) && (DevConfig.hide.string_len > 0))		//��ʹ�������ַ��������ַ����ȴ���0ʱ
	{
		data_index=KMP((char*)&TxBuffer[DATA_IDX],TX_DATA_LEN, (char*)&DevConfig.hide.string_hide[0], DevConfig.hide.string_len);	//Ѱ��index
		if(data_index > -1)
		{
			for(i=0;i<DevConfig.hide.string_len;i++)
			{
				TxBuffer[DATA_IDX + data_index + i] = 0;	//����
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
    next[0] = -1;//next[0]��ʼ��Ϊ-1��-1��ʾ��������ͬ�����ǰ׺������׺
    k = -1;//k��ʼ��Ϊ-1
    for (q = 1; q <= len-1; q++)
    {
        while (k > -1 && str[k + 1] != str[q])//�����һ����ͬ����ôk�ͱ��next[k]��ע��next[k]��С��k�ģ�����kȡ�κ�ֵ��
        {
            k = next[k];//��ǰ����
        }
        if (str[k + 1] == str[q])//�����ͬ��k++
        {
            k = k + 1;
        }
        next[q] = k;//����ǰ����k��ֵ��������ͬ�����ǰ׺������׺��������next[q]
    }
}

static int g_next[15];
int KMP(char *str, int slen, char *ptr, int plen)
{
	int k,i;
    int *next = g_next;
    cal_next(ptr, next, plen);//����next����
    k = -1;
    for (i = 0; i < slen; i++)
    {
        while (k >-1&& ptr[k + 1] != str[i])//ptr��str��ƥ�䣬��k>-1����ʾptr��str�в���ƥ�䣩
            k = next[k];//��ǰ����
        if (ptr[k + 1] == str[i])
            k = k + 1;
        if (k == plen-1)//˵��k�ƶ���ptr����ĩ��
        {
            //cout << "��λ��" << i-plen+1<< endl;
            //k = -1;//���³�ʼ����Ѱ����һ��
            //i = i - plen + 1;//i��λ����λ�ã����forѭ��i++���Լ�������һ��������Ĭ�ϴ�������ƥ���ַ������Բ����ص�������л������ͬѧָ������
            return i-plen+1;//������Ӧ��λ��
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
				ch-=32;		//Сдת��д
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
				ch+=32;		//��дתСд
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
				ch-=32; 	//Сдת��д
				TxBuffer[DATA_IDX+i] = ch;
			}
			
			else if((ch>='A') && (ch<='Z'))
			{
				ch+=32;		//��дתСд
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


///*��ȡ������ַ�������*/
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
//	uint16_t temp = tx_len-1;		//��ȡ��������Ԫ�ص��±�
//	uint8_t len = _get_str_len(grp_index);	//��ȡ�ַ����ĳ���

//	//������ݲ��������򲻲���
//	if(pos>data_tail)
//	{
//		return;
//	}
//	
//	for(;temp>=pos;temp--)	//�������len�ֽ�
//	{
//		TxBuffer[temp+len] = TxBuffer[temp];
//	}

//	for(i=0;i<len;i++)
//	{
//		TxBuffer[pos+i] = array_insert[grp_index].str[i];
//	}
//	tx_len += len;	//����ӳ�
//}


//static void _string_insert(uint8_t grp_index, uint16_t pos_insert)
//{
//	uint16_t pos = DATA_IDX;
//	uint8_t	len;
//	
//	while(pos_insert)
//	{
//		if(0 != TxBuffer[pos])	//��Ϊ0�ż���
//		{
//			pos_insert--;
//		}
//		pos++;
//	}
//	//��ʱ�õ�ʵ�ʲ���λ��
//	
//	_insert(pos,grp_index);	//����λ�ú��Ԫ�غ���N����������ĳ��ȣ�

//	

//	string_insert_flag |= (1<<grp_index);	//��λ��Ӧstring_insert_flag

//	
//}





//static void _find_grp(uint16_t *l_pos_insert, uint8_t *grp_index)
//{
//	uint8_t i;

//	
//	for(i=0;i<8;i++)	//�ҵ����index
//	{
//		if(array_insert[i].enable == 1)
//		{
//			if(0 == (string_insert_flag & (1<<i)))		//��û�����ʱ
//			{
//				if(array_insert[i].pos_insert >= *l_pos_insert)
//				{
//					*l_pos_insert = array_insert[i].pos_insert;	//ȡ����Ӧindex
//					*grp_index = i;
//				}
//			}
//		}
//	}
//}






//void string_insert()
//{
//	//�ҵ����index����
//	//���ݺ���
//	//��������
//	//���
//	uint16_t l_pos_insert;
//	uint8_t i=0,grp_index=0;

//	uint8_t num = _get_num_insert();	//��Ҫ���������
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
//	array_insert[grp_index].len =0; 		//�ַ�������
//	array_insert[grp_index].enable =0;
//	insert_character_set_flag=1;	//�Ծ�Ϊ�����ַ���			
//	save_String_insert_array();
//}


//void insert_pos_init()
//{
//	array_insert[grp_index].pos_insert =0;
//	array_insert[grp_index].enable =0;
//	insert_pos_flag =1; //��־Ҫ����λ��
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


////�ж��Ƿ����ʹ��
//void insert_en_dis(uint8_t grp_index)
//{
//	if(array_insert[grp_index].enable == 0)	//����ûenable
//	{
//		//�ж��Ƿ����ʹ��
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


