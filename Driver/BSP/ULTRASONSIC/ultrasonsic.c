#include "ultrasonsic.h"

void SR04_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE(); //使能时钟
	
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pin = GPIO_PIN_2;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_Init); //GPIOA2设置为推挽输出，控制信号
	
	GPIO_Init.Mode = GPIO_MODE_INPUT;
	GPIO_Init.Pin = GPIO_PIN_3;
	GPIO_Init.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_Init); //GPIOA3设置为输入，接收信号
}
	
double Get_DistanceData(void)
{
	double dis;
	double distance;
	
	Trig(1); //发出高电平信号
	delay_us(20); //延时20us以触发传感器
	Trig(0); //高电平结束
	while(!Echo); //等待接收信号到来
	resetCNT();//到来后先清理标志
	while(Echo);  //等待接收信号结束
	dis = getCNT();
	distance = dis/580.0; //此处得到的距离是m 
	distance = distance*1000;
					//修改这个公式就可以实现单位转换
 	distance = 290-distance;
	printf("dis=%fs, Distance=%fmm ",dis ,distance); //单位转换后串口的信息需要转换
	return distance;
}
