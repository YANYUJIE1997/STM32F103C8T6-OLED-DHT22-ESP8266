/************************************************************************************
*  Copyright (c), 2014, HelTec Automatic Technology co.,LTD.
*            All rights reserved.
*
* Http:    www.heltec.cn
* Email:   cn.heltec@gmail.com
* WebShop: heltec.taobao.com
*
* File name: main.c
* Project  : HelTec.uvprij
* Processor: STM32F103C8T6
* Compiler : MDK fo ARM
* 
* Author : С��
* Version: 1.00
* Date   : 2014.4.8
* Email  : hello14blog@gmail.com
* Modification: none
* 
* Description:128*64�����OLED��ʾ�������ļ����������ڻ����Զ���(heltec.taobao.com)��SD1306����IICͨ�ŷ�ʽ��ʾ��
*
* Others: none;
*
* Function List:
*	1. int main(void);//������
*
* History: none;
*aan 
*************************************************************************************/
#include "stm32f10x.h"
#include "OLED_I2C.h"
#include "delay.h"
#include "stdio.h"
#include "usart.h"
#include "rtc.h"
#include "dht11.h"
#include "sys.h"
#include <string.h>
#include <stdint.h>

//u8 temperature=0; //�¶� 	    
//u8 humidity=0;  
int buf[10]={48,49,50,51,52,53,54,55,56,57};

void GPIO_Configuration(void);


int main(void)
{
	unsigned char i;  
	  u8 t;
	  u8 len;
	  u16 times=0;
	  int a=0;        //��ǰ�յ����ر�־��0��1��
	  int b=0;        //������ؿյ���־��0��1��
	  int c=0;        //ʪ��
	  int d=0;        //ʪ�ȵ�ʮλ����
	  int e=0;        //ʪ�ȵĸ�λ����
	  int f=0;        //�¶�
	  int g=0;        //�¶ȵ�ʮλ����
	  int h=0;        //�¶ȵĸ�λ����
	//extern const unsigned char BMP1[]; 
	u8 temperature;  	    
	u8 humidity; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);
	
	delay_init();
	I2C_Configuration();
	OLED_Init();
	RTC_Init();
	RTC_Set(2019,11,10,19,51,00);
	DHT22_Init();
	delay_ms(1000);
	OLED_CLS();//����
	GPIO_Configuration();
	

while(1) 
		
	{  

			DHT22_Init();
			delay_ms(1000);
			delay_ms(1000);
	    DHT22_Read_Data(&humidity,&temperature);

			OLED_CLS(); 
			for(i=0;i<4;i++)
			{
				OLED_ShowCN(28+i*16,0,i);//������ʾ����
			}
			OLED_ShowCN(0,2,4);//ʪ
			OLED_ShowCN(18,2,6);//��
			OLED_ShowStr(36,2,":",2);
			OLED_ShowStr(100,2,"%RH",2);
			OLED_ShowNum(62,2,humidity,2,16);
			
			OLED_ShowCN(0,4,5);//��
			OLED_ShowCN(18,4,6);//��
			OLED_ShowStr(38,4,":",2);
			OLED_ShowCN(100,4,7);//��
			OLED_ShowNum(62,4,temperature,2,16);

      if(USART_RX_STA&0x8000)       //�ж��Ƿ����������
			{
				len=USART_RX_STA&0x3f;    //�õ��˴ν��յ������ݳ���   ǰ��λΪ״̬λ������Ҫȥ��
				printf("\r\n�����͵���ϢΪ��\r\n\r\n");
				for(t=0;t<len;t++)          //ͨ�����Ƚ���ѭ��
				{
					USART_SendData(USART1,USART_RX_BUF[t]);                 //������ɼ��õ�����һ���򴮿�1����
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);  //�ȴ����ͽ���
				}
				printf("\r\n\r\n");         //���뻻��
				//USART_RX_STA=0;             //��������Ĵ�����Ϊ�´βɼ���׼��
				b=(int)USART_RX_BUF[0];
				USART_SendData(USART1,b);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
				printf("\r\n\r\n");
				if(b==0)
				{
					GPIO_ResetBits(GPIOC,GPIO_Pin_13);
					delay_ms(1000);
				}
				else if(b==1)
				{
					GPIO_SetBits(GPIOC,GPIO_Pin_13);
					delay_ms(1000);
				}
				USART_RX_STA=0;
				/*GPIO_SetBits(GPIOA,GPIO_Pin_1);
				delay_ms(1000);
				GPIO_ResetBits(GPIOA,GPIO_Pin_1);
				delay_ms(1000);*/
			}
			else
			{
				//a=sizeof(humidity);
				//b=sizeof(temperature);
				c=humidity;
				d=c/10;
				e=c%10;
				f=temperature;
				g=f/10;
				h=f%10;
				printf("��ǰʪ��Ϊ��");
				printf("%d",humidity);
				printf("%");
				printf("\r\n\r\n");
				USART_SendData(USART1,buf[d]);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
				delay_ms(10);
				USART_SendData(USART1,buf[e]);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
				delay_ms(10);
				printf("\r\n\r\n");
				printf("��ǰ�¶�Ϊ��");
				printf("%d ��",temperature);
				printf("\r\n\r\n");
				USART_SendData(USART1,buf[g]);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
				delay_ms(10);
				USART_SendData(USART1,buf[h]);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
				delay_ms(10);
				printf("\r\n\r\n");
			}
			delay_ms(1000);
			delay_ms(1000);
			delay_ms(1000);
			delay_ms(1000);
			delay_ms(1000);
			delay_ms(1000);
			OLED_CLS();//����
			for(i=0;i<4;i++)
		{
			OLED_ShowCN(28+i*16,0,i);//������ʾ����
		}
		OLED_ShowNum(30,5,calendar.w_year,4,12);//��ʾASCII�ַ�����ֵ 
		OLED_ShowNum(60,5,calendar.w_month,2,12);
		OLED_ShowNum(78,5,calendar.w_date,2,12);
		OLED_ShowChar(54,5,'-',12);
		OLED_ShowChar(72,5,'-',12);
		if(calendar.min<=9)
		{
			OLED_ShowNum(57,3,0,1,16);
			OLED_ShowNum(65,3,calendar.min,1,16);
		}
		if(calendar.min>=10)
		{
			OLED_ShowNum(57,3,calendar.min,2,16);
		}

		if(calendar.sec<=9)
		{
			OLED_ShowNum(80,3,0,1,16);
			OLED_ShowNum(88,3,calendar.sec,1,16);
		}
		if(calendar.sec>=10)
		{
			OLED_ShowNum(80,3,calendar.sec,2,16);
		}
		OLED_ShowNum(30,3,calendar.hour,2,16);
		OLED_ShowChar(48,3,':',16);
		OLED_ShowChar(72,3,':',16);

		delay_ms(1000);
		
	}
}
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE); 						 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

