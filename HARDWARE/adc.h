#ifndef __ADC_H
#define __ADC_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//ADC��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
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
