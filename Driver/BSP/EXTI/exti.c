/**
 ****************************************************************************************************
 * @file        exti.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-19
 * @brief       �ⲿ�ж� ��������
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
 * �޸�˵��
 * V1.0 20200420
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "exti.h"

extern uint8_t model = 0;  // ģʽ��ʶ��0~3
extern uint8_t high = 150;   // ������ʶ��150mm(����)
extern uint8_t low = 50;    // �½���ʶ��50mm(����)
extern uint8_t page = 0;   // ��¼ҳ��

/**
 * @brief       KEY0 �ⲿ�жϷ������
 * @param       ��
 * @retval      ��
 */
void KEY0_INT_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(KEY0_INT_GPIO_PIN);         /* �����жϴ����ú��� ���KEY0�����ж��� ���жϱ�־λ */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY0_INT_GPIO_PIN);         /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
}

/**
 * @brief       KEY1 �ⲿ�жϷ������
 * @param       ��
 * @retval      ��
 */
void KEY1_INT_IRQHandler(void)
{ 
    HAL_GPIO_EXTI_IRQHandler(KEY1_INT_GPIO_PIN);         /* �����жϴ����ú��� ���KEY1�����ж��� ���жϱ�־λ���ж��°벿��HAL_GPIO_EXTI_Callbackִ�� */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY1_INT_GPIO_PIN);         /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
}

/**
 * @brief       KEY2 �ⲿ�жϷ������
 * @param       ��
 * @retval      ��
 */
void KEY2_INT_IRQHandler(void)
{ 
    HAL_GPIO_EXTI_IRQHandler(KEY2_INT_GPIO_PIN);        /* �����жϴ����ú��� ���KEY2�����ж��� ���жϱ�־λ���ж��°벿��HAL_GPIO_EXTI_Callbackִ�� */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY2_INT_GPIO_PIN);        /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
}

/**
 * @brief       WK_UP �ⲿ�жϷ������
 * @param       ��
 * @retval      ��
 */
void WKUP_INT_IRQHandler(void)
{ 
    HAL_GPIO_EXTI_IRQHandler(WKUP_INT_GPIO_PIN);        /* �����жϴ����ú��� ���KEY_UP�����ж��� ���жϱ�־λ���ж��°벿��HAL_GPIO_EXTI_Callbackִ�� */
    __HAL_GPIO_EXTI_CLEAR_IT(WKUP_INT_GPIO_PIN);        /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
}

