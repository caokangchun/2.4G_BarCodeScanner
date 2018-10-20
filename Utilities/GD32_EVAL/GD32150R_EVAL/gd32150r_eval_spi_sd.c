/**
  ******************************************************************************
  * @file    gd32150r_eval_spi_sd.c
  * @author  MCU SD
  * @version V1.0.1
  * @date    6-Sep-2014
  * @brief   This file provides a set of functions needed to manage the SPI SD 
  *          Card memory mounted on GD32150R-EVAL board.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32150r_eval_spi_sd.h"

/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup GD32_EVAL
  * @{
  */ 

/** @addtogroup GD32150R_EVAL
  * @{
  */
  
/** @addtogroup GD32150R_EVAL_SPI_SD
  * @brief      This file includes the SD card driver of GD32-EVAL boards.
  * @{
  */

/* Private function prototypes -----------------------------------------------*/
static SD_Error SD_Select    (void);
static SD_Error SD_RecvData  (uint8_t *pbuf, uint16_t Len);
static SD_Error SD_SendCmd   (uint8_t Cmd, uint32_t Arg, uint8_t Crc);
static SD_Error SD_SendBlock (uint8_t *pBuffer, uint8_t Cmd);

/** @defgroup GD32150R_EVAL_SPI_SD_Private_Functions
  * @{
  */
uint8_t  SD_Type = 0;

/**
  * @brief  De-Initializes the SD/SD communication.
  * @param  None
  * @retval None
  */
void SD_DeInit (void)
{
    SD_LowLevel_DeInit();
}

/**
  * @brief  Initializes the SD/SD communication.
  * @param  None
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_Initialize(void)
{
    uint8_t Re = 0x00;
    uint8_t buf[4];
    uint16_t i;
    uint16_t Retry;

    SD_LowLevel_Init();

    SD_SPI_ReadWriteByte(SD_DUMMY_BYTE);

    SD_CS_HIGH();

    /* Send dummy byte 0xFF, 10 times with CS high */
    /* Rise CS and MOSI for 80 clocks cycles */
    for(i = 0; i < 10; i++)
    {
        SD_SPI_ReadWriteByte(SD_DUMMY_BYTE);
    }

    for(Retry = 20; (Re != 0x01) && Retry; Retry --)
    {
        /* Go to Idle state */
        Re = SD_SendCmd(SD_CMD_GO_IDLE_STATE, 0, 0x95);
    }

    SD_Type = 0;

    if(Re == 0x01)
    {
        if(SD_SendCmd(SD_CMD_SEND_IF_COND, 0x1AA, 0x87) == 1)//SD V2.0
        {
            for(i = 0; i < 4; i++)
            {
                /* Get trailing return value of R7 resp */
                buf[i] = SD_SPI_ReadWriteByte(SD_DUMMY_BYTE);
            }

            /* Check the card support 2.7~3.6V */
            if(buf[2] == 0X01 && buf[3] == 0xaa)
            {
                for(Retry = 0xfffe; Re && Retry; Retry --)
                {
                    /* Send CMD55 */
                    SD_SendCmd(SD_CMD55, 0, 0x01);

                    /* Send CMD41 */
                    Re = SD_SendCmd(SD_CMD41, 0x40000000, 0x01);
                }

                /* Check version start from SD2.0 */
                if(Retry && SD_SendCmd(SD_CMD_READ_OCR, 0, 0x01) == 0)
                {
                    for(i = 0; i < 4; i++)
                    {
                        /* Get OCR value */
                        buf[i] = SD_SPI_ReadWriteByte(SD_DUMMY_BYTE);
                    }

                    /* Check CCS */
                    if(buf[0] & 0x40)
                        SD_Type = SD_TYPE_V2HC;
                    else
                        SD_Type=SD_TYPE_V2;
                }
            }
        }
        else//SD V1.x/ MMC  V3
        {

            SD_SendCmd(SD_CMD55, 0, 0x01);
            Re = SD_SendCmd(SD_CMD41, 0, 0x01);

            if(Re <= 1)
            {
                SD_Type = SD_TYPE_V1;

                for(Retry = 0xfffe; Re && Retry; Retry --)
                {
                    /* Send CMD55 */
                    SD_SendCmd(SD_CMD55, 0, 0x01);

                    /* Send CMD41 */
                    Re = SD_SendCmd(SD_CMD41, 0x40000000, 0x01);
                }
            }
            else
            {
                SD_Type = SD_TYPE_MMC;//MMC V3

                /* Wait for go out of idle mode */
                for(Retry = 0xfffe; Re && Retry; Retry --)
                {
                    /* Send CMD1 */
                    Re = SD_SendCmd(SD_CMD_SEND_OP_COND, 0, 0x01);
                } 
            }

            if(Retry == 0 || SD_SendCmd(SD_CMD_SET_BLOCKLEN, 512, 0x01) != 0)
                SD_Type = SD_TYPE_ERR;
        }
    }

    SD_CS_HIGH();

    SPI1->CTLR1 &= 0xffc7;
    SPI1->CTLR1 |= SPI_PSC_4;
    SPI_Enable(SPI1, ENABLE);

    if(SD_Type)
        return SD_RESPONSE_NO_ERROR;
    else if(Re)
        return (SD_Error)Re;

    return SD_RESPONSE_FAILURE;
}

