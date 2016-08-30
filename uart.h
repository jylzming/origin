#ifndef _UART_H_
#define _UART_H_

#include "stm32f10x.h"
#include "stdio.h"

void UartConfig(void);
void USART_SendStr(USART_TypeDef* USARTx, uint8_t *str);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
#endif
