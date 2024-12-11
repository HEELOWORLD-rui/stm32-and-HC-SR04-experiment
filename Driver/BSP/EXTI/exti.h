/**
 ****************************************************************************************************
 * @file        exti.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-20
 * @brief       外部中断 驱动代码
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
 * 修改说明
 * V1.0 20200419
 * 第一次发布
 *
 ****************************************************************************************************
 */

#ifndef __EXTI_H
#define __EXTI_H

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/BEEP/beep.h"
#include "./BSP/KEY/key.h"
#include "./BSP/EXTI/exti.h"
#include "lcd.h"
#include "./TEXT/text.h"

/******************************************************************************************/
/* 引脚 和 中断编号 & 中断服务函数 定义 */ 

#define KEY0_INT_GPIO_PORT              GPIOE
#define KEY0_INT_GPIO_PIN               GPIO_PIN_4
#define KEY0_INT_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE口时钟使能 */
#define KEY0_INT_IRQn                   EXTI4_IRQn
#define KEY0_INT_IRQHandler             EXTI4_IRQHandler

#define KEY1_INT_GPIO_PORT              GPIOE
#define KEY1_INT_GPIO_PIN               GPIO_PIN_3
#define KEY1_INT_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE口时钟使能 */
#define KEY1_INT_IRQn                   EXTI3_IRQn
#define KEY1_INT_IRQHandler             EXTI3_IRQHandler

#define KEY2_INT_GPIO_PORT              GPIOE
#define KEY2_INT_GPIO_PIN               GPIO_PIN_2
#define KEY2_INT_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE口时钟使能 */
#define KEY2_INT_IRQn                   EXTI2_IRQn
#define KEY2_INT_IRQHandler             EXTI2_IRQHandler

#define WKUP_INT_GPIO_PORT              GPIOA
#define WKUP_INT_GPIO_PIN               GPIO_PIN_0
#define WKUP_INT_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */
#define WKUP_INT_IRQn                   EXTI0_IRQn
#define WKUP_INT_IRQHandler             EXTI0_IRQHandler

#define HighLimit                       250 //经过测量后写入
#define LowLimit                        15
#define PageHigh                        200 //经过测量后写入
#define PageLow                         0
/******************************************************************************************/

extern uint8_t model;
extern uint8_t high;
extern uint8_t low;
extern uint8_t page;

void extix_init(void);  /* 外部中断初始化 */

#endif

























