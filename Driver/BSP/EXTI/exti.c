/**
 ****************************************************************************************************
 * @file        exti.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-19
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
 * V1.0 20200420
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "exti.h"

extern uint8_t model = 0;  // 模式标识符0~3
extern uint8_t high = 150;   // 上升标识符150mm(上限)
extern uint8_t low = 50;    // 下降标识符50mm(下限)
extern uint8_t page = 0;   // 记录页数

/**
 * @brief       KEY0 外部中断服务程序
 * @param       无
 * @retval      无
 */
void KEY0_INT_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(KEY0_INT_GPIO_PIN);         /* 调用中断处理公用函数 清除KEY0所在中断线 的中断标志位 */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY0_INT_GPIO_PIN);         /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
}

/**
 * @brief       KEY1 外部中断服务程序
 * @param       无
 * @retval      无
 */
void KEY1_INT_IRQHandler(void)
{ 
    HAL_GPIO_EXTI_IRQHandler(KEY1_INT_GPIO_PIN);         /* 调用中断处理公用函数 清除KEY1所在中断线 的中断标志位，中断下半部在HAL_GPIO_EXTI_Callback执行 */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY1_INT_GPIO_PIN);         /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
}

/**
 * @brief       KEY2 外部中断服务程序
 * @param       无
 * @retval      无
 */
void KEY2_INT_IRQHandler(void)
{ 
    HAL_GPIO_EXTI_IRQHandler(KEY2_INT_GPIO_PIN);        /* 调用中断处理公用函数 清除KEY2所在中断线 的中断标志位，中断下半部在HAL_GPIO_EXTI_Callback执行 */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY2_INT_GPIO_PIN);        /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
}

/**
 * @brief       WK_UP 外部中断服务程序
 * @param       无
 * @retval      无
 */
void WKUP_INT_IRQHandler(void)
{ 
    HAL_GPIO_EXTI_IRQHandler(WKUP_INT_GPIO_PIN);        /* 调用中断处理公用函数 清除KEY_UP所在中断线 的中断标志位，中断下半部在HAL_GPIO_EXTI_Callback执行 */
    __HAL_GPIO_EXTI_CLEAR_IT(WKUP_INT_GPIO_PIN);        /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
}

/**
 * @brief       中断服务程序中需要做的事情
                在HAL库中所有的外部中断服务函数都会调用此函数
 * @param       GPIO_Pin:中断引脚号
 * @retval      无
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
// 按键的设置如下：WKUP退回主界面，KEY2减，KEY1加，KEY0模式调整
    delay_ms(20);      /* 消抖 */
    switch(GPIO_Pin)
    {
        case KEY0_INT_GPIO_PIN:
            if (KEY0 == 0)
            {
				while(!KEY0);
				delay_ms(20);
				model++;
				lcd_clear(WHITE); // lcd清屏白色，同时也设定了背景颜色
				if(model==4)
					model=0;
            }
            break;
        case KEY1_INT_GPIO_PIN:
            if (KEY1 == 0)
            {
				while(!KEY1);
				delay_ms(20);
				if(model == 1)  //上限调整
				{
					high++;
					if (high==HighLimit)
					{
						text_show_string(30, 60, 240, 16, "上限值无法超过水箱高度25mm！", 16, 0, BLACK);
						high--;
					}
					else 
						text_show_string(30, 60, 240, 16, "                           ", 16, 0, BLACK);
				}
				else if(model == 2) // 下限模式
				{
					low++;
					if(low==100) // 到达上限值
					{
						text_show_string(30, 60, 240, 16, "下限值无法超过10mm", 16, 0, BLACK);
						low--;
					}
					else 
						text_show_string(30, 60, 240, 16, "                           ", 16, 0, BLACK);
				}
				else if(model == 3) // 内存模式
				{
					page++;
					if(page==PageHigh) // 到达上限值
					{
						text_show_string(30, 60, 240, 16, "页数到达上限值！", 16, 0, BLACK);
						page--;
					}
					else 
						text_show_string(30, 60, 240, 16, "                           ", 16, 0, BLACK);
				}
            }
            break;
        case KEY2_INT_GPIO_PIN:
            if (KEY2 == 0)
            {
				while(!KEY2);
				delay_ms(20);
				if(model == 1) // 上限设置
				{
					high--;
					if (high==100)
					{
						text_show_string(30, 60, 240, 16, "上限值无法低于100mm！", 16, 0, BLACK);
						high++;
					}
					else 
						text_show_string(30, 60, 240, 16, "                           ", 16, 0, BLACK);
				}
				else if(model == 2) // 下限设置
				{
					low--;
					if (low==LowLimit)
					{
						text_show_string(30, 60, 240, 16, "下限值无法低于15mm！", 16, 0, BLACK);
						low++;
					}
					else 
						text_show_string(30, 60, 240, 16, "                           ", 16, 0, BLACK);
				}
				else if(model == 3) // 内存模式
				{
					page--;
					if(page==PageLow) // 到达下限值
					{
						text_show_string(30, 60, 240, 16, "页数无法小于0！", 16, 0, BLACK);
						page++;
					}
					else 
						text_show_string(30, 60, 240, 16, "                           ", 16, 0, BLACK);
				}
            }
            break;
        case WKUP_INT_GPIO_PIN:
            if (WK_UP == 1)
            {
				while(WK_UP);
				delay_ms(20);
				lcd_clear(WHITE); // lcd清屏白色，同时也设定了背景颜色
				model = 0;
            }
            break;
    }
}

