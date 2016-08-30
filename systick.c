#include "systick.h"

static uint8_t sysclk = 72;
static uint8_t fac_us = 0;
static uint16_t fac_ms = 0;


/*SysTickʱ��Դ��HCLK/8�ṩ����ϵͳƵ��Ϊ72MHzʱ��С��������Ϊ1/9MHz,
����9��Ϊ1us��fac_us��9Ϊ��λ*/
void SysTick_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//72MHz/8 = 9MHz
	NVIC_SetPriority(SysTick_IRQn, 0);
	fac_us = sysclk / 8;//72/8 = 9,9��1uS
	fac_ms = (u16)fac_us * 1000;
}

void Delay_us(u32 us)
{
//	volatile uint32_t temp;
	SysTick->LOAD = (u32)us * fac_us;
	SysTick->VAL  = 0x00;//��ռ�����
	SysTick->CTRL = 0x01;//��ʼ����
//	temp  = SysTick->VAL;
	//����������ֵ��С��0��ʱ��CTRL�Ĵ�����λ16����1
	while((SysTick->CTRL & (0x01 << 16)) == 0);
	SysTick->CTRL = 0x00;//ֹͣ����
	SysTick->VAL  = 0x00;//��ռ�����
}

void Delay_ms(u32 ms)
{
//	volatile uint32_t temp;
	SysTick->LOAD = (u32)ms * fac_ms;
	SysTick->VAL  = 0x00;//��ռ�����
	SysTick->CTRL = 0x01;//��ʼ����
//	temp  = SysTick->VAL;
	//����������ֵ��С��0��ʱ��CTRL�Ĵ�����λ16����1
	while((SysTick->CTRL & (0x01 << 16)) == 0);
	SysTick->CTRL = 0x00;//ֹͣ����
	SysTick->VAL  = 0x00;//��ռ�����
}
