/**
  ******************************************************************************
  * @file    gd32150g_start.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    6-Sep-2014
  * @brief   GD32150R_EVAL's Leds, Keys and COM ports hardware resources defines.
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F1X0_GD32F150G_START_H
#define __GD32F1X0_GD32F150G_START_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gd32f1x0.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup GD32_EVAL
  * @{
  */

/** @addtogroup GD32150G_START
  * @{
  */
      
/** @addtogroup GD32150G_START_LOW_LEVEL
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/
typedef enum 
{
    LED1 = 0
} 
Led_TypeDef;

typedef enum 
{
    KEY_RIGHT = 0
}
Key_TypeDef;

typedef enum 
{
    KEY_MODE_GPIO = 0,
    KEY_MODE_EXTI = 1
}
KeyMode_TypeDef;

typedef enum 
{
    MOUSE_NONE = 0,
    MOUSE_DOWN = 1,
    MOUSE_LEFT = 2,
    MOUSE_RIGHT = 3,
    MOUSE_UP = 4,
}
MOUSEState_TypeDef;  

/* Exported constants --------------------------------------------------------*/

/** @defgroup GD32150R_EVAL_LOW_LEVEL_Exported_Constants
  * @{
  */ 

/** 
  * @brief  Define for GD32150R_EVAL board  
  */

/** @addtogroup GD32150R_EVAL_LOW_LEVEL_LED
  * @{
  */
#define LEDn                             1

#define LED1_PIN                         GPIO_PIN_1
#define LED1_GPIO_PORT                   GPIOA
#define LED1_GPIO_CLK                    RCC_AHBPERIPH_GPIOA

/**
  * @}
  */

/** @addtogroup GD32150R_EVAL_LOW_LEVEL_BUTTON
  * @{
  */  
#define KEYn                             1

/**
 * @brief User push-button
 */
#define USER_KEY_PIN                     GPIO_PIN_0
#define USER_KEY_GPIO_PORT               GPIOA
#define USER_KEY_GPIO_CLK                RCC_AHBPERIPH_GPIOA
#define USER_KEY_EXTI_LINE               EXTI_LINE0
#define USER_KEY_EXTI_PORT_SOURCE        EXTI_SOURCE_GPIOA
#define USER_KEY_EXTI_PIN_SOURCE         EXTI_SOURCE_PIN0
#define USER_KEY_EXTI_IRQn               EXTI0_1_IRQn 

/**
 * @brief Right push-button
 */
#define RIGHT_KEY_PIN                    GPIO_PIN_0
#define RIGHT_KEY_GPIO_PORT              GPIOA
#define RIGHT_KEY_GPIO_CLK               RCC_AHBPERIPH_GPIOA
#define RIGHT_KEY_EXTI_LINE              EXTI_LINE0
#define RIGHT_KEY_EXTI_PORT_SOURCE       EXTI_SOURCE_GPIOA
#define RIGHT_KEY_EXTI_PIN_SOURCE        EXTI_SOURCE_PIN0
#define RIGHT_KEY_EXTI_IRQn              EXTI0_1_IRQn

/**
  * @}
  */ 


/** @addtogroup GD32150R_EVAL_LOW_LEVEL_COM
  * @{
  */

/**
 * @brief Definition for COM port1, connected to USART1
 */ 
#define EVAL_COM1                        USART1
#define EVAL_COM1_CLK                    RCC_APB2PERIPH_USART1

#define EVAL_COM1_TX_PIN                 GPIO_PIN_9
#define EVAL_COM1_TX_GPIO_PORT           GPIOA
#define EVAL_COM1_TX_GPIO_CLK            RCC_AHBPERIPH_GPIOA
#define EVAL_COM1_TX_SOURCE              GPIO_PINSOURCE9
#define EVAL_COM1_TX_AF                  GPIO_AF_1

#define EVAL_COM1_RX_PIN                 GPIO_PIN_10
#define EVAL_COM1_RX_GPIO_PORT           GPIOA
#define EVAL_COM1_RX_GPIO_CLK            RCC_AHBPERIPH_GPIOA
#define EVAL_COM1_RX_SOURCE              GPIO_PINSOURCE10
#define EVAL_COM1_RX_AF                  GPIO_AF_1

#define EVAL_COM1_CTS_PIN                GPIO_PIN_3
#define EVAL_COM1_CTS_GPIO_PORT          GPIOD
#define EVAL_COM1_CTS_GPIO_CLK           RCC_AHBPERIPH_GPIOD
#define EVAL_COM1_CTS_SOURCE             GPIO_PINSOURCE3
#define EVAL_COM1_CTS_AF                 GPIO_AF_0

#define EVAL_COM1_RTS_PIN                GPIO_PIN_4
#define EVAL_COM1_RTS_GPIO_PORT          GPIOD
#define EVAL_COM1_RTS_GPIO_CLK           RCC_AHBPERIPH_GPIOD
#define EVAL_COM1_RTS_SOURCE             GPIO_PINSOURCE4
#define EVAL_COM1_RTS_AF                 GPIO_AF_0
   
#define EVAL_COM1_IRQn                   USART1_IRQn

/**
  * @}
  */


/** @defgroup GD32150R_EVAL_LOW_LEVEL_Exported_Functions
  * @{
  */
void GD_EVAL_LEDInit          (Led_TypeDef LedNum);
void GD_EVAL_LEDOn            (Led_TypeDef LedNum);
void GD_EVAL_LEDOff           (Led_TypeDef LedNum);
void GD_EVAL_LEDToggle        (Led_TypeDef LedNum);
void GD_EVAL_KeyInit          (Key_TypeDef KeyNum, KeyMode_TypeDef Key_Mode);
uint8_t GD_EVAL_KeyGetState   (Key_TypeDef KeyNum);
void GD_EVAL_COMInit          (USART_InitPara* USART_InitStruct);

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __GD32F1x0_GD32F150C_START_H */
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
