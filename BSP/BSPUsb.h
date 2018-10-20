#ifndef __BSPUSB_H__
#define __BSPUSB_H__

#include "usbd_std.h"
#include "hid_core.h"
#include "gd32f1x0_libopt.h"
#include "config.h"
#include "BSPPlay.h"

#include "keyboard.h"



#define USB_PULLUP                      GPIOA
#define USB_PULLUP_PIN                  GPIO_PIN_15
#define RCC_AHBPeriph_GPIO_PULLUP       RCU_GPIOA



extern void BSPUsbInit(void);

extern void SendWire(void);

#endif

