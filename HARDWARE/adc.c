/** 
ADC1的配置完成3通道的数据采集
1 ----------- adc_config();                     //配置adc
 2 ----------- adc_start();                      //启动adc,开始转换
3 ----------- get_ChannelValue(ADC_Channel_x);  //读取相应通道的adc值
**/
#include "stm32f10x.h"
#include "adc.h"
#define CH_NUM  2           //要使用的ADC通道数

static uint16_t ad_value[CH_NUM]={0};
/** 
  adc对应的gpio初始化
  */
void adc_gpio_init(void)
{
    GPIO_InitTypeDef gpio;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    gpio.GPIO_Mode = GPIO_Mode_AIN;                         //配置为模拟输入
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;   //ADC1 --channel0,1,2
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&gpio); 
}
/** 
  * adc配置
  */
void adc_init(void)
{
    ADC_InitTypeDef adc;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);                       //设置ad时钟为72/6=12M,ad最大时钟频率为14M
    
    ADC_DeInit(ADC1);
    adc.ADC_Mode                = ADC_Mode_Independent;     //工作在独立模式
    adc.ADC_ContinuousConvMode  = ENABLE;                   //连续转换模式
    adc.ADC_DataAlign           = ADC_DataAlign_Right;      //数据右对齐
    adc.ADC_ExternalTrigConv    = ADC_ExternalTrigConv_None;//不使用外部触发
    adc.ADC_NbrOfChannel        = CH_NUM ;                       //转换3个通道
    adc.ADC_ScanConvMode        = ENABLE;                   //连续转换模式
    ADC_Init(ADC1,&adc);
    //设定指定的ADC通道组,设置转化顺序和采样时间
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
    //开启adc的DMA
    ADC_DMACmd(ADC1,ENABLE);
    //使能ADC1
    ADC_Cmd(ADC1, ENABLE);
    /* Enable ADC1 reset calibaration register */   
    ADC_ResetCalibration(ADC1);	  //重新校准
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));  //等待重新校准完成
    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1);		//开始校准
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));	   //等待校准完成

}
/** 
配置dma
  */
void adc_dma_init()
{
    DMA_InitTypeDef dma;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    DMA_DeInit(DMA1_Channel1);
    dma.DMA_PeripheralBaseAddr  = (u32)&(ADC1->DR);     //DMA对应的外设基地址
    dma.DMA_MemoryBaseAddr      = (u32)&ad_value;       //内存存储基地址
    dma.DMA_DIR                 = DMA_DIR_PeripheralSRC;//DMA的转换模式为SRC模式，由外设搬移到内存
    dma.DMA_M2M                 = DMA_M2M_Disable;      //M2M模式禁用
    dma.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord;//定义外设数据宽度为16位
    dma.DMA_MemoryDataSize      = DMA_MemoryDataSize_HalfWord;  //DMA搬移数据尺寸，HalfWord就是为16位    
    dma.DMA_BufferSize          = CH_NUM;                       //DMA缓存大小CH_NUM个
    dma.DMA_MemoryInc           = DMA_MemoryInc_Enable;         //接收一次数据后，目标内存地址后移
    dma.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;    //接收一次数据后，设备地址禁止后移
    dma.DMA_Mode                = DMA_Mode_Circular;            //转换模式，循环缓存模式。
    dma.DMA_Priority            = DMA_Priority_High;            //DMA优先级高
    DMA_Init(DMA1_Channel1,&dma); 
}
/** 
完成adc配置
  */
void adc_config(void)
{
    adc_gpio_init();
    adc_dma_init();
    adc_init();
}
/**
启动adc,开始转换
  */
void adc_start(void)
{
    /* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//连续转换开始，ADC通过DMA方式不断的更新RAM区。 
    DMA_Cmd(DMA1_Channel1,ENABLE);    
}

/** 
ADC_Channel_x 的ADC值
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
