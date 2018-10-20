#include "lt8920.h"
#include "BSPWireless.h"
#include "BSPEeprom.h"
#include "gd32f1x0.h"
#include "stdlib.h"	
#include "systick.h"



#define LT8920_BUF_LEN		64						//LT8920包长度64

#define CHANNEL_RANGE		83

#define DATA_LENTH			49

//根据数据最大长度，分包缓冲区大小
#define DATA_DIVIDED_BUF	( WIRELESS_DATA_LEN_MAX/DATA_LENTH + ( (WIRELESS_DATA_LEN_MAX % DATA_LENTH)?1:0) )

#define Dummy_Byte 0xFF       			//空字符

#define INSERT				4			//插入小于7的频道			
#define CHANNEL_AMOUNT		4			//频道表

#define LT8920_SPI_CLK			RCU_SPI0
#define LT8920_PIN_CLK			RCU_GPIOA


#define LT8920_SPI_PORT			SPI0
#define LT8920_PIN_PORT			GPIOA


#define LT8920_PIN_RST			GPIO_PIN_2
#define LT8920_PIN_IRQ			GPIO_PIN_3
#define LT8920_PIN_NSS			GPIO_PIN_4
#define LT8920_PIN_SPICLK		GPIO_PIN_5
#define LT8920_PIN_MISO			GPIO_PIN_6
#define LT8920_PIN_MOSI			GPIO_PIN_7


#define	Lt8920CsLow()			gpio_bit_reset(LT8920_PIN_PORT,LT8920_PIN_NSS)
#define	Lt8920CsHigh()			gpio_bit_set(LT8920_PIN_PORT,LT8920_PIN_NSS)

#define Lt8920PktStatus()	  	gpio_input_bit_get(LT8920_PIN_PORT, LT8920_PIN_IRQ)	



#define Lt8920Clear()	do{	\
							SpiLt8920Write(7, 0x00, 0x00);	\
							SpiLt8920Write(52, 0x80, 0x80);	\
}while(0);




/*包类型*/
typedef enum 									
{
/*匹配*/
	BROADCAST=1,			//广播
	BROADCAST_ACK,			//广播响应
	REQUEST_MATCH,			//请求匹配
	MATCHED,				//匹配成功
	
							/*扫码发送数据*/
	DATA_SEND,				//发送数据包
	RECEIVE_ACK,			//收到数据响应

	DUMMY_SEND,				//发空包
	PKT_CLASS_NULL
}PktClassDef;




typedef struct				
{
	PktClassDef 			PktClass;			//包类型
	uint8_t 				Pid;				//pid
	uint16_t 				SourceAddr;			//源地址
	uint8_t  				ForceMatch;			//强制匹配
	uint8_t  				Channel;			//Channel
	uint16_t 				Crc;
}BroadcastDef;



/*接收器响应包*/
typedef struct				
{
	PktClassDef 			PktClass;				//包类型
	uint8_t 				Pid;					//pid
	uint16_t 				SourceAddr;				//源地址
	uint16_t 				DestAddr;				//目标地址
	uint8_t  				Reserve;				//重发次数
	uint8_t  				Channel;				//channel
	uint16_t 				Crc;
}BroadcastAckDef;




/*扫码枪请求配对包*/
typedef struct				
{
	PktClassDef 			PktClass;				//包类型
	uint8_t 				Pid;					//pid
	uint16_t 				SourceAddr;				//源地址
	uint16_t 				DestAddr;				//目标地址
	uint8_t  				Reserve;				//重发次数
	uint16_t 				Crc;	
}RequestMatchDef;




/*接收器响应，配对成功包*/
typedef struct				
{
	PktClassDef 			PktClass;				//包类型
	uint8_t 				Pid;					//pid
	uint16_t 				SourceAddr;				//源地址
	uint16_t 				DestAddr;				//目标地址
	uint8_t  				Reserve;				//重发次数
	uint16_t 				Crc;		
}MatchedDef;





/*数据发送包*/
typedef struct 
{
	PktClassDef 			PktClass;				//包类型
	uint8_t 				Pid;					//pid	
	uint16_t 				SourceAddr;				//源地址
	uint16_t 				DestAddr;				//目标地址
	uint8_t 				TotalNum;
	uint8_t 				Num;
	uint8_t 				DataLen;
	uint8_t 				Data[DATA_LENTH];	
	uint8_t 				Channel;					//保留
	uint8_t 				SuccessPid;			//0...255
	uint16_t 				Crc;
}DataSendDef;



/*数据发送响应*/
typedef struct
{
	PktClassDef 			PktClass;				//包类型
	uint8_t 				Pid;
	uint8_t 				Reserve1[2];			//用于数据对齐，且后续可用于扩充
	uint16_t 				SourceAddr;
	uint16_t 				DestAddr;
	uint32_t 				Waiting;
	uint8_t 				Reserve2[2];			//用于数据对齐，且后续可用于扩充
	uint16_t				Crc;
}DataSendAck;




