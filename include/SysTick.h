#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f4xx.h"

void SysTick_Init(void);
void delay_ns(u32 nns);



#endif /* __SYSTICK_H */