/**
  * @brief  Reads data from the SD
  * @param  pBuffer: pointer to the buffer that receives the data read from the SD
  * @param  SectorNumber: sector number
  * @param  SectorCount: sector count
  * @retval The SD Response:
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_ReadDisk(uint8_t *pBuffer, uint32_t SectorNumber, uint8_t SectorCount)
{
    uint8_t Re;

    if(SD_Type != SD_TYPE_V2HC) SectorNumber <<= 9;

    if(SectorCount == 1)
    {
        /* Send read command */
        Re = SD_SendCmd(SD_CMD_READ_SINGLE_BLOCK, SectorNumber, 0x01);

        if(Re == 0)
        {
            /* Receive 512 bytes */
            Re = SD_RecvData(pBuffer, 512);
        }
    }
    else
    {
        Re = SD_SendCmd(SD_CMD_READ_MULT_BLOCK, SectorNumber, 0x01);

        do
        {
            Re = SD_RecvData(pBuffer, 512);

            pBuffer += 512;
        }while(--SectorCount && Re == 0);

        /* Send stop command */
        SD_SendCmd(SD_CMD_STOP_TRANSMISSION, 0, 0x01);
    }

    SD_CS_HIGH();

    return (SD_Error)Re;
}

/**
  * @brief  Write data on the SD
  * @param  pBuffer: pointer to the buffer containing the data to be written on the SD
  * @param  SectorNumber: address to write on.
  * @param  SectorCount: the SD card Data block size.
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_WriteDisk(uint8_t *pBuffer, uint32_t SectorNumber, uint8_t SectorCount)
{
    uint8_t Re;

    if(SD_Type != SD_TYPE_V2HC) SectorNumber *= 512;

    if(SectorCount == 1)
    {
        /* Send read command */
        Re = SD_SendCmd(SD_CMD_WRITE_SINGLE_BLOCK, SectorNumber, 0x01);

        if(Re == 0)
        {
            /* Write 512 bytes */
            Re = SD_SendBlock(pBuffer, 0xfe);
        }
    }
    else
    {
        if(SD_Type != SD_TYPE_MMC)
        {
            SD_SendCmd(SD_CMD55, 0, 0x01);
            SD_SendCmd(SD_CMD_SET_BLOCK_COUNT, SectorCount, 0x01);
        }

        Re = SD_SendCmd(SD_CMD_WRITE_MULT_BLOCK, SectorNumber, 0x01);

        if(Re == 0)
        {
            do
            {
                Re = SD_SendBlock(pBuffer, 0xfc);

                pBuffer += 512;
            }while(--SectorCount && Re == 0);

            /* Receive 512 bytes */
            Re = SD_SendBlock(0,0xfd);
        }
    }

    SD_CS_HIGH();

    return (SD_Error)Re;
}

/**
  * @brief  Get the CSD information from SD card, include capacity and speed information
  * @param  csd_data: pointer to csd information
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_GetCSD(uint8_t *csd_data)
{
    uint8_t Re;

    /* Send CMD9 command, read CSD */
    Re = SD_SendCmd(SD_CMD_SEND_CSD,0,0x01);

    if(Re == 0)
    {
        /* Receive 16 bytes data */
        Re = SD_RecvData(csd_data, 16);
    }

    SD_CS_HIGH();

    if(Re)
        return (SD_Error)Re;
    else
        return SD_RESPONSE_NO_ERROR;
}  

/**
  * @brief  Get the CID information from SD card, include manufacture information
  * @param  cid_data: pointer to cid information
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_GetCID(uint8_t *cid_data)
{
    uint8_t Re;

    /* Send CMD10 command, read CID */
    Re = SD_SendCmd(SD_CMD_SEND_CID, 0, 0x01);

    if(Re == 0x00)
    {
        /* Receive 16 bytes data */
        Re = SD_RecvData(cid_data, 16);
    }

    SD_CS_HIGH();

    if(Re)
        return (SD_Error)Re;
    else
        return SD_RESPONSE_NO_ERROR;
}

/**
  * @brief  Wait for SD card's response
  * @param  Response: the respons which want to get
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_GetResponse(uint8_t Response)
{
    uint16_t Count = 0xffff;

    while ((SD_SPI_ReadWriteByte(0xff) != Response) && Count)
    {
        /* Wait for right response */
        Count--;
    }

    if (Count == 0)
        return SD_RESPONSE_FAILURE;
    else
        return SD_RESPONSE_NO_ERROR;
}

/**
  * @brief  Read/Write a byte on the SD by SPIx
  * @param  TxData: data byte to send.
  * @retval The received byte.
  */
