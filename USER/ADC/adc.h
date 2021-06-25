#ifndef __ADC_H
#define __ADC_H	
#include "sys.h" 
					   
void Adc_Init(void); 				//ADC通道初始化
u16  Get_Adc(u8 ch); 				//获得某个通道值 
u16 Get_Adc_Average(u8 ch,u8 times);//得到某个通道给定次数采样的平均值  
void NVIC_Configuration(void);				    					    
void DMA_Config(DMA_Stream_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);
void DMA_Enable(DMA_Stream_TypeDef*DMA_CHx,u16 MEM_LEN);
u16 DMA_send_feedback(DMA_Stream_TypeDef* DMA_CHx);
void DMA1_Channel6_IRQHandler(void);
void TIM1_PWM_Init(u16 arr,u16 psc);
#endif 















