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

void Init(void);        // 初始化函数
void MainMode(void);    // 主界面
void High_Adjust(void);  // 上限调整
void Low_Adjust(void);   // 下限调整
void MemoryMode(void);  // 存储器界面
void WarningMode(void); // 报警界面
double dis = 0.0;
int main(void)
{
	uint8_t lcd_id[12];
	Init();
	sprintf((char *)lcd_id, "LCD ID:%04X", lcddev.id);  /* 将LCD ID打印到lcd_id数组 */
	while(1)
	{ 
		lcd_show_string(100, 10, 240, 16, 16, (char *)lcd_id, BLACK); /* 显示LCD ID */
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
	HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* 设置时钟, 72Mhz */
    delay_init(72);                         /* 延时初始化 */
    usart_init(115200);                     /* 串口初始化为115200 */
	usmart_dev.init(72);                    /* 初始化USMART */
	key_init();                             /* 初始化按键 */
    sram_init();                            /* SRAM初始化 */
    my_mem_init(SRAMIN);                    /* 初始化内部SRAM内存池 */
    my_mem_init(SRAMEX);                    /* 初始化外部SRAM内存池 */
    led_init();                             /* 初始化LED */
	lcd_init(); // LCD初始化 
	g_point_color = RED; // 红色笔
	SR04_Init();
	beep_init();
	extix_init();
	
	exfuns_init();                          /* 为fatfs相关变量申请内存 */
    f_mount(fs[0], "0:", 1);                /* 挂载SD卡 */
    f_mount(fs[1], "1:", 1);                /* 挂载FLASH */
	
    btim_timx_int_init(10 - 1, 72 - 1); /* 10us */
	lcd_clear(WHITE); // lcd清屏白色，同时也设定了背景颜色
	
	while (fonts_init())                    /* 检查字库 */
    {
		lcd_show_string(30, 130, 240, 16, 16, "ERROR!", RED);
	}
}	

void MainMode(void)
{
	dis = Get_DistanceData();
	text_show_string(100, 280, 240, 16, "主界面", 16, 0, BLACK);
	text_show_string(30, 30, 240, 16, "当前上限值：", 16, 0, BLACK);
	lcd_show_num(120, 30, high, 3, 16, BLACK);
	text_show_string(30, 60, 240, 16, "当前下限值：", 16, 0, BLACK);
	lcd_show_num(120, 60, low, 3, 16, BLACK);
	text_show_string(30, 90, 240, 16, "当前水位：", 16, 0, BLACK);
	lcd_show_num(120, 90, dis, 3, 16, BLACK);
	
	text_show_string(30, 120, 240, 16, "吕瑞 8210222124", 16, 0, BLACK);
	text_show_string(30, 150, 240, 16, "魏宇航 8207221821", 16, 0, BLACK);
	text_show_string(30, 180, 240, 16, "魏浩东 8207221917", 16, 0, BLACK);
	text_show_string(30, 210, 240, 16, "蒋浩 8210220814", 16, 0, BLACK);
	
	if(dis > high-2 && dis <= high-0.1) // 逐步报警
	{
		text_show_string(180, 100, 240, 24, "过高！", 24, 0, BLACK);
		delay_ms((-dis+high)*1000);
		text_show_string(180, 100, 240, 24, "      ", 24, 0, BLACK);
		delay_ms((-dis+high)*1000);
	}
	else if(dis < low+2 && dis >= low-0.1)
	{
		text_show_string(180, 100, 240, 24, "过低！", 24, 0, BLACK);
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
	text_show_string(100, 240, 240, 16, "存储器模式", 16, 0, BLACK);
	text_show_string(60, 30, 240, 16, "当前页面：", 16, 0, BLACK);
	// 读取Flash内容
}

void WarningMode(void)
{
	lcd_clear(WHITE);
	HAL_NVIC_DisableIRQ(EXTI0_IRQn);
	HAL_NVIC_DisableIRQ(EXTI1_IRQn);
	HAL_NVIC_DisableIRQ(EXTI2_IRQn);
	HAL_NVIC_DisableIRQ(EXTI3_IRQn);
	// 关闭所有中断
	while(dis<=low || dis>=high)//卡死在循环中，只有重置能出来
	{
		dis = Get_DistanceData();
		text_show_string(160, 100, 240, 24, "警报！警报！", 24, 0, RED); //展示汉字
		LED0_TOGGLE();  //LED0翻转
		BEEP_TOGGLE();  //蜂鸣器翻转
		delay_ms(10);
		text_show_string(160, 100, 240, 24, "           ", 24, 0, RED); //展示汉字
		LED0_TOGGLE();  //LED0翻转
		BEEP_TOGGLE();  //蜂鸣器翻转
		delay_ms(10);
	}
}

void High_Adjust(void)
{
	text_show_string(100, 280, 240, 16, "调整上限值", 16, 0, BLACK);
	text_show_string(30, 30, 240, 16, "当前上限值：", 16, 0, BLACK);
	lcd_show_num(120, 30, high, 3, 16, BLACK);
}
void Low_Adjust(void)
{
	text_show_string(100, 280, 240, 16, "调整下限值", 16, 0, BLACK);
	text_show_string(30, 30, 240, 16, "当前下限值：", 16, 0, BLACK);
	lcd_show_num(120, 30, low, 3, 16, BLACK);
}













