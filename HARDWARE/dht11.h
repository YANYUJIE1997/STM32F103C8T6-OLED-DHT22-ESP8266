#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"   

////IO��������											   
#define	DHT22_DQ_OUT PAout(8) //���ݶ˿�	PA0 
#define	DHT22_DQ_IN  PAin(8)  //���ݶ˿�	PA0

u8 DHT22_Init(void);			//��ʼ��DHT22
u8 DHT22_Read_Data(u8 *humi,u8 *temp);	//��ȡ��ʪ��
u8 DHT22_Read_Byte(void);		//����һ���ֽ�
u8 DHT22_Read_Bit(void);		//����һ��λ
u8 DHT22_Check(void);			//����Ƿ����DHT22
void DHT22_Rst(void);			//��λDHT22    
#endif

