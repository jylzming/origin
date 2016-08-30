#ifndef _RTC_H_
#define _RTC_H_

#include "stm32f10x.h"

void NVIC_RTC_Config(void);
void RTC_Config(void);
void RTC_Check(void);
uint32_t Read_RTC(void);

#endif

