/**
  ******************************************************************************
  * @file    gd32150r_eval.c
  * @author  MCU SD
  * @version V1.0.1
  * @date    6-Sep-2014
  * @brief   Firmware functions to manage Leds, Keys, COM ports
  ******************************************************************************
  */
  
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
    
/** @defgroup GD32150R_EVAL_LOW_LEVEL
  * @brief This file provides firmware functions to manage Leds, Keys, COM ports
  * @{
  */ 

/* Private variables ---------------------------------------------------------*/
GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT, LED4_GPIO_PORT};
const uint16_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN};
const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK, LED3_GPIO_CLK, LED4_GPIO_CLK};

GPIO_TypeDef* KEY_PORT[KEYn] = {TAMPER_KEY_GPIO_PORT, RIGHT_KEY_GPIO_PORT,
                                LEFT_KEY_GPIO_PORT, DOWN_KEY_GPIO_PORT};

const uint16_t KEY_PIN[KEYn] = {TAMPER_KEY_PIN, RIGHT_KEY_PIN,
                                LEFT_KEY_PIN, DOWN_KEY_PIN};

const uint32_t KEY_CLK[KEYn] = {TAMPER_KEY_GPIO_CLK, RIGHT_KEY_GPIO_CLK,
                                LEFT_KEY_GPIO_CLK, DOWN_KEY_GPIO_CLK};

const uint16_t KEY_EXTI_LINE[KEYn] = {TAMPER_KEY_EXTI_LINE,
                                      RIGHT_KEY_EXTI_LINE,
                                      LEFT_KEY_EXTI_LINE,
                                      DOWN_KEY_EXTI_LINE};

const uint16_t KEY_PORT_SOURCE[KEYn] = {TAMPER_KEY_EXTI_PORT_SOURCE,
                                        RIGHT_KEY_EXTI_PORT_SOURCE,
                                        LEFT_KEY_EXTI_PORT_SOURCE,
                                        DOWN_KEY_EXTI_PORT_SOURCE};

const uint16_t KEY_PIN_SOURCE[KEYn] = {TAMPER_KEY_EXTI_PIN_SOURCE,
                                       RIGHT_KEY_EXTI_PIN_SOURCE,
                                       LEFT_KEY_EXTI_PIN_SOURCE,
                                       DOWN_KEY_EXTI_PIN_SOURCE};

const uint16_t KEY_IRQn[KEYn] = {TAMPER_KEY_EXTI_IRQn, RIGHT_KEY_EXTI_IRQn,
                                 LEFT_KEY_EXTI_IRQn, DOWN_KEY_EXTI_IRQn};

/* Private functions ---------------------------------------------------------*/ 

/** @defgroup GD32150R_EVAL_LOW_LEVEL_Private_Functions
  * @{
  */

/**
  * @brief  Configures LED GPIO
  * @param  LedNum: Specifies the Led which is in (0..3) to be configured
  * @retval None
  */
