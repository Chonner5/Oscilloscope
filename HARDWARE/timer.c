#include "timer.h"
//#include "led.h"
//#include "dac.h"

const float mysample_time[5]={1500000,1000000,500000,200000,100000};//5�ֲ����ʵ�λMhz ���1.5Mhz



TIM_HandleTypeDef TIM2_Handler;
//TIM_HandleTypeDef TIM3_Handler;



//ͨ�ö�ʱ��2��ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!( /2)
void TIM2_Init(u16 arr,u16 psc)
{  
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	
    TIM2_Handler.Instance=TIM2;                          //ͨ�ö�ʱ��2
    TIM2_Handler.Init.Prescaler=psc;                     //��Ƶ
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM2_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
		HAL_TIM_Base_Init(&TIM2_Handler);
		//��ʱ��ʱ��Դ����
		sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		if (HAL_TIM_ConfigClockSource(&TIM2_Handler, &sClockSourceConfig) != HAL_OK)
		{
            //			Error_Handler();
		}
		///�������������DMAģʽ�¶���Ҫ�������ã����������ʱ��Դ
		sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;		//��ʱ�����´���
		sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;//��ʱ�����ӻ�ģʽ�ر�
		
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
    
	/* ʹ�ܶ�ʱ��1 */
   __HAL_RCC_TIM1_CLK_ENABLE(); 	
    
	/* �Ƚ��������� */
	HAL_TIM_PWM_DeInit(&TIM1_Handler);


    /*
     ********************************************************************************

    HCLK = SYSCLK / 1     (AHB1Periph)
    PCLK2 = HCLK / 2      (APB2Periph)
    PCLK1 = HCLK / 4      (APB1Periph)
		SYSCLK(Hz)                              180000000
		HCLK(Hz)                                180000000


    ��ΪAPB1 prescaler != 1, ���� APB1�ϵ�TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2;
    ��ΪAPB2 prescaler != 1, ���� APB2�ϵ�TIMxCLK = PCLK2 x 2 = SystemCoreClock;

    APB1 ��ʱ���� TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13, TIM14
    APB2 ��ʱ���� TIM1, TIM8 ,TIM9, TIM10, TIM11
          
    TIM1 ���������� = TIM1CLK / ��TIM_Period + 1��/��TIM_Prescaler + 1��


    APB2 TIMER1 = 180M
    ********************************************************************************
    */
    //TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 	                                   //��ʼ����ʱ��1�ļĴ���Ϊ��λֵ
    TIM1_Handler.Instance=TIM1;
	TIM1_Handler.Init.Period =  180000000/mysample_time[num]-1;    //ARR�Զ���װ�ؼĴ������ڵ�ֵ(��ʱʱ�䣩������Ƶ�ʺ���������»����ж�(Ҳ��˵��ʱʱ�䵽)
    TIM1_Handler.Init.Prescaler = 0;   		   //PSCʱ��Ԥ��Ƶ�� ���磺ʱ��Ƶ��=TIM1CLK/(ʱ��Ԥ��Ƶ+1)
    TIM1_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;    	            //CR1->CKDʱ��ָ�ֵ  #define TIM_CKD_DIV1                       ((uint16_t)0x0000)
    TIM1_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;     //CR1->CMS[1:0]��DIR��ʱ��ģʽ ���ϼ���
	TIM1_Handler.Init.RepetitionCounter = 0x0000;		    /* TIM1 �� TIM8 �������� */	
    //---------HAL_TIM_PWM_Init(&TIM1_Handler);
    HAL_TIM_Base_Init(&TIM1_Handler);


    /**************ADC1�Ĵ���***********************************************/
    TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1; 				   //CCMR2�����ϼ���ʱ��һ��TIMx_CNT<TIMx_CCR1ʱͨ��1Ϊ��Ч��ƽ������Ϊ��Ч��ƽ
    TIM_OCInitStructure.Pulse = TIM1_Handler.Init.Period/2;//CCR3ͬ������TIMx_CNT�ıȽϣ�����OC1�˿��ϲ�������ź� 
    TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_LOW;    	   //CCER�����������
    //TIM_OCInitStructure.OCFastMode = TIM_OCFAST_ENABLE;	   /* only for TIM1 and TIM8. �˴����������Ų�ͬ */	
	TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_HIGH;		   /* only for TIM1 and TIM8. */		 
	TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_RESET;	   /* only for TIM1 and TIM8. */
	TIM_OCInitStructure.OCNIdleState = TIM_OCNIDLESTATE_RESET;	   /* only for TIM1 and TIM8. */	
    //HAL_TIM_OC_Init(&TIM1_Handler);//ԭ����
	//-------HAL_TIM_PWM_ConfigChannel(&TIM1_Handler,&TIM_OCInitStructure,TIM_CHANNEL_1);

    HAL_TIM_OC_ConfigChannel(&TIM1_Handler,&TIM_OCInitStructure,TIM_CHANNEL_1);

    //HAL_TIM_Base_Start(&TIM1_Handler);	
	//HAL_TIM_Base_Init(&TIM1_Handler);


    /* ʹ��PWM��� */	
    HAL_TIM_PWM_Start(&TIM1_Handler,TIM_CHANNEL_1);  		
}



//��ʱ���ײ�����������ʱ��
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM2)
    {
        __HAL_RCC_TIM2_CLK_ENABLE();            //ʹ��TIM2ʱ��
    }
	if(htim->Instance==TIM1)
    {
        __HAL_RCC_TIM1_CLK_ENABLE();            //ʹ��TIM1ʱ��
    }    


}

