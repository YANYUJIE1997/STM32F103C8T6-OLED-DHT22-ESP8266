#ifndef ADC_H_MJL
#define ADC_H_MJL
#include "stm32f10x.h"
void adc_gpio_init(void);
void adc_dma_init(void);
void adc_init(void);
void adc_config(void);
void adc_start(void);
uint16_t get_ChannelVale(uint8_t ADC_Channel);
#endif
