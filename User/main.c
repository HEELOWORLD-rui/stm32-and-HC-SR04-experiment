/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-22
 * @brief       基本定时器中断 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
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
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* 设置时钟, 72Mhz */
    delay_init(72);                         /* 延时初始化 */
    usart_init(115200);                     /* 串口初始化为115200 */
    led_init();                             /* 初始化LED */
    btim_timx_int_init(10 - 1, 72 - 1); /* 10Khz的计数频率，计数5K次为500ms */
	SR04_Init();
	
    while (1)
    {
		dis = Get_DistanceData();
		printf("当前距离：%lf\r\n",dis);
		delay_ms(1000);
		
		if(dis <300 - 150)
		{
			printf("液面过高\r\n");
			LED0(0);
			delay_ms(100);
			LED0(1);
			delay_ms(100);
		}
		else if(dis > 300 - 50)
		{
			printf("液面过低\r\n");
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






