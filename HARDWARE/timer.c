#include "timer.h"
//#include "led.h"
//#include "dac.h"

const float mysample_time[5]={1500000,1000000,500000,200000,100000};//5种采样率单位Mhz 最大1.5Mhz



TIM_HandleTypeDef TIM2_Handler;
//TIM_HandleTypeDef TIM3_Handler;



//通用定时器2初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!( /2)
void TIM2_Init(u16 arr,u16 psc)
{  
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	
    TIM2_Handler.Instance=TIM2;                          //通用定时器2
    TIM2_Handler.Init.Prescaler=psc;                     //分频
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM2_Handler.Init.Period=arr;                        //自动装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
		HAL_TIM_Base_Init(&TIM2_Handler);
		//定时器时钟源配置
		sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		if (HAL_TIM_ConfigClockSource(&TIM2_Handler, &sClockSourceConfig) != HAL_OK)
		{
            //			Error_Handler();
		}
		///下面两个句柄在DMA模式下都需要进行配置，包括上面的时钟源
		sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;		//定时器更新触发
		sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;//定时器主从机模式关闭
		
		if (HAL_TIMEx_MasterConfigSynchronization(&TIM2_Handler, &sMasterConfig) != HAL_OK)
		{
            //			Error_Handler();
		}
    
    HAL_TIM_Base_Start(&TIM2_Handler);      //Time Base Start

}


//90000000/mysample_time[_choseid]-1;
void TIM1_Config(int num)
{
    TIM_HandleTypeDef  TIM1_Handler;
    TIM_OC_InitTypeDef  TIM_OCInitStructure;
    
	/* 使能定时器1 */
   __HAL_RCC_TIM1_CLK_ENABLE(); 	
    
	/* 先禁能再配置 */
	HAL_TIM_PWM_DeInit(&TIM1_Handler);


    /*
     ********************************************************************************

    HCLK = SYSCLK / 1     (AHB1Periph)
    PCLK2 = HCLK / 2      (APB2Periph)
    PCLK1 = HCLK / 4      (APB1Periph)
		SYSCLK(Hz)                              180000000
		HCLK(Hz)                                180000000


    因为APB1 prescaler != 1, 所以 APB1上的TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2;
    因为APB2 prescaler != 1, 所以 APB2上的TIMxCLK = PCLK2 x 2 = SystemCoreClock;

    APB1 定时器有 TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13, TIM14
    APB2 定时器有 TIM1, TIM8 ,TIM9, TIM10, TIM11
          
    TIM1 更新周期是 = TIM1CLK / （TIM_Period + 1）/（TIM_Prescaler + 1）


    APB2 TIMER1 = 180M
    ********************************************************************************
    */
    //TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 	                                   //初始化定时器1的寄存器为复位值
    TIM1_Handler.Instance=TIM1;
	TIM1_Handler.Init.Period =  180000000/mysample_time[num]-1;    //ARR自动重装载寄存器周期的值(定时时间）到设置频率后产生个更新或者中断(也是说定时时间到)
    TIM1_Handler.Init.Prescaler = 0;   		   //PSC时钟预分频数 例如：时钟频率=TIM1CLK/(时钟预分频+1)
    TIM1_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;    	            //CR1->CKD时间分割值  #define TIM_CKD_DIV1                       ((uint16_t)0x0000)
    TIM1_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;     //CR1->CMS[1:0]和DIR定时器模式 向上计数
	TIM1_Handler.Init.RepetitionCounter = 0x0000;		    /* TIM1 和 TIM8 必须设置 */	
    //---------HAL_TIM_PWM_Init(&TIM1_Handler);
    HAL_TIM_Base_Init(&TIM1_Handler);


    /**************ADC1的触发***********************************************/
    TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1; 				   //CCMR2在向上计数时，一旦TIMx_CNT<TIMx_CCR1时通道1为有效电平，否则为无效电平
    TIM_OCInitStructure.Pulse = TIM1_Handler.Init.Period/2;//CCR3同计数器TIMx_CNT的比较，并在OC1端口上产生输出信号 
    TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_LOW;    	   //CCER输出极性设置
    //TIM_OCInitStructure.OCFastMode = TIM_OCFAST_ENABLE;	   /* only for TIM1 and TIM8. 此处和正相引脚不同 */	
	TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_HIGH;		   /* only for TIM1 and TIM8. */		 
	TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_RESET;	   /* only for TIM1 and TIM8. */
	TIM_OCInitStructure.OCNIdleState = TIM_OCNIDLESTATE_RESET;	   /* only for TIM1 and TIM8. */	
    //HAL_TIM_OC_Init(&TIM1_Handler);//原来的
	//-------HAL_TIM_PWM_ConfigChannel(&TIM1_Handler,&TIM_OCInitStructure,TIM_CHANNEL_1);

    HAL_TIM_OC_ConfigChannel(&TIM1_Handler,&TIM_OCInitStructure,TIM_CHANNEL_1);

    //HAL_TIM_Base_Start(&TIM1_Handler);	
	//HAL_TIM_Base_Init(&TIM1_Handler);


    /* 使能PWM输出 */	
    HAL_TIM_PWM_Start(&TIM1_Handler,TIM_CHANNEL_1);  		
}



//定时器底册驱动，开启时钟
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM2)
    {
        __HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
    }
	if(htim->Instance==TIM1)
    {
        __HAL_RCC_TIM1_CLK_ENABLE();            //使能TIM1时钟
    }    


}

