/**
  ******************************************************************************
  * @file    systick.h
  * @author  MCU SD
  * @version V2.0.0
  * @date    15-Jan-2016
  * @brief   The header file of the SysTick configuration.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYS_TICK_H
#define __SYS_TICK_H
#include "gd32f1x0.h"
#include "data_type.h"
/* Exported functions ------------------------------------------------------- */
void SysTick_Configuration(void) __attribute__((section ("SysTick_Configuration")));
void LED_Spark(void);
void delay_1ms(uint32_t count);
void delay_1us(uint32_t count);
void delay_decrement(void);
#endif

/******************* (C) COPYRIGHT 2016 GIGADEVICE *****END OF FILE****/
