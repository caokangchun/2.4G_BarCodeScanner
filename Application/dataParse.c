#include "dataParse.h"

#include "gd32f1x0.h"
#include "string.h"
#include "config.h"



const char CommandClrRecor[]="€XMCRR";		//Çå³þrecord


const char CommandRequestMatch[]="REQUEST_MATCH";
const char CommandRequestMatchForce[]="€XMPD";
const char CommandWireWireless[]="€XMJK2";


const char CommandSingle[]="€XMMS1";
const char CommandContinual[]="€XMMS5";
const char CommandBeepEn[]="€XMSY1";
const char CommandBeepDis[]="€XMSY0";

/*ÐÝÃßÊ±¼ä*/
const char CommandSlpHfMIN[]="€XMS30S";
const char CommandSlp1MIN[]="€XMS01M";
const char CommandSlp5MIN[]="€XMS05M";
const char CommandSlp10MIN[]="€XMS10M";
const char CommandSlp20MIN[]="€XMS20M";
const char CommandShutdown[]="€XMGJ";


/*STX,ETX*/
const char CommandSTXEn[]="€XMSTX1";
const char CommandSTXDis[]="€XMSTX0";
const char CommandETXEn[]="€XMETX1";
const char CommandETXDis[]="€XMETX0";



const char CommandPrefixEn[]="€XMTJZ2";
const char CommandPrefixDis[]="€XMTJZ0";
const char CommandPrefixModify[]="€XMTJZ1";



const char CommandCodeIdEn[]="€XMIDZ1";
const char CommandCodeIdDis[]="€XMIDZ0";



const char CommandCodeTypeDispEn[]="€XMMX1";
const char CommandCodeTypeDispDis[]="€XMMX0";




const char CommandSuffixCR[]="€XMHZ1";
const char CommandSuffixLF[]="€XMHZ2";
const char CommandSuffixCRLF[]="€XMHZ3";
const char CommandSuffixHT[]="€XMHZ4";
const char CommandSuffixSPACE[]="€XMHZ5";
const char CommandSuffixEMPTY[]="€XMHZ0";


const char CommandSuffixEn[]="€XMTJY2";
const char CommandSuffixDis[]="€XMTJY0";
const char CommandSuddixModify[]="€XMTJY1";

/*Í·Òþ²Ø*/
const char CommandHeadHideEn[]="€XMYCZ2";
const char CommandHeadHideDis[]="€XMYCZ0";
const char CommandHeadHideLen[]="€XMYCZ1";
/*Î²Òþ²Ø*/
const char CommandTailHideEn[]="€XMYCY2";
const char CommandTailHideDis[]="€XMYCY0";
const char CommandTailHideLen[]="€XMYCY1";

/*×Ö·û´®Òþ²Ø*/
const char CommandStringHideEn[]="€XMSC2";
const char CommandStringHideDis[]="€XMSC0";
const char CommandStringHideSet[]="€XMSC1";

					
/*×Ö·û×ª»»*/
const char CommandCharNoAction[]="€XMZMHF";
const char CommandUppercaseConversion[]="€XMZMDX";
const char CommandLowercaseConversion[]="€XMZMXX";
const char CommandCharConversion[]="€XMZMFZ";


///*×Ö·û×é²åÈë*/
//const char command_INSERT1[]="€XMCR1";
//const char command_INSERT2[]="€XMCR2";
//const char command_INSERT3[]="€XMCR3";
//const char command_INSERT4[]="€XMCR4";
//const char command_INSERT5[]="€XMCR5";
//const char command_INSERT6[]="€XMCR6";
//const char command_INSERT7[]="€XMCR7";
//const char command_INSERT8[]="€XMCR8";
//const char command_INSERT_SET[]="€XMCR9";
//const char command_INSERT_DIS[]="€XMCR0";









/*²¨ÌØÂÊ*/
const char CommandBaud1200[]="€XMBTL1";
const char CommandBaud4800[]="€XMBTL2";
const char CommandBaud9600[]="€XMBTL3";
const char CommandBaud14400[]="€XMBTL4";
const char CommandBaud19200[]="€XMBTL5";
const char CommandBaud38400[]="€XMBTL6";
const char CommandBaud57600[]="€XMBTL7";
const char CommandBaud115200[]="€XMBTL8";



/*EAN13*/
const char CommandEAN13En[]="€XMd01";
const char CommandEAN13Dis[]="€XMd00";

/*EAN8*/
const char CommandEAN8En[]="€XMg01";
const char CommandEAN8Dis[]="€XMg00";


/*UPCA*/
const char CommandUPCAEn[]="€XMc01";
const char CommandUPCADis[]="€XMc00";