/*空包，提示成功后，不断发送*/
typedef struct
{
	PktClassDef 			PktClass;				//包类型
	uint16_t 				SourceAddr;
	uint16_t 				DestAddr;
	uint8_t 				SuccessPid;			//0...255
	uint8_t 				Channel;				//当前频道
	uint16_t					Crc;
}DummyDef;



//根据数据长度计算总包数
#define TotalCal(len)		( len/DATA_LENTH + ((len%DATA_LENTH)?1:0) )






static BroadcastDef 	Broadcast;				//广播包
static BroadcastAckDef	*PBroadcastAck;			//广播响应
static RequestMatchDef	PktRequestConnect;		//请求连接包
static MatchedDef		*PMatchAck;				//连接响应
static DataSendAck		*PDataSendAck;			//数据包响应
static DummyDef			Dummy;					//空包


static uint8_t RxBuffer[LT8920_BUF_LEN];		//接收缓冲区

static DataSendDef ArrayForSend[DATA_DIVIDED_BUF];			//数据分包缓存
#define ss WIRELESS_DATA_LEN_MAX/DATA_LENTH+ ((WIRELESS_DATA_LEN_MAX % DATA_LENTH)?1:0)

static uint16_t RandomVal=0;					//随机值
static uint16_t	SrcAddr=0x1234,DestAddr=0x5678;	//连接码

					
static uint8_t ChannelList[CHANNEL_AMOUNT]={0,10,29,55};		//由随机码计算出频道表
static uint8_t Channel;							//当前频道

static uint8_t Pid=0;							//不同的包标识相同的Pid，表示属于同一数据
static uint8_t SPid=0;							//告知接收器上一条成功的数据是哪个


static uint8_t Total=0;							//数据的总包数
static uint32_t Waiting=0;						//表示待发送的包还有哪些

DummyDef *PDummy=&Dummy;
uint32_t *PWaiting = &Waiting;
uint32_t *PChannelList = (uint32_t*)ChannelList;

//获取地址
static void ReadSrcDest(void);

//获取Pid SPid
static void ReadPidSPid(void);		

//空包初始化，告诉接收器上一条成功的数据，且也用于接收器找频
static void DummyInit(void);		

//根据源地址，目标地址，随机出一组频道
static void ChannelRand(void);

//告知接收器已经成功
static void InformSuccess(void);

static void ChannelSync(void);

static void ChannelHop(void);

static void DataArrayChannelSet(void);

//广播包初始化
static void BroadcastInit(ConnectMode mode);

//在所有频道中发布配对的消息，搜寻有响应的频道
static bool SearchChannel(void);

//等待广播响应
static bool WaitBroadcastAck(void);

//开始连接
static bool Connect(void);

//初始化请求连接
static void RequestConnectInit(void);

//等待请求响应
static bool WaitConnectAck(void);

//等待数据发送成功
static bool WaitingSuccess(void);

//CRC计算
static uint16_t CRCCal(uint8_t *ptr, uint8_t len);

//CRC检查
static bool CheckCrc(uint8_t *ptr, uint8_t len);

//发送单一的数据包
static void SendPkt(uint8_t* p_pkt, uint8_t len);

//读取数据包
static uint8_t ReadPkt(uint8_t* data);

//进入接收状态
static void ModeReceive(void);

//判断是否收到数据包，且硬件CRC校验没错
static bool ReceivedData(void);	

//数据分拆成数据包
static void DatasDivide(const uint8_t *datas, uint16_t len);

//计算等待发送的数据包num
static void WaitingCal(void);

//发送正在等待发送的数据包
static void SendWaitingDividedData(void);

static bool SendWaitingDividedDataNew(bool first);


//包ID自增，用于区分不同的数据，或相同Pid的数据包属于同一条数据
static void PidIncrease(void);

//SPI接口引脚初始化
static void Lt8920SpiInit(void);

//Lt8920写命令
static void SpiLt8920Write(uint16_t addr,uint16_t data1,uint16_t data2);

//写数据
static uint16_t SpiWrite(uint16_t data);

static void SaveSPid(uint32_t s_pid);
static void SavePid(uint32_t pid);
static uint8_t GetSPid(void);
static uint8_t GetPid(void);





/*********************************************
brief:		SPI引脚初始化，增加ISR到定时器中断函数
parameter:	void
return:		void
*********************************************/
void LT8920Init(void)
{
	Lt8920SpiInit();
}


