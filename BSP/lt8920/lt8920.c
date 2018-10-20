#include "lt8920.h"
#include "BSPWireless.h"
#include "BSPEeprom.h"
#include "gd32f1x0.h"
#include "stdlib.h"	
#include "systick.h"



#define LT8920_BUF_LEN		64						//LT8920������64

#define CHANNEL_RANGE		83

#define DATA_LENTH			49

//����������󳤶ȣ��ְ���������С
#define DATA_DIVIDED_BUF	( WIRELESS_DATA_LEN_MAX/DATA_LENTH + ( (WIRELESS_DATA_LEN_MAX % DATA_LENTH)?1:0) )

#define Dummy_Byte 0xFF       			//���ַ�

#define INSERT				4			//����С��7��Ƶ��			
#define CHANNEL_AMOUNT		4			//Ƶ����

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




/*������*/
typedef enum 									
{
/*ƥ��*/
	BROADCAST=1,			//�㲥
	BROADCAST_ACK,			//�㲥��Ӧ
	REQUEST_MATCH,			//����ƥ��
	MATCHED,				//ƥ��ɹ�
	
							/*ɨ�뷢������*/
	DATA_SEND,				//�������ݰ�
	RECEIVE_ACK,			//�յ�������Ӧ

	DUMMY_SEND,				//���հ�
	PKT_CLASS_NULL
}PktClassDef;




typedef struct				
{
	PktClassDef 			PktClass;			//������
	uint8_t 				Pid;				//pid
	uint16_t 				SourceAddr;			//Դ��ַ
	uint8_t  				ForceMatch;			//ǿ��ƥ��
	uint8_t  				Channel;			//Channel
	uint16_t 				Crc;
}BroadcastDef;



/*��������Ӧ��*/
typedef struct				
{
	PktClassDef 			PktClass;				//������
	uint8_t 				Pid;					//pid
	uint16_t 				SourceAddr;				//Դ��ַ
	uint16_t 				DestAddr;				//Ŀ���ַ
	uint8_t  				Reserve;				//�ط�����
	uint8_t  				Channel;				//channel
	uint16_t 				Crc;
}BroadcastAckDef;




/*ɨ��ǹ������԰�*/
typedef struct				
{
	PktClassDef 			PktClass;				//������
	uint8_t 				Pid;					//pid
	uint16_t 				SourceAddr;				//Դ��ַ
	uint16_t 				DestAddr;				//Ŀ���ַ
	uint8_t  				Reserve;				//�ط�����
	uint16_t 				Crc;	
}RequestMatchDef;




/*��������Ӧ����Գɹ���*/
typedef struct				
{
	PktClassDef 			PktClass;				//������
	uint8_t 				Pid;					//pid
	uint16_t 				SourceAddr;				//Դ��ַ
	uint16_t 				DestAddr;				//Ŀ���ַ
	uint8_t  				Reserve;				//�ط�����
	uint16_t 				Crc;		
}MatchedDef;





/*���ݷ��Ͱ�*/
typedef struct 
{
	PktClassDef 			PktClass;				//������
	uint8_t 				Pid;					//pid	
	uint16_t 				SourceAddr;				//Դ��ַ
	uint16_t 				DestAddr;				//Ŀ���ַ
	uint8_t 				TotalNum;
	uint8_t 				Num;
	uint8_t 				DataLen;
	uint8_t 				Data[DATA_LENTH];	
	uint8_t 				Channel;					//����
	uint8_t 				SuccessPid;			//0...255
	uint16_t 				Crc;
}DataSendDef;



/*���ݷ�����Ӧ*/
typedef struct
{
	PktClassDef 			PktClass;				//������
	uint8_t 				Pid;
	uint8_t 				Reserve1[2];			//�������ݶ��룬�Һ�������������
	uint16_t 				SourceAddr;
	uint16_t 				DestAddr;
	uint32_t 				Waiting;
	uint8_t 				Reserve2[2];			//�������ݶ��룬�Һ�������������
	uint16_t				Crc;
}DataSendAck;




