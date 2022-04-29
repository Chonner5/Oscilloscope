#include "adc.h"
#include "delay.h"
#include "graphyt.h"


int Adc_state_flag;                      //ADC״̬��־λ
__IO uint16_t ADC_ConvertedValue[ADC_SIZE];     //ADC����ֵͨ��DMA�����

DMA_HandleTypeDef DMA_Init_Handle;              //DMA���
ADC_HandleTypeDef ADC_Handle;                   //ADC���
ADC_ChannelConfTypeDef ADC_Config;              //ADC��ʼ�����





static void Rheostat_ADC_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // ʹ�� GPIO ʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();//__HAL_RCC_GPIOC_CLK_ENABLE
        
    // ���� IO
    GPIO_InitStructure.Pin = GPIO_PIN_5;    //GPIO_PIN_5
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	   // 
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //������������
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);//GPIOC->GPIOA		
}

static void Rheostat_ADC_Mode_Config(void)
{

    // ------------------DMA Init �ṹ����� ��ʼ��--------------------------
    // ADC1ʹ��DMA2��������0��ͨ��0��������ֲ�̶�����
    // ����DMAʱ��
    __HAL_RCC_DMA2_CLK_ENABLE();//__HAL_RCC_DMA2_CLK_ENABLE
    // ���ݴ���ͨ��
    DMA_Init_Handle.Instance = DMA2_Stream0;//DMA2_Stream0
    // ���ݴ��䷽��Ϊ���赽�洢��	
    DMA_Init_Handle.Init.Direction = DMA_PERIPH_TO_MEMORY;	
    // ����Ĵ���ֻ��һ������ַ���õ���
    DMA_Init_Handle.Init.PeriphInc = DMA_PINC_DISABLE;
    // �洢����ַ����
    DMA_Init_Handle.Init.MemInc = DMA_MINC_ENABLE; 
    // // �������ݴ�СΪ���֣��������ֽ� 
    DMA_Init_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; 
    //	�洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
    DMA_Init_Handle.Init.MemDataAlignment = DMA_PDATAALIGN_HALFWORD;	
    // ѭ������ģʽ
    DMA_Init_Handle.Init.Mode = DMA_CIRCULAR;
    // DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
    DMA_Init_Handle.Init.Priority = DMA_PRIORITY_HIGH;
    // ��ֹDMA FIFO	��ʹ��ֱ��ģʽ
    DMA_Init_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;  
    // FIFO ��С��FIFOģʽ��ֹʱ�������������	
    // DMA_Init_Handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    // DMA_Init_Handle.Init.MemBurst = DMA_MBURST_SINGLE;
    // DMA_Init_Handle.Init.PeriphBurst = DMA_PBURST_SINGLE;  
    // ѡ�� DMA ͨ����ͨ������������
    DMA_Init_Handle.Init.Channel = DMA_CHANNEL_0; //DMA_CHANNEL_0
    //��ʼ��DMA�������൱��һ����Ĺܵ����ܵ������кܶ�ͨ��
    HAL_DMA_Init(&DMA_Init_Handle); 

    
    HAL_DMA_Start(&DMA_Init_Handle,RHEOSTAT_ADC_DR_ADDR,(uint32_t)&ADC_ConvertedValue,ADC_SIZE);
    
    
    

    // ����ADCʱ��
    __HAL_RCC_ADC1_CLK_ENABLE();//__HAL_RCC_ADC1_CLK_ENABLE
    // -------------------ADC Init �ṹ�� ���� ��ʼ��------------------------
    // ADC1
    ADC_Handle.Instance = ADC1;//ADC1
    // ʱ��Ϊfpclk 4��Ƶ	
    ADC_Handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
    // ADC �ֱ���
    ADC_Handle.Init.Resolution = ADC_RESOLUTION_12B;
    // ��ֹɨ��ģʽ����ͨ���ɼ�����Ҫ	
    ADC_Handle.Init.ScanConvMode = DISABLE; 
    // ����ת��	
    ADC_Handle.Init.ContinuousConvMode = DISABLE;     //ENABLE
    // ������ת��	
    ADC_Handle.Init.DiscontinuousConvMode = DISABLE;
    // ������ת������
    ADC_Handle.Init.NbrOfDiscConversion   = 0;
    //��ֹ�ⲿ���ش���    
    ADC_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
    //ʹ������������ⲿ�����������ã�ע�͵�����



    ADC_Handle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;



    //�����Ҷ���	
    ADC_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    //ת��ͨ�� 1��
    ADC_Handle.Init.NbrOfConversion = 1;
    //ʹ������ת������
    ADC_Handle.Init.DMAContinuousRequests = ENABLE;
    //ת����ɱ�־
    ADC_Handle.Init.EOCSelection          = DISABLE;    
    // ��ʼ��ADC	                          
    HAL_ADC_Init(&ADC_Handle);
    //---------------------------------------------------------------------------
    ADC_Config.Channel      = ADC_CHANNEL_5;//ADC_CHANNEL_5
    ADC_Config.Rank         = 1;
    // ����ʱ����	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    ADC_Config.Offset       = 0;
    // ���� ADC ͨ��ת��˳��Ϊ1����һ��ת��������ʱ��Ϊ3��ʱ������
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);

    //HAL_ADC_Start(&ADC_Handle);

    //�ú������Զ����ADC�Ƿ��������û�������Զ���������˾Ͳ���Ҫ����ĺ�����
    HAL_ADC_Start_DMA(&ADC_Handle,(uint32_t*)&ADC_ConvertedValue, ADC_SIZE);
    
    
    Adc_state_flag=1;
}



void Rheostat_Init(void)    //ͳһ��ʼ����main����
{
	Rheostat_ADC_GPIO_Config();
	Rheostat_ADC_Mode_Config();
}

void Open_ADC(void){        //��ADC����
    HAL_ADC_Start_DMA(&ADC_Handle, (uint32_t*)&ADC_ConvertedValue, ADC_SIZE);
    Adc_state_flag=1;
}

void Close_ADC(void){       //  �ر�ADC����
    HAL_ADC_Stop(&ADC_Handle);
    HAL_ADC_Stop_DMA(&ADC_Handle);
    Adc_state_flag=0;
}