/*********************************************
brief:		lt8920参数设置
parameter:	void
return:		void
*********************************************/
void LT8920Config(void)
{
	
    /*reset*/
	LT8920Reset();
	
	SpiLt8920Write( 0, 0x6f,0xef );
	SpiLt8920Write( 1, 0x56,0x81 );
	SpiLt8920Write( 2, 0x66,0x17 );
	SpiLt8920Write( 4, 0x9c,0xc9 );
	SpiLt8920Write( 5, 0x66,0x37 );						
	SpiLt8920Write( 7, 0x00,0x00 );							  //annel is 2402Mhz
	SpiLt8920Write( 8, 0x6c,0x90 );
	SpiLt8920Write( 9, 0x48,0x00 );			  				  //PA -12.2dbm

	SpiLt8920Write(10, 0x7f,0xfd );
	SpiLt8920Write(11, 0x00,0x08 );
	SpiLt8920Write(13, 0x48,0xbd );
	SpiLt8920Write(22, 0x00,0xff );
	SpiLt8920Write(23, 0x80,0x05 );
	SpiLt8920Write(24, 0x00,0x67 );
	SpiLt8920Write(25, 0x16,0x59 );
	SpiLt8920Write(26, 0x19,0xe0 );
	SpiLt8920Write(27, 0x13,0x00 );
	SpiLt8920Write(28, 0x18,0x00 );
	SpiLt8920Write(32, 0x48,0x00 );
	SpiLt8920Write(33, 0x3f,0xc7 );
	SpiLt8920Write(34, 0x20,0x00 );
	SpiLt8920Write(35, 0x03,0x00 );			//0a00
	SpiLt8920Write(36, 0x03,0x80 );
	SpiLt8920Write(37, 0x03,0x80 );
	SpiLt8920Write(38, 0x5a,0x5a );
	SpiLt8920Write(39, 0x03,0x80 );
//	SpiLt8920Write(40, 0x21,0x04 );
	SpiLt8920Write(40, 0x44,0x02 );
	SpiLt8920Write(41, 0xb8,0x07 );	   		//b8
	SpiLt8920Write(42, 0xfd,0xb0 );			/*CRC is ON; scramble is OFF; AUTO_ACK is ON*/
	SpiLt8920Write(43, 0x00,0x0f );
	SpiLt8920Write(44, 0x10,0x00 );			//62.5Kbps
//	SpiLt8920Write(44, 0x01,0x00 );			//1Mbps
	SpiLt8920Write(45, 0x05,0x52 );		 
	SpiLt8920Write(50, 0x00,0x00 );
	
	ReadSrcDest();							//获取匹配地址
	ReadPidSPid();							//读取Pid SPid
	
	ChannelRand();							//计算出频道列表
	DummyInit();							//初始化空包
	
	delay_1ms(100);
}



/*********************************************
brief:		lt8920关闭
parameter:	void
return:		
	TRUE
	FALSE
*********************************************/
bool LT8920Close(void)	
{
	SpiLt8920Write(35, 0x43,0x00 );				//睡眠
	Lt8920CsHigh();								//nss置高
	
	return TRUE;
}


/*********************************************
brief:		lt8920唤醒
parameter:	void
return:		
	TRUE
	FALSE
*********************************************/
bool LT8920Open(void)	
{
	Lt8920CsLow();								//nss拉低唤醒
	delay_1ms(5);								//等待唤醒
	Lt8920CsHigh();								//nss置高
	
	return TRUE;
}


/*********************************************
brief:		lt8920复位
parameter:	void
return:		
	TRUE
	FALSE
*********************************************/
bool LT8920Reset(void)
{
    /*reset*/
	gpio_bit_reset(LT8920_PIN_PORT,LT8920_PIN_RST);
	delay_1ms(10);
	gpio_bit_set(LT8920_PIN_PORT,LT8920_PIN_RST);
	delay_1ms(50);
	
	return TRUE;
}


/*********************************************
brief:		lt8920建立连接
parameter:	
	CONNECT_NORMAL		初次连接
	CONNECT_FORCE		非初次连接
return:		
	TRUE
	FALSE
*********************************************/
bool LT8920CreateConnect(ConnectMode mode)
{
	uint16_t srcTemp = SrcAddr,destTemp=DestAddr;
	SrcAddr = RandomVal;	//获取随机值
	BroadcastInit(mode);	//广播包初始化
	if(SearchChannel())
	{
		if(Connect())
		{
			ChannelRand();		//随机出频道列表
			DummyInit();
			return TRUE;
		}
	}
	//恢复SRC DEST
	SrcAddr = srcTemp;
	DestAddr = destTemp;
	return FALSE;
}



static bool Send(uint8_t n)
{
	static bool first;
	
	first = TRUE;
	while(n--)
	{
		ChannelSync();								//频率同步
		if(SendWaitingDividedDataNew(first))		//等待响应	
		{
			InformSuccess();
			return TRUE;
		}
		ChannelHop();
		first = FALSE;								//第一次操作过后
	}	
	return FALSE;
}



