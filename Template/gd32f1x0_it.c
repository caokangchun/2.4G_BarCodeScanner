/*!
    \file  gd32f1x0_it.c
    \brief interrupt service routines
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0_it.h"
#include "systick.h"
#include "BSPPlay.h"

#include "keyboard.h"
#include "usbd_int.h"

/* Private define ------------------------------------------------------------*/
#define BufferSize  32

/* Exported variables ------------------------------------------------------------*/
extern  uint16_t  semaphore;


/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}


/*!
    \brief      this function handles USBD interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void  USBD_LP_IRQHandler (void)
{
    usbd_isr();
}

/*!
    \brief      this function handles EXTI14_15_IRQ Handler.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void  EXTI4_15_IRQHandler (void)
{
    if (RESET != exti_interrupt_flag_get(TAMPER_KEY_EXTI_LINE)) {
        /* check if the remote wakeup feature is enabled */
        if (usb_device_dev.remote_wakeup) {
            /* exit low power mode and re-configure clocks */
            usbd_remote_wakeup_active();
            usb_device_dev.status = usb_device_dev.prev_status;

            /* disable Remote wakeup feature*/
            usb_device_dev.remote_wakeup = 0;
        }

        /* clear the EXTI line pending bit */
        exti_interrupt_flag_clear(TAMPER_KEY_EXTI_LINE);
    }
}

#ifdef USBD_LOWPWR_MODE_ENABLE

/*!
    \brief      this function handles USBD wakeup interrupt request.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void  USBDWakeUp_IRQHandler (void)
{
    exti_interrupt_flag_clear(EXTI_18);
}

#endif /* USBD_LOWPWR_MODE_ENABLE */

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint32_t time=0;

extern void LT8920IncreaseRandom(void);
void SysTick_Handler(void)
{
	time++;
	delay_decrement();
}


u32 scan_time=0;
u32 scan_delay=0;

void TIMER0_BRK_UP_TRG_COM_IRQHandler(void)
{
	if(timer_interrupt_flag_get(TIMER0 ,TIMER_INT_FLAG_UP))
	{
		LT8920IncreaseRandom();
		scan_delay++;
		scan_time++;
		BSPPlay();
		timer_interrupt_flag_clear(TIMER0,TIMER_INT_FLAG_UP);	//clear
	}
	
}
