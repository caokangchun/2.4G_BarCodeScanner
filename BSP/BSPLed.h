#ifndef __BSPLED_H__
#define __BSPLED_H__	1

#include "gd32f1x0.h"






#define BSPLedIdle()	gpio_bit_reset(GPIOB,GPIO_PIN_7)
#define BSPLedWork()	gpio_bit_set(GPIOB,GPIO_PIN_7)

#endif
