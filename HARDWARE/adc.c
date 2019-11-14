/** 
ADC1���������3ͨ�������ݲɼ�
1 ----------- adc_config();                     //����adc
 2 ----------- adc_start();                      //����adc,��ʼת��
3 ----------- get_ChannelValue(ADC_Channel_x);  //��ȡ��Ӧͨ����adcֵ
**/
#include "stm32f10x.h"
#include "adc.h"
#define CH_NUM  2           //Ҫʹ�õ�ADCͨ����

static uint16_t ad_value[CH_NUM]={0};
/** 
  adc��Ӧ��gpio��ʼ��
  */
void adc_gpio_init(void)
{
    GPIO_InitTypeDef gpio;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    gpio.GPIO_Mode = GPIO_Mode_AIN;                         //����Ϊģ������
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;   //ADC1 --channel0,1,2
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&gpio); 
}
/** 
  * adc����
  */
void adc_init(void)
{
    ADC_InitTypeDef adc;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);                       //����adʱ��Ϊ72/6=12M,ad���ʱ��Ƶ��Ϊ14M
    
    ADC_DeInit(ADC1);
    adc.ADC_Mode                = ADC_Mode_Independent;     //�����ڶ���ģʽ
    adc.ADC_ContinuousConvMode  = ENABLE;                   //����ת��ģʽ
    adc.ADC_DataAlign           = ADC_DataAlign_Right;      //�����Ҷ���
    adc.ADC_ExternalTrigConv    = ADC_ExternalTrigConv_None;//��ʹ���ⲿ����
    adc.ADC_NbrOfChannel        = CH_NUM ;                       //ת��3��ͨ��
    adc.ADC_ScanConvMode        = ENABLE;                   //����ת��ģʽ
    ADC_Init(ADC1,&adc);
    //�趨ָ����ADCͨ����,����ת��˳��Ͳ���ʱ��
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
    //����adc��DMA
    ADC_DMACmd(ADC1,ENABLE);
    //ʹ��ADC1
    ADC_Cmd(ADC1, ENABLE);
    /* Enable ADC1 reset calibaration register */   
    ADC_ResetCalibration(ADC1);	  //����У׼
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));  //�ȴ�����У׼���
    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1);		//��ʼУ׼
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));	   //�ȴ�У׼���

}
/** 
����dma
  */
void adc_dma_init()
{
    DMA_InitTypeDef dma;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    DMA_DeInit(DMA1_Channel1);
    dma.DMA_PeripheralBaseAddr  = (u32)&(ADC1->DR);     //DMA��Ӧ���������ַ
    dma.DMA_MemoryBaseAddr      = (u32)&ad_value;       //�ڴ�洢����ַ
    dma.DMA_DIR                 = DMA_DIR_PeripheralSRC;//DMA��ת��ģʽΪSRCģʽ����������Ƶ��ڴ�
    dma.DMA_M2M                 = DMA_M2M_Disable;      //M2Mģʽ����
    dma.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord;//�����������ݿ��Ϊ16λ
    dma.DMA_MemoryDataSize      = DMA_MemoryDataSize_HalfWord;  //DMA�������ݳߴ磬HalfWord����Ϊ16λ    
    dma.DMA_BufferSize          = CH_NUM;                       //DMA�����СCH_NUM��
    dma.DMA_MemoryInc           = DMA_MemoryInc_Enable;         //����һ�����ݺ�Ŀ���ڴ��ַ����
    dma.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;    //����һ�����ݺ��豸��ַ��ֹ����
    dma.DMA_Mode                = DMA_Mode_Circular;            //ת��ģʽ��ѭ������ģʽ��
    dma.DMA_Priority            = DMA_Priority_High;            //DMA���ȼ���
    DMA_Init(DMA1_Channel1,&dma); 
}
/** 
���adc����
  */
void adc_config(void)
{
    adc_gpio_init();
    adc_dma_init();
    adc_init();
}
/**
����adc,��ʼת��
  */
void adc_start(void)
{
    /* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//����ת����ʼ��ADCͨ��DMA��ʽ���ϵĸ���RAM���� 
    DMA_Cmd(DMA1_Channel1,ENABLE);    
}

/** 
ADC_Channel_x ��ADCֵ
  */
uint16_t get_ChannelVale(uint8_t ADC_Channel)
{
    uint16_t ret = 0;
    switch(ADC_Channel)
    {
        case ADC_Channel_0:
            ret = ad_value[0];
            break;
        case ADC_Channel_1:
            ret = ad_value[1];
            break;
        
    }
    return ret;
}
