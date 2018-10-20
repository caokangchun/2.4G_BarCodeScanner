/**
  ******************************************************************************
  * @file    systick.c
  * @author  MCU SD
  * @version V2.0.0
  * @date    15-Jan-2016
  * @brief   The SysTick configuration file.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f1x0.h"
#include "systick.h"
static uint32_t delay;
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Configure SysTick.
  * @param  None
  * @retval None
  */
void SysTick_Configuration(void)
{
    /* Setup SysTick Timer for 1000Hz interrupts  */
    if (SysTick_Config(SystemCoreClock / 1000000))	//1000000
    {
        /* Capture error */
        while(1);
    }
    /* Configure the SysTick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00);
}
/*!
    \brief      delay a time in milliseconds
    \param[in]  count: count in milliseconds
    \param[out] none
    \retval     none
*/



void delay_1ms(uint32_t count)
{
		delay = count*1000;
	    while(0U != delay){
    }
}

void delay_1us(uint32_t count)
{
	delay = count;
	    while(0U != delay){
    }
}

/*!
    \brief      delay decrement
    \param[in]  none
    \param[out] none
    \retval     none
*/
void delay_decrement(void)
{
    if (0 != delay){
        delay--;
    }
}



/******************* (C) COPYRIGHT 2016 GIGADEVICE *****END OF FILE****/
