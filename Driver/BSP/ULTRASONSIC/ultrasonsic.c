#include "ultrasonsic.h"

void SR04_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE(); //ʹ��ʱ��
	
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pin = GPIO_PIN_2;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_Init); //GPIOA2����Ϊ��������������ź�
	
	GPIO_Init.Mode = GPIO_MODE_INPUT;
	GPIO_Init.Pin = GPIO_PIN_3;
	GPIO_Init.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_Init); //GPIOA3����Ϊ���룬�����ź�
}
	
double Get_DistanceData(void)
{
	double dis;
	double distance;
	
	Trig(1); //�����ߵ�ƽ�ź�
	delay_us(20); //��ʱ20us�Դ���������
	Trig(0); //�ߵ�ƽ����
	while(!Echo); //�ȴ������źŵ���
	resetCNT();//�������������־
	while(Echo);  //�ȴ������źŽ���
	dis = getCNT();
	distance = dis/580.0; //�˴��õ��ľ�����m 
	distance = distance*1000;
					//�޸������ʽ�Ϳ���ʵ�ֵ�λת��
 	distance = 290-distance;
	printf("dis=%fs, Distance=%fmm ",dis ,distance); //��λת���󴮿ڵ���Ϣ��Ҫת��
	return distance;
}