/**
 * @brief       �жϷ����������Ҫ��������
                ��HAL�������е��ⲿ�жϷ�����������ô˺���
 * @param       GPIO_Pin:�ж����ź�
 * @retval      ��
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
// �������������£�WKUP�˻������棬KEY2����KEY1�ӣ�KEY0ģʽ����
    delay_ms(20);      /* ���� */
    switch(GPIO_Pin)
    {
        case KEY0_INT_GPIO_PIN:
            if (KEY0 == 0)
            {
				while(!KEY0);
				delay_ms(20);
				model++;
				lcd_clear(WHITE); // lcd������ɫ��ͬʱҲ�趨�˱�����ɫ
				if(model==4)
					model=0;
            }
            break;
        case KEY1_INT_GPIO_PIN:
            if (KEY1 == 0)
            {
				while(!KEY1);
				delay_ms(20);
				if(model == 1)  //���޵���
				{
					high++;
					if (high==HighLimit)
					{
						text_show_string(30, 60, 240, 16, "����ֵ�޷�����ˮ��߶�25mm��", 16, 0, BLACK);
						high--;
					}
					else 
						text_show_string(30, 60, 240, 16, "                           ", 16, 0, BLACK);
				}
				else if(model == 2) // ����ģʽ
				{
					low++;
					if(low==100) // ��������ֵ
					{
						text_show_string(30, 60, 240, 16, "����ֵ�޷�����10mm", 16, 0, BLACK);
						low--;
					}
					else 
						text_show_string(30, 60, 240, 16, "                           ", 16, 0, BLACK);
				}
				else if(model == 3) // �ڴ�ģʽ
				{
					page++;
					if(page==PageHigh) // ��������ֵ
					{
						text_show_string(30, 60, 240, 16, "ҳ����������ֵ��", 16, 0, BLACK);
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
				if(model == 1) // ��������
				{
					high--;
					if (high==100)
					{
						text_show_string(30, 60, 240, 16, "����ֵ�޷�����100mm��", 16, 0, BLACK);
						high++;
					}
					else 
						text_show_string(30, 60, 240, 16, "                           ", 16, 0, BLACK);
				}
				else if(model == 2) // ��������
				{
					low--;
					if (low==LowLimit)
					{
						text_show_string(30, 60, 240, 16, "����ֵ�޷�����15mm��", 16, 0, BLACK);
						low++;
					}
					else 
						text_show_string(30, 60, 240, 16, "                           ", 16, 0, BLACK);
				}
				else if(model == 3) // �ڴ�ģʽ
				{
					page--;
					if(page==PageLow) // ��������ֵ
					{
						text_show_string(30, 60, 240, 16, "ҳ���޷�С��0��", 16, 0, BLACK);
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
				lcd_clear(WHITE); // lcd������ɫ��ͬʱҲ�趨�˱�����ɫ
				model = 0;
            }
            break;
    }
}

/**
 * @brief       �ⲿ�жϳ�ʼ������
 * @param       ��
 * @retval      ��
 */
void extix_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    KEY0_GPIO_CLK_ENABLE();                                  /* KEY0ʱ��ʹ�� */
    KEY1_GPIO_CLK_ENABLE();                                  /* KEY1ʱ��ʹ�� */
    KEY2_GPIO_CLK_ENABLE();                                  /* KEY2ʱ��ʹ�� */
    WKUP_GPIO_CLK_ENABLE();                                  /* WKUPʱ��ʹ�� */

    gpio_init_struct.Pin = KEY0_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* �����ش��� */
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY0_INT_GPIO_PORT, &gpio_init_struct);    /* KEY0����Ϊ�½��ش����ж� */

    gpio_init_struct.Pin = KEY1_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* �����ش��� */
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY1_INT_GPIO_PORT, &gpio_init_struct);    /* KEY1����Ϊ�½��ش����ж� */
    
    gpio_init_struct.Pin = KEY2_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* �����ش��� */
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY2_INT_GPIO_PORT, &gpio_init_struct);    /* KEY2����Ϊ�½��ش����ж� */
    
    gpio_init_struct.Pin = WKUP_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_RISING;             /* �����ش��� */
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(WKUP_GPIO_PORT, &gpio_init_struct);        /* WKUP����Ϊ�½��ش����ж� */

    HAL_NVIC_SetPriority(KEY0_INT_IRQn, 0, 2);               /* ��ռ0�������ȼ�2 */
    HAL_NVIC_EnableIRQ(KEY0_INT_IRQn);                       /* ʹ���ж���1 */

    HAL_NVIC_SetPriority(KEY1_INT_IRQn, 1, 2);               /* ��ռ1�������ȼ�2 */
    HAL_NVIC_EnableIRQ(KEY1_INT_IRQn);                       /* ʹ���ж���15 */
    
    HAL_NVIC_SetPriority(KEY2_INT_IRQn, 2, 2);               /* ��ռ2�������ȼ�2 */
    HAL_NVIC_EnableIRQ(KEY2_INT_IRQn);                       /* ʹ���ж���15 */

    HAL_NVIC_SetPriority(WKUP_INT_IRQn, 3, 2);               /* ��ռ3�������ȼ�2 */
    HAL_NVIC_EnableIRQ(WKUP_INT_IRQn);                       /* ʹ���ж���0 */
}












