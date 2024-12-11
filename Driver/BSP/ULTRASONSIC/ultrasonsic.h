#ifndef __ULTRASONSIC_H
#define __ULTRASONSIC_H

#include "stm32f1xx.h"
#include "stm32f1xx_hal_gpio.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "btim.h"
#include <stdio.h>

#define Trig(x) (x?\
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET)\
	:HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET)\
)
#define Echo (GPIOA->IDR & (1<<3))

extern uint32_t getCNT(void);
extern void resetCNT(void);

void SR04_Init(void);
double Get_DistanceData(void);

#endif