void  GD_EVAL_LEDInit (Led_TypeDef LedNum)
{
    GPIO_InitPara  GPIO_InitStructure;

    /* Enable the LED Clock */
    RCC_AHBPeriphClock_Enable(GPIO_CLK[LedNum], ENABLE);

    /* Configure the LED pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN[LedNum];
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OTYPE_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PUPD_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIO_PORT[LedNum], &GPIO_InitStructure);
    GPIO_PORT[LedNum]->BCR = GPIO_PIN[LedNum] ;
}

/**
  * @brief  Turn on selected LED 
  * @param  LedNum: Specifies the Led which is in (0..3) to be turn on
  * @retval None
  */
void GD_EVAL_LEDOn(Led_TypeDef LedNum)
{
    GPIO_PORT[LedNum]->BOR = GPIO_PIN[LedNum];
}

/**
  * @brief  Turn off selected LED
  * @param  Led: Specifies the Led which is in (0..3) to be turn off 
  * @retval None
  */
void GD_EVAL_LEDOff(Led_TypeDef LedNum)
{
    GPIO_PORT[LedNum]->BCR = GPIO_PIN[LedNum];
}

/**
  * @brief  Toggle the selected LED.
  * @param  Led: Specifies the Led is in (0..3) to be toggled
  * @retval None
  */
void GD_EVAL_LEDToggle(Led_TypeDef LedNum)
{
    GPIO_PORT[LedNum]->DOR ^= GPIO_PIN[LedNum];
}

/**
  * @brief  Configure Key
  * @param  KeyNum: Specifies the key to be configured.
  *         This parameter can be one of following parameters:
  *            @arg KEY_TAMPER: Tamper key
  *            @arg KEY_RIGHT: Right key 
  *            @arg KEY_LEFT: Left key 
  *            @arg KEY_DOWN: Down key
  * @param  Button_Mode: Specifies Button mode.
  *         This parameter can be one of following parameters:   
  *            @arg KEY_MODE_GPIO: key will be used as simple IO 
  *            @arg KEY_MODE_EXTI: Key will be connected to EXTI line with interrupt
  *                                generation capability
  * @retval None
  */
void GD_EVAL_KeyInit(Key_TypeDef KeyNum, KeyMode_TypeDef Key_Mode)
{
    GPIO_InitPara GPIO_InitStructure;
    EXTI_InitPara EXTI_InitStructure;
    NVIC_InitPara NVIC_InitStructure;

    /* Enable the KEY Clock */
    RCC_AHBPeriphClock_Enable(KEY_CLK[KeyNum], ENABLE);
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_CFG, ENABLE);

    /* Configure Button pin as input */
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PUPD_NOPULL;
    GPIO_InitStructure.GPIO_Pin = KEY_PIN[KeyNum];
    GPIO_Init(KEY_PORT[KeyNum], &GPIO_InitStructure);

    if (Key_Mode == KEY_MODE_EXTI)
    {
        /* Enable and set key EXTI Interrupt to the lowest priority */
        NVIC_InitStructure.NVIC_IRQ = KEY_IRQn[KeyNum];
        NVIC_InitStructure.NVIC_IRQPreemptPriority = 2;
        NVIC_InitStructure.NVIC_IRQSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
        NVIC_Init(&NVIC_InitStructure);

        /* Connect key EXTI Line to key GPIO Pin */
        SYSCFG_EXTILine_Config(KEY_PORT_SOURCE[KeyNum], KEY_PIN_SOURCE[KeyNum]);

        /* Configure key EXTI line */
        EXTI_InitStructure.EXTI_LINE = KEY_EXTI_LINE[KeyNum];
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;   
        EXTI_InitStructure.EXTI_LINEEnable = ENABLE;
        EXTI_Init(&EXTI_InitStructure);

        EXTI_ClearIntBitState(KEY_EXTI_LINE[KeyNum]);
    }
}

/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *         This parameter can be one of following parameters:
  *            @arg KEY_TAMPER: Tamper key
  *            @arg KEY_RIGHT: Right key
  *            @arg KEY_LEFT: Left key
  *            @arg KEY_DOWN: Down key
  * @retval The Button GPIO pin value.
  */
uint8_t GD_EVAL_KeyGetState(Key_TypeDef Button)
{
    return GPIO_ReadInputBit(KEY_PORT[Button], KEY_PIN[Button]);
}

/**
  * @brief  Configures COM port
  * @param  COM: Specifies the COM port to be configured
  *         This parameter can be one of following parameters:
  *              @arg COM1
  * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that
  *         contains the configuration information for the specified USART peripheral
  * @retval None
  */
void GD_EVAL_COMInit(USART_InitPara * USART_InitStruct)
{
    GPIO_InitPara GPIO_InitStructure;

    /* Enable GPIO clock */
    RCC_AHBPeriphClock_Enable(EVAL_COM1_TX_GPIO_CLK | EVAL_COM1_RX_GPIO_CLK, ENABLE);

    /* Enable USART clock */
    RCC_APB2PeriphClock_Enable(EVAL_COM1_CLK, ENABLE); 

    /* Connect PXx to USARTx_Tx */
    GPIO_PinAFConfig(EVAL_COM1_TX_GPIO_PORT, EVAL_COM1_TX_SOURCE, EVAL_COM1_TX_AF);

    /* Connect PXx to USARTx_Rx */
    GPIO_PinAFConfig(EVAL_COM1_RX_GPIO_PORT, EVAL_COM1_RX_SOURCE, EVAL_COM1_TX_AF);

    /* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = EVAL_COM1_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_10MHZ;
    GPIO_InitStructure.GPIO_OType = GPIO_OTYPE_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PUPD_PULLUP;
    GPIO_Init(EVAL_COM1_TX_GPIO_PORT, &GPIO_InitStructure);

    /* Configure USART Rx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = EVAL_COM1_RX_PIN;
    GPIO_Init(EVAL_COM1_RX_GPIO_PORT, &GPIO_InitStructure);

    /* USART configuration */
    USART_Init(EVAL_COM1, USART_InitStruct);

    /* Enable USART */
    USART_Enable(EVAL_COM1, ENABLE);
}

