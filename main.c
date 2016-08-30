/**********************************************************************************
 * 文件名  ：main.c
 * 描述    ：         
 * 库版本  ：ST3.5.0
 * 作者    ：牧梦苍穹  
 * 博客    ：http://jylzming.blog.163.com/
**********************************************************************************/

#include "stm32f10x.h"
#include "ht1621.h"
#include "systick.h"
#include "rtc.h"
#include "uart.h"
#include "stdio.h"
#include "string.h"


uint8_t Ht1621Tab[]={0x00,0x00,0x00,0x00};
uint8_t flag_datachange;//串口修改时间标志
uint32_t time = 0;
extern uint32_t h, m, s;
extern uint16_t aready_set;//RTC定时器修改后的备份区域修改标志
extern unsigned char recv_data[256];//接收串口的数据存放数组


int main(void)
{	
	// 程序来到main函数之前，启动文件：statup_stm32f10x_hd.s已经调用
	//SystemInit();//函数把系统时钟初始化成72MHZ
	// SystemInit()在system_stm32f10x.c中定义
	// 如果用户想修改系统时钟，可自行编写程序修改
	UartConfig();
	printf("LCD Test\r\n");
	SysTick_Init();
	HT1621_Init();
	NVIC_RTC_Config();
	RTC_Check();//上电时检测是否需要配置，此时若之前已经配置过，则aready_set = 0xA5A5
//	HT1621_WriteAllData(0, Ht1621Tab, 16);//清除LCD显示数据
	Delay_ms(1);
	Display();
	printf("please input hh：mm：ss to change time\r\n");
	while( 1 )
	{
		/**bug：systick延时函数异常，以下代码执行异常**/
//		GPIO_SetBits(GPIOB, GPIO_Pin_5);//LED灭
//		Delay_ms(1000);
//		GPIO_ResetBits(GPIOB, GPIO_Pin_5);//LED亮
//		Delay_ms(1000);
		
		/*如果串口接收到新数据，更新时间数据，并更改aready_set，此时RTC_Check判断与BKP_DR1数据不相等，执行配置*/
		if(flag_datachange == 1)
		{
			h = (recv_data[0]-0x30) * 10 + (recv_data[1]-0x30);
			m = (recv_data[4]-0x30) * 10 + (recv_data[5]-0x30);
			s = (recv_data[8]-0x30) * 10 + (recv_data[9]-0x30);
			time = h*3600 + m*60 + s;
			aready_set += 1;
			
			//超过uint16_t最大值，这里aready_set = 这里aready_set+1;多次接受到串口数据就自加1，重启后为0xA5A5
			if((aready_set + 1) < aready_set)
			{
				aready_set = 0x01;
			}
		
			RTC_Check();//只有串口接收到新的时间串才会执行新的配置


			flag_datachange = 0;
		}
	}
}



