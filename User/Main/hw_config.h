#ifndef __HW_CONFIG_H_
#define __HW_CONFIG_H_

#include "stm32f10x_conf.h"
#include "delay.h"

//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08
 
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //���? 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //���? 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //���? 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //���? 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //���? 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //���? 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //���? 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����
 
   
/*------------------------LED---------------------------*/
#define RCC_APB2Periph_LED	RCC_APB2Periph_GPIOD
#define LED_PORT			GPIOD
#define LED1_PIN			GPIO_Pin_2	  //PD2
#define LED2_PIN			GPIO_Pin_3	  //PD3
#define LED3_PIN			GPIO_Pin_4	  //PD4
#define LED4_PIN			GPIO_Pin_7	  //PD7
#define LED5_PIN			GPIO_Pin_7    //PB7


#define LED1(x)   ((x) ? (GPIO_SetBits(LED_PORT, LED1_PIN)) : (GPIO_ResetBits(LED_PORT, LED1_PIN)));
#define LED2(x)   ((x) ? (GPIO_SetBits(LED_PORT, LED2_PIN)) : (GPIO_ResetBits(LED_PORT, LED2_PIN)));
#define LED3(x)   ((x) ? (GPIO_SetBits(LED_PORT, LED3_PIN)) : (GPIO_ResetBits(LED_PORT, LED3_PIN)));
#define LED4(x)   ((x) ? (GPIO_SetBits(LED_PORT, LED4_PIN)) : (GPIO_ResetBits(LED_PORT, LED4_PIN)));
#define LED5(x)   ((x) ? (GPIO_SetBits(GPIOB, LED5_PIN)) : (GPIO_ResetBits(GPIOB, LED5_PIN)));
#define LEDRed(x)   ((x) ? (GPIO_SetBits(GPIOB, GPIO_Pin_1)) : (GPIO_ResetBits(GPIOB, GPIO_Pin_1)));
#define LEDGreen(x)   ((x) ? (GPIO_SetBits(GPIOB, GPIO_Pin_2) : (GPIO_ResetBits(GPIOB, GPIO_Pin_2)));
#define RELAY1(x) ((x) ? (GPIO_SetBits(GPIOC, GPIO_Pin_4)) : (GPIO_ResetBits(GPIOC, GPIO_Pin_4)));
#define RELAY2(x) ((x) ? (GPIO_SetBits(GPIOC, GPIO_Pin_15)) : (GPIO_ResetBits(GPIOC, GPIO_Pin_15)));
#define Motor_PORT		GPIOB
#define IA_PIN			GPIO_Pin_8    //PB8
#define IB_PIN			GPIO_Pin_9    //PB9
#define IA(x)   ((x) ? (GPIO_SetBits(Motor_PORT, IA_PIN)) : (GPIO_ResetBits(Motor_PORT, IA_PIN)))
#define IB(x)   ((x) ? (GPIO_SetBits(Motor_PORT, IB_PIN)) : (GPIO_ResetBits(Motor_PORT, IB_PIN)))
#define BEEP(x)   ((x) ? (GPIO_SetBits(GPIOC, BEEP_PIN)) : (GPIO_ResetBits(GPIOC, BEEP_PIN)));
#define BEEP_PIN			GPIO_Pin_0	/* PC0 */

/*encrypt*/
#define right true
#define left false

/*
 *3 bytes master command
 *1B bluetooth address
 *2B byte data id
 *3B checksum
 */

 /*
  *4 bytes slave command
  *1B   bluetooth address
  *2B   data id
  *3B   data
  *4B   checksum
  */

#define MasterSlave         0x00
#define Myaddr              (u8)0x01
#define REQ_WeightD         (u8)0x31
#define REQ_AlcoholD   			(u8)0X32
#define REQ_BloodFatD       (u8)0x33
#define REQ_TemperatureD    (u8)0x34
#define REQ_HeartBD         (u8)0x35
#define REQ_HBMAX						(u8)0x36

void GPIO_Configuration(void);
void NVIC_Configuration(void);
void EXTI_Configuration(void);
void TIM_Configuration(void);
void USART_BHT_Configuration(u32 UART3_Baud);
void USART_SendWData(uint8_t did ,   uint32_t data);
void ADC_Configuration(void);
uint16_t Read_ADC1_MultiChannel(u8 channNo);
void	Read_HeartBD_Data(u8	channNo);
void Init_HX711pin(void);
void Delay_Init(void);
void Delay_ms(u16 nms);
void Delay_us(u32 nus);
void Delay_10ms(uint32_t nCount);
void Alcohol_Pin_Init(void);
uint16_t Read_ADC_Ave(u8 channNo);
void ADC_Configuration(void);

#endif
