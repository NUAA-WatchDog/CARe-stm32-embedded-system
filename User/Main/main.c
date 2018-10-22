/********************************
			NUAA	WD
*********************************/
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "delay.h"
#include "hw_config.h"
#include "../HX711/HX711.h"
#include "../Lcd/lcd.h"
#include "ds18b20.h"
#include "../MAX30102/max_30102.h"
#include "../MAX30102/bsp_i2c_gpio.h"

#define U3Baud   9600


#define Y1  0.00
#define Ym1  100.00
#define N1  2483.0
#define Nm1  3420.0
#define Y2  0.00
#define Ym2  1000.00
#define N2  792.0
#define Nm2  3969.0


int i = 0 ;

/*USART*/
uint8_t ReceiveFlag=0;
extern u16 INDEX;
extern u8 USART_RX_BUF[10];
extern u8 USART_TX_BUF[10];

/****************************************************
*
* Variable Definition
*	HeartBeat Part
* Description: NULL
*
****************************************************/
extern	int	BPM	;
extern	unsigned char QS	;

int OBPM = 70;
float a = 0.6;
uint16_t	heartbeatreg[1024]	=	{0};
uint16_t	heartbeatpt	=	0	;
uint32_t	heartbeat	=	0	;
uint32_t  pheartbeat = 0;

extern	uint32_t	HeartBeatAverage();

/*Weight*/
uint8_t butkey = 0;

/****************************************************
*
* Variable Definition
* Description: NULL
*
****************************************************/
u32 value;
float T_value = 37.0;
extern uint32_t temperature;

/*Alcohol*/
uint8_t alcohol[1024];
uint8_t alcoholpt = 0;
uint32_t alcohol_start = 0;
uint32_t alcohol_but = 0;
uint32_t n_cnt ;
float alcohol_rate = 0.0;

/*max30102*/
u8	hb_max30102[1024] = {0};
uint16_t hb_pt = 0;
uint32_t hb_max30102_avg();

int main(void)
{			
	SystemInit();
	Delay_Init();
	LCD_Init();
	Alcohol_Pin_Init();
	Init_HX711pin();
	ADC_Configuration();
	TIM_Configuration();
	USART_BHT_Configuration(U3Baud);
	NVIC_Configuration();
	GPIO_Configuration();
//	bsp_InitI2C();
//	max30102_init();
	
	DrawTestPage("soft bei");
	Show_Str(0,80,GREEN,BLACK, "HeartBD_Data:",24,1);
  Show_Str(0,100,GREEN,BLACK,"Temperature:",24,1);
  Show_Str(0,120,GREEN,BLACK,"Weight:",24,1);
	Show_Str(0,140,GREEN,BLACK,"Alcohol:",24,1);
	
	/*Weight Error Erasiing*/
	Eliminate_Error();				//get weight0
	Delay_ms(1000);
	Delay_ms(1000);
	Eliminate_Error();				// get weight0

	while(1)
	{	
		if((GPIOD->IDR&(1<<11)) == 0) butkey = 1;
		else butkey = 0;
		
		if((GPIOD->IDR&(1<<12)) == 0) alcohol_but = 1;
		else alcohol_but = 0;
		
		if(butkey){
			Get_Weight();
			LCD_ShowNum(160,120,Weight_Data,7,16);
			Delay_ms(200);
			butkey = 0 ;
		}
		
		if(alcohol_start && !alcohol_but){
			if((GPIOC->IDR&(1<<7)) == 0) alcohol[alcoholpt] = 1;
			else alcohol[alcoholpt] = 0 ;
			alcoholpt  = (alcoholpt + 1)%1024;
			Delay_ms(10);
		}
		else if(alcohol_start && alcohol_but){
			 for(i = 0 ; i < alcoholpt ; i++){
				 if(alcohol[i]) n_cnt ++;
			 }
			 alcohol_rate = (n_cnt+0.0)/alcoholpt;
			 if(alcohol_rate > 0.3 ) USART_SendWData(REQ_AlcoholD,1);
			 else USART_SendWData(REQ_AlcoholD,0);
			 LCD_ShowNum(180,140,alcoholpt,5,16);
			 Delay_ms(200);
			 alcohol_start = 0;
			 alcohol_but = 0;
		}
		
		if(ReceiveFlag	==	1){
			/*check	address*/
			if(USART_RX_BUF[0]	==	(u8)0x31){
				switch(USART_RX_BUF[1]){
					
					case	REQ_WeightD:
								USART_SendWData(REQ_WeightD,Weight_Data);
								ReceiveFlag	=	0	;
								break;
					
					case	REQ_AlcoholD:
								alcohol_start = 1;
					      
								ReceiveFlag	=	0	;
								break;
								
					case	REQ_TemperatureD:
								T_Get_Temp();
								if(temperature < 25) temperature = 25*100;
								USART_SendWData(REQ_TemperatureD, temperature);
								LCD_ShowNum(180,100,temperature,5,16);
								ReceiveFlag = 0;
								break;

					case	REQ_HeartBD:
								heartbeat	=	HeartBeatAverage();
								if(heartbeat != 0){
									pheartbeat = heartbeat;
									USART_SendWData(REQ_HeartBD,heartbeat);
									Show_HeartBD_Data(heartbeatpt);
									heartbeatpt	=	0	;
								}else{
									heartbeat = pheartbeat;
									USART_SendWData(REQ_HeartBD,heartbeat);
									Show_HeartBD_Data(heartbeatpt);
									heartbeatpt	=	0	;
								}
								ReceiveFlag	=	0	;
								break;
								
					case	REQ_HBMAX:
								heartbeat = hb_max30102_avg();
								USART_SendWData(REQ_HBMAX,heartbeat);
								Show_HeartBD_Data(heartbeatpt);
								hb_pt = 0;
								ReceiveFlag	=	0	;
					      break;

					default:break;
				}
			}
		}
		else{
			if(QS){ 
			    QS	=	0	;
					if(BPM > 150 || ((BPM - OBPM) >= 30 )) heartbeatreg[heartbeatpt] = OBPM;
					else{
							heartbeatreg[heartbeatpt]	=	(a*BPM + (1-a)*OBPM);
							OBPM = BPM;
					}
					heartbeatpt++;
					heartbeatpt	=	heartbeatpt%1024;
			}
		}
	}
}


uint32_t hb_max30102_avg(){
	int i;
	uint32_t total = 0;
	for(i = 0 ; i < hb_pt ; i++){
		total += hb_max30102[i];
	}
	return (hb_pt != 0) ? total/hb_pt : 0;
}
