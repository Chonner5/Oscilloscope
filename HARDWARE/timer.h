#ifndef TIMER_H
#define TIMER_H
#include "sys.h"


extern const float mysample_time[5];

extern u16 code;
extern TIM_HandleTypeDef TIM2_Handler;


void TIM2_Init(u16 arr,u16 psc);
void TIM1_Config(int num);

#endif