/**
  * @brief  DeInitializes the SD/SD communication.
  * @param  None
  * @retval None
  */
void SD_LowLevel_DeInit(void)
{
    GPIO_InitPara  GPIO_InitStructure;

    /* SD_SPI disable */
    SPI_Enable(SD_SPI, DISABLE);

    /* De-Initializes the SD_SPI */
    SPI_I2S_DeInit(SD_SPI);

    /* SD_SPI Periph clock disable */
    RCC_APB1PeriphClock_Enable(SD_SPI_CLK, DISABLE); 

    /* Configure SD_SPI pins: SCK */
    GPIO_InitStructure.GPIO_Pin = SD_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN;
    GPIO_Init(SD_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

    /* Configure SD_SPI pins: MISO */
    GPIO_InitStructure.GPIO_Pin = SD_SPI_MISO_PIN;
    GPIO_Init(SD_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

    /* Configure SD_SPI pins: MOSI */
    GPIO_InitStructure.GPIO_Pin = SD_SPI_MOSI_PIN;
    GPIO_Init(SD_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

    /* Configure SD_SPI_CS_PIN pin: SD Card CS pin */
    GPIO_InitStructure.GPIO_Pin = SD_CS_PIN;
    GPIO_Init(SD_CS_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  Initializes the SD_SPI and CS pins.
  * @param  None
  * @retval None
  */
void SD_LowLevel_Init(void)
{
    GPIO_InitPara  GPIO_InitStructure;
    SPI_InitPara   SPI_InitStructure;

    /* GPIO Periph clock enable */
    RCC_AHBPeriphClock_Enable(SD_CS_GPIO_CLK | SD_SPI_MOSI_GPIO_CLK | SD_SPI_MISO_GPIO_CLK | \
                              SD_SPI_SCK_GPIO_CLK, ENABLE);

    /* SD_SPI Periph clock enable */
    RCC_APB2PeriphClock_Enable(SD_SPI_CLK, ENABLE);

    /* Configure SCK MISO MOSI pins: SCK */
    GPIO_InitStructure.GPIO_Pin = SD_SPI_MOSI_PIN | SD_SPI_MISO_PIN | SD_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OTYPE_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PUPD_NOPULL;
    
    GPIO_Init(SD_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(SD_SPI_SCK_GPIO_PORT, SD_SPI_SCK_SOURCE, GPIO_AF_0);

    GPIO_Init(SD_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(SD_SPI_MOSI_GPIO_PORT, SD_SPI_MOSI_SOURCE, GPIO_AF_0);

    GPIO_Init(SD_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(SD_SPI_MISO_GPIO_PORT, SD_SPI_MISO_SOURCE, GPIO_AF_0);

    GPIO_InitStructure.GPIO_Pin = SD_CS_PIN;
    GPIO_Init(SD_CS_GPIO_PORT, &GPIO_InitStructure);

    /* SD_SPI Config */
    SPI_InitStructure.SPI_TransType = SPI_TRANSTYPE_FULLDUPLEX;
    SPI_InitStructure.SPI_Mode = SPI_MODE_MASTER;
    SPI_InitStructure.SPI_FrameFormat = SPI_FRAMEFORMAT_8BIT;
    SPI_InitStructure.SPI_SCKPL = SPI_SCKPL_HIGH;
    SPI_InitStructure.SPI_SCKPH = SPI_SCKPH_2EDGE;
    SPI_InitStructure.SPI_SWNSSEN = SPI_SWNSS_SOFT;
    SPI_InitStructure.SPI_PSC = SPI_PSC_2;  // SPI_BaudRatePrescaler_32
    SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;
    SPI_InitStructure.SPI_CRCPOL = 7;
    SPI_Init(SD_SPI, &SPI_InitStructure);

    SPI_Enable(SD_SPI, ENABLE); /* SD_SPI enable */
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

