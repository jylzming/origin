/**********************************************************************************
 * �ļ���  ��main.c
 * ����    ��         
 * ��汾  ��ST3.5.0
 * ����    �����β��  
 * ����    ��http://jylzming.blog.163.com/
**********************************************************************************/

#include "stm32f10x.h"
#include "ht1621.h"
#include "systick.h"
#include "rtc.h"
#include "uart.h"
#include "stdio.h"
#include "string.h"


uint8_t Ht1621Tab[]={0x00,0x00,0x00,0x00};
uint8_t flag_datachange;//�����޸�ʱ���־
uint32_t time = 0;
extern uint32_t h, m, s;
extern uint16_t aready_set;//RTC��ʱ���޸ĺ�ı��������޸ı�־
extern unsigned char recv_data[256];//���մ��ڵ����ݴ������


int main(void)
{	
	// ��������main����֮ǰ�������ļ���statup_stm32f10x_hd.s�Ѿ�����
	//SystemInit();//������ϵͳʱ�ӳ�ʼ����72MHZ
	// SystemInit()��system_stm32f10x.c�ж���
	// ����û����޸�ϵͳʱ�ӣ������б�д�����޸�
	UartConfig();
	printf("LCD Test\r\n");
	SysTick_Init();
	HT1621_Init();
	NVIC_RTC_Config();
	RTC_Check();//�ϵ�ʱ����Ƿ���Ҫ���ã���ʱ��֮ǰ�Ѿ����ù�����aready_set = 0xA5A5
//	HT1621_WriteAllData(0, Ht1621Tab, 16);//���LCD��ʾ����
	Delay_ms(1);
	Display();
	printf("please input hh��mm��ss to change time\r\n");
	while( 1 )
	{
		/**bug��systick��ʱ�����쳣�����´���ִ���쳣**/
//		GPIO_SetBits(GPIOB, GPIO_Pin_5);//LED��
//		Delay_ms(1000);
//		GPIO_ResetBits(GPIOB, GPIO_Pin_5);//LED��
//		Delay_ms(1000);
		
		/*������ڽ��յ������ݣ�����ʱ�����ݣ�������aready_set����ʱRTC_Check�ж���BKP_DR1���ݲ���ȣ�ִ������*/
		if(flag_datachange == 1)
		{
			h = (recv_data[0]-0x30) * 10 + (recv_data[1]-0x30);
			m = (recv_data[4]-0x30) * 10 + (recv_data[5]-0x30);
			s = (recv_data[8]-0x30) * 10 + (recv_data[9]-0x30);
			time = h*3600 + m*60 + s;
			aready_set += 1;
			
			//����uint16_t���ֵ������aready_set = ����aready_set+1;��ν��ܵ��������ݾ��Լ�1��������Ϊ0xA5A5
			if((aready_set + 1) < aready_set)
			{
				aready_set = 0x01;
			}
		
			RTC_Check();//ֻ�д��ڽ��յ��µ�ʱ�䴮�Ż�ִ���µ�����


			flag_datachange = 0;
		}
	}
}