/*�հ�����ʾ�ɹ��󣬲��Ϸ���*/
typedef struct
{
	PktClassDef 			PktClass;				//������
	uint16_t 				SourceAddr;
	uint16_t 				DestAddr;
	uint8_t 				SuccessPid;			//0...255
	uint8_t 				Channel;				//��ǰƵ��
	uint16_t					Crc;
}DummyDef;



//�������ݳ��ȼ����ܰ���
#define TotalCal(len)		( len/DATA_LENTH + ((len%DATA_LENTH)?1:0) )






static BroadcastDef 	Broadcast;				//�㲥��
static BroadcastAckDef	*PBroadcastAck;			//�㲥��Ӧ
static RequestMatchDef	PktRequestConnect;		//�������Ӱ�
static MatchedDef		*PMatchAck;				//������Ӧ
static DataSendAck		*PDataSendAck;			//���ݰ���Ӧ
static DummyDef			Dummy;					//�հ�


static uint8_t RxBuffer[LT8920_BUF_LEN];		//���ջ�����

static DataSendDef ArrayForSend[DATA_DIVIDED_BUF];			//���ݷְ�����
#define ss WIRELESS_DATA_LEN_MAX/DATA_LENTH+ ((WIRELESS_DATA_LEN_MAX % DATA_LENTH)?1:0)

static uint16_t RandomVal=0;					//���ֵ
static uint16_t	SrcAddr=0x1234,DestAddr=0x5678;	//������

					
static uint8_t ChannelList[CHANNEL_AMOUNT]={0,10,29,55};		//�����������Ƶ����
static uint8_t Channel;							//��ǰƵ��

static uint8_t Pid=0;							//��ͬ�İ���ʶ��ͬ��Pid����ʾ����ͬһ����
static uint8_t SPid=0;							//��֪��������һ���ɹ����������ĸ�


static uint8_t Total=0;							//���ݵ��ܰ���
static uint32_t Waiting=0;						//��ʾ�����͵İ�������Щ

DummyDef *PDummy=&Dummy;
uint32_t *PWaiting = &Waiting;
uint32_t *PChannelList = (uint32_t*)ChannelList;

//��ȡ��ַ
static void ReadSrcDest(void);

//��ȡPid SPid
static void ReadPidSPid(void);		

//�հ���ʼ�������߽�������һ���ɹ������ݣ���Ҳ���ڽ�������Ƶ
static void DummyInit(void);		

//����Դ��ַ��Ŀ���ַ�������һ��Ƶ��
static void ChannelRand(void);

//��֪�������Ѿ��ɹ�
static void InformSuccess(void);

static void ChannelSync(void);

static void ChannelHop(void);

static void DataArrayChannelSet(void);

//�㲥����ʼ��
static void BroadcastInit(ConnectMode mode);

//������Ƶ���з�����Ե���Ϣ����Ѱ����Ӧ��Ƶ��
static bool SearchChannel(void);

//�ȴ��㲥��Ӧ
static bool WaitBroadcastAck(void);

//��ʼ����
static bool Connect(void);

//��ʼ����������
static void RequestConnectInit(void);

//�ȴ�������Ӧ
static bool WaitConnectAck(void);

//�ȴ����ݷ��ͳɹ�
static bool WaitingSuccess(void);

//CRC����
static uint16_t CRCCal(uint8_t *ptr, uint8_t len);

//CRC���
static bool CheckCrc(uint8_t *ptr, uint8_t len);

//���͵�һ�����ݰ�
static void SendPkt(uint8_t* p_pkt, uint8_t len);

//��ȡ���ݰ�
static uint8_t ReadPkt(uint8_t* data);

//�������״̬
static void ModeReceive(void);

//�ж��Ƿ��յ����ݰ�����Ӳ��CRCУ��û��
static bool ReceivedData(void);	

//���ݷֲ�����ݰ�
static void DatasDivide(const uint8_t *datas, uint16_t len);

//����ȴ����͵����ݰ�num
static void WaitingCal(void);

//�������ڵȴ����͵����ݰ�
static void SendWaitingDividedData(void);

static bool SendWaitingDividedDataNew(bool first);


