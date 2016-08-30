#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include "stm32f10x.h"

void SysTick_Init(void);
void Delay_us(u32 us);
void Delay_ms(u32 ms);


#endif
