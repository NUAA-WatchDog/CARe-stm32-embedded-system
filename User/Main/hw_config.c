#include "hw_config.h"
#include <stdbool.h>

#define Sample_Num 40
#define ChannelNum 1
#define ADC_DR_Address    ((u32)0x4001244C) // DMA addr


/*To Store the converted value to the array*/
u16 ADC_ConvertedValue[Sample_Num];

/*********************************************
*
*	Function Name: GPIO Configuration
* Description: GPIO Config
*
*
*********************************************/
void GPIO_Configuration(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LED5_PIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	LED1(1);
	LED2(1);
	LED3(1);
	LED4(1);			
	LED5(1);
}

/*********************************************
*
*	Function Name: ADC_DMA Configuration
* Description:DMA Config
*
*
*********************************************/

void ADC_Configuration(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_ADC1,ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//	PA3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	                
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;					    					
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;									
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	            
	ADC_InitStructure.ADC_NbrOfChannel = 1;	                            
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);	                     
	ADC_ResetCalibration(ADC1);	                 
	while(ADC_GetResetCalibrationStatus(ADC1));	
	ADC_StartCalibration(ADC1);			         		 
	while(ADC_GetCalibrationStatus(ADC1));	     
	ADC_RegularChannelConfig(ADC1, 3, 1, ADC_SampleTime_71Cycles5 );
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		 	
}

/*********************************************
*
*	Function Name: Read_ADC_Channel
* Description: NULL
*
*
*********************************************/

uint16_t Read_ADC1_MultiChannel(u8 channNo){
		uint16_t  ADC_data;
    ADC_RegularChannelConfig(ADC1, channNo, 1, ADC_SampleTime_239Cycles5 );
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		 	
    while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));	
    ADC_data=ADC_GetConversionValue(ADC1); 
		ADC_SoftwareStartConvCmd(ADC1, DISABLE);	
		/* ADC_data =	ADC_data>>2; */
		return(ADC_data);
}

/*********************************************
*
*	Function Name: Read ADC Average
* Description: NULL
*
*
*********************************************/

uint16_t Read_ADC_Ave(u8 channNo){
		uint32_t v_temp = 0;
		u8 i;
		for(i = 0; i < Sample_Num; i++){
				v_temp += ADC_ConvertedValue[i];
				Delay_ms(5);
		}
		return v_temp/Sample_Num;
}

/*********************************************
*
*	Function Name: NVIC
* Description: Interruption
*
*
*********************************************/

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			  
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 		  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);  	
	/*TIM1*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/*TIM2*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

/****************************************************
*
* Function Name: Bluetooth Config
* Description: NULL	
* Parameters: 
* @ u32 UART3_Baud 
*
****************************************************/

void USART_BHT_Configuration(u32 UART3_Baud)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	 

	/*USART3	PB10	TX	PB11	RX*/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 ;			
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = UART3_Baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART3, &USART_InitStructure);
	
	USART_Cmd(USART3, ENABLE);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}

/*********************************************
*
*	Function Name: Initialize Weight Sensor
* Description: HX711 Initialization
*
*
*********************************************/

void Init_HX711pin(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 
	//HX711_SCK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOD, &GPIO_InitStructure);					 
	
	//HX711_DOUT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
  
	/*PD11 KEY1 PD12 KEY2*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOD,GPIO_Pin_5);					
}


/****************************************************
*
* Function Name: Alcohol Config
* Description: NULL	
* Parameters: 
* @ void
*
****************************************************/

void Alcohol_Pin_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


/****************************************************
*
* Function Name: USART Tx 
* Description: NULL	
* Parameters: 
* @ uint8_t	did	
* @ uint32_t	data
*
****************************************************/

uint8_t	bdata[6];
uint8_t	tbyte[4];

uint8_t	Key[8] = {5,15,2,4,9,11,16,3};
bool	Direction[8] = {left,left,right,left,right,right,right,left};

void	USART_SendWData(uint8_t	did	,	uint32_t	data){
	int i ,	j;
	
	/*Caesar encrypt*/
	for (i = 0; i < 8; i++)
	{
		if (Direction[i])
			data = ((unsigned int)data >> Key[i]) | (data << (32 - Key[i]));
		else
			data = (data << Key[i]) | ((unsigned int)data >> (32 - Key[i]));
	}
	
	/*Organize data frame*/
	tbyte[0]	=	(uint8_t)(data & 0x000000FF);
	tbyte[1]	=	(uint8_t)((data>>8) & 0x000000FF);
	tbyte[2]	=	(uint8_t)((data>>16)& 0x000000FF);
	tbyte[3]	=	(uint8_t)((data>>24)& 0x000000FF);
	bdata[0]	=	(uint8_t)0x01		;	//1	byte	slave	address
	bdata[1]	=	(uint8_t)(did)	;	//1	byte	data	id
	bdata[2]	=	tbyte[0];	//[7:0]
	bdata[3]	=	tbyte[1];	//[15:8]
	bdata[4]	=	tbyte[2]; //[23:16]
	bdata[5]	=	tbyte[3]; //[31:24]
	
	/*Send data frame*/
	for(i = 0 ; i < 6 ; i++){
		USART_SendData(USART3,(uint8_t)(bdata[i]&0x00FF));
		while(!USART_GetFlagStatus(USART3,USART_FLAG_TXE));
	}
}


void TIM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
						 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0	;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 10*4-1;	//10*3-1
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;//7199
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	/* TIM Interrupts enable */
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

	/* TIM1 enable counter */
	TIM_Cmd(TIM1, ENABLE);

	/* TIM2 base configuration*/
	TIM_TimeBaseStructure.TIM_Period = 10*500-1;	//10*3-1
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;//7199
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* TIM Interrupts enable */
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	/* TIM1 enable counter */
	TIM_Cmd(TIM2, ENABLE);

}

