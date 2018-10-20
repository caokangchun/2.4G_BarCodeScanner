#ifndef	__BSP_H__
#define	__BSP_H__

#include "gd32f1x0.h"


//中断回调函数类型
typedef void(* CallBackISR)(void);





extern void BSPRoutine(void);


#endif