/*********************************************
brief:		无线发送数据
parameter:
	Data 	数据
	Len		长度
return:
	TRUE	成功
	FALSE	失败
*********************************************/
//bool LT8920Send(const uint8_t* Data, uint16_t Len)
//{
//	uint8_t n=10;
//	DatasDivide(Data, Len);	//分成数据包
//	while(n--)
//	{
//		ChannelSync();				//频率同步
//		SendWaitingDividedData();	//发送数据包
//		if(WaitingSuccess())		//等待响应	
//		{
//			InformSuccess();
//			return TRUE;
//		}
//		ChannelHop();
//		BSPWatchDogReset();
//	}
//	return FALSE;
//}



bool LT8920Send(const uint8_t* Data, uint16_t Len)
{
	DatasDivide(Data, Len);	//分成数据包

	return Send(10);
}



/************************************
btief:	需要循环调用，用于频率稳定，以及通知成功发送
parameter:	void
return:		void	
************************************/
void LT8920DummySend(void)
{
	static uint16_t temp=0;
	if(temp == RandomVal/500)
	{
		return;
	}
	temp = RandomVal/500;
	SendPkt((uint8_t*)&Dummy,sizeof(DummyDef));	//发送空包	
	
}





/************************************
btief:	同步频道
parameter:	void
return:		void	
************************************/
extern uint32_t time;
static void ChannelSync(void)
{
	uint8_t n=5;
	
	while(n--)
	{
		SendPkt((uint8_t*)&Dummy,sizeof(DummyDef));	//发送空包	
//		delay_1ms(5);	
	}

}




void LT8920FactoryReset()
{
	SaveSPid(0);		//SPid
	SavePid(0);			//Pid		
}



void LT8920SavePidSpid(void)
{
	SavePid(Pid);
	SaveSPid(SPid);
}



/*保存s_pid*/
void SaveSPid(uint32_t s_pid)	
{
	BSPEepromWrite(FLASH_LAST_SPID,&s_pid,1);	//写一个字	
}


/*保存PID*/
void SavePid(uint32_t pid)
{
	BSPEepromWrite(FLASH_LAST_PID,&pid,1);	//写一个字	
}



/*********************************************
brief:		随机值和延时时间++
parameter:	void
return:		void
*********************************************/
void LT8920IncreaseRandom(void)
{
	if(RandomVal++ >65500)
	{
		RandomVal=0;
	}
}


/*********************************************
brief:		发送接收器等待的数据包
parameter:	void
return:		void
*********************************************/
static void SendWaitingDividedData(void)	
{
	uint8_t i;

	uint32_t temp=Waiting;
	
	for(i=0;i<Total;i++)	//写实际数据
	{		
		if(temp & (uint32_t)1)
		{
			SendPkt((uint8_t*)&ArrayForSend[i],sizeof(DataSendDef));	//发送
		}
		temp>>=1;
		
		if(0 == temp)	//如果等于0 提前退出
		{
			return;
		}
//		delay_1ms(5);
	}
}




static bool SendWaitingDividedDataNew(bool first)
{

	uint8_t i;

	uint32_t temp=Waiting;
	
	for(i=0;i<Total;i++)	//写实际数据
	{		
		BSPWatchDogReset();
		if(temp & (uint32_t)1)
		{
			SendPkt((uint8_t*)&ArrayForSend[i],sizeof(DataSendDef));	//发送
			if((FALSE == first) || (i>=Total-1))		//第一次之后 或 发送完第一组
			{
				if(WaitingSuccess())
				{
					return TRUE;
				}			
			}
		}
		temp>>=1;
		
		if(0 == temp)	//如果等于0 提前退出
		{
			return FALSE;
		}
	}
	return FALSE;
}


/*********************************************
brief:		SPI初始化
parameter:	void
return:		void
*********************************************/
static void Lt8920SpiInit(void)
{
	spi_parameter_struct SPI_InitStructure;
	
	//时钟
	rcu_periph_clock_enable(LT8920_SPI_CLK);
	rcu_periph_clock_enable(LT8920_PIN_CLK);	
	
	//开启复用
	gpio_af_set(LT8920_PIN_PORT,
				GPIO_AF_0,
				LT8920_PIN_SPICLK|LT8920_PIN_MISO|LT8920_PIN_MOSI);


	//SPICLK	MISO	MOSI
	gpio_mode_set(LT8920_PIN_PORT,
					GPIO_MODE_AF,
					GPIO_PUPD_NONE,
					LT8920_PIN_SPICLK|LT8920_PIN_MISO|LT8920_PIN_MOSI);
	
	
	gpio_output_options_set(LT8920_PIN_PORT,
							GPIO_OTYPE_PP,
							GPIO_OSPEED_50MHZ,
							LT8920_PIN_SPICLK|LT8920_PIN_MISO|LT8920_PIN_MOSI);
	
	
	
	//RST	SPI_NSS
	gpio_mode_set(LT8920_PIN_PORT,
					GPIO_MODE_OUTPUT,
					GPIO_PUPD_NONE,
					LT8920_PIN_RST| LT8920_PIN_NSS);
					
	gpio_output_options_set(LT8920_PIN_PORT,
							GPIO_OTYPE_PP,
							GPIO_OSPEED_50MHZ,
							LT8920_PIN_RST| LT8920_PIN_NSS);
	
	//IRQ						
	gpio_mode_set(LT8920_PIN_PORT,
					GPIO_MODE_INPUT,
					GPIO_PUPD_NONE,
					LT8920_PIN_IRQ);	
	
	
	//SPI0初始化
	spi_disable(LT8920_SPI_PORT);
		
    SPI_InitStructure.trans_mode  = SPI_TRANSMODE_FULLDUPLEX;
    SPI_InitStructure.device_mode = SPI_MASTER;
    SPI_InitStructure.frame_size  = SPI_FRAMESIZE_8BIT;
	SPI_InitStructure.clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;
    SPI_InitStructure.nss= SPI_NSS_SOFT;
    SPI_InitStructure.prescale = SPI_PSC_8;
    SPI_InitStructure.endian = SPI_ENDIAN_MSB;

	spi_init(LT8920_SPI_PORT,&SPI_InitStructure);
	
	//NSS初始高
	Lt8920CsHigh();

    /* Enable SPI1  */
	spi_enable(SPI0);		
}