//��ID�������������ֲ�ͬ�����ݣ�����ͬPid�����ݰ�����ͬһ������
static void PidIncrease(void);

//SPI�ӿ����ų�ʼ��
static void Lt8920SpiInit(void);

//Lt8920д����
static void SpiLt8920Write(uint16_t addr,uint16_t data1,uint16_t data2);

//д����
static uint16_t SpiWrite(uint16_t data);

static void SaveSPid(uint32_t s_pid);
static void SavePid(uint32_t pid);
static uint8_t GetSPid(void);
static uint8_t GetPid(void);





/*********************************************
brief:		SPI���ų�ʼ��������ISR����ʱ���жϺ���
parameter:	void
return:		void
*********************************************/
void LT8920Init(void)
{
	Lt8920SpiInit();
}


/*********************************************
brief:		lt8920��������
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
	
	ReadSrcDest();							//��ȡƥ���ַ
	ReadPidSPid();							//��ȡPid SPid
	
	ChannelRand();							//�����Ƶ���б�
	DummyInit();							//��ʼ���հ�
	
	delay_1ms(100);
}



/*********************************************
brief:		lt8920�ر�
parameter:	void
return:		
	TRUE
	FALSE
*********************************************/
bool LT8920Close(void)	
{
	SpiLt8920Write(35, 0x43,0x00 );				//˯��
	Lt8920CsHigh();								//nss�ø�
	
	return TRUE;
}


/*********************************************
brief:		lt8920����
parameter:	void
return:		
	TRUE
	FALSE
*********************************************/
bool LT8920Open(void)	
{
	Lt8920CsLow();								//nss���ͻ���
	delay_1ms(5);								//�ȴ�����
	Lt8920CsHigh();								//nss�ø�
	
	return TRUE;
}


/*********************************************
brief:		lt8920��λ
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
brief:		lt8920��������
parameter:	
	CONNECT_NORMAL		��������
	CONNECT_FORCE		�ǳ�������
return:		
	TRUE
	FALSE
*********************************************/
bool LT8920CreateConnect(ConnectMode mode)
{
	uint16_t srcTemp = SrcAddr,destTemp=DestAddr;
	SrcAddr = RandomVal;	//��ȡ���ֵ
	BroadcastInit(mode);	//�㲥����ʼ��
	if(SearchChannel())
	{
		if(Connect())
		{
			ChannelRand();		//�����Ƶ���б�
			DummyInit();
			return TRUE;
		}
	}
	//�ָ�SRC DEST
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
		ChannelSync();								//Ƶ��ͬ��
		if(SendWaitingDividedDataNew(first))		//�ȴ���Ӧ	
		{
			InformSuccess();
			return TRUE;
		}
		ChannelHop();
		first = FALSE;								//��һ�β�������
	}	
	return FALSE;
}



/*********************************************
brief:		���߷�������
parameter:
	Data 	����
	Len		����
return:
	TRUE	�ɹ�
	FALSE	ʧ��
*********************************************/
//bool LT8920Send(const uint8_t* Data, uint16_t Len)
//{
//	uint8_t n=10;
//	DatasDivide(Data, Len);	//�ֳ����ݰ�
//	while(n--)
//	{
//		ChannelSync();				//Ƶ��ͬ��
//		SendWaitingDividedData();	//�������ݰ�
//		if(WaitingSuccess())		//�ȴ���Ӧ	
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
	DatasDivide(Data, Len);	//�ֳ����ݰ�

	return Send(10);
}



