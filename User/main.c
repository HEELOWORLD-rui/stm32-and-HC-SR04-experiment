/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-22
 * @brief       ������ʱ���ж� ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "stdio.h"
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/TIMER/btim.h"
#include "ultrasonsic.h"
#include "usart.h"

double dis = 0;
int main(void)
{
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* ����ʱ��, 72Mhz */
    delay_init(72);                         /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                             /* ��ʼ��LED */
    btim_timx_int_init(10 - 1, 72 - 1); /* 10Khz�ļ���Ƶ�ʣ�����5K��Ϊ500ms */
	SR04_Init();
	
    while (1)
    {
		dis = Get_DistanceData();
		printf("��ǰ���룺%lf\r\n",dis);
		delay_ms(1000);
		
		if(dis <300 - 150)
		{
			printf("Һ�����\r\n");
			LED0(0);
			delay_ms(100);
			LED0(1);
			delay_ms(100);
		}
		else if(dis > 300 - 50)
		{
			printf("Һ�����\r\n");
			LED0(0);
			delay_ms(100);
			LED0(1);
			delay_ms(100);
		}
		else
		{
			LED0(1);
		}
    }
}






