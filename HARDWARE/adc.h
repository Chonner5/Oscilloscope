#ifndef __ADC_H
#define __ADC_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//ADC驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/1/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
#define ADC_SIZE 400

#define RHEOSTAT_ADC_DR_ADDR                ((uint32_t)ADC1+0x4c)
void Rheostat_Init(void);

extern  __IO uint16_t ADC_ConvertedValue[ADC_SIZE];
extern void Open_ADC(void);
extern void Close_ADC(void);

// extern get_voltage_range(void);
extern int Adc_state_flag;


#endif 
