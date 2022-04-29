#include "dac.h"


DAC_HandleTypeDef DAC1_Handler;//DAC句柄
DMA_HandleTypeDef  DAC1_DMA_Handler; 

//初始化DAC
void DAC1_Init(void)
{
    DAC_ChannelConfTypeDef DACCH1_Config;
    
    DAC1_Handler.Instance=DAC;
		//DAC1_Handler.DMA_Handle1=&DAC1_DMA_Handler;
	
    HAL_DAC_Init(&DAC1_Handler);                 //初始化DAC
    
    DACCH1_Config.DAC_Trigger=DAC_TRIGGER_T2_TRGO;             //不使用触发功能
    DACCH1_Config.DAC_OutputBuffer=DAC_OUTPUTBUFFER_DISABLE;//DAC1输出缓冲关闭
		
    HAL_DAC_ConfigChannel(&DAC1_Handler,&DACCH1_Config,DAC_CHANNEL_1);//DAC通道1配置
	
    //HAL_DAC_Start(&DAC1_Handler,DAC_CHANNEL_1);  //开启DAC通道1，DMA模式下，该函数关闭
	
		HAL_DAC_Start_DMA(&DAC1_Handler,DAC_CHANNEL_1, (uint32_t *)SIN, 256, DAC_ALIGN_12B_R);	
}

//DAC底层驱动，时钟配置，引脚 配置
//此函数会被HAL_DAC_Init()调用
//hdac:DAC句柄
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{      
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_DAC_CLK_ENABLE();             //使能DAC时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_4;            //PA4
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
    GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);


}



//设置通道1输出电压
//vol:0~3300,代表0~3.3V
void DAC1_Set_Vol(u16 vol)
{
	double temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
    HAL_DAC_SetValue(&DAC1_Handler,DAC_CHANNEL_1,DAC_ALIGN_12B_R,temp);//12位右对齐数据格式设置DAC值
}





void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx)
{ 
	
    __HAL_RCC_DMA1_CLK_ENABLE();//DMA1时钟使能 
    __HAL_LINKDMA(&DAC1_Handler,DMA_Handle1,DAC1_DMA_Handler);    //将DMA与DAC1联系起来(发送DMA)
    
    //Tx DMA配置
    DAC1_DMA_Handler.Instance=DMA_Streamx;                            //数据流选择 DMA1_Stream5
    DAC1_DMA_Handler.Init.Channel=chx;                                //通道选择 DMA_CHANNEL_7
		
    DAC1_DMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;             //存储器到外设
    DAC1_DMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
    DAC1_DMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
    DAC1_DMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_HALFWORD;//外设数据长度:8位
    DAC1_DMA_Handler.Init.MemDataAlignment=DMA_PDATAALIGN_HALFWORD;//存储器数据长度:8位
    DAC1_DMA_Handler.Init.Mode=DMA_CIRCULAR;                      //外设循环模式，设置为普通模式波形出不来
    DAC1_DMA_Handler.Init.Priority=DMA_PRIORITY_HIGH;            //中等优先级
    DAC1_DMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;           //关闭FIFO模式
    DAC1_DMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;   //随便设的，因为已经被关掉了
    DAC1_DMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;              //存储器突发单次传输
    DAC1_DMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;           //外设突发单次传输
		
    HAL_DMA_Init(&DAC1_DMA_Handler);
		//HAL_DMA_DeInit(&DAC1_DMA_Handler);
}


///经测试，该函数即对DMA的中断设置可加可不加，注释好像也无影响
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

