/**
  ******************************************************************************
  * @file    gd32150r_eval.h
  * @author  MCU SD
  * @version V1.0.1
  * @date    6-Sep-2014
  * @brief   GD32150R_EVAL's Leds, Keys and COM ports hardware resources defines.
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F1X0_GD32F150R_EVAL_H
#define __GD32F1X0_GD32F150R_EVAL_H

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

/** @addtogroup GD32150R_EVAL
  * @{
  */
      
/** @addtogroup GD32150R_EVAL_LOW_LEVEL
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/
typedef enum 
{
    LED1 = 0,
    LED2 = 1,
    LED3 = 2,
    LED4 = 3
} 
Led_TypeDef;

typedef enum 
{
    KEY_TAMPER = 0,
    KEY_RIGHT = 1,
    KEY_LEFT = 2,
    KEY_DOWN = 3
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
    MOUSE_RIGHT = 3
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
#define LEDn                             4

#define LED1_PIN                         GPIO_PIN_10
#define LED1_GPIO_PORT                   GPIOC
#define LED1_GPIO_CLK                    RCC_AHBPERIPH_GPIOC
  
#define LED2_PIN                         GPIO_PIN_11
#define LED2_GPIO_PORT                   GPIOC
#define LED2_GPIO_CLK                    RCC_AHBPERIPH_GPIOC
  
#define LED3_PIN                         GPIO_PIN_12
#define LED3_GPIO_PORT                   GPIOC
#define LED3_GPIO_CLK                    RCC_AHBPERIPH_GPIOC
  
#define LED4_PIN                         GPIO_PIN_2
#define LED4_GPIO_PORT                   GPIOD
#define LED4_GPIO_CLK                    RCC_AHBPERIPH_GPIOD

/**
  * @}
  */ 

/** @addtogroup GD32150R_EVAL_LOW_LEVEL_BUTTON
  * @{
  */  
#define KEYn                             4

/**
 * @brief Tamper push-button
 */
#define TAMPER_KEY_PIN                   GPIO_PIN_13
#define TAMPER_KEY_GPIO_PORT             GPIOC
#define TAMPER_KEY_GPIO_CLK              RCC_AHBPERIPH_GPIOC
#define TAMPER_KEY_EXTI_LINE             EXTI_LINE13
#define TAMPER_KEY_EXTI_PORT_SOURCE      EXTI_SOURCE_GPIOC
#define TAMPER_KEY_EXTI_PIN_SOURCE       EXTI_SOURCE_PIN13
#define TAMPER_KEY_EXTI_IRQn             EXTI4_15_IRQn 

/**
 * @brief Right push-button
 */
#define RIGHT_KEY_PIN                    GPIO_PIN_13
#define RIGHT_KEY_GPIO_PORT              GPIOC
#define RIGHT_KEY_GPIO_CLK               RCC_AHBPERIPH_GPIOC
#define RIGHT_KEY_EXTI_LINE              EXTI_LINE13
#define RIGHT_KEY_EXTI_PORT_SOURCE       EXTI_SOURCE_GPIOC
#define RIGHT_KEY_EXTI_PIN_SOURCE        EXTI_SOURCE_PIN13
#define RIGHT_KEY_EXTI_IRQn              EXTI4_15_IRQn

/**
 * @brief Left push-button
 */
#define LEFT_KEY_PIN                     GPIO_PIN_0
#define LEFT_KEY_GPIO_PORT               GPIOA
#define LEFT_KEY_GPIO_CLK                RCC_AHBPERIPH_GPIOA
#define LEFT_KEY_EXTI_LINE               EXTI_LINE0
#define LEFT_KEY_EXTI_PORT_SOURCE        EXTI_SOURCE_GPIOA
#define LEFT_KEY_EXTI_PIN_SOURCE         EXTI_SOURCE_PIN0
#define LEFT_KEY_EXTI_IRQn               EXTI0_1_IRQn  

/**
 * @brief Down push-button
 */  
#define DOWN_KEY_PIN                     GPIO_PIN_7
#define DOWN_KEY_GPIO_PORT               GPIOF
#define DOWN_KEY_GPIO_CLK                RCC_AHBPERIPH_GPIOF
#define DOWN_KEY_EXTI_LINE               EXTI_LINE7
#define DOWN_KEY_EXTI_PORT_SOURCE        EXTI_SOURCE_GPIOF
#define DOWN_KEY_EXTI_PIN_SOURCE         EXTI_SOURCE_PIN7
#define DOWN_KEY_EXTI_IRQn               EXTI4_15_IRQn

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
  * @brief  SD SPI Interface pins
  */
#define SD_SPI                           SPI1
#define SD_SPI_CLK                       RCC_APB2PERIPH_SPI1

#define SD_SPI_SCK_PIN                   GPIO_PIN_3                   /* PB.03 */
#define SD_SPI_SCK_GPIO_PORT             GPIOB                        /* GPIOB */
#define SD_SPI_SCK_SOURCE                GPIO_PINSOURCE3
#define SD_SPI_SCK_GPIO_CLK              RCC_AHBPERIPH_GPIOB

#define SD_SPI_MISO_PIN                  GPIO_PIN_4                   /* PB.04 */
#define SD_SPI_MISO_GPIO_PORT            GPIOB                        /* GPIOB */
#define SD_SPI_MISO_SOURCE               GPIO_PINSOURCE4
#define SD_SPI_MISO_GPIO_CLK             RCC_AHBPERIPH_GPIOB

#define SD_SPI_MOSI_PIN                  GPIO_PIN_5                   /* PB.05 */
#define SD_SPI_MOSI_GPIO_PORT            GPIOB                        /* GPIOB */
#define SD_SPI_MOSI_SOURCE               GPIO_PINSOURCE5
#define SD_SPI_MOSI_GPIO_CLK             RCC_AHBPERIPH_GPIOB

#define SD_CS_PIN                        GPIO_PIN_4                   /* PF.04 */
#define SD_CS_GPIO_PORT                  GPIOF                        /* GPIOF */
#define SD_CS_SOURCE                     GPIO_PINSOURCE4
#define SD_CS_GPIO_CLK                   RCC_AHBPERIPH_GPIOF

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
void SD_LowLevel_DeInit       (void);
void SD_LowLevel_Init         (void);

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __GD32F1x0_GD32F150R_EVAL_H */
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
