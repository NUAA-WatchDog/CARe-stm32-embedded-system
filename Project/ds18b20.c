#include "delay.h"
#include "ds18b20.h"

uint32_t temperature;

void DS18B20_IO_OUT(void){
		{GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);}
}

void DS18B20_IO_IN(void){
		{GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOB, &GPIO_InitStructure);}
}

//??DS18B20
void DS18B20_Rst(void)     
{        
    DS18B20_IO_OUT();
    DS18B20_DQ_OUT_LOW(); 
    Delay_us(750);    
    DS18B20_DQ_OUT_HIGH(); //DQ=1 
    Delay_us(15);     //15US
}


u8 DS18B20_Check(void)     
{   
    u8 retry=0;
    DS18B20_IO_IN();
    while (DS18B20_DQ_IN()&&retry<200)
    {
        retry++;
        Delay_us(1);
    };   
    if(retry>=200)return 1;
    else retry=0;
    while (!DS18B20_DQ_IN()&&retry<240)
    {
        retry++;
        Delay_us(1);
    };
    if(retry>=240)return 1;     
    return 0;
}


u8 DS18B20_Read_Bit(void)            // read one bit
{
    u8 data;
    DS18B20_IO_OUT();//SET PA0 OUTPUT 
    DS18B20_DQ_OUT_LOW(); 
    Delay_us(2);
    DS18B20_DQ_OUT_HIGH() ; 
    DS18B20_IO_IN();//SET PA0 INPUT
    Delay_us(12);
    if(DS18B20_DQ_IN())data=1;
    else data=0;     
    Delay_us(50);           
    return data;
}


u8 DS18B20_Read_Byte(void)    // read one byte
{        
    u8 i,j,dat;
    dat=0;
    for (i=1;i<=8;i++) 
    {
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }                           
    return dat;
}



void DS18B20_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
    DS18B20_IO_OUT();
    for (j=1;j<=8;j++) 
    {
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DS18B20_DQ_OUT_LOW();// Write 1
            Delay_us(2);                            
            DS18B20_DQ_OUT_HIGH();
            Delay_us(60);             
        }
        else 
        {
            DS18B20_DQ_OUT_LOW();// Write 0
            Delay_us(60);             
            DS18B20_DQ_OUT_HIGH();
            Delay_us(2);                          
        }
    }
}

void DS18B20_Start(void)// ds1820 start convert
{                                          
    DS18B20_Rst();     
    DS18B20_Check();     
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
} 
    
u8 DS18B20_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);    //PB10

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;               
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_SetBits(GPIOB,GPIO_Pin_10);    //PB10

    DS18B20_Rst();

    return DS18B20_Check();
}  


short DS18B20_Get_Temp(void)
{
    u8 temp;
    u8 TL,TH;
    short tem;
    DS18B20_Start ();                    // ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();     
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0xbe);// convert     
    TL=DS18B20_Read_Byte(); // LSB   
    TH=DS18B20_Read_Byte(); // MSB  

    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0; 
    }else temp=1;     
    tem=TH; 
    tem<<=8;    
    tem+=TL;
    tem=(float)tem*0.625;   
    if(temp)return tem; 
    else return -tem;    
} 

/****************************************************
*
* Funcition 
*	GetTemp
* Description: NULL
*
****************************************************/

void T_Get_Temp(void){
		temperature = DS18B20_Get_Temp();
		temperature *= 10;
		//tem_qian = temperature/1000;
		//tem_bai = (temperature%1000)/100;
		//tem_shi = ((temperature%1000)%100)/10;
		//tem_ge = temperature%10;
}