/************************************
btief:	��Ҫѭ�����ã�����Ƶ���ȶ����Լ�֪ͨ�ɹ�����
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
	SendPkt((uint8_t*)&Dummy,sizeof(DummyDef));	//���Ϳհ�	
	
}





/************************************
btief:	ͬ��Ƶ��
parameter:	void
return:		void	
************************************/
extern uint32_t time;
static void ChannelSync(void)
{
	uint8_t n=5;
	
	while(n--)
	{
		SendPkt((uint8_t*)&Dummy,sizeof(DummyDef));	//���Ϳհ�	
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



/*����s_pid*/
void SaveSPid(uint32_t s_pid)	
{
	BSPEepromWrite(FLASH_LAST_SPID,&s_pid,1);	//дһ����	
}


/*����PID*/
void SavePid(uint32_t pid)
{
	BSPEepromWrite(FLASH_LAST_PID,&pid,1);	//дһ����	
}



/*********************************************
brief:		���ֵ����ʱʱ��++
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
brief:		���ͽ������ȴ������ݰ�
parameter:	void
return:		void
*********************************************/
static void SendWaitingDividedData(void)	
{
	uint8_t i;

	uint32_t temp=Waiting;
	
	for(i=0;i<Total;i++)	//дʵ������
	{		
		if(temp & (uint32_t)1)
		{
			SendPkt((uint8_t*)&ArrayForSend[i],sizeof(DataSendDef));	//����
		}
		temp>>=1;
		
		if(0 == temp)	//�������0 ��ǰ�˳�
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
	
	for(i=0;i<Total;i++)	//дʵ������
	{		
		BSPWatchDogReset();
		if(temp & (uint32_t)1)
		{
			SendPkt((uint8_t*)&ArrayForSend[i],sizeof(DataSendDef));	//����
			if((FALSE == first) || (i>=Total-1))		//��һ��֮�� �� �������һ��
			{
				if(WaitingSuccess())
				{
					return TRUE;
				}			
			}
		}
		temp>>=1;
		
		if(0 == temp)	//�������0 ��ǰ�˳�
		{
			return FALSE;
		}
	}
	return FALSE;
}


/*********************************************
brief:		SPI��ʼ��
parameter:	void
return:		void
*********************************************/
static void Lt8920SpiInit(void)
{
	spi_parameter_struct SPI_InitStructure;
	
	//ʱ��
	rcu_periph_clock_enable(LT8920_SPI_CLK);
	rcu_periph_clock_enable(LT8920_PIN_CLK);	
	
	//��������
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
	
	
	//SPI0��ʼ��
	spi_disable(LT8920_SPI_PORT);
		
    SPI_InitStructure.trans_mode  = SPI_TRANSMODE_FULLDUPLEX;
    SPI_InitStructure.device_mode = SPI_MASTER;
    SPI_InitStructure.frame_size  = SPI_FRAMESIZE_8BIT;
	SPI_InitStructure.clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;
    SPI_InitStructure.nss= SPI_NSS_SOFT;
    SPI_InitStructure.prescale = SPI_PSC_8;
    SPI_InitStructure.endian = SPI_ENDIAN_MSB;

	spi_init(LT8920_SPI_PORT,&SPI_InitStructure);
	
	//NSS��ʼ��
	Lt8920CsHigh();

    /* Enable SPI1  */
	spi_enable(SPI0);		
}



/*********************************************
brief:		Lt8920д����
parameter:	
	addr	д��ַ
	data1	���ݸ��ֽ�
	data2	���ݵ��ֽ�
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
brief:		Lt8920������
parameter:	
	addr	д��ַ
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
brief:		SPIд����
parameter:	
	data	����
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
brief:		�㲥����ʼ��
parameter:	
	CONNECT_NORMAL	��������
	CONNECT_FORCE	�ǳ�������
return:		void
*********************************************/
static void BroadcastInit(ConnectMode mode)	
{
	Broadcast.PktClass = BROADCAST;		
	Broadcast.Pid = Pid++;		
	Broadcast.SourceAddr = SrcAddr; 	//���汾�ص�ַ
	Broadcast.ForceMatch =mode;			//һ��ƥ��-������δƥ��ʱ
}



/*********************************************
brief:		��������ӦƵ��
parameter:	void
return:	
	TRUE
	FALSE
*********************************************/
static bool SearchChannel(void)
{
	uint8_t n=70;	//7x������
	
	while(n--)
	{
		BSPWatchDogReset();
		
		Channel = (Channel+1)%CHANNEL_AMOUNT;	
		Broadcast.Channel = Channel;
		Broadcast.Crc = CRCCal((uint8_t*)&Broadcast,sizeof(BroadcastDef)-2);	//���Crc	
		SendPkt((uint8_t*)&Broadcast, sizeof(BroadcastDef));
		if(WaitBroadcastAck())
		{
			return TRUE;	//�յ���Ӧ������1
		}	
	}
	Channel = ChannelList[0];	//û�յ���Ӧʱ channel[0]
	return FALSE;				//����0 ʧ��
}




/*********************************************
brief:		�������ģʽ
parameter:	void
return: 	void
*********************************************/
static void ModeReceive(void)
{
	SpiLt8920Write(52, 0x80, 0x80); //��� RXTX
	
	SpiLt8920Write(7, 0x00, 0x80|Channel);	//����ģʽ	
	
	delay_1us(100);
}

/*********************************************
brief:		��ʼ����
parameter:	void
return:	
	TRUE
	FALSE
*********************************************/
static bool Connect(void)
{
	uint8_t overtime=20;
	
	RequestConnectInit();		//��ʼ��ƥ������� 
		
	while(overtime--)
	{
		BSPWatchDogReset();
		
		SendPkt((uint8_t*)&PktRequestConnect, sizeof(RequestMatchDef));		//����ƥ������
		
		if(WaitConnectAck())
		{
			return TRUE;
		}
	}
	return FALSE;	//ʧ��
}



/*********************************************
brief:		�ȴ��㲥��Ӧ
parameter:	void
return:	
	TRUE
	FALSE
*********************************************/
static bool WaitBroadcastAck(void)
{
	uint16_t wait_count;
				 

	ModeReceive();//�������ģʽ

	wait_count=0;
	while(!ReceivedData()) 			//��δ�յ�����ʱ
	{
		delay_1ms(1);
		if(wait_count++ >50)		//������200ms
		{
			return FALSE;
		}
	}		
		

	if(CheckCrc(RxBuffer,ReadPkt(RxBuffer)))			//check crc
	{
		PBroadcastAck=(BroadcastAckDef*)RxBuffer; 		//ת�����ݸ�ʽ
		
		if(PBroadcastAck->DestAddr == SrcAddr) 			//�Ƿ񷢸��Լ��İ� 
		{
			if(PBroadcastAck->PktClass == BROADCAST_ACK)
			{
				DestAddr = PBroadcastAck->SourceAddr;	//��ȡ��Ч����,Դ��ַ
				Channel = PBroadcastAck->Channel;
				return TRUE;	
			}
		}			
	}	
	
	return FALSE;	//û��Ӧ������0
}





/*********************************************
brief:		�ȴ�������Ӧ
parameter:	void
return:	
	TRUE
	FALSE
*********************************************/
static bool WaitConnectAck(void)
{
	uint16_t wait_count;
				 
	ModeReceive();//�������ģʽ

	wait_count=0;
	while(!ReceivedData()) 			//��δ�յ�����ʱ
	{
		delay_1ms(1);
		if(wait_count++ >50)		//������200ms
		{
			return FALSE;
		}
	}		
		
	
	if(CheckCrc(RxBuffer,ReadPkt(RxBuffer)))			//check crc
	{
		PMatchAck=(MatchedDef*)RxBuffer; 		//ת�����ݸ�ʽ

		

		//�Ƿ�ƥ���
		if( (PMatchAck->PktClass == MATCHED) && (PMatchAck->Pid == PktRequestConnect.Pid) )
		{
			//�Ƿ񷢸��Լ��İ�	,������Դ��Ŀ�ĵ��Ƿ����
			if( (PMatchAck->DestAddr == SrcAddr) && (PMatchAck->SourceAddr == DestAddr) )
			{
				return TRUE;
			}					
		}		
	}	
	return FALSE;	//û��Ӧ������0
}


/*********************************************
brief:		�ȴ����ݰ���Ӧ
parameter:	void
return:	
	TRUE
	FALSE
*********************************************/
static bool WaitingSuccess(void)
{
	uint16_t wait_count;
				 
	ModeReceive();//�������ģʽ

	wait_count=0;
	while(!ReceivedData()) 			//��δ�յ�����ʱ
	{
		delay_1ms(1);
		if(wait_count++ >20)		//������ ms
		{
			return FALSE;
		}
	}		
		
	
	if(CheckCrc(RxBuffer,ReadPkt(RxBuffer)))			//check crc
	{
		PDataSendAck=(DataSendAck*)RxBuffer; 		//ת�����ݸ�ʽ
			
		//�Ƿ񷢸��Լ��İ�	,������Դ��Ŀ�ĵ��Ƿ����
		if( (PDataSendAck->DestAddr == SrcAddr) && (PDataSendAck->SourceAddr == DestAddr) )
		{
			Waiting = PDataSendAck->Waiting;

			if(Waiting==0)
			{
				if(PDataSendAck->Pid == Pid)	//�п����յ�����֮ǰ����Ӧ
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
	
	return FALSE;	//û��Ӧ������0
}






/*******************************************************
brief:	CRC_CCITT(2 bytes)

parameter:
	ptr		У������
	len		���ݳ���

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
			if((crc&0x10000) !=0) //��һλ CRC �� 2 ������λ�� 1������� 0x11021
				crc ^= 0x11021;
			if((*ptr&i) != 0) //�����λ�� 1����ô CRC = ��һλ�� CRC + ��λ/CRC_CCITT
				crc ^= 0x1021;
		}
		ptr++;
	}
	return (uint16_t)crc;
}




/*******************************************************
brief:	CRC_CCITT(2 bytes),data[],check

parameter:	
	ptr		��У������
	len		����

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
brief:		�������ݰ�
parameter:	
	p_pkt	����
	len		����
return:		void
*********************************************/
static void SendPkt(uint8_t* p_pkt, uint8_t len)
{
	uint8_t j;
	uint16_t overTime;
	
	
	Lt8920Clear();

	Lt8920CsLow();	//nss low
	
	delay_1us(1);		//us
	
	SpiWrite(50);			//д�Ĵ�����ַ

	SpiWrite(len);		//д����
	
	
	for(j=0;j<len;j++)		//дʵ������
	{
		SpiWrite(p_pkt[j]);	
	}
	Lt8920CsHigh();		//nss high		

	SpiLt8920Write(7, 0x01, Channel);	

	delay_1ms(1);
	/***********************ʱ����****************************/
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
brief:		�յ����ݣ���Ӳ���ж�CRC�Ƿ����
parameter:	void
return: 	void
*********************************************/
static bool ReceivedData()
{
	if(Lt8920PktStatus() == 1)
	{
		if((SPI_LT8920_Read(48) & 0x8000) == 0)     //Test CRC
			return TRUE;																	//CRC�޴��� ��һ���������󣬼Ĵ��� Reg48 bit15 λ CRC_ERROR �����Զ��� 1�����´ο�ʼ RX �� TX ʱ�����������
					
		ModeReceive();	//����ģʽ		
	}
	return FALSE;
}




/*********************************************
brief:		�����ݰ�
parameter:	data	���뻺����
return: 	len		��ȡ���ݳ���
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
brief:		�����������ʼ��
parameter:	void
return: 	void
*********************************************/
static void RequestConnectInit(void)
{
	PktRequestConnect.PktClass = REQUEST_MATCH;
	PktRequestConnect.Pid = Pid++;
	PktRequestConnect.SourceAddr = SrcAddr;
	PktRequestConnect.DestAddr = DestAddr;
	
	//���Crc
	PktRequestConnect.Crc = CRCCal((uint8_t*)&PktRequestConnect,sizeof(RequestMatchDef)-2);		
}



/*********************************************
brief:		���ݷֲ�
parameter:	
	datas	����
	len		����
return: 	void
*********************************************/
static void DatasDivide(const uint8_t *datas, uint16_t len)
{
	uint8_t i=0,j=0;
	
	/*�����ܰ���*/
	Total = TotalCal(len);

	WaitingCal();
	
	/*����pid*/
	PidIncrease();

	
	/*��ʼ��ʼ��*/
	for(i=0;i<Total;i++)
	{
		ArrayForSend[i].PktClass = DATA_SEND;			//��DATA_SEND�Ļ����Ϸָ��4λ��ʾ�������ԣ���4λ��ʾ������
		ArrayForSend[i].SourceAddr = SrcAddr;
		ArrayForSend[i].DestAddr = DestAddr;
		ArrayForSend[i].Pid = Pid;
		ArrayForSend[i].TotalNum = Total;
		ArrayForSend[i].Num = i;
		ArrayForSend[i].Channel = Channel;	//֪ͨ������ ��ǰ��channel
		ArrayForSend[i].SuccessPid = SPid;				//0����û����Ҫ�ϴ���
		
		/********************��Ч���ݳ���************************/
		if(len>=DATA_LENTH)
			ArrayForSend[i].DataLen = DATA_LENTH;	
		else
			ArrayForSend[i].DataLen = len;		
		/*-----------------------------------------------------*/
		
		for(j=0;j<ArrayForSend[i].DataLen;j++)					//�ṹ���ڵ����鸳ֵ
		{			
			ArrayForSend[i].Data[j] = *(datas++);
			len--;
		}

		/*����CRC*/
		ArrayForSend[i].Crc = CRCCal((uint8_t*)&ArrayForSend[i],sizeof(DataSendDef)-2);		//���crc
	}
}


/*********************************************
brief:		����waiting ��Ӧλ��1����ʾ�ð���Ҫ����
			10001,��ʾ 0����4����Ҫ���ͣ�123���Ѿ��ɹ�����
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
			���ݵ�ID���������֤������ͬ������
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
	Dummy.Crc = CRCCal((uint8_t*)&Dummy, sizeof(DummyDef)-2);	//���crc
}




//�������Ƶ�����������תƵ��
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
	
			if(ChannelList[i]<INSERT)				//������С��INSERT��channel
			{
				get_match_channel=1;				//���ٲ���С��INSERT��channel
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

	if(get_match_channel == 0)	//��δ���ƥ��Ƶ������ȡ
	{
		ChannelList[0] = ChannelList[0]%CHANNEL_AMOUNT;
	}

	Channel = ChannelList[0];	
}



static uint8_t index = 0;
static void InformSuccess(void)
{
	uint16_t at_least=5;
	
	Dummy.SuccessPid= SPid;		//�հ�����s_pid
	
	Dummy.Crc = CRCCal((uint8_t*)&Dummy, sizeof(DummyDef)-2);	//���crc
	while(at_least--)	//���ٷ�5��
	{
		SendPkt((uint8_t*)&Dummy,sizeof(DummyDef));	//���Ϳհ�	
//		delay_1ms(5);	
	}
}




static void ChannelHop(void)
{
	index=(index+1)%CHANNEL_AMOUNT;	//��һ��Ƶ������
	
	//���÷���Ƶ��
	Channel = ChannelList[index];	//��Ƶ
	
	DataArrayChannelSet();			//�������ݰ�Ƶ��
	
	Dummy.Channel = Channel;		//���ÿհ���Ƶ��
	
	Dummy.Crc = CRCCal((uint8_t*)&Dummy, sizeof(DummyDef)-2);	//���crc
	
}


static void DataArrayChannelSet(void)
{
	uint8_t i;
	for(i=0;i<Total;i++)
	{
		ArrayForSend[i].Channel = Channel;
		
		/*����CRC*/
		ArrayForSend[i].Crc = CRCCal((uint8_t*)&ArrayForSend[i],sizeof(DataSendDef)-2);		//���crc			
	}
}


static void ReadSrcDest(void)
{
	uint32_t addr;
	BSPEepromRead(FLASH_SRC_DEST,&addr,1);	//��һ����
	
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
	BSPEepromRead(FLASH_LAST_SPID,&s_pid,1);	//��һ����
	
	return (uint8_t)s_pid;
}



static uint8_t GetPid()
{
	uint32_t pid;
	BSPEepromRead(FLASH_LAST_PID,&pid,1);	//��һ����
	
	return (uint8_t)pid;
}



void LT8920SaveSrcDest(void)
{
	//����16λ������ƴ��32λ�֣���λsrc����dest
	uint32_t com_addr=SrcAddr;	
	com_addr <<=16;
	com_addr |= DestAddr;
	
	BSPEepromWrite(FLASH_SRC_DEST,&com_addr,1);	//дһ����
}

