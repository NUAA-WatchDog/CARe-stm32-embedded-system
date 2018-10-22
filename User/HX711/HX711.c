#include "HX711.h"
#include "delay.h"
u32 HX711_Buffer;
u32 Weight_Error;
u32 Weight_Data = 0;
u8 Flag_Error = 0;

#define GapValue 40.5

u32 HX711_Read(void)
{
	unsigned long count; 
	unsigned char i;
	GPIO_SetBits(GPIOD,GPIO_Pin_6);	
	Delay_us(1);
	GPIO_ResetBits(GPIOD,GPIO_Pin_5);
  count=0; 
  while((GPIOD->IDR&(1<<6))); 
  for(i=0;i<24;i++)
	{ 
		GPIO_SetBits(GPIOD,GPIO_Pin_5);	
	  count=count<<1; 
		Delay_us(1);
		GPIO_ResetBits(GPIOD,GPIO_Pin_5);	
	  if((GPIOD->IDR&(1<<6)))
			count++; 
		Delay_us(1);
	} 
	GPIO_SetBits(GPIOD,GPIO_Pin_5);
  count=count^0x800000;
	Delay_us(1);
	GPIO_ResetBits(GPIOD,GPIO_Pin_5);
	return(count);
}

void Eliminate_Error(void)
{
	Weight_Error = HX711_Read();	
} 

void Get_Weight(void)
{
	HX711_Buffer = HX711_Read();
	if(HX711_Buffer > Weight_Error)			
	{
		Weight_Data = HX711_Buffer;
		Weight_Data = Weight_Data - Weight_Error;
		Weight_Data = (u32)((float)Weight_Data/GapValue);
	}

	
}
