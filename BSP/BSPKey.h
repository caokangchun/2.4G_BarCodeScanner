#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

#include <gd32f1x0.h>
#include "systick.h"
#include "data_type.h"

enum {
	LONG_PRESS=1,
	PRESS,
	NO_PRESS
};


#define key_press()	gpio_input_bit_get(GPIOB, GPIO_PIN_5)

extern uint8_t BSPKeyScanInContinual(void);
extern uint8_t BSPKeyScan(void);
extern void BSPKeyInit(void) __attribute__((section ("BSPKeyInit")));

extern void BSPKeyWaitRelease(void);




#endif
