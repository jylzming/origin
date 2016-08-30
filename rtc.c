#include "rtc.h"

uint32_t h = 0, m = 0, s = 0;
extern uint32_t time;
uint16_t aready_set = 0xA5A5;//��������鿴�Ƿ��Ѿ����ã���ֵ��㶨��

void NVIC_RTC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

/***********************************************************************
*1����ѯRTOFFλֱ��Ϊ1����RTC_WaitForLastTask������
*2����CNFֵΪ1����������ģʽ
*3����һ������RTC�Ĵ�������д����
*4�����CNF��־λ���˳�����ģʽ
*5����ѯRTOFF��ֱ��Ϊ1��ȷ�ϲ������
*����CNF��־λ�����ʱ��д�������ܽ��У��������������Ҫ3��RTCCLK���ڡ�
**********************************************************************/
void RTC_Config(void)
{
	PWR_DeInit();
	PWR_BackupAccessCmd(ENABLE);
	
	BKP_DeInit();	
	
	RCC_LSEConfig(RCC_LSE_ON);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForSynchro();
	
	RTC_WaitForLastTask();
	RTC_EnterConfigMode();
	
	RTC_SetPrescaler(32767);//1S
	RTC_WaitForLastTask();
	
//	RTC_SetCounter(1471427930 + 28800);//Unix time
	RTC_SetCounter(time);//set time
	RTC_WaitForLastTask();
	
	RTC_ITConfig(RTC_IT_SEC, ENABLE);//���жϴ�
	RTC_WaitForLastTask();
	
	RTC_ExitConfigMode();
	RTC_WaitForLastTask();
	
	NVIC_RTC_Config();
	
	BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);//�ϵ�����Ĭ��ֵ����Ϊ0xA5A5
}

void RTC_Check(void)
{	//aready_set�ڴ��ڽ��յ��µ�ʱ���ᱻ�ı䣬BKP_DR1����������ȣ�ִ������
	if(BKP_ReadBackupRegister(BKP_DR1) == aready_set)//���RTC�Ƿ��Ѿ�����
	{//�����Ѿ����ã���ͬ������ж�
		RTC_WaitForSynchro();
		
		RTC_ITConfig(RTC_IT_SEC, ENABLE);//���жϴ�
		RTC_WaitForLastTask();
	}
	else//����û�����ã�ִ�����ô���
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);	

		RTC_Config();
	}
}

uint32_t Read_RTC(void)
{
	return RTC_GetCounter();
}