/*UPCE0*/
const char CommandUPCE0En[]="€XMc001";
const char CommandUPCE0Dis[]="€XMc000";


/*UPCE1*/
const char CommandUPCE1En[]="€XMc101";
const char CommandUPCE1Dis[]="€XMc100";


/*CODE128*/
const char CommandCODE128En[]="€XMj01";
const char CommandCODE128Dis[]="€XMj00";
const char CommandCODE128Least0[]="€XMj02";	//×î¶Ì0
const char CommandCODE128Least4[]="€XMj03";	//×î¶Ì4
const char CommandCODE128Most32[]="€XMj04";	//×î³¤32
const char CommandCODE128Most255[]="€XMj05";	//×î³¤255

/*CODE39*/
const char CommandCODE39En[]="€XMb01";
const char CommandCODE39Dis[]="€XMb00";
const char CommandCODE39Least0[]="€XMb02";	//×î¶Ì0
const char CommandCODE39Least4[]="€XMb03";	//×î¶Ì4
const char CommandCODE39Most32[]="€XMb04";	//×î³¤32
const char CommandCODE39Most255[]="€XMb05";	//×î³¤255

/*CODE93*/
const char CommandCODE93En[]="€XMy01";
const char CommandCODE93Dis[]="€XMy00";
const char CommandCODE93Least0[]="€XMy02";	//×î¶Ì0
const char CommandCODE93Least4[]="€XMy03";	//×î¶Ì4
const char CommandCODE93Most32[]="€XMy04";	//×î³¤32
const char CommandCODE93Most255[]="€XMy05";	//×î³¤255


/*CODEBAR*/
const char CommandCODEBAREn[]="€XMa01";
const char CommandCODEBARDis[]="€XMa00";
const char CommandCODEBARLeast0[]="€XMa02";	//×î¶Ì0
const char CommandCODEBARLeast4[]="€XMa03";	//×î¶Ì4
const char CommandCODEBARMost32[]="€XMa04";	//×î³¤32
const char CommandCODEBARMost255[]="€XMa05";	//×î³¤255
const char CommandCODEBARSendNon[]="€XMa06";			//²»·¢ÆðÖ¹·û
const char CommandCODEBARSendStrEnd[]="€XMa07";		//·¢ÆðÖ¹·û

/*QR*/
const char CommandQREn[]="€XMQ01";
const char CommandQRDis[]="€XMQ00";

/*INTERLEAVED 25*/
const char CommandINTERLEAVED25En[]="€XMe01";
const char CommandINTERLEAVED25Dis[]="€XMe00";
const char CommandINTERLEAVED25least0[]="€XMe02";	//×î¶Ì0
const char CommandINTERLEAVED25least4[]="€XMe03";	//×î¶Ì4
const char CommandINTERLEAVED25Most32[]="€XMe04";	//×î³¤32
const char CommandINTERLEAVED25Most255[]="€XMe05";	//×î³¤255

/*INDUSTRIAL 25*/
const char CommandINDUSTRIAL25En[]="€XMi01";
const char CommandINDUSTRIAL25Dis[]="€XMi00";
const char CommandINDUSTRIAL25Least0[]="€XMi02";	//×î¶Ì0
const char CommandINDUSTRIAL25Least4[]="€XMi03";	//×î¶Ì4
const char CommandINDUSTRIAL25Most32[]="€XMi04";	//×î³¤32
const char CommandINDUSTRIAL25Most255[]="€XMi05";	//×î³¤255

/*MATRIX25*/
const char CommandMATRIX25En[]="€XMv01";
const char CommandMATRIX25Dis[]="€XMv00";
const char CommandMATRIX25Least0[]="€XMv02";	//×î¶Ì0
const char CommandMATRIX25Least4[]="€XMv03";	//×î¶Ì4
const char CommandMATRIX25Most32[]="€XMv04";	//×î³¤32
const char CommandMATRIX25Most255[]="€XMv05";	//×î³¤255

/*CODE11*/
const char CommandCODE11En[]="€XMz01";
const char CommandCODE11Dis[]="€XMz00";
const char CommandCODE11Least0[]="€XMz02";	//×î¶Ì0
const char CommandCODE11Least4[]="€XMz03";	//×î¶Ì4
const char CommandCODE11Most32[]="€XMz04";	//×î³¤32
const char CommandCODE11Most255[]="€XMz05";	//×î³¤255

/*MSI*/
const char CommandMSIEn[]="€XMm01";
const char CommandMSIDis[]="€XMm00";
const char CommandMSILeast0[]="€XMm02";	//×î¶Ì0
const char CommandMSILeast4[]="€XMm03";	//×î¶Ì4
const char CommandMSIMost32[]="€XMm04";	//×î³¤32
const char CommandMSIMost255[]="€XMm05";	//×î³¤255

