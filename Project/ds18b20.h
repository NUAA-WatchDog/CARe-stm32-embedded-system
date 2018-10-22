#ifndef __DS18B20_H
#define __DS18B20_H 

#include "stm32f10x_conf.h"
#include "hw_config.h"
										   
#define	DS18B20_DQ_OUT_HIGH()     GPIO_SetBits(GPIOB, GPIO_Pin_0) 
#define	DS18B20_DQ_OUT_LOW()      GPIO_ResetBits(GPIOB, GPIO_Pin_0) 
 
#define	DS18B20_DQ_IN()             GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) 


void DS18B20_IO_OUT(void);
void DS18B20_IO_IN(void);
uint8_t DS18B20_Init(void);
short DS18B20_Get_Temp(void);  
void DS18B20_Start(void);       //??????
void DS18B20_Write_Byte(u8 dat);//??????
u8 DS18B20_Read_Byte(void);     //??????
u8 DS18B20_Read_Bit(void);      //?????
u8 DS18B20_Check(void);         //??????DS18B20
void DS18B20_Rst(void);         //??DS18B20   

void T_Get_Temp(void);
#endif