uint8_t SD_SPI_ReadWriteByte(uint8_t TxData)
{
    uint8_t Retry = 0;

    while (SPI_I2S_GetBitState(SPI1, SPI_FLAG_TBE) == RESET)
    {
        Retry++;

        if(Retry > 200)return 0;
    }

    SPI_I2S_SendData(SPI1, TxData);
    Retry = 0;

    while (SPI_I2S_GetBitState(SPI1, SPI_FLAG_RBNE) == RESET)
    {
        Retry++;

        if(Retry > 200)return 0;
    }

    return SPI_I2S_ReceiveData(SPI1);
}

/**
  * @brief  Wait for SD card is ready
  * @param  None
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_WaitReady(void)
{
    uint32_t i = 0;

    do
    {
        if(SD_SPI_ReadWriteByte(0xff) == 0xff) return SD_RESPONSE_NO_ERROR;
        i++;
    }while(i < 0xffffff);

    return SD_RESPONSE_FAILURE;
}

/**
  * @brief  Get sector count
  * @param  None
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
uint32_t SD_GetSectorCount(void)
{
    uint8_t n;
    uint8_t csd[16];
    uint16_t csize;
    uint32_t Capacity;

    if(SD_GetCSD(csd) != 0) return 0;

    /* SDHC card */
    if((csd[0] & 0xc0) == 0x40) //V2.00
    {
        csize = csd[9] + ((uint16_t)csd[8] << 8) + 1;
        Capacity = (uint32_t)csize << 10;
    }
    else//V1.XX
    {
        n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
        csize = (csd[8] >> 6) + ((uint16_t)csd[7] << 2) + ((uint16_t)(csd[6] & 3) << 10) + 1;
        Capacity = (uint32_t)csize << (n - 9);
    }

    return Capacity;
}

/**
  * @brief  Select SD card and wait for it is ready
  * @param  None
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
static SD_Error SD_Select(void)
{
    SD_CS_LOW();

    if(SD_WaitReady() == 0) return SD_RESPONSE_NO_ERROR;

    SD_CS_HIGH();

    return SD_RESPONSE_FAILURE;
}

/**
  * @brief  Read a data packet from SD card
  * @param  pbuf: pointer to data buffer
  * @param  Len: data length
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
static SD_Error SD_RecvData(uint8_t *pbuf, uint16_t Len)
{
    /* Wait for SD card send start token 0xfe */
    if(SD_GetResponse(0xfe)) return SD_RESPONSE_FAILURE;

    while(Len--)
    {
        *pbuf = SD_SPI_ReadWriteByte(0xff);
        pbuf++;
    }

    /* 2 dummy CRC */
    SD_SPI_ReadWriteByte(0xff);
    SD_SPI_ReadWriteByte(0xff);

    return SD_RESPONSE_NO_ERROR;
}

/**
  * @brief  Send a command to SD card
  * @param  Cmd: command
  * @param  Arg: command parameter
  * @param  Crc: CRC value
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
static SD_Error SD_SendCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc)
{
    uint8_t Re;
    uint8_t Retry = 0;

    SD_CS_HIGH();

    if(SD_Select()) return SD_RESPONSE_FAILURE;

    SD_SPI_ReadWriteByte(Cmd | 0x40);
    SD_SPI_ReadWriteByte(Arg >> 24);
    SD_SPI_ReadWriteByte(Arg >> 16);
    SD_SPI_ReadWriteByte(Arg >> 8);
    SD_SPI_ReadWriteByte(Arg);
    SD_SPI_ReadWriteByte(Crc); 

    if(Cmd == SD_CMD_STOP_TRANSMISSION)
    {
        /* Skip a stuff byte when stop reading */
        SD_SPI_ReadWriteByte(0xff);
    }

    /* Wait for response or exit in time-out */
    Retry = 0x1f;

    do
    {
        Re = SD_SPI_ReadWriteByte(0xff);
    }while((Re & 0x80) && Retry--);

    return (SD_Error)Re;
}

/**
  * @brief  Write a data packet(512 bytes) to the SD
  * @param  pBuffer: pointer to the buffer that the data write to the SD
  * @param  Cmd: command
  * @retval The SD Response:
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
static SD_Error SD_SendBlock(uint8_t *pBuffer, uint8_t Cmd)
{
    uint16_t i;

    if(SD_WaitReady())  return SD_RESPONSE_FAILURE;

    SD_SPI_ReadWriteByte(Cmd);

    if(Cmd != 0xfd)
    {
        for(i = 0; i < 512; i++)
        {
            SD_SPI_ReadWriteByte(pBuffer[i]);
        }

        /* Ignore CRC */
        SD_SPI_ReadWriteByte(0xff);
        SD_SPI_ReadWriteByte(0xff);

        if((SD_SPI_ReadWriteByte(0xff) & 0x1f) != 0x05) return SD_RESPONSE_FAILURE;
    }

    return SD_RESPONSE_NO_ERROR;
}


/**
  * @}
  */


/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */  

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
