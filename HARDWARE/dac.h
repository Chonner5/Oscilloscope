#ifndef __DAC_H
#define __DAC_H
#include "sys.h"
extern unsigned short SIN[256];

extern DAC_HandleTypeDef DAC1_Handler;//DAC

extern void DAC1_Init(void);

extern void DAC1_Set_Vol(u16 vol);


extern DMA_HandleTypeDef  DAC1_DMA_Handler;      //DMA??

extern void MX_DMA_Init(void);

extern void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx);

#endif