/*RSS*/
const char CommandRSS14En[]="€XMR1401";	//RSS-14
const char CommandRSS14Dis[]="€XMR1400";
const char CommandRSSLMTEn[]="€XMRL01";		//RSSÏÞ¶¨Ê½
const char CommandRSSLMTDis[]="€XMRL00";
const char CommandRSSEXTEn[]="€XMRE01";		//RSSÀ©Õ¹Ê½
const char CommandRSSEXTDis[]="€XMRE00";
const char CommandRSSLeast0[]="€XMR02";	//×î¶Ì0
const char CommandRSSLeast4[]="€XMR03";	//×î¶Ì4
const char CommandRSSMost32[]="€XMR04";	//×î³¤32
const char CommandRSSMost255[]="€XMR05";	//×î³¤255



/*DM*/
const char CommandDMEn[]="€XMu01";
const char CommandDMDis[]="€XMu00";

/*PDF417*/
const char CommandPDF417En[]="€XMr01";
const char CommandPDF417Dis[]="€XMr00";


const char CommandAmerica[]="€XMMG";
const char CommandGermany[]="€XMDG";
const char CommandFranceCAN[]="€XMFJ";
const char CommandFrance[]="€XMFG";
const char CommandItay[]="€XMYD";
const char CommandPortuguese[]="€XMPT";
const char CommandPortugueseBr[]="€XMPB";
const char CommandSpanish[]="€XMXG";




extern uint8_t prefix_mdy_flag;
extern uint8_t suffix_mdy_flag;
extern uint8_t head_hide_mdy_flag;
extern uint8_t tail_hide_mdy_flag;
extern uint8_t string_hide_mdy_flag;

extern uint8_t insert_character_set_flag,insert_pos_flag;
extern uint8_t grp_index;


extern uint8_t TxBuffer[];
extern uint16_t tx_len;

static uint8_t ParseCmd(const char *command);




static uint8_t ParsingPrefixMdychar()
{
	if(1 == prefix_mdy_flag)
	{
		//ÅÐ¶ÏÊäÈëµÄÊÇ·ñÐÞ¸Ä×Ö·û
		if(2 == tx_len - STX_CODETYPE_CODEID_SUFFIX)	//ÊäÈë×Ö·ûÖ»ÓÐ2byte
		{
			if(TxBuffer[DATA_IDX] == '$')
			{

				return 1;
			}
		}
	}

	return 0;

}






static uint8_t ParsingSuffixMdychar()
{
	if(1 == suffix_mdy_flag)
	{
		//ÅÐ¶ÏÊäÈëµÄÊÇ·ñÐÞ¸Ä×Ö·û
		if(2 == tx_len - STX_CODETYPE_CODEID_SUFFIX)	//ÊäÈë×Ö·ûÖ»ÓÐ2byte
		{
			if(TxBuffer[DATA_IDX] == '$')
			{
				return 1;
			}
		}
	}

	return 0;

}




static uint8_t ParsingHeadLenMdy()
{
	if(1 == head_hide_mdy_flag)
	{
		//ÅÐ¶ÏÊäÈëµÄÊÇ·ñÐÞ¸Ä×Ö·û
		if(2 == tx_len - STX_CODETYPE_CODEID_SUFFIX)	//ÊäÈë×Ö·ûÖ»ÓÐ2byte
		{
			if(TxBuffer[DATA_IDX] == '$')
			{
				return 1;
			}
		}
	}

	return 0;

}







static uint8_t ParsingTailLenMdy()
{
	if(1 == tail_hide_mdy_flag)
	{
		//ÅÐ¶ÏÊäÈëµÄÊÇ·ñÐÞ¸Ä×Ö·û
		if(2 == tx_len - STX_CODETYPE_CODEID_SUFFIX)	//ÊäÈë×Ö·ûÖ»ÓÐ2byte
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
		//ÅÐ¶ÏÊäÈëµÄÊÇ·ñÐÞ¸Ä×Ö·û
		if(2 == tx_len - STX_CODETYPE_CODEID_SUFFIX)	//ÊäÈë×Ö·ûÖ»ÓÐ2byte
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
		//ÅÐ¶ÏÊäÈëµÄÊÇ·ñÐÞ¸Ä×Ö·û
		if(2 == tx_len - STX_CODETYPE_CODEID_SUFFIX)	//ÊäÈë×Ö·ûÖ»ÓÐ2byte
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
		//ÅÐ¶ÏÊäÈëµÄÊÇ·ñÐÞ¸Ä×Ö·û
		if(2 == tx_len - STX_CODETYPE_CODEID_SUFFIX)	//ÊäÈë×Ö·ûÖ»ÓÐ2byte
		{
			if(TxBuffer[DATA_IDX] == '$')
			{
				return 1;
			}
		}
	}

	return 0;

}




