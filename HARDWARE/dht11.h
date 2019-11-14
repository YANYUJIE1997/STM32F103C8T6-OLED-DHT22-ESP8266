#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"   

////IO操作函数											   
#define	DHT22_DQ_OUT PAout(8) //数据端口	PA0 
#define	DHT22_DQ_IN  PAin(8)  //数据端口	PA0

u8 DHT22_Init(void);			//初始化DHT22
u8 DHT22_Read_Data(u8 *humi,u8 *temp);	//读取温湿度
u8 DHT22_Read_Byte(void);		//读出一个字节
u8 DHT22_Read_Bit(void);		//读出一个位
u8 DHT22_Check(void);			//检测是否存在DHT22
void DHT22_Rst(void);			//复位DHT22    
#endif

