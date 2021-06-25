#include "stm32f4xx.h"
#include "mInclude.h"

DMA_InitTypeDef DMA_InitStructure;
u16 DMA1_MEM_LEN;	 /* 保存DMA每次数据传送的长度 */

//初始化ADC															   
void  Adc_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟


  //先初始化ADC1通道5 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;//PA5 通道5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
 	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//关闭连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
	
	ADC_Cmd(ADC1, ENABLE);//开启AD转换器	

}				  
//获得ADC值
//ch: @ref ADC_channels 
//通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
//返回值:转换结果
u16 Get_Adc(u8 ch)   
{
	volatile uint16_t val;
	
	ADC_Cmd(ADC1, ENABLE);//开启AD转换器	
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
	ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	val = ADC_GetConversionValue(ADC1);
	ADC_SoftwareStartConv(ADC1);
	ADC_Cmd(ADC1, DISABLE);//开启AD转换器	
	return val;	//返回最近一次ADC1规则组的转换结果
}
//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
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
	
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);	/* 使能DMA钟源 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);	/* 使能DMA钟源 */
	delay_ms(5);
	
  DMA_DeInit(DMA_CHx);   /* 将DMA的通道1寄存器重设为缺省值 */

	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  /* DMA外设基地址 */
	DMA_InitStructure.DMA_Channel = DMA_Channel_6;
	DMA_InitStructure.DMA_Memory0BaseAddr = cmar;  /* DMA内存基地址 */
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;  /* 数据传输方向，从内存读取发送到外设 */
	DMA_InitStructure.DMA_BufferSize = 5;  /* DMA通道的DMA缓存的大小 */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  /* 外设地址寄存器不变 */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  /* 内存地址寄存器递增 */
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  /* 数据宽度为16位 */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; /* 数据宽度为16位 */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  /* 工作在正常模式 */
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; /* DMA通道 x拥有中优先级 */
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
	DMA_Init(DMA_CHx, &DMA_InitStructure);  	
	
	DMA_ClearFlag(DMA_CHx,DMA_FLAG_TCIF6);
	
	
}
/* 开启一次DMA传输 */
void DMA_Enable(DMA_Stream_TypeDef*DMA_CHx,u16 MEM_LEN)
{
	DMA_Cmd(DMA_CHx, DISABLE );
	
	//TIM1->ARR = 101;	/* 由于最后一项是0，所以在最后的时刻ARR会被清零，导致下一次启动无效。*/
  
	while (DMA_GetCmdStatus(DMA_CHx) != DISABLE){}	//确保DMA可以被设置
		
	DMA_SetCurrDataCounter(DMA_CHx,MEM_LEN);
		
  DMA_Cmd(DMA_CHx, ENABLE);
		
	TIM_Cmd(TIM1, ENABLE);  /* 使能TIM3 */
		
	TIM1->EGR = 0x00000001;
}	  

/*
 *进度反馈，返回剩下的数据量
 */
u16 DMA_send_feedback(DMA_Stream_TypeDef* DMA_CHx)
{
	return DMA_CHx->NDTR;
} 

void TIM1_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	/* 使能定时器3时钟 */
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  /* 使能GPIO外设 */   
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1); //GPIOB7复用为定时器8
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; /* TIM_CH1*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  /* 复用推挽输出 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);/* 初始化GPIO */
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	
   /* 初始化TIM3 */
	TIM_TimeBaseStructure.TIM_Period = arr; /* 设置在下一个更新事件装入活动的自动重装载寄存器周期的值 */
	TIM_TimeBaseStructure.TIM_Prescaler =psc; /* 设置用来作为TIMx时钟频率除数的预分频值 */
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; /* 设置时钟分割:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /*TIM向上计数模式 */
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); /* 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 */
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; /* 选择定时器模式:TIM脉冲宽度调制模式1 */
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; /* 比较输出使能 */
	TIM_OCInitStructure.TIM_Pulse= 50;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; /* 输出极性:TIM输出比较极性高 */
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  /* 根据T指定的参数初始化外设TIM3 OC1 */
	//TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM8在CCR1上的预装载寄存器
  //TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM1, ENABLE);  /* 使能TIM3 */
	//TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_DMACmd(TIM1, TIM_DMA_CC1, ENABLE);
	
}





