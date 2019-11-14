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
* Author : 小林
* Version: 1.00
* Date   : 2014.4.8
* Email  : hello14blog@gmail.com
* Modification: none
* 
* Description:128*64点阵的OLED显示屏测试文件，仅适用于惠特自动化(heltec.taobao.com)的SD1306驱动IIC通信方式显示屏
*
* Others: none;
*
* Function List:
*	1. int main(void);//主函数
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

//u8 temperature=0; //温度 	    
//u8 humidity=0;  
int buf[10]={48,49,50,51,52,53,54,55,56,57};

void GPIO_Configuration(void);


int main(void)
{
	unsigned char i;  
	  u8 t;
	  u8 len;
	  u16 times=0;
	  int a=0;        //当前空调开关标志，0关1开
	  int b=0;        //软件开关空调标志，0关1开
	  int c=0;        //湿度
	  int d=0;        //湿度的十位数字
	  int e=0;        //湿度的个位数字
	  int f=0;        //温度
	  int g=0;        //温度的十位数字
	  int h=0;        //温度的个位数字
	//extern const unsigned char BMP1[]; 
	u8 temperature;  	    
	u8 humidity; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);
	
	delay_init();
	I2C_Configuration();
	OLED_Init();
	RTC_Init();
	RTC_Set(2019,11,10,19,51,00);
	DHT22_Init();
	delay_ms(1000);
	OLED_CLS();//清屏
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
				OLED_ShowCN(28+i*16,0,i);//测试显示中文
			}
			OLED_ShowCN(0,2,4);//湿
			OLED_ShowCN(18,2,6);//度
			OLED_ShowStr(36,2,":",2);
			OLED_ShowStr(100,2,"%RH",2);
			OLED_ShowNum(62,2,humidity,2,16);
			
			OLED_ShowCN(0,4,5);//温
			OLED_ShowCN(18,4,6);//度
			OLED_ShowStr(38,4,":",2);
			OLED_ShowCN(100,4,7);//℃
			OLED_ShowNum(62,4,temperature,2,16);

      if(USART_RX_STA&0x8000)       //判断是否接收完数据
			{
				len=USART_RX_STA&0x3f;    //得到此次接收到的数据长度   前两位为状态位，所以要去掉
				printf("\r\n您发送的信息为：\r\n\r\n");
				for(t=0;t<len;t++)          //通过长度进行循环
				{
					USART_SendData(USART1,USART_RX_BUF[t]);                 //将数组采集好的数据一次向串口1发送
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);  //等待发送结束
				}
				printf("\r\n\r\n");         //插入换行
				//USART_RX_STA=0;             //清零虚拟寄存器，为下次采集做准备
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
				printf("当前湿度为：");
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
				printf("当前温度为：");
				printf("%d 度",temperature);
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
			OLED_CLS();//清屏
			for(i=0;i<4;i++)
		{
			OLED_ShowCN(28+i*16,0,i);//测试显示中文
		}
		OLED_ShowNum(30,5,calendar.w_year,4,12);//显示ASCII字符的码值 
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

