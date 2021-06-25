#include "stm32f4xx.h"
#include "mInclude.h"

DMA_InitTypeDef DMA_InitStructure;
u16 DMA1_MEM_LEN;	 /* ����DMAÿ�����ݴ��͵ĳ��� */

//��ʼ��ADC															   
void  Adc_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��


  //�ȳ�ʼ��ADC1ͨ��5 IO��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;//PA5 ͨ��5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
 	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//�ر�����ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;//�Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	
	ADC_Cmd(ADC1, ENABLE);//����ADת����	

}				  
//���ADCֵ
//ch: @ref ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_Channel_0~ADC_Channel_16
//����ֵ:ת�����
u16 Get_Adc(u8 ch)   
{
	volatile uint16_t val;
	
	ADC_Cmd(ADC1, ENABLE);//����ADת����	
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
	ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	val = ADC_GetConversionValue(ADC1);
	ADC_SoftwareStartConv(ADC1);
	ADC_Cmd(ADC1, DISABLE);//����ADת����	
	return val;	//�������һ��ADC1�������ת�����
}
//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 
	 
void DMA_Config(DMA_Stream_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);	/* ʹ��DMA��Դ */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);	/* ʹ��DMA��Դ */
	delay_ms(5);
	
  DMA_DeInit(DMA_CHx);   /* ��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ */

	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  /* DMA�������ַ */
	DMA_InitStructure.DMA_Channel = DMA_Channel_6;
	DMA_InitStructure.DMA_Memory0BaseAddr = cmar;  /* DMA�ڴ����ַ */
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;  /* ���ݴ��䷽�򣬴��ڴ��ȡ���͵����� */
	DMA_InitStructure.DMA_BufferSize = 5;  /* DMAͨ����DMA����Ĵ�С */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  /* �����ַ�Ĵ������� */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  /* �ڴ��ַ�Ĵ������� */
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  /* ���ݿ��Ϊ16λ */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; /* ���ݿ��Ϊ16λ */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  /* ����������ģʽ */
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; /* DMAͨ�� xӵ�������ȼ� */
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
	DMA_Init(DMA_CHx, &DMA_InitStructure);  	
	
	DMA_ClearFlag(DMA_CHx,DMA_FLAG_TCIF6);
	
	
}
/* ����һ��DMA���� */
void DMA_Enable(DMA_Stream_TypeDef*DMA_CHx,u16 MEM_LEN)
{
	DMA_Cmd(DMA_CHx, DISABLE );
	
	//TIM1->ARR = 101;	/* �������һ����0������������ʱ��ARR�ᱻ���㣬������һ��������Ч��*/
  
	while (DMA_GetCmdStatus(DMA_CHx) != DISABLE){}	//ȷ��DMA���Ա�����
		
	DMA_SetCurrDataCounter(DMA_CHx,MEM_LEN);
		
  DMA_Cmd(DMA_CHx, ENABLE);
		
	TIM_Cmd(TIM1, ENABLE);  /* ʹ��TIM3 */
		
	TIM1->EGR = 0x00000001;
}	  

/*
 *���ȷ���������ʣ�µ�������
 */
u16 DMA_send_feedback(DMA_Stream_TypeDef* DMA_CHx)
{
	return DMA_CHx->NDTR;
} 

void TIM1_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	/* ʹ�ܶ�ʱ��3ʱ�� */
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  /* ʹ��GPIO���� */   
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1); //GPIOB7����Ϊ��ʱ��8
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; /* TIM_CH1*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  /* ����������� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);/* ��ʼ��GPIO */
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	
   /* ��ʼ��TIM3 */
	TIM_TimeBaseStructure.TIM_Period = arr; /* ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ */
	TIM_TimeBaseStructure.TIM_Prescaler =psc; /* ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ */
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; /* ����ʱ�ӷָ�:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /*TIM���ϼ���ģʽ */
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); /* ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ */
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; /* ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1 */
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; /* �Ƚ����ʹ�� */
	TIM_OCInitStructure.TIM_Pulse= 50;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; /* �������:TIM����Ƚϼ��Ը� */
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  /* ����Tָ���Ĳ�����ʼ������TIM3 OC1 */
	//TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM8��CCR1�ϵ�Ԥװ�ؼĴ���
  //TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM1, ENABLE);  /* ʹ��TIM3 */
	//TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_DMACmd(TIM1, TIM_DMA_CC1, ENABLE);
	
}