/*********************************************
brief:		Lt8920写命令
parameter:	
	addr	写地址
	data1	数据高字节
	data2	数据低字节
return:		void
*********************************************/
static void SpiLt8920Write(uint16_t addr,uint16_t data1,uint16_t data2)
{
	Lt8920CsLow();
	delay_1us(1);				//ms->us		1
	SpiWrite(addr);
	SpiWrite(data1);
	SpiWrite(data2);
	Lt8920CsHigh();
}


/*********************************************
brief:		Lt8920读命令
parameter:	
	addr	写地址
return:		uint16_t
*********************************************/
static uint16_t SPI_LT8920_Read(uint16_t addr)
{
	uint16_t val;
	addr |=0x80;
	Lt8920CsLow();
	SpiWrite(addr);
	val=(((uint16_t)(SpiWrite(0xFF)))<<8);
	val|=(uint16_t)(SpiWrite(0xFF));
	Lt8920CsHigh();
	return val;
}


/*********************************************
brief:		SPI写数据
parameter:	
	data	数据
return:		uint16_t
*********************************************/
#if (LT8920_ERR == 0)

static uint16_t SpiWrite(uint16_t data)
{
	uint16_t status;
						
	while (spi_i2s_flag_get(SPI0, SPI_FLAG_TBE) == RESET);
	spi_i2s_data_transmit(SPI0, data);
	while (spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE) == RESET);
	status = spi_i2s_data_receive(SPI0);
	
	return status;
}

#else

static uint16_t SpiWrite(uint16_t data)
{
	uint16_t status;
	uint16_t usCount=0;
	
	while (spi_i2s_flag_get(SPI0, SPI_FLAG_TBE) == RESET)	
	{
		delay_1us(1);
		if(usCount++ >0xf000)
		{
			LT8920Init();
			break;
		}
	}
	spi_i2s_data_transmit(SPI0, data);
	while (spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE) == RESET)
	{
		delay_1us(1);
		if(usCount++ >0xf000)
		{
			LT8920Init();
			break;
		}
	}
		
	status = spi_i2s_data_receive(SPI0);
	
	return status;
}

#endif



/*********************************************
brief:		广播包初始化
parameter:	
	CONNECT_NORMAL	初次连接
	CONNECT_FORCE	非初次连接
return:		void
*********************************************/
static void BroadcastInit(ConnectMode mode)	
{
	Broadcast.PktClass = BROADCAST;		
	Broadcast.Pid = Pid++;		
	Broadcast.SourceAddr = SrcAddr; 	//保存本地地址
	Broadcast.ForceMatch =mode;			//一般匹配-接收器未匹配时
}



/*********************************************
brief:		查找有响应频道
parameter:	void
return:	
	TRUE
	FALSE
*********************************************/
static bool SearchChannel(void)
{
	uint8_t n=70;	//7x次搜索
	
	while(n--)
	{
		BSPWatchDogReset();
		
		Channel = (Channel+1)%CHANNEL_AMOUNT;	
		Broadcast.Channel = Channel;
		Broadcast.Crc = CRCCal((uint8_t*)&Broadcast,sizeof(BroadcastDef)-2);	//添加Crc	
		SendPkt((uint8_t*)&Broadcast, sizeof(BroadcastDef));
		if(WaitBroadcastAck())
		{
			return TRUE;	//收到响应，返回1
		}	
	}
	Channel = ChannelList[0];	//没收到响应时 channel[0]
	return FALSE;				//返回0 失败
}




/*********************************************
brief:		进入接收模式
parameter:	void
return: 	void
*********************************************/
static void ModeReceive(void)
{
	SpiLt8920Write(52, 0x80, 0x80); //清空 RXTX
	
	SpiLt8920Write(7, 0x00, 0x80|Channel);	//接收模式	
	
	delay_1us(100);
}

