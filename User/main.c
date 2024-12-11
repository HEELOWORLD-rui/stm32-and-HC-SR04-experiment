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

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/TIMER/btim.h"
#include "./USMART/usmart.h"
#include "./MALLOC/malloc.h"
#include "./FATFS/exfuns/exfuns.h"
#include "./TEXT/text.h"
#include "./BSP/SRAM/sram.h"
#include "./BSP/SDIO/sdio_sdcard.h"
#include "./BSP/NORFLASH/norflash.h"
#include "lcd.h"
#include "key.h"
#include "exti.h"
#include "ultrasonsic.h"
#include "beep.h"

void Init(void);        // ��ʼ������
void MainMode(void);    // ������
void High_Adjust(void);  // ���޵���
void Low_Adjust(void);   // ���޵���
void MemoryMode(void);  // �洢������
void WarningMode(void); // ��������
double dis = 0.0;
int main(void)
{
	uint8_t lcd_id[12];
	Init();
	sprintf((char *)lcd_id, "LCD ID:%04X", lcddev.id);  /* ��LCD ID��ӡ��lcd_id���� */
	while(1)
	{ 
		lcd_show_string(100, 10, 240, 16, 16, (char *)lcd_id, BLACK); /* ��ʾLCD ID */
		switch(model)
		{
			case 0:
				MainMode();
				break;
			case 1:
				High_Adjust();
				break;
			case 2:
				Low_Adjust();
				break;
			case 3:
				MemoryMode();
				break;
			default:
				break;
		}
	}
}

void Init(void)
{
	HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* ����ʱ��, 72Mhz */
    delay_init(72);                         /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
	usmart_dev.init(72);                    /* ��ʼ��USMART */
	key_init();                             /* ��ʼ������ */
    sram_init();                            /* SRAM��ʼ�� */
    my_mem_init(SRAMIN);                    /* ��ʼ���ڲ�SRAM�ڴ�� */
    my_mem_init(SRAMEX);                    /* ��ʼ���ⲿSRAM�ڴ�� */
    led_init();                             /* ��ʼ��LED */
	lcd_init(); // LCD��ʼ�� 
	g_point_color = RED; // ��ɫ��
	SR04_Init();
	beep_init();
	extix_init();
	
	exfuns_init();                          /* Ϊfatfs��ر��������ڴ� */
    f_mount(fs[0], "0:", 1);                /* ����SD�� */
    f_mount(fs[1], "1:", 1);                /* ����FLASH */
	
    btim_timx_int_init(10 - 1, 72 - 1); /* 10us */
	lcd_clear(WHITE); // lcd������ɫ��ͬʱҲ�趨�˱�����ɫ
	
	while (fonts_init())                    /* ����ֿ� */
    {
		lcd_show_string(30, 130, 240, 16, 16, "ERROR!", RED);
	}
}	

void MainMode(void)
{
	dis = Get_DistanceData();
	text_show_string(100, 280, 240, 16, "������", 16, 0, BLACK);
	text_show_string(30, 30, 240, 16, "��ǰ����ֵ��", 16, 0, BLACK);
	lcd_show_num(120, 30, high, 3, 16, BLACK);
	text_show_string(30, 60, 240, 16, "��ǰ����ֵ��", 16, 0, BLACK);
	lcd_show_num(120, 60, low, 3, 16, BLACK);
	text_show_string(30, 90, 240, 16, "��ǰˮλ��", 16, 0, BLACK);
	lcd_show_num(120, 90, dis, 3, 16, BLACK);
	
	text_show_string(30, 120, 240, 16, "���� 8210222124", 16, 0, BLACK);
	text_show_string(30, 150, 240, 16, "κ� 8207221821", 16, 0, BLACK);
	text_show_string(30, 180, 240, 16, "κ�ƶ� 8207221917", 16, 0, BLACK);
	text_show_string(30, 210, 240, 16, "���� 8210220814", 16, 0, BLACK);
	
	if(dis > high-2 && dis <= high-0.1) // �𲽱���
	{
		text_show_string(180, 100, 240, 24, "���ߣ�", 24, 0, BLACK);
		delay_ms((-dis+high)*1000);
		text_show_string(180, 100, 240, 24, "      ", 24, 0, BLACK);
		delay_ms((-dis+high)*1000);
	}
	else if(dis < low+2 && dis >= low-0.1)
	{
		text_show_string(180, 100, 240, 24, "���ͣ�", 24, 0, BLACK);
		delay_ms((dis-low)*1000);
		text_show_string(180, 100, 240, 24, "      ", 24, 0, BLACK);
		delay_ms((dis-low)*1000);
	}
	else if(dis<=low || dis>=high)
	{
		WarningMode();
	}
}

void MemoryMode(void)
{
	text_show_string(100, 240, 240, 16, "�洢��ģʽ", 16, 0, BLACK);
	text_show_string(60, 30, 240, 16, "��ǰҳ�棺", 16, 0, BLACK);
	// ��ȡFlash����
}

void WarningMode(void)
{
	lcd_clear(WHITE);
	HAL_NVIC_DisableIRQ(EXTI0_IRQn);
	HAL_NVIC_DisableIRQ(EXTI1_IRQn);
	HAL_NVIC_DisableIRQ(EXTI2_IRQn);
	HAL_NVIC_DisableIRQ(EXTI3_IRQn);
	// �ر������ж�
	while(dis<=low || dis>=high)//������ѭ���У�ֻ�������ܳ���
	{
		dis = Get_DistanceData();
		text_show_string(160, 100, 240, 24, "������������", 24, 0, RED); //չʾ����
		LED0_TOGGLE();  //LED0��ת
		BEEP_TOGGLE();  //��������ת
		delay_ms(10);
		text_show_string(160, 100, 240, 24, "           ", 24, 0, RED); //չʾ����
		LED0_TOGGLE();  //LED0��ת
		BEEP_TOGGLE();  //��������ת
		delay_ms(10);
	}
}

void High_Adjust(void)
{
	text_show_string(100, 280, 240, 16, "��������ֵ", 16, 0, BLACK);
	text_show_string(30, 30, 240, 16, "��ǰ����ֵ��", 16, 0, BLACK);
	lcd_show_num(120, 30, high, 3, 16, BLACK);
}
void Low_Adjust(void)
{
	text_show_string(100, 280, 240, 16, "��������ֵ", 16, 0, BLACK);
	text_show_string(30, 30, 240, 16, "��ǰ����ֵ��", 16, 0, BLACK);
	lcd_show_num(120, 30, low, 3, 16, BLACK);
}













