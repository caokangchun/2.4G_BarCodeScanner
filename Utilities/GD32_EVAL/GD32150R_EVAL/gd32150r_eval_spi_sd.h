/**
  ******************************************************************************
  * @file    gd32150r_eval_spi_sd.h
  * @author  MCU SD
  * @version V1.0.1
  * @date    6-Sep-2014
  * @brief   This file contains all the functions prototypes for the 
  *          gd32150r_eval_spi_sd firmware driver.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32150R_EVAL_SPI_SD_H
#define __GD32150R_EVAL_SPI_SD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gd32150r_eval.h"

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
  * @{
  */  

/** @defgroup GD32150R_EVAL_SPI_SD_Exported_Types
  * @{
  */
extern uint32_t csd2size;
extern uint8_t SD_Type;

typedef enum
{
/**
  * @brief  SD reponses and error flags
  */
    SD_RESPONSE_NO_ERROR      = (0x00),
    SD_IN_IDLE_STATE          = (0x01),
    SD_ERASE_RESET            = (0x02),
    SD_ILLEGAL_COMMAND        = (0x04),
    SD_COM_CRC_ERROR          = (0x08),
    SD_ERASE_SEQUENCE_ERROR   = (0x10),
    SD_ADDRESS_ERROR          = (0x20),
    SD_PARAMETER_ERROR        = (0x40),
    SD_RESPONSE_FAILURE       = (0xFF),

/**
  * @brief  Data response error
  */
    SD_DATA_OK                = (0x05),
    SD_DATA_CRC_ERROR         = (0x0B),
    SD_DATA_WRITE_ERROR       = (0x0D),
    SD_DATA_OTHER_ERROR       = (0xFF)
} SD_Error;

/**
  * @}
  */
  
/** @defgroup GD32150R_EVAL_SPI_SD_Exported_Constants
  * @{
  */ 
    
/**
  * @brief  Block Size
  */
#define SD_BLOCK_SIZE    0x200

/**
  * @brief  Dummy byte
  */
#define SD_DUMMY_BYTE   0xFF

/* SD card type define */
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06

/**
  * @brief  Start Data tokens:
  *         Tokens (necessary because at nop/idle (and CS active) only 0xff is 
  *         on the data/command line)  
  */
#define SD_START_DATA_SINGLE_BLOCK_READ    0xFE  /*!< Data token start byte, Start Single Block Read */
#define SD_START_DATA_MULTIPLE_BLOCK_READ  0xFE  /*!< Data token start byte, Start Multiple Block Read */
#define SD_START_DATA_SINGLE_BLOCK_WRITE   0xFE  /*!< Data token start byte, Start Single Block Write */
#define SD_START_DATA_MULTIPLE_BLOCK_WRITE 0xFD  /*!< Data token start byte, Start Multiple Block Write */
#define SD_STOP_DATA_MULTIPLE_BLOCK_WRITE  0xFD  /*!< Data toke stop byte, Stop Multiple Block Write */


/**
  * @brief  Commands: CMDxx = CMD-number | 0x40
  */
#define SD_CMD_GO_IDLE_STATE          0   /*!< CMD0 = 0x40 */
#define SD_CMD_SEND_OP_COND           1   /*!< CMD1 = 0x41 */
#define SD_CMD_SEND_IF_COND           8   /*!< CMD8 = 0x48 */
#define SD_CMD_SEND_CSD               9   /*!< CMD9 = 0x49 */
#define SD_CMD_SEND_CID               10  /*!< CMD10 = 0x4A */
#define SD_CMD_STOP_TRANSMISSION      12  /*!< CMD12 = 0x4C */
#define SD_CMD_SEND_STATUS            13  /*!< CMD13 = 0x4D */
#define SD_CMD_SET_BLOCKLEN           16  /*!< CMD16 = 0x50 */
#define SD_CMD_READ_SINGLE_BLOCK      17  /*!< CMD17 = 0x51 */
#define SD_CMD_READ_MULT_BLOCK        18  /*!< CMD18 = 0x52 */
#define SD_CMD_SET_BLOCK_COUNT        23  /*!< CMD23 = 0x57 */
#define SD_CMD_WRITE_SINGLE_BLOCK     24  /*!< CMD24 = 0x58 */
#define SD_CMD_WRITE_MULT_BLOCK       25  /*!< CMD25 = 0x59 */
#define SD_CMD_PROG_CSD               27  /*!< CMD27 = 0x5B */
#define SD_CMD_SET_WRITE_PROT         28  /*!< CMD28 = 0x5C */
#define SD_CMD_CLR_WRITE_PROT         29  /*!< CMD29 = 0x5D */
#define SD_CMD_SEND_WRITE_PROT        30  /*!< CMD30 = 0x5E */
#define SD_CMD_SD_ERASE_GRP_START     32  /*!< CMD32 = 0x60 */
#define SD_CMD_SD_ERASE_GRP_END       33  /*!< CMD33 = 0x61 */
#define SD_CMD_UNTAG_SECTOR           34  /*!< CMD34 = 0x62 */
#define SD_CMD_ERASE_GRP_START        35  /*!< CMD35 = 0x63 */
#define SD_CMD_ERASE_GRP_END          36  /*!< CMD36 = 0x64 */
#define SD_CMD_UNTAG_ERASE_GROUP      37  /*!< CMD37 = 0x65 */
#define SD_CMD_ERASE                  38  /*!< CMD38 = 0x66 */
#define SD_CMD41                      41  /*!< CMD41 = 0x69 */
#define SD_CMD55                      55  /*!< CMD55 = 0x77 */
#define SD_CMD_READ_OCR               58  /*!< CMD58 = 0x7A */
#define SD_CMD_EN_DISABLE_CRC         59  /*!< CMD59 = 0x7B */

/**
  * @}
  */ 
  
/** @defgroup GD32150R_EVAL_SPI_SD_Exported_Macros
  * @{
  */
/** 
  * @brief  Select SD Card: ChipSelect pin low   
  */  
#define SD_CS_LOW()     GPIO_ResetBits(SD_CS_GPIO_PORT, SD_CS_PIN)

/** 
  * @brief  Deselect SD Card: ChipSelect pin high   
  */ 
#define SD_CS_HIGH()    GPIO_SetBits(SD_CS_GPIO_PORT, SD_CS_PIN)

/**
  * @}
  */ 

/** @defgroup GD32150R_EVAL_SPI_SD_Exported_Functions
  * @{
  */
void  SD_DeInit               (void);

uint8_t SD_SPI_ReadWriteByte  (uint8_t TxData);

SD_Error SD_Initialize        (void);
SD_Error SD_GetCID            (uint8_t *cid_data);
SD_Error SD_GetCSD            (uint8_t *csd_data);
SD_Error SD_WaitReady         (void);
SD_Error SD_GetResponse       (uint8_t Response);
SD_Error SD_ReadDisk          (uint8_t *pBuffer, uint32_t SectorNumber, uint8_t SectorCount);
SD_Error SD_WriteDisk         (uint8_t *pBuffer, uint32_t sectorNumber, uint8_t SectorCount);

uint32_t SD_GetSectorCount    (void);


#ifdef __cplusplus
}
#endif

#endif /* __GD32150R_EVAL_SPI_SD_H */
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