/*********************************************
brief:		开始连接
parameter:	void
return:	
	TRUE
	FALSE
*********************************************/
static bool Connect(void)
{
	uint8_t overtime=20;
	
	RequestConnectInit();		//初始化匹配请求包 
		
	while(overtime--)
	{
		BSPWatchDogReset();
		
		SendPkt((uint8_t*)&PktRequestConnect, sizeof(RequestMatchDef));		//发送匹配请求
		
		if(WaitConnectAck())
		{
			return TRUE;
		}
	}
	return FALSE;	//失败
}



/*********************************************
brief:		等待广播响应
parameter:	void
return:	
	TRUE
	FALSE
*********************************************/
static bool WaitBroadcastAck(void)
{
	uint16_t wait_count;
				 

	ModeReceive();//进入接收模式

	wait_count=0;
	while(!ReceivedData()) 			//当未收到数据时
	{
		delay_1ms(1);
		if(wait_count++ >50)		//不超过200ms
		{
			return FALSE;
		}
	}		
		

	if(CheckCrc(RxBuffer,ReadPkt(RxBuffer)))			//check crc
	{
		PBroadcastAck=(BroadcastAckDef*)RxBuffer; 		//转换数据格式
		
		if(PBroadcastAck->DestAddr == SrcAddr) 			//是否发给自己的包 
		{
			if(PBroadcastAck->PktClass == BROADCAST_ACK)
			{
				DestAddr = PBroadcastAck->SourceAddr;	//提取有效数据,源地址
				Channel = PBroadcastAck->Channel;
				return TRUE;	
			}
		}			
	}	
	
	return FALSE;	//没响应，返回0
}





/*********************************************
brief:		等待连接响应
parameter:	void
return:	
	TRUE
	FALSE
*********************************************/
static bool WaitConnectAck(void)
{
	uint16_t wait_count;
				 
	ModeReceive();//进入接收模式

	wait_count=0;
	while(!ReceivedData()) 			//当未收到数据时
	{
		delay_1ms(1);
		if(wait_count++ >50)		//不超过200ms
		{
			return FALSE;
		}
	}		
		
	
	if(CheckCrc(RxBuffer,ReadPkt(RxBuffer)))			//check crc
	{
		PMatchAck=(MatchedDef*)RxBuffer; 		//转换数据格式

		

		//是否匹配包
		if( (PMatchAck->PktClass == MATCHED) && (PMatchAck->Pid == PktRequestConnect.Pid) )
		{
			//是否发给自己的包	,检查包来源，目的地是否相符
			if( (PMatchAck->DestAddr == SrcAddr) && (PMatchAck->SourceAddr == DestAddr) )
			{
				return TRUE;
			}					
		}		
	}	
	return FALSE;	//没响应，返回0
}


/*********************************************
brief:		等待数据包响应
parameter:	void
return:	
	TRUE
	FALSE
*********************************************/
static bool WaitingSuccess(void)
{
	uint16_t wait_count;
				 
	ModeReceive();//进入接收模式

	wait_count=0;
	while(!ReceivedData()) 			//当未收到数据时
	{
		delay_1ms(1);
		if(wait_count++ >20)		//不超过 ms
		{
			return FALSE;
		}
	}		
		
	
	if(CheckCrc(RxBuffer,ReadPkt(RxBuffer)))			//check crc
	{
		PDataSendAck=(DataSendAck*)RxBuffer; 		//转换数据格式
			
		//是否发给自己的包	,检查包来源，目的地是否相符
		if( (PDataSendAck->DestAddr == SrcAddr) && (PDataSendAck->SourceAddr == DestAddr) )
		{
			Waiting = PDataSendAck->Waiting;

			if(Waiting==0)
			{
				if(PDataSendAck->Pid == Pid)	//有可能收到的是之前的响应
				{
					SPid =PDataSendAck->Pid;	
					PDataSendAck->Pid=0;		//clear
					PDataSendAck->PktClass = PKT_CLASS_NULL;
					return TRUE;		
				}
				return FALSE;
			}					
		}				
	}
	
	return FALSE;	//没响应，返回0
}






/*******************************************************
brief:	CRC_CCITT(2 bytes)

parameter:
	ptr		校验数据
	len		数据长度

return:	crc
*******************************************************/
static uint16_t CRCCal(uint8_t *ptr, uint8_t len)
{
	uint32_t crc = 0;
	uint8_t i;

	while(len-- != 0)
	{
		for(i = 0x80; i != 0; i /= 2)
		{
			crc *= 2;
			if((crc&0x10000) !=0) //上一位 CRC 乘 2 后，若首位是 1，则除以 0x11021
				crc ^= 0x11021;
			if((*ptr&i) != 0) //如果本位是 1，那么 CRC = 上一位的 CRC + 本位/CRC_CCITT
				crc ^= 0x1021;
		}
		ptr++;
	}
	return (uint16_t)crc;
}




