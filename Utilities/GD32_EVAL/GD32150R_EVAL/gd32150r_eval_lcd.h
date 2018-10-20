/**
  ******************************************************************************
  * @file    gd32150r_eval_lcd.h
  * @author  MCU SD
  * @version V1.0.1
  * @date    6-Sep-2014
  * @brief   This file contains all the functions prototypes for the lcd firmware driver.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32150R_EVAL_LCD_H
#define __GD32150R_EVAL_LCD_H

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
    
/** @addtogroup GD32150R_EVAL_LCD
  * @{
  */

/** @defgroup GD32150R_EVAL_LCD_Exported_Constants
  * @{
  */

/* Colors */
#define RED             0xf800
#define GREEN           0x07e0
#define BLUE            0x001f
#define WHITE           0xffff
#define BLACK           0x0000
#define YELLOW          0xFFE0

/* GRAYs */
#define GRAY0           0xEF7D
#define GRAY1           0x8410
#define GRAY2           0x4208

/* PF5 TFT CS */
#define LCD_CS_SET      GPIOF->BOR = GPIO_PIN_5
#define LCD_CS_CLR      GPIOF->BCR = GPIO_PIN_5

/* PC3 TFT RS/DC */
#define LCD_RS_SET      GPIOC->BOR = GPIO_PIN_3
#define LCD_RS_CLR      GPIOC->BCR = GPIO_PIN_3

/* PC4 TFT RST */
#define LCD_RST_SET     GPIOC->BOR = GPIO_PIN_4
#define LCD_RST_CLR     GPIOC->BCR = GPIO_PIN_4

/** @defgroup GD3210C_EVAL_LCD_Exported_Functions
  * @{
  */
void LCD_Clear          (uint16_t Color);
void LCD_SetXY          (uint16_t X, uint16_t Y);
void LCD_DrawPoint      (uint16_t X, uint16_t Y, uint16_t Data);

void LCD_SetRegion      (uint16_t StartX, 
                         uint16_t StartY, 
                         uint16_t EndX, 
                         uint16_t EndY);

void GD32150R_LCD_Init  (void);

uint16_t LCD_BGR2RGB    (uint16_t Color);

void LCD_DrawCircle     (uint16_t X, 
                         uint16_t Y, 
                         uint16_t R, 
                         uint16_t fc); 

void LCD_DrawLine       (uint16_t X0, 
                         uint16_t Y0, 
                         uint16_t X1, 
                         uint16_t Y1, 
                         uint16_t Color);

void LCD_Box            (uint16_t X, 
                         uint16_t Y, 
                         uint16_t W, 
                         uint16_t H, 
                         uint16_t bc);

void LCD_Box2           (uint16_t X, 
                         uint16_t Y, 
                         uint16_t W, 
                         uint16_t H, 
                         uint8_t  mode);

void LCD_Rect           (uint16_t X1, 
                         uint16_t Y1, 
                         uint16_t X2, 
                         uint16_t Y2, 
                         uint16_t fc);

void DisplayButtonDown  (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void DisplayButtonUp    (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

void LCD_DrawFont_GBK16 (uint16_t X, 
                         uint16_t Y, 
                         uint16_t fc, 
                         uint16_t bc, 
                         uint8_t *s);

void LCD_DrawFont_GBK24 (uint16_t X, 
                         uint16_t Y, 
                         uint16_t fc, 
                         uint16_t bc, 
                         uint8_t *s);

void LCD_DrawFont_Num32 (uint16_t X, 
                         uint16_t Y, 
                         uint16_t fc, 
                         uint16_t bc, 
                         uint16_t num);

#ifdef __cplusplus
}
#endif

#endif /* __GD3210C_EVAL_LCD_H */
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
