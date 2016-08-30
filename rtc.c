#include "rtc.h"

uint32_t h = 0, m = 0, s = 0;
extern uint32_t time;
uint16_t aready_set = 0xA5A5;//备份区域查看是否已经设置，数值随便定义

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
*1、查询RTOFF位直到为1，即RTC_WaitForLastTask（）；
*2、置CNF值为1，进入配置模式
*3、对一个或多个RTC寄存器进行写操作
*4、清除CNF标志位，退出配置模式
*5、查询RTOFF，直到为1以确认操作完成
*仅当CNF标志位被清除时，写操作才能进行，这个过程至少需要3个RTCCLK周期。
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
	
	RTC_ITConfig(RTC_IT_SEC, ENABLE);//秒中断打开
	RTC_WaitForLastTask();
	
	RTC_ExitConfigMode();
	RTC_WaitForLastTask();
	
	NVIC_RTC_Config();
	
	BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);//上电启动默认值设置为0xA5A5
}

void RTC_Check(void)
{	//aready_set在串口接收到新的时间后会被改变，BKP_DR1将与它不相等，执行配置
	if(BKP_ReadBackupRegister(BKP_DR1) == aready_set)//检查RTC是否已经配置
	{//若是已经配置，待同步后打开中断
		RTC_WaitForSynchro();
		
		RTC_ITConfig(RTC_IT_SEC, ENABLE);//秒中断打开
		RTC_WaitForLastTask();
	}
	else//若是没有配置，执行配置代码
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