/*******************************************************
brief:	CRC_CCITT(2 bytes),data[],check

parameter:	
	ptr		待校验数据
	len		长度

return:	TRUE,FALSE
*******************************************************/
static bool CheckCrc(uint8_t *ptr, uint8_t len)
{
	uint16_t crc = CRCCal(ptr,len-2);
	
	uint16_t get_crc = ptr[len-1];
	get_crc <<=8;
	get_crc |= ptr[len-2];
	
	if(crc == get_crc)
		return TRUE;
	return FALSE;
}



/*********************************************
brief:		发送数据包
parameter:	
	p_pkt	数据
	len		长度
return:		void
*********************************************/
static void SendPkt(uint8_t* p_pkt, uint8_t len)
{
	uint8_t j;
	uint16_t overTime;
	
	
	Lt8920Clear();

	Lt8920CsLow();	//nss low
	
	delay_1us(1);		//us
	
	SpiWrite(50);			//写寄存器地址

	SpiWrite(len);		//写长度
	
	
	for(j=0;j<len;j++)		//写实际数据
	{
		SpiWrite(p_pkt[j]);	
	}
	Lt8920CsHigh();		//nss high		

	SpiLt8920Write(7, 0x01, Channel);	

	delay_1ms(1);
	/***********************时间间隔****************************/
	overTime=1000;		
	while (gpio_input_bit_get(GPIOA,GPIO_PIN_3) == 0)
	{
		BSPWatchDogReset();
		
		overTime--;
		delay_1ms(1);

		if(overTime==0)
		{
			break;
		}
	}		
}







/*********************************************
brief:		收到数据，且硬件判断CRC是否出错
parameter:	void
return: 	void
*********************************************/
static bool ReceivedData()
{
	if(Lt8920PktStatus() == 1)
	{
		if((SPI_LT8920_Read(48) & 0x8000) == 0)     //Test CRC
			return TRUE;																	//CRC无错误 ，一旦发生错误，寄存器 Reg48 bit15 位 CRC_ERROR 将被自动置 1，在下次开始 RX 或 TX 时它将被清除。
					
		ModeReceive();	//接收模式		
	}
	return FALSE;
}




/*********************************************
brief:		读数据包
parameter:	data	读入缓冲区
return: 	len		读取数据长度
*********************************************/
static uint8_t ReadPkt(uint8_t* data)
{	
	uint8_t len,j;

	Lt8920CsLow();  
	delay_1us(1);
	SpiWrite(50|0x80);
	delay_1us(1);
	
	len = (uint8_t)SpiWrite(Dummy_Byte);
	for(j=0;j<len;j++)
	{
		delay_1us(1);
		data[j] = (uint8_t)SpiWrite(Dummy_Byte);
	}

	delay_1us(1);
	Lt8920CsHigh();
	delay_1us(1);
	
	return len;
}




/*********************************************
brief:		连接请求包初始化
parameter:	void
return: 	void
*********************************************/
static void RequestConnectInit(void)
{
	PktRequestConnect.PktClass = REQUEST_MATCH;
	PktRequestConnect.Pid = Pid++;
	PktRequestConnect.SourceAddr = SrcAddr;
	PktRequestConnect.DestAddr = DestAddr;
	
	//添加Crc
	PktRequestConnect.Crc = CRCCal((uint8_t*)&PktRequestConnect,sizeof(RequestMatchDef)-2);		
}



/*********************************************
brief:		数据分拆
parameter:	
	datas	数据
	len		长度
return: 	void
*********************************************/
static void DatasDivide(const uint8_t *datas, uint16_t len)
{
	uint8_t i=0,j=0;
	
	/*计算总包数*/
	Total = TotalCal(len);

	WaitingCal();
	
	/*本包pid*/
	PidIncrease();

	
	/*开始初始化*/
	for(i=0;i<Total;i++)
	{
		ArrayForSend[i].PktClass = DATA_SEND;			//在DATA_SEND的基础上分割，高4位表示键盘语言，低4位表示包类型
		ArrayForSend[i].SourceAddr = SrcAddr;
		ArrayForSend[i].DestAddr = DestAddr;
		ArrayForSend[i].Pid = Pid;
		ArrayForSend[i].TotalNum = Total;
		ArrayForSend[i].Num = i;
		ArrayForSend[i].Channel = Channel;	//通知接收器 当前的channel
		ArrayForSend[i].SuccessPid = SPid;				//0代表没有需要上传的
		
		/********************有效数据长度************************/
		if(len>=DATA_LENTH)
			ArrayForSend[i].DataLen = DATA_LENTH;	
		else
			ArrayForSend[i].DataLen = len;		
		/*-----------------------------------------------------*/
		
		for(j=0;j<ArrayForSend[i].DataLen;j++)					//结构体内的数组赋值
		{			
			ArrayForSend[i].Data[j] = *(datas++);
			len--;
		}

		/*计算CRC*/
		ArrayForSend[i].Crc = CRCCal((uint8_t*)&ArrayForSend[i],sizeof(DataSendDef)-2);		//添加crc
	}
}