/**
 * @brief       外部中断初始化程序
 * @param       无
 * @retval      无
 */
void extix_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    KEY0_GPIO_CLK_ENABLE();                                  /* KEY0时钟使能 */
    KEY1_GPIO_CLK_ENABLE();                                  /* KEY1时钟使能 */
    KEY2_GPIO_CLK_ENABLE();                                  /* KEY2时钟使能 */
    WKUP_GPIO_CLK_ENABLE();                                  /* WKUP时钟使能 */

    gpio_init_struct.Pin = KEY0_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* 下升沿触发 */
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY0_INT_GPIO_PORT, &gpio_init_struct);    /* KEY0配置为下降沿触发中断 */

    gpio_init_struct.Pin = KEY1_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* 下升沿触发 */
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY1_INT_GPIO_PORT, &gpio_init_struct);    /* KEY1配置为下降沿触发中断 */
    
    gpio_init_struct.Pin = KEY2_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* 下升沿触发 */
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY2_INT_GPIO_PORT, &gpio_init_struct);    /* KEY2配置为下降沿触发中断 */
    
    gpio_init_struct.Pin = WKUP_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_RISING;             /* 上升沿触发 */
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(WKUP_GPIO_PORT, &gpio_init_struct);        /* WKUP配置为下降沿触发中断 */

    HAL_NVIC_SetPriority(KEY0_INT_IRQn, 0, 2);               /* 抢占0，子优先级2 */
    HAL_NVIC_EnableIRQ(KEY0_INT_IRQn);                       /* 使能中断线1 */

    HAL_NVIC_SetPriority(KEY1_INT_IRQn, 1, 2);               /* 抢占1，子优先级2 */
    HAL_NVIC_EnableIRQ(KEY1_INT_IRQn);                       /* 使能中断线15 */
    
    HAL_NVIC_SetPriority(KEY2_INT_IRQn, 2, 2);               /* 抢占2，子优先级2 */
    HAL_NVIC_EnableIRQ(KEY2_INT_IRQn);                       /* 使能中断线15 */

    HAL_NVIC_SetPriority(WKUP_INT_IRQn, 3, 2);               /* 抢占3，子优先级2 */
    HAL_NVIC_EnableIRQ(WKUP_INT_IRQn);                       /* 使能中断线0 */
}