extern	uint16_t	heartbeatreg[128];
extern	uint16_t	heartbeatpt;

uint32_t	HeartBeatAverage(){
	int	tmp	;	
	int	heartbeatsum	=	0;
	int max = 0;
	for(tmp	=	0	;	tmp	<	heartbeatpt;	tmp++){
			heartbeatsum	+=	heartbeatreg[tmp];
	}
	for(tmp = 0 ; tmp < heartbeatpt ; tmp ++){
	
			if(heartbeatreg[tmp] > max)
				max = heartbeatreg[tmp];
	}
	if(heartbeatpt	>	0){
		heartbeatsum -= max;
		heartbeatsum += 60;
		return	heartbeatsum/heartbeatpt;
	}
	else
		return	0;
}

int BPM;                   // used to hold the pulse rate
int Signal;                // holds the incoming raw data
int IBI = 650;             // holds the time between beats, must be seeded! 
unsigned char Pulse = 0;     // true when pulse wave is high, false when it's low
unsigned char QS = 0;        // becomes true when Arduoino finds a beat.
int rate[10];                    // array to hold last ten IBI values
unsigned long sampleCounter = 0;          // used to determine pulse timing
unsigned long lastBeatTime = 0;           // used to find IBI
int P =512;                      // used to find peak in pulse wave, seeded
int T = 512;                     // used to find trough in pulse wave, seeded
int thresh = 512;                // used to find instant moment of heart beat, seeded
int amp = 100;                   // used to hold amplitude of pulse waveform, seeded
int Num;
unsigned char firstBeat = 1;        // used to seed rate array so we startup with reasonable BPM
unsigned char secondBeat = 0;      // used to seed rate array so we startup with reasonable BPM

void	Read_HeartBD_Data(u8 channNo){
	int	i	;
	unsigned int runningTotal;
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		 		
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));	
	Signal=ADC_GetConversionValue(ADC1)>>2;
	sampleCounter += 2;                         // keep track of the time in mS with this variable
	Num = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise

		//  find the peak and trough of the pulse wave
  if(Signal < thresh && Num > (IBI/5)*3){       // avoid dichrotic noise by waiting 3/5 of last IBI
    if (Signal < T){                        // T is the trough
      T = Signal;                         // keep track of lowest point in pulse wave 
    }
  }

  if(Signal > thresh && Signal > P){          // thresh condition helps avoid noise
    P = Signal;                             // P is the peak
  }                                        // keep track of highest point in pulse wave

  //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
  if (Num > 250){                                   // avoid high frequency noise
    if ( (Signal > thresh) && (Pulse == 0) && (Num > (IBI/5)*3) ){        
      Pulse = 1;                               // set the Pulse flag when we think there is a pulse
      //HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);                // turn on pin 13 LED
			GPIOD->ODR	&=	~(1<<2);
      IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
      lastBeatTime = sampleCounter;               // keep track of time for next pulse

      if(secondBeat){                        // if this is the second beat, if secondBeat == TRUE
        secondBeat = 0;                  // clear secondBeat flag
        for(i=0; i<=9; i++){             // seed the running total to get a realisitic BPM at startup
          rate[i] = IBI;                      
        }
      }

      if(firstBeat){                         // if it's the first time we found a beat, if firstBeat == TRUE
        firstBeat = 0;                   // clear firstBeat flag
        secondBeat = 1;                   // set the second beat flag
 //       sei();                               // enable interrupts again
        return;                              // IBI value is unreliable so discard it
      }   

      // keep a running total of the last 10 IBI values
      runningTotal = 0;                  // clear the runningTotal variable    

      for(i=0; i<=8; i++){                // shift data in the rate array
        rate[i] = rate[i+1];                  // and drop the oldest IBI value 
        runningTotal += rate[i];              // add up the 9 oldest IBI values
      }

      rate[9] = IBI;                          // add the latest IBI to the rate array
      runningTotal += rate[9];                // add the latest IBI to runningTotal
      runningTotal /= 10;                     // average the last 10 IBI values 
      BPM = 60000/runningTotal;               // how many beats can fit into a minute? that's BPM!
      QS = 1;                              // set Quantified Self flag 
      // QS FLAG IS NOT CLEARED INSIDE THIS ISR
    }                       
  }

  if (Signal < thresh && Pulse == 1){   // when the values are going down, the beat is over
     //HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);            // turn off pin 13 LED
		GPIOD->ODR	|=	(1<<2);
    Pulse = 0;                         // reset the Pulse flag so we can do it again
    amp = P - T;                           // get amplitude of the pulse wave
    thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
    P = thresh;                            // reset these for next time
    T = thresh;
  }

  if (Num > 2500){                           // if 2.5 seconds go by without a beat
    thresh = 512;                          // set thresh default
    P = 512;                               // set P default
    T = 512;                               // set T default
    lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date        
    firstBeat = 1;                      // set these to avoid noise
    secondBeat = 0;                    // when we get the heartbeat back
  }
}

