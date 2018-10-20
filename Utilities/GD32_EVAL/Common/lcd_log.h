/**
  ******************************************************************************
  * @file    lcd_log.h
  * @author  MCU SD
  * @version V1.0.1
  * @date    6-Sep-2014
  * @brief   header for the lcd_log.c file
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  __GD32F1X0_LCD_LOG_H
#define  __GD32F1X0_LCD_LOG_H

/* Includes ------------------------------------------------------------------*/
#include "gd32150r_eval_lcd.h"

/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup GD32_EVAL
  * @{
  */

/** @addtogroup Common
  * @{
  */

/** @addtogroup LCD_LOG
  * @{
  */

/** @defgroup LCD_LOG_Exported_FunctionsPrototype
  * @{
  */
void LCD_LOG_Init           (void);
void LCD_LOG_DeInit         (void);
void LCD_LOG_SetHeader      (uint8_t *pTitle, uint16_t StartX);
void LCD_LOG_SetFooter      (uint8_t *pStatus, uint16_t StartX);

void LCD_LOG_Print          (uint8_t *pStr, 
                             uint16_t *pLine,
                             uint16_t CharColor, 
                             uint16_t BackColor);

void LCD_LOG_ClearTextZone  (uint16_t StartX,
                             uint16_t StartY,
                             uint16_t EndX,
                             uint16_t EndY);

/**
  * @}
  */ 


#endif /* __GD32F1X0_LCD_LOG_H */

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
