/**
  ******************************************************************************
  * @file    lcd_log.c
  * @author  MCU SD
  * @version V1.0.1
  * @date    6-Sep-2014
  * @brief   This file provides all the LCD Log firmware functions.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lcd_log.h"
#include "usbd_conf.h"
#include <string.h>

#ifdef USE_LCD_LOG

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup GD32_EVAL
  * @{
  */

/** @addtogroup Common
  * @{
  */

/** @defgroup LCD_LOG
  * @{
  */

/** @defgroup LCD_LOG_Private_Functions
  * @{
  */

/**
  * @brief  Initialize the LCD Log module
  * @param  None
  * @retval None
  */
void LCD_LOG_Init (void)
{
    LCD_Clear(BLACK);
}

/**
  * @brief  De-Initialize the LCD Log module
  * @param  None
  * @retval None
  */
void LCD_LOG_DeInit (void)
{
}

/**
* @brief  Display the application header (title) on the LCD screen 
* @param  pTitle: pointer to the string to be displayed
* @retval None
*/
void LCD_LOG_SetHeader (uint8_t *pTitle, uint16_t StartX)
{
    LCD_Rect(0, 0, 240, 40, BLUE);

    LCD_DrawFont_GBK16(StartX, 10, RED, BLUE, pTitle);

}

/**
  * @brief  Display the application footer (status) on the LCD screen 
  * @param  pStatus: pointer to the string to be displayed
  * @retval None
  */
void LCD_LOG_SetFooter (uint8_t *pStatus, uint16_t StartX)
{
    LCD_Rect(0, 280, 240, 320, BLUE);

    LCD_DrawFont_GBK16(StartX, 290, RED, BLUE, pStatus);

}

/**
  * @brief  Clear the Text Zone 
  * @param  None 
  * @retval None
  */
void LCD_LOG_ClearTextZone(uint16_t StartX,
                            uint16_t StartY,
                            uint16_t EndX,
                            uint16_t EndY)
{
    LCD_Rect(StartX, StartY, EndX, EndY, BLACK);
}

/**
  * @brief  Redirect the printf to the lcd 
  * @param  pStr: pointer to string to be displayed
  * @param  Len: string length
  * @retval None
  */
void LCD_LOG_Print (uint8_t *pStr, 
                    uint16_t *pLine, 
                    uint16_t CharColor, 
                    uint16_t BackColor)
{
    if(*pLine >= 280)
    {
        LCD_Rect(0, 40, 240, 280, BLACK);
    }

    LCD_DrawFont_GBK16(2, *pLine, CharColor, BackColor, pStr);

    *pLine += 20;
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

#endif

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
