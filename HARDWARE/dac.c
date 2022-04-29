#include "dac.h"


DAC_HandleTypeDef DAC1_Handler;//DAC���
DMA_HandleTypeDef  DAC1_DMA_Handler; 

//��ʼ��DAC
void DAC1_Init(void)
{
    DAC_ChannelConfTypeDef DACCH1_Config;
    
    DAC1_Handler.Instance=DAC;
		//DAC1_Handler.DMA_Handle1=&DAC1_DMA_Handler;
	
    HAL_DAC_Init(&DAC1_Handler);                 //��ʼ��DAC
    
    DACCH1_Config.DAC_Trigger=DAC_TRIGGER_T2_TRGO;             //��ʹ�ô�������
    DACCH1_Config.DAC_OutputBuffer=DAC_OUTPUTBUFFER_DISABLE;//DAC1�������ر�
		
    HAL_DAC_ConfigChannel(&DAC1_Handler,&DACCH1_Config,DAC_CHANNEL_1);//DACͨ��1����
	
    //HAL_DAC_Start(&DAC1_Handler,DAC_CHANNEL_1);  //����DACͨ��1��DMAģʽ�£��ú����ر�
	
		HAL_DAC_Start_DMA(&DAC1_Handler,DAC_CHANNEL_1, (uint32_t *)SIN, 256, DAC_ALIGN_12B_R);	
}

//DAC�ײ�������ʱ�����ã����� ����
//�˺����ᱻHAL_DAC_Init()����
//hdac:DAC���
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{      
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_DAC_CLK_ENABLE();             //ʹ��DACʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_4;            //PA4
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
    GPIO_Initure.Pull=GPIO_NOPULL;          //����������
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);


}



//����ͨ��1�����ѹ
//vol:0~3300,����0~3.3V
void DAC1_Set_Vol(u16 vol)
{
	double temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
    HAL_DAC_SetValue(&DAC1_Handler,DAC_CHANNEL_1,DAC_ALIGN_12B_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ
}





void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx)
{ 
	
    __HAL_RCC_DMA1_CLK_ENABLE();//DMA1ʱ��ʹ�� 
    __HAL_LINKDMA(&DAC1_Handler,DMA_Handle1,DAC1_DMA_Handler);    //��DMA��DAC1��ϵ����(����DMA)
    
    //Tx DMA����
    DAC1_DMA_Handler.Instance=DMA_Streamx;                            //������ѡ�� DMA1_Stream5
    DAC1_DMA_Handler.Init.Channel=chx;                                //ͨ��ѡ�� DMA_CHANNEL_7
		
    DAC1_DMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;             //�洢��������
    DAC1_DMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
    DAC1_DMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
    DAC1_DMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_HALFWORD;//�������ݳ���:8λ
    DAC1_DMA_Handler.Init.MemDataAlignment=DMA_PDATAALIGN_HALFWORD;//�洢�����ݳ���:8λ
    DAC1_DMA_Handler.Init.Mode=DMA_CIRCULAR;                      //����ѭ��ģʽ������Ϊ��ͨģʽ���γ�����
    DAC1_DMA_Handler.Init.Priority=DMA_PRIORITY_HIGH;            //�е����ȼ�
    DAC1_DMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;           //�ر�FIFOģʽ
    DAC1_DMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;   //�����ģ���Ϊ�Ѿ����ص���
    DAC1_DMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;              //�洢��ͻ�����δ���
    DAC1_DMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;           //����ͻ�����δ���
		
    HAL_DMA_Init(&DAC1_DMA_Handler);
		//HAL_DMA_DeInit(&DAC1_DMA_Handler);
}


///�����ԣ��ú�������DMA���ж����ÿɼӿɲ��ӣ�ע�ͺ���Ҳ��Ӱ��
void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

unsigned short SIN[256]={2048, 2098, 2148, 2198, 2248, 2298, 2348, 2398, 2447, 2496,
 2545, 2594, 2642, 2690, 2737, 2785, 2831, 2877, 2923, 2968,
 3013, 3057, 3100, 3143, 3185, 3227, 3267, 3307, 3347, 3385,
 3423, 3460, 3496, 3531, 3565, 3598, 3631, 3662, 3692, 3722,
 3750, 3778, 3804, 3829, 3854, 3877, 3899, 3920, 3940, 3958,
 3976, 3992, 4007, 4021, 4034, 4046, 4056, 4065, 4073, 4080,
 4086, 4090, 4093, 4095, 4095, 4095, 4093, 4090, 4086, 4080,
 4073, 4065, 4056, 4046, 4034, 4021, 4007, 3992, 3976, 3958,
 3940, 3920, 3899, 3877, 3854, 3829, 3804, 3778, 3750, 3722,
 3692, 3662, 3631, 3598, 3565, 3531, 3496, 3460, 3423, 3385,
 3347, 3307, 3267, 3227, 3185, 3143, 3100, 3057, 3013, 2968,
 2923, 2877, 2831, 2785, 2737, 2690, 2642, 2594, 2545, 2496,
 2447, 2398, 2348, 2298, 2248, 2198, 2148, 2098, 2047, 1997,
 1947, 1897, 1847, 1797, 1747, 1697, 1648, 1599, 1550, 1501,
 1453, 1405, 1358, 1310, 1264, 1218, 1172, 1127, 1082, 1038,
 995, 952, 910, 868, 828, 788, 748, 710, 672, 635,
 599, 564, 530, 497, 464, 433, 403, 373, 345, 317,
 291, 266, 241, 218, 196, 175, 155, 137, 119, 103,
 88, 74, 61, 49, 39, 30, 22, 15, 9, 5,
 2, 0, 0, 0, 2, 5, 9, 15, 22, 30,
 39, 49, 61, 74, 88, 103, 119, 137, 155, 175,
 196, 218, 241, 266, 291, 317, 345, 373, 403, 433,
 464, 497, 530, 564, 599, 635, 672, 710, 748, 788,
 828, 868, 910, 952, 995, 1038, 1082, 1127, 1172, 1218,
 1264, 1310, 1358, 1405, 1453, 1501, 1550, 1599, 1648, 1697,
 1747, 1797, 1847, 1897, 1947, 1997};