uint8_t Parsing()
{
#ifdef DEBUG_SUCCESS_ERROR	
	if(1 == ParseCmd(CommandClrRecor))
	{
		return _CLEAR_RECORD;
	}
#endif
	
	if(1 == ParseCmd(CommandRequestMatch))
	{
		return _REQUEST_MATCH;
	}
		

	if(1 == ParseCmd(CommandRequestMatchForce))
	{
		return _REQUEST_MATCH_FORCE;
	}
		


	if(1 == ParseCmd(CommandWireWireless))
	{
		return _WIRE_WIRELESS;
	}


	if(1 == ParseCmd(CommandSingle))
	{
		return _SINGLE;
	}

	if(1 == ParseCmd(CommandContinual))
	{
		return _CONTINUAL;
	}	

	if(1 == ParseCmd(CommandBeepEn))
	{
		return _BEEP_EN;
	}

	if(1 == ParseCmd(CommandBeepDis))
	{
		return _BEEP_DIS;
	}		

	if(1 == ParseCmd(CommandSlpHfMIN))
	{
		return _SLEEP_HF_MIN;
	}

	if(1 == ParseCmd(CommandSlp1MIN))
	{
		return _SLEEP_1MIN;
	}

	if(1 == ParseCmd(CommandSlp5MIN))
	{
		return _SLEEP_5MIN;
	}	

	if(1 == ParseCmd(CommandSlp10MIN))
	{
		return _SLEEP_10MIN;
	}	

	if(1 == ParseCmd(CommandSlp20MIN))
	{
		return _SLEEP_20MIN;
	}	

	if(1 == ParseCmd(CommandShutdown))
	{
		return _SHUTDOWN;
	}

	if(1 == ParseCmd(CommandSTXEn))
	{
		return _STX_EN;
	}

	if(1 == ParseCmd(CommandSTXDis))
	{
		return _STX_DIS;
	}


	if(1 == ParseCmd(CommandETXEn))
	{
		return _ETX_EN;
	}


	if(1 == ParseCmd(CommandETXDis))
	{
		return _ETX_DIS;
	}




	if(1 == ParseCmd(CommandPrefixEn))
	{
		return _PREFIX_EN;
	}

	if(1 == ParseCmd(CommandPrefixDis))
	{
		return _PREFIX_DIS;
	}


	if(1 == ParseCmd(CommandPrefixModify))
	{
		return _PREFIX_MODIFY;
	}	

	if(1 == ParsingPrefixMdychar())
	{
		return _PREFIX_MDYCHAR;
	}

	if(1 == ParseCmd(CommandCodeIdEn))
	{
		return _CODEID_EN;
	}

	if(1 == ParseCmd(CommandCodeIdDis))
	{
		return _CODEID_DIS;
	}



	if(1 == ParseCmd(CommandCodeTypeDispEn))
	{
		return _CODE_TYPE_DISP_EN;
	}

	if(1 == ParseCmd(CommandCodeTypeDispDis))
	{
		return _CODE_TYPE_DISP_DIS;
	}




	if(1 == ParseCmd(CommandSuffixCR))
	{
		return _END_CR;
	}

	if(1 == ParseCmd(CommandSuffixLF))
	{
		return _END_LF;
	}

	if(1 == ParseCmd(CommandSuffixCRLF))
	{
		return _END_CR_LF;
	}

	if(1 == ParseCmd(CommandSuffixHT))
	{
		return _END_HT;
	}		

	if(1 == ParseCmd(CommandSuffixSPACE))
	{
		return _END_SPACE;
	}

	if(1 == ParseCmd(CommandSuffixEMPTY))
	{
		return _END_EMPTY;
	}






	if(1 == ParseCmd(CommandSuffixEn))
	{
		return _SUFFIX_EN;
	}

	if(1 == ParseCmd(CommandSuffixDis))
	{
		return _SUFFIX_DIS;
	}	

	if(1 == ParseCmd(CommandSuddixModify))
	{
		return _SUFFIX_MODIFY;
	}	

	if(1 == ParsingSuffixMdychar())
	{
		return _SUFFIX_MDYCHAR;
	}		


	if(1 == ParseCmd(CommandHeadHideEn))
	{
		return _HEAD_HIDE_EN;
	}

	if(1 == ParseCmd(CommandHeadHideDis))
	{
		return _HEAD_HIDE_DIS;
	}	

	if(1 == ParseCmd(CommandHeadHideLen))	//Òþ²ØÍ·³¤¶È
	{
		return _HEAD_HIDE_LEN_MODIFY;
	}	

	if(1 == ParsingHeadLenMdy())//³¤¶ÈÉèÖÃ
	{
		return _HEAD_HIDE_LEN_MOYCHAR;
	}	



	if(1 == ParseCmd(CommandTailHideEn))
	{
		return _TAIL_HIDE_EN;
	}

	if(1 == ParseCmd(CommandTailHideDis))
	{
		return _TAIL_HIDE_DIS;
	}	

	if(1 == ParseCmd(CommandTailHideLen))	//Òþ²ØÍ·³¤¶È
	{
		return _TAIL_HIDE_LEN_MODIFY;
	}	

	if(1 == ParsingTailLenMdy())//³¤¶ÈÉèÖÃ
	{
		return _TAIL_HIDE_LEN_MOYCHAR;
	}	

	/*Òþ²Ø×Ö·û´®*/

	if(1 == ParseCmd(CommandStringHideEn))
	{
		return _STRING_HIDE_EN;
	}

	if(1 == ParseCmd(CommandStringHideDis))
	{
		return _STRING_HIDE_DIS;
	}	

	if(1 == ParseCmd(CommandStringHideSet))	//Òþ²ØÍ·³¤¶È
	{
		return _STRING_HIDE_MODIFY;
	}	

	if(1 == parsing_string_MDYCHAR())//³¤¶ÈÉèÖÃ
	{
		return _STRING_HIDE_MOYCHAR;
	}	



	if(1 == ParseCmd(CommandCharNoAction))	//ÎÞ¶¯×÷
	{
		return _NO_CONVERSION;
	}	


	if(1 == ParseCmd(CommandUppercaseConversion))	//´óÐ´×ª»»
	{
		return _UPPERCASE_CONVERSION;
	}	

	if(1 == ParseCmd(CommandLowercaseConversion))	//Ð¡Ð´×ª»»
	{
		return _LOWERCASE_CONVERSION;
	}		

	if(1 == ParseCmd(CommandCharConversion))	//´óÐ¡Ð´×ª»»
	{
		return _CHARACTER_CONVERSION;
	}



//	/****************************/
//	if(1 == ParseCmd(command_INSERT1))					//²åÈë×é1
//	{
//		grp_index = 0;
//		if(0==insert_character_set_flag)	//ÉèÖÃÎ»ÖÃpos
//		{
//			return _INSERT1_POS;
//		}
//		else	//ÉèÖÃ²åÈë×Ö·û
//		{
//			return _INSERT1_CHAR;
//		}

//	}


//	if(1 == ParseCmd(command_INSERT2))					////²åÈë×é2
//	{
//		grp_index = 1;
//		if(0==insert_character_set_flag)	//ÉèÖÃÎ»ÖÃpos
//		{
//			return _INSERT2_POS;
//		}
//		else	//ÉèÖÃ²åÈë×Ö·û
//		{
//			return _INSERT2_CHAR;
//		}		
//	}


//	if(1 == ParseCmd(command_INSERT3))	
//	{
//		grp_index = 2;
//		if(0==insert_character_set_flag)	//ÉèÖÃÎ»ÖÃpos
//		{
//			return _INSERT3_POS;
//		}
//		else	//ÉèÖÃ²åÈë×Ö·û
//		{
//			return _INSERT3_CHAR;
//		}

//	}

//	if(1 == ParseCmd(command_INSERT4))	
//	{
//		grp_index = 3;
//		if(0==insert_character_set_flag)	//ÉèÖÃÎ»ÖÃpos
//		{
//			return _INSERT4_POS;
//		}
//		else	//ÉèÖÃ²åÈë×Ö·û
//		{
//			return _INSERT4_CHAR;
//		}

//	}


//	if(1 == ParseCmd(command_INSERT5))	
//	{
//		grp_index = 4;
//		if(0==insert_character_set_flag)	//ÉèÖÃÎ»ÖÃpos
//		{
//			return _INSERT5_POS;
//		}
//		else	//ÉèÖÃ²åÈë×Ö·û
//		{
//			return _INSERT5_CHAR;
//		}

//	}


//	if(1 == ParseCmd(command_INSERT6))
//	{
//		grp_index = 5;
//		if(0==insert_character_set_flag)	//ÉèÖÃÎ»ÖÃpos
//		{
//			return _INSERT6_POS;
//		}
//		else	//ÉèÖÃ²åÈë×Ö·û
//		{
//			return _INSERT6_CHAR;
//		}

//	}


//	if(1 == ParseCmd(command_INSERT7))	
//	{
//		grp_index = 6;
//		if(0==insert_character_set_flag)	//ÉèÖÃÎ»ÖÃpos
//		{
//			return _INSERT7_POS;
//		}
//		else	//ÉèÖÃ²åÈë×Ö·û
//		{
//			return _INSERT7_CHAR;
//		}

//	}


//	if(1 == ParseCmd(command_INSERT8))	
//	{
//		grp_index = 7;
//		if(0==insert_character_set_flag)	//ÉèÖÃÎ»ÖÃpos
//		{
//			return _INSERT8_POS;
//		}
//		else	//ÉèÖÃ²åÈë×Ö·û
//		{
//			return _INSERT8_CHAR;
//		}

//	}


//	if(1 == ParseCmd(command_INSERT_SET))	
//	{
//		return _INSERT_SET;
//	}

//	if(1 == ParseCmd(command_INSERT_DIS))	
//	{
//		return _INSERT_DIS;
//	}


//	if(1 == parsing_insert_string_MDYCHAR())//³¤¶ÈÉèÖÃ
//	{
//		return _INSERT_STRING_MDYCHAR;
//	}	

//	if(1 == parsing_insert_pos_MDY())//³¤¶ÈÉèÖÃ
//	{
//		return _INSERT_POS_MDY;
//	}




	/***²¨ÌØÂÊ****/
	if(1 == ParseCmd(CommandBaud1200))	//´óÐ¡Ð´×ª»»
	{
		return _1200;
	}

	if(1 == ParseCmd(CommandBaud4800))	//´óÐ¡Ð´×ª»»
	{
		return _4800;
	}

	if(1 == ParseCmd(CommandBaud9600))	//´óÐ¡Ð´×ª»»
	{
		return _9600;
	}

	if(1 == ParseCmd(CommandBaud14400))	//´óÐ¡Ð´×ª»»
	{
		return _14400;
	}

	if(1 == ParseCmd(CommandBaud19200))	//´óÐ¡Ð´×ª»»
	{
		return _19200;
	}

	if(1 == ParseCmd(CommandBaud38400))	//´óÐ¡Ð´×ª»»
	{
		return _38400;
	}

	if(1 == ParseCmd(CommandBaud57600))	//´óÐ¡Ð´×ª»»
	{
		return _57600;
	}

	if(1 == ParseCmd(CommandBaud115200))	//´óÐ¡Ð´×ª»»
	{
		return _115200;
	}


	/*EAN13*/
	if(1 == ParseCmd(CommandEAN13En))	
	{
		return _EAN13_EN;
	}

	if(1 == ParseCmd(CommandEAN13Dis))	
	{
		return _EAN13_DIS;
	}		

	/*EAN8*/
	if(1 == ParseCmd(CommandEAN8En))	
	{
		return _EAN8_EN;
	}

	if(1 == ParseCmd(CommandEAN8Dis))	
	{
		return _EAN8_DIS;
	}		

	/*UPCA*/
	if(1 == ParseCmd(CommandUPCAEn))	
	{
		return _UPCA_EN;
	}

	if(1 == ParseCmd(CommandUPCADis))	
	{
		return _UPCA_DIS;
	}	

	/*UPCE0*/
	if(1 == ParseCmd(CommandUPCE0En)) 
	{
		return _UPCE0_EN;
	}

	if(1 == ParseCmd(CommandUPCE0Dis))	
	{
		return _UPCE0_DIS;
	}	


	/*UPCE1*/
	if(1 == ParseCmd(CommandUPCE1En)) 
	{
		return _UPCE1_EN;
	}

	if(1 == ParseCmd(CommandUPCE1Dis))	
	{
		return _UPCE1_DIS;
	}	

	/*CODE128*/
	if(1 == ParseCmd(CommandCODE128En)) 
	{
		return _CODE128_EN;
	}

	if(1 == ParseCmd(CommandCODE128Dis))	
	{
		return _CODE128_DIS;
	}	

	if(1 == ParseCmd(CommandCODE128Least0)) 
	{
		return _CODE128_LEAST0;
	}

	if(1 == ParseCmd(CommandCODE128Least4))	
	{
		return _CODE128_LEAST4;
	}	

	if(1 == ParseCmd(CommandCODE128Most32)) 
	{
		return _CODE128_MOST32;
	}

	if(1 == ParseCmd(CommandCODE128Most255))	
	{
		return _CODE128_MOST255;
	}	


	/*CODE39*/
	if(1 == ParseCmd(CommandCODE39En)) 
	{
		return _CODE39_EN;
	}

	if(1 == ParseCmd(CommandCODE39Dis)) 
	{
		return _CODE39_DIS;
	}	

	if(1 == ParseCmd(CommandCODE39Least0)) 
	{
		return _CODE39_LEAST0;
	}

	if(1 == ParseCmd(CommandCODE39Least4))	
	{
		return _CODE39_LEAST4;
	}	

	if(1 == ParseCmd(CommandCODE39Most32)) 
	{
		return _CODE39_MOST32;
	}

	if(1 == ParseCmd(CommandCODE39Most255)) 
	{
		return _CODE39_MOST255;
	}	

	
	/*CODE93*/
	if(1 == ParseCmd(CommandCODE93En)) 
	{
		return _CODE93_EN;
	}

	if(1 == ParseCmd(CommandCODE93Dis)) 
	{
		return _CODE93_DIS;
	}	

	if(1 == ParseCmd(CommandCODE93Least0)) 
	{
		return _CODE93_LEAST0;
	}

	if(1 == ParseCmd(CommandCODE93Least4))	
	{
		return _CODE93_LEAST4;
	}	

	if(1 == ParseCmd(CommandCODE93Most32)) 
	{
		return _CODE93_MOST32;
	}

	if(1 == ParseCmd(CommandCODE93Most255)) 
	{
		return _CODE93_MOST255;
	}	

	
	

	/*CODEBAR*/
	if(1 == ParseCmd(CommandCODEBAREn)) 
	{
		return _CODEBAR_EN;
	}

	if(1 == ParseCmd(CommandCODEBARDis)) 
	{
		return _CODEBAR_DIS;
	}	

	if(1 == ParseCmd(CommandCODEBARLeast0)) 
	{
		return _CODEBAR_LEAST0;
	}

	if(1 == ParseCmd(CommandCODEBARLeast4))	
	{
		return _CODEBAR_LEAST4;
	}	

	if(1 == ParseCmd(CommandCODEBARMost32)) 
	{
		return _CODEBAR_MOST32;
	}

	if(1 == ParseCmd(CommandCODEBARMost255)) 
	{
		return _CODEBAR_MOST255;
	}

	if(1 == ParseCmd(CommandCODEBARSendNon)) 
	{
		return _CODEBAR_SEND_NON;
	}

	if(1 == ParseCmd(CommandCODEBARSendStrEnd)) 
	{
		return _CODEBAR_SEND_SRT_END;
	}		


	/*QR*/
	if(1 == ParseCmd(CommandQREn)) 
	{
		return _QR_EN;
	}

	if(1 == ParseCmd(CommandQRDis)) 
	{
		return _QR_DIS;
	}	

	

	/*INTERLEAVED 25*/
	if(1 == ParseCmd(CommandINTERLEAVED25En)) 
	{
		return _INTERLEAVED25_EN;
	}

	if(1 == ParseCmd(CommandINTERLEAVED25Dis)) 
	{
		return _INTERLEAVED25_DIS;
	}	

	if(1 == ParseCmd(CommandINTERLEAVED25least0)) 
	{
		return _INTERLEAVED25_LEAST0;
	}

	if(1 == ParseCmd(CommandINTERLEAVED25least4))	
	{
		return _INTERLEAVED25_LEAST4;
	}	

	if(1 == ParseCmd(CommandINTERLEAVED25Most32)) 
	{
		return _INTERLEAVED25_MOST32;
	}

	if(1 == ParseCmd(CommandINTERLEAVED25Most255)) 
	{
		return _INTERLEAVED25_MOST255;
	}	

	

	
	/*INDUSTRIAL 25*/
	if(1 == ParseCmd(CommandINDUSTRIAL25En)) 
	{
		return _INDUSTRIAL25_EN;
	}

	if(1 == ParseCmd(CommandINDUSTRIAL25Dis)) 
	{
		return _INDUSTRIAL25_DIS;
	}	

	if(1 == ParseCmd(CommandINDUSTRIAL25Least0)) 
	{
		return _INDUSTRIAL25_LEAST0;
	}

	if(1 == ParseCmd(CommandINDUSTRIAL25Least4))	
	{
		return _INDUSTRIAL25_LEAST4;
	}	

	if(1 == ParseCmd(CommandINDUSTRIAL25Most32)) 
	{
		return _INDUSTRIAL25_MOST32;
	}

	if(1 == ParseCmd(CommandINDUSTRIAL25Most255)) 
	{
		return _INDUSTRIAL25_MOST255;
	}	

	

	/*MATRIX25*/
	if(1 == ParseCmd(CommandMATRIX25En)) 
	{
		return _MATRIX25_EN;
	}

	if(1 == ParseCmd(CommandMATRIX25Dis)) 
	{
		return _MATRIX25_DIS;
	}	

	if(1 == ParseCmd(CommandMATRIX25Least0)) 
	{
		return _MATRIX25_LEAST0;
	}

	if(1 == ParseCmd(CommandMATRIX25Least4)) 
	{
		return _MATRIX25_LEAST4;
	}	

	if(1 == ParseCmd(CommandMATRIX25Most32)) 
	{
		return _MATRIX25_MOST32;
	}

	if(1 == ParseCmd(CommandMATRIX25Most255)) 
	{
		return _MATRIX25_MOST255;
	}	

	
	
	/*CODE11*/
	if(1 == ParseCmd(CommandCODE11En)) 
	{
		return _CODE11_EN;
	}

	if(1 == ParseCmd(CommandCODE11Dis)) 
	{
		return _CODE11_DIS;
	}	

	if(1 == ParseCmd(CommandCODE11Least0)) 
	{
		return _CODE11_LEAST0;
	}

	if(1 == ParseCmd(CommandCODE11Least4)) 
	{
		return _CODE11_LEAST4;
	}	

	if(1 == ParseCmd(CommandCODE11Most32)) 
	{
		return _CODE11_MOST32;
	}

	if(1 == ParseCmd(CommandCODE11Most255)) 
	{
		return _CODE11_MOST255;
	}	

	
	/*MSI*/
	if(1 == ParseCmd(CommandMSIEn)) 
	{
		return _MSI_EN;
	}

	if(1 == ParseCmd(CommandMSIDis)) 
	{
		return _MSI_DIS;
	}	

	if(1 == ParseCmd(CommandMSILeast0)) 
	{
		return _MSI_LEAST0;
	}

	if(1 == ParseCmd(CommandMSILeast4)) 
	{
		return _MSI_LEAST4;
	}	

	if(1 == ParseCmd(CommandMSIMost32)) 
	{
		return _MSI_MOST32;
	}

	if(1 == ParseCmd(CommandMSIMost255)) 
	{
		return _MSI_MOST255;
	}	


	/*RSS*/
	if(1 == ParseCmd(CommandRSS14En)) 
	{
		return _RSS14_EN;
	}

	if(1 == ParseCmd(CommandRSS14Dis)) 
	{
		return _RSS14_DIS;
	}	

	if(1 == ParseCmd(CommandRSSLMTEn)) 
	{
		return _RSS_LMT_EN;
	}

	if(1 == ParseCmd(CommandRSSLMTDis)) 
	{
		return _RSS_LMT_DIS;
	}	

	if(1 == ParseCmd(CommandRSSEXTEn)) 
	{
		return _RSS_EXT_EN;
	}

	if(1 == ParseCmd(CommandRSSEXTDis)) 
	{
		return _RSS_EXT_DIS;
	}			

	if(1 == ParseCmd(CommandRSSLeast0)) 
	{
		return _RSS_LEAST0;
	}

	if(1 == ParseCmd(CommandRSSLeast4)) 
	{
		return _RSS_LEAST4;
	}	

	if(1 == ParseCmd(CommandRSSMost32)) 
	{
		return _RSS_MOST32;
	}

	if(1 == ParseCmd(CommandRSSMost255)) 
	{
		return _RSS_MOST255;
	}

	

	/*DM*/
	if(1 == ParseCmd(CommandDMEn)) 
	{
		return _DM_EN;
	}

	if(1 == ParseCmd(CommandDMDis)) 
	{
		return _DM_DIS;
	}	


	/*PDF417*/
	if(1 == ParseCmd(CommandPDF417En)) 
	{
		return _PDF417_EN;
	}

	if(1 == ParseCmd(CommandPDF417Dis)) 
	{
		return _PDF417_DIS;
	}	

	



	if(1 == ParseCmd(CommandAmerica))
	{
		return _AMERICA;
	}		


	if(1 == ParseCmd(CommandGermany))
	{
		return _GERMANY;
	}	


	if(1 == ParseCmd(CommandFranceCAN))
	{
		return _FRANCE_CAN;
	}			


	if(1 == ParseCmd(CommandFrance))
	{
		return _FRANCE;
	}			


	if(1 == ParseCmd(CommandItay))
	{
		return _ITAY;
	}

	if(1 == ParseCmd(CommandPortuguese))		//ÆÏÌÑÑÀ
	{
		return _PORTUGUESE;
	}

	if(1 == ParseCmd(CommandPortugueseBr))				//°ÍÎ÷ÆÏÌÑÑÀ
	{
		return _PORTUGUESE_BR;
	}

	if(1 == ParseCmd(CommandSpanish))				//Î÷°àÑÀ
	{
		return _SPANISH;
	}		




	/*É¨ÂëÊý¾Ý*/	
	return _DATA;
}



static uint8_t ParseCmd(const char *command)
{
	uint8_t i=0;
	if(strlen(command) == tx_len-STX_CODETYPE_CODEID_SUFFIX)
	{
		for(i=0;i<tx_len-STX_CODETYPE_CODEID_SUFFIX;i++)
		{
			if(TxBuffer[i+DATA_IDX] != command[i])
				return 0;
		}
		return 1;
	}
	return 0;
}