/*********************************************
brief:		计算waiting 对应位置1，表示该包需要发送
			10001,表示 0包，4包需要发送，123包已经成功发送
parameter:	void
return: 	void
*********************************************/
static void WaitingCal(void)
{
	uint8_t i;
	Waiting = 1;
	for(i=0;i<Total-1;i++)
	{
		Waiting <<= 1;
		Waiting |= 1;
	}
}

/*********************************************
brief:		Pid++
			数据的ID，类似身份证，区别不同的数据
parameter:	void
return: 	void
*********************************************/
static void PidIncrease(void)
{
	Pid++;
	if(Pid==0)	
	{
		Pid=1;
	}
}



static void DummyInit(void)
{
	Dummy.PktClass = DUMMY_SEND;
	Dummy.SourceAddr = SrcAddr;
	Dummy.DestAddr = DestAddr;
	Dummy.SuccessPid = SPid;
	Dummy.Channel = Channel;
	Dummy.Crc = CRCCal((uint8_t*)&Dummy, sizeof(DummyDef)-2);	//添加crc
}




//根据配对频道，计算出跳转频道
static void ChannelRand(void)
{
	uint8_t i=0,j,equal_flag=0;
	uint8_t get_match_channel=0;


	srand((uint16_t)(SrcAddr + DestAddr));

	for(i=0;i<CHANNEL_AMOUNT;i++)
	{
		do
		{
			equal_flag=0;

			ChannelList[i] = rand()% CHANNEL_RANGE;
	
			if(ChannelList[i]<INSERT)				//若已有小于INSERT的channel
			{
				get_match_channel=1;				//则不再插入小于INSERT的channel
			}
			
			for(j=0;j<i;j++)
			{
				if(ChannelList[i] == ChannelList[j])
				{
					equal_flag = 1;
					break;
				}
			}		
		}
		while(equal_flag == 1);
		
	}

	if(get_match_channel == 0)	//当未获得匹配频道，获取
	{
		ChannelList[0] = ChannelList[0]%CHANNEL_AMOUNT;
	}

	Channel = ChannelList[0];	
}



static uint8_t index = 0;
static void InformSuccess(void)
{
	uint16_t at_least=5;
	
	Dummy.SuccessPid= SPid;		//空包加上s_pid
	
	Dummy.Crc = CRCCal((uint8_t*)&Dummy, sizeof(DummyDef)-2);	//添加crc
	while(at_least--)	//至少发5个
	{
		SendPkt((uint8_t*)&Dummy,sizeof(DummyDef));	//发送空包	
//		delay_1ms(5);	
	}
}




static void ChannelHop(void)
{
	index=(index+1)%CHANNEL_AMOUNT;	//下一个频点索引
	
	//设置发送频道
	Channel = ChannelList[index];	//跳频
	
	DataArrayChannelSet();			//设置数据包频道
	
	Dummy.Channel = Channel;		//设置空包的频道
	
	Dummy.Crc = CRCCal((uint8_t*)&Dummy, sizeof(DummyDef)-2);	//添加crc
	
}


static void DataArrayChannelSet(void)
{
	uint8_t i;
	for(i=0;i<Total;i++)
	{
		ArrayForSend[i].Channel = Channel;
		
		/*计算CRC*/
		ArrayForSend[i].Crc = CRCCal((uint8_t*)&ArrayForSend[i],sizeof(DataSendDef)-2);		//添加crc			
	}
}


static void ReadSrcDest(void)
{
	uint32_t addr;
	BSPEepromRead(FLASH_SRC_DEST,&addr,1);	//读一个字
	
	DestAddr = (uint16_t)(addr & 0x0000ffff);
	
	addr>>=16;
	
	SrcAddr = (uint16_t)(addr & 0x0000ffff);	
}


static void ReadPidSPid(void)
{
	Pid = GetPid();
	SPid = GetSPid();
}


static uint8_t GetSPid()
{
	uint32_t s_pid;
	BSPEepromRead(FLASH_LAST_SPID,&s_pid,1);	//读一个字
	
	return (uint8_t)s_pid;
}



static uint8_t GetPid()
{
	uint32_t pid;
	BSPEepromRead(FLASH_LAST_PID,&pid,1);	//读一个字
	
	return (uint8_t)pid;
}



void LT8920SaveSrcDest(void)
{
	//两个16位变量，拼成32位字，高位src，低dest
	uint32_t com_addr=SrcAddr;	
	com_addr <<=16;
	com_addr |= DestAddr;
	
	BSPEepromWrite(FLASH_SRC_DEST,&com_addr,1);	//写一个字
}

