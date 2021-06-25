#include "mInclude.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx.h"
////////////
#define size 9601
#define psc_init 839
#define arr_init 0
cyc_task_fun cycTask[]={cyc2ms,cyc4ms,cyc20ms,cyc100ms,cyc500ms,cycLongTsk};
//////////Delare/////////
SysParameter_type gSystemPara;
gMachineIO_type gMachineIO;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

u16 send_buf[100];
static uint64_t Encoder_TIM;
/////for LED/////
uint8_t cntFlash,cntFlash500;
bool flashLED[10];
/////for SystemPara/////
uint8_t Parameter[800] = {0};
uint8_t ReadTemp[800] = {0};
/////for bauderate/////
uint32_t ComBauderate[4] = {9600,19200,38400,115200};

////////////////////
#define Uart6RS485RE SetDO(RS485DE,0)
#define Uart6RS485SE SetDO(RS485DE,1)

#define Uart1RS485RE SetDO(RS485DE2,0)
#define Uart1RS485SE SetDO(RS485DE2,1)

void TIM5_IRQHandler(void){
//	gMachineIO.oldEncoder[0] = gMachineIO.oldEncoder[0];
	TIM_ClearITPendingBit(TIM5,TIM_FLAG_Update);
	if(TIM5->CNT > 32767)
		gMachineIO.exeedTimes--;
	else
		gMachineIO.exeedTimes++;
}



void Encoder_Init_TIM5(void)
{
	GPIO_InitTypeDef            GPIO_Initstruc;
	TIM_TimeBaseInitTypeDef     TIM_TimeBaseInit_Struc;
	TIM_ICInitTypeDef           TIM_ICInit_Struc;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);// 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//  GPIOA????

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);//??PA0?PA1?????

	GPIO_Initstruc.GPIO_Mode=GPIO_Mode_AF;
	GPIO_Initstruc.GPIO_OType=GPIO_OType_PP;
	GPIO_Initstruc.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_Initstruc.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Initstruc.GPIO_Speed=GPIO_Speed_100MHz;

	GPIO_Init(GPIOA,&GPIO_Initstruc);

	TIM_TimeBaseStructInit(&TIM_TimeBaseInit_Struc);

	TIM_TimeBaseInit_Struc.TIM_Prescaler=0x00;                  //??????
	TIM_TimeBaseInit_Struc.TIM_Period=0xffffffff;                     //??????????
	TIM_TimeBaseInit_Struc.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit_Struc.TIM_CounterMode=TIM_CounterMode_Up;


	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInit_Struc);

	TIM_EncoderInterfaceConfig(TIM5,TIM_EncoderMode_TI12,
	TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);          //??????Tim5?TI1????


	TIM_ICStructInit(&TIM_ICInit_Struc);                         //?????????
	TIM_ICInit_Struc.TIM_Channel = TIM_Channel_1;
	TIM_ICInit_Struc.TIM_ICFilter = 0x00;
	TIM_ICInit_Struc.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInit_Struc.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInit_Struc.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM5,&TIM_ICInit_Struc);

	TIM_ICInit_Struc.TIM_Channel = TIM_Channel_2;
	TIM_ICInit_Struc.TIM_ICFilter = 0x00;
	TIM_ICInit_Struc.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInit_Struc.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInit_Struc.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM5,&TIM_ICInit_Struc);

	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01;
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;
	

	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ClearFlag(TIM5,TIM_FLAG_Update);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	TIM5->CNT=0;
	TIM_Cmd(TIM5,ENABLE);//?????5
}

/******************************************
???????????,?????
********************************************/

void SystemConfig()
{
	delay_init(168);
	HwCfgInit();
};


void HwCfgInit()
{
	/////////////initial output///////////
	CfgPINOut(DO1);
	CfgPINOut(DO2);
	CfgPINOut(DO3);
	CfgPINOut(DO4);
	CfgPINOut(DO5);
	CfgPINOut(DO6);
	CfgPINOut(DO7);
	CfgPINOut(DO8);
	CfgPINOut(DO9);
	CfgPINOut(DO10);
	CfgPINOut(DO11);
	CfgPINOut(DO12);

	CfgPINOut(DOLED);	

	CfgPINOut(SPI1NSS);	
	CfgPINOut(RS485DE);	
	CfgPINOut(RS485DE2);	
	
	CfgPINIn(DI1);
	CfgPINIn(DI2);
	CfgPINIn(DI3);
	CfgPINIn(DI4);
	CfgPINIn(DI5);
	CfgPINIn(DI6);
	CfgPINIn(DI7);
	CfgPINIn(DI8);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	////////////
	SPI1_Init();

	//LATCH_HIGH;//stop communication
	/////////////Obtain the buffParameter/////////
	STMFLASH_Read(FLASH_SAVE_ADDR,( uint32_t * )ReadTemp,800);
	/////////////
	if(0xAB==ReadTemp[0]&&0xCD==ReadTemp[799]){
		memcpy(&gSystemPara,&ReadTemp,sizeof(gSystemPara));
	}
	else{
		////////////////////////
		gSystemPara.BufferHead = 0xAB;
		gSystemPara.CANBusBauderate = 1;
		gSystemPara.CanNode = 0x100;
		gSystemPara.RS232Bauderate = 0;
		gSystemPara.RS485Bauderate = 0;
		gSystemPara.RS485Node = 10;
		gSystemPara.AOMode = 5;
		gSystemPara.BoardType = 0;
		memcpy(&Parameter,&gSystemPara,sizeof(gSystemPara));
		Parameter[799] = 0xCD;
		STMFLASH_Write(FLASH_SAVE_ADDR,( uint32_t * )Parameter,800);
	}
	
	////////////
	gSystemPara.CANBusBauderate = 1; // 250k
	
	if(gSystemPara.RS232Bauderate>3)
		gSystemPara.RS232Bauderate = 0;
	if(gSystemPara.TTLBauderate>3)
		gSystemPara.TTLBauderate = 0;
	if(gSystemPara.RS485Bauderate>3)
		gSystemPara.RS485Bauderate = 0;
	
	gSystemPara.RS232Bauderate = 3;
	////////////
//	CfgUartx(Uartx3,gSystemPara.RS232Bauderate,Uart3TX,Uart3RX);
	
	CfgUartx(Uartx1,3,Uart1TX,Uart1RX);
	
	CfgUartx(Uartx6,gSystemPara.RS485Bauderate,Uart6TX,Uart6RX);
	
	bsp_InitUart();// FOR MODBUS
	bsp_InitTimer(); // FOR MODBUS
	
	////////////
	Uart6RS485RE;
	Uart1RS485RE;
	////////////
	TimCfg(1000,TIMx3); //TIM3 1MS interrupte for cyc programe .. TIM2 for canopen master.. TIM4 for encoder 
	
	TimCfg(1000,TIMx2);
	///////////////////////////

	///////////////////////////
	
	RobotEventInit();
	//////////////////////////////////////////////////////
	////////////
	CAN_Configuration(gSystemPara.CANBusBauderate);
	/////////////
//	Adc_Init();

	delay_ms(2);
	Encoder_Init_TIM5();
	delay_ms(10);
	TIM5->CNT = 0;
};
void CfgPINIn(uint32_t GPIOx, uint16_t GPIO_Pin,GPIOMode_TypeDef GPIOMode,GPIOPuPd_TypeDef GPIOPupd)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  APBCLKCfg(GPIOx, ENABLE); 						// 使能PC端口时钟
	//////////////设置成输入的，频率是50M/////////////
  GPIO_InitStructure.GPIO_Mode = GPIOMode;
	GPIO_InitStructure.GPIO_PuPd = GPIOPupd;
	///////////////////////////////////////////////////
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;	
  GPIO_Init((GPIO_TypeDef *)GPIOx,&GPIO_InitStructure);
};
void CfgPINOut(uint32_t GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef GPIOMode,GPIOPuPd_TypeDef GPIOPupd,GPIOOType_TypeDef OType)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	APBCLKCfg(GPIOx, ENABLE); 						// 使能PC端口时钟
	//////////////设置成输出的，频率是50M//////////////
  GPIO_InitStructure.GPIO_Mode = GPIOMode;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIOPupd;
	GPIO_InitStructure.GPIO_OType = OType;
	////////////////////////////////////////////////////
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;	
  GPIO_Init((GPIO_TypeDef *)GPIOx,&GPIO_InitStructure);
  GPIO_ResetBits((GPIO_TypeDef *)GPIOx,GPIO_Pin);
};

void APBCLKCfg( uint32_t GPIOx,FunctionalState NewState)
{
		if(GPIOx>=GPIOA_BASE && GPIOx<=DMA1_BASE)
			RCC_AHB1PeriphClockCmd(GPIO2APB2CLK(GPIOx), NewState);	
		else if(GPIOx>=TIM1_BASE && GPIOx<=SAI1_BASE)
			RCC_APB2PeriphClockCmd(GPIO2APB2CLK(GPIOx), NewState);	
		else if(GPIOx>=TIM2_BASE && GPIOx<=UART8_BASE)
			RCC_APB1PeriphClockCmd(GPIO2APB2CLK(GPIOx), NewState);	
	
}
uint32_t GPIO2APB2CLK(uint32_t  GPIOx)
{
	switch (GPIOx)
	{
		case PxA:
			return RCC_AHB1Periph_GPIOA;
	
		
		case PxB:
			return RCC_AHB1Periph_GPIOB;
		
		
		case PxC:
			return RCC_AHB1Periph_GPIOC;

		
		case PxD:
			return RCC_AHB1Periph_GPIOD;

		
		case Uartx1:
			return RCC_APB2Periph_USART1;

		
		case Uartx6:
			return RCC_APB2Periph_USART6;
		
		
		case Uartx3:
			return RCC_APB1Periph_USART3;
		
		
		case TIMx3:
			return RCC_APB1Periph_TIM3;
		
	}
	return 0;
}

uint8_t UAartAFR(uint32_t UartX)
{
	switch (UartX)
	{
		case USART1_BASE: 
			return GPIO_AF_USART1;

		case USART6_BASE:
			return GPIO_AF_USART6;

		case USART3_BASE:
			return GPIO_AF_USART3;
	}
	return 0;
}

uint8_t AFPinsource(uint16_t PinDefine)
{
	switch (PinDefine)
	{
		case GPIO_Pin_0: 
			return GPIO_PinSource0;

		case GPIO_Pin_1: 
			return GPIO_PinSource1;

		case GPIO_Pin_2: 
			return GPIO_PinSource2;

		case GPIO_Pin_3: 
			return GPIO_PinSource3;

		case GPIO_Pin_4: 
			return GPIO_PinSource4;

		case GPIO_Pin_5: 
			return GPIO_PinSource5;

		case GPIO_Pin_6: 
			return GPIO_PinSource6;

		case GPIO_Pin_7: 
			return GPIO_PinSource7;

		case GPIO_Pin_8: 
			return GPIO_PinSource8;

		case GPIO_Pin_9: 
			return GPIO_PinSource9;

		case GPIO_Pin_10: 
			return GPIO_PinSource10;

		case GPIO_Pin_11: 
			return GPIO_PinSource11;

		case GPIO_Pin_12: 
			return GPIO_PinSource12;

		case GPIO_Pin_13: 
			return GPIO_PinSource13;

		case GPIO_Pin_14: 
			return GPIO_PinSource14;

		case GPIO_Pin_15: 
			return GPIO_PinSource15;
	}
	return 0;
}

void SPI1_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;	
	
	macSPI1_APBxClock_FUN ( macSPI1_CLK, ENABLE );
	
	CfgPINOut(SPI1SCK);
	CfgPINOut(SPI1MISO);
	CfgPINOut(SPI1MOSI);	
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	
	SetDO(SPI1NSS,1);
	
	SPI_I2S_DeInit(macSPI1);
  SPI_InitStructure.SPI_Direction 				= SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode 							= SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize 					= SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL 							= SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA 							= SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS 							= SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStructure.SPI_FirstBit 					= SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial 		= 7;
  SPI_Init(macSPI1 , &SPI_InitStructure);
  SPI_Cmd(macSPI1 , ENABLE);
	SPI1_ReadWriteByte(0xff);
}

uint8_t ReadIn(uint32_t GPIOxADR, uint16_t GPIO_Pin,GPIOMode_TypeDef GPIOMode,GPIOPuPd_TypeDef GPIOPupd)
{
	
	GPIO_TypeDef* GPIOx;
	uint8_t bitstatus = 0x00;
	GPIOx=(GPIO_TypeDef*)GPIOxADR;
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin)); 
  if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)Bit_RESET)
    bitstatus = (uint8_t)Bit_SET;
  else
    bitstatus = (uint8_t)Bit_RESET;
  return bitstatus;
	
}
void SetDO(uint32_t GPIOxADR,uint16_t GPIO_Pin,GPIOMode_TypeDef GPIOMode,GPIOPuPd_TypeDef GPIOPupd,GPIOOType_TypeDef OType,bool state)
{
	
	GPIO_TypeDef* GPIOx;
	GPIOx=(GPIO_TypeDef*)GPIOxADR;	
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
	if(state) GPIOx->BSRRL = GPIO_Pin;
	else GPIOx->BSRRH = GPIO_Pin;
}

void CfgUartx(uint32_t UartX,uint8_t uartPar,uint32_t GPTx, uint16_t GPTX_Pin,uint32_t GPRx, uint16_t GPRX_Pin)
{
	
	GPIO_InitTypeDef GPIO_InitStructureTx;
	GPIO_InitTypeDef GPIO_InitStructureRx;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	APBCLKCfg(UartX, ENABLE);
	APBCLKCfg(GPTx, ENABLE);
	APBCLKCfg(GPRx, ENABLE);
	
	GPIO_PinAFConfig((GPIO_TypeDef *)GPTx, AFPinsource(GPTX_Pin), UAartAFR(UartX));//Standard meathod
	GPIO_PinAFConfig((GPIO_TypeDef *)GPRx, AFPinsource(GPRX_Pin), UAartAFR(UartX));//Standard meathod
	
  GPIO_InitStructureTx.GPIO_Pin = GPTX_Pin;
  GPIO_InitStructureTx.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructureTx.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureTx.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructureTx.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init((GPIO_TypeDef *)GPTx, &GPIO_InitStructureTx); 
		
  GPIO_InitStructureRx.GPIO_Pin = GPRX_Pin;
  GPIO_InitStructureRx.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructureRx.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureRx.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructureRx.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init((GPIO_TypeDef *)GPRx, &GPIO_InitStructureRx); 
	
	USART_InitStructure.USART_BaudRate = ComBauderate[uartPar];											//波特率设置：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 													//停止位设置：1位
	
	USART_InitStructure.USART_Parity = USART_Parity_No ;  													//是否奇偶校验：无
	
	if(Uartx3==UartX){
		USART_InitStructure.USART_Parity = USART_Parity_Even;
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;	
	}
	
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//接收与发送都使能
	
	USART_Init((USART_TypeDef *)UartX, &USART_InitStructure);  
	USART_Cmd((USART_TypeDef *)UartX, ENABLE);		
	USART_ITConfig((USART_TypeDef *)UartX,USART_IT_RXNE,ENABLE);										// 初始化USART1
																																									// USART1使能
	NVIC_InitStruct.NVIC_IRQChannel = MapIRQn(UartX);
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;     											// 主优先级为1
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;            											// 次优先级为0
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
}
enum IRQn MapIRQn(uint32_t BASEType)
{
	switch (BASEType)
	{
		case Uartx1:
			return USART1_IRQn;
		case Uartx6:
			return USART6_IRQn;
		case Uartx3:
			return USART3_IRQn;
		case TIMx3:
			return TIM3_IRQn;	
		case TIMx2:
			return TIM2_IRQn;	
	}
	return TIM3_IRQn;
}
void USART1_IRQHandler(void)
{
	FillUrtBuf(&(gMachineIO.Uart1Data),Uartx1);//////TTL//////
}
/*
void USART3_IRQHandler(void)
{
	FillUrtBuf(&(gMachineIO.Uart3Data),Uartx3);//////232//////
}*/
void USART6_IRQHandler(void)
{
	FillUrtBuf(&(gMachineIO.Uart6Data),Uartx6);//////485//////
}
void FillUrtBuf(UrtBuf_type * pBoxIO,uint32_t USARTx)
{
	USART_TypeDef * Uarts;
	Uarts = (USART_TypeDef*)USARTx;
	if(USART_GetITStatus(Uarts, USART_IT_RXNE) != RESET)
	{
		uint8_t ResData;
		uint8_t iFill;
		ResData = Uarts->DR;		
		iFill=pBoxIO->pRfil;
		pBoxIO->rBuffer[iFill] = ResData;
		pBoxIO->pRfil++;		
		if(pBoxIO->pRfil >= UrtBfLen)
		{
			if(pBoxIO->pRder>0)pBoxIO->pRder--;
			pBoxIO->pRfil = UrtBfLen-1;
			MBLArry((int8_t*)pBoxIO->rBuffer,UrtBfLen);
		}
	}
}
void MBLArry(int8_t *buffer,uint8_t bufLen)
{
	uint8_t iArry;
	if(bufLen<1) bufLen=1;
	for(iArry=0;iArry<bufLen-1;iArry++)
	{
		buffer[iArry] = buffer[iArry+1];
	}
}

void SendUrtBuf(UrtBuf_type * pBoxIO,uint32_t USARTx)
{
		unsigned char i;
		unsigned char bufLen = pBoxIO->sLen;
		USART_TypeDef * Uarts;
		Uarts = (USART_TypeDef*)USARTx;
		switch(USARTx)
		{
		  case Uartx6:
				Uart6RS485SE;
				break;
		  case Uartx1:
				Uart1RS485SE;
				break;
			default:
				;
				break;
		}
		for(i=0;i<bufLen;i++)
		{	
			USART_SendData(Uarts, pBoxIO->sBuffer[i]);
			while (USART_GetFlagStatus(Uarts, USART_FLAG_TXE) == RESET);			
		}
		while(USART_GetFlagStatus(Uarts, USART_FLAG_TC)==RESET);
		switch(USARTx)
		{
		  case Uartx6:
				Uart6RS485RE;
				break;
			case Uartx1:
				Uart1RS485RE;
				break;
			default:
				;
				break;
		}
};
void TimCfg(uint32_t timeUs ,uint32_t BASEType)
{
	uint16_t arr=timeUs/100-1;

	TIM_TypeDef * TIMxS;
	TIMxS = (TIM_TypeDef * )BASEType;
	APBCLKCfg(BASEType, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	switch(BASEType){
		case TIMx3:
			NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01;
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
			break;
		case TIMx2:
			NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
			break;
		default:
			NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;
			break;
	}
		
  TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler=8400-1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIMxS,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIMxS,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIMxS,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel=MapIRQn(BASEType);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;
	

	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void TIM2_IRQHandler(void)
{
	static uint32_t oldTimer1Stamp[6];
	if(TIM2->SR&0X0001)
	{	
		TIM2->SR&=~(1<<0);			
		oldTimer1Stamp[task4ms]++;
		oldTimer1Stamp[taskLong]++;
		;
	}
	TIM2->SR&=~(1<<0);	
}
void TIM3_IRQHandler(void)
{
	static uint32_t oldTimeStamp[6];
	if(TIM3->SR&0X0001)
	{		
		cyc1ms();
		gMachineIO.TimeStamp++;
		oldTimeStamp[task2ms]++;
		oldTimeStamp[task4ms]++;
		oldTimeStamp[task20ms]++;
		oldTimeStamp[task100ms]++;
		oldTimeStamp[task500ms]++;
		oldTimeStamp[taskLong]++;		
		/////////////////2ms////////////////////
		if(oldTimeStamp[task2ms]>=(time2ms))
		{
			cycTask[task2ms]();
			oldTimeStamp[task2ms] = 0;
		};
		/////////////////4mS////////////////////
		if(oldTimeStamp[task4ms]>=(time4ms))
		{		
			cycTask[task4ms]();
			oldTimeStamp[task4ms] = 0;
		};
		/////////////////20mS////////////////////
		if(oldTimeStamp[task20ms]>=(time20ms))
		{
			cycTask[task20ms]();
			oldTimeStamp[task20ms] = 0;
		};
		/////////////////100ms///////////////////
		if(oldTimeStamp[task100ms]>=(time100ms))
		{
			cycTask[task100ms]();
			oldTimeStamp[task100ms] = 0;
		};
				/////////////////500ms///////////////////
		if(oldTimeStamp[task500ms]>=(time500ms))
		{
			cycTask[task500ms]();
			oldTimeStamp[task500ms] = 0;
		};
		///////////////Long time cyc/////////
		if(oldTimeStamp[taskLong]>=(timeLong))
		{	
			cycTask[taskLong]();
			oldTimeStamp[taskLong] = 0;
		};
	}
	TIM3->SR&=~(1<<0);	 
}

void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	
	CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
	
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);	
	
}
uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//
	
	SPI_I2S_SendData(SPI1, TxData); //
		
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //
 
	return SPI_I2S_ReceiveData(SPI1); //
}

void usec_delay(unsigned int t)
{
	delay_us(100);
}
void PVMap(){
	///////////////////////////////
	gMachineIO.DI[0] = ReadIn(DI1);
	gMachineIO.DI[1] = ReadIn(DI2);
	gMachineIO.DI[2] = ReadIn(DI3);
	gMachineIO.DI[3] = ReadIn(DI4);
	gMachineIO.DI[4] = ReadIn(DI5);
	gMachineIO.DI[5] = ReadIn(DI6);
	gMachineIO.DI[6] = ReadIn(DI7);
	gMachineIO.DI[7] = ReadIn(DI8);
	

	///////////////////////////////
	SetDO(DO1,gMachineIO.DO[0]);
	SetDO(DO2,gMachineIO.DO[1]);
	SetDO(DO3,gMachineIO.DO[2]);
	SetDO(DO4,gMachineIO.DO[3]);
	SetDO(DO5,gMachineIO.DO[4]);
	SetDO(DO6,gMachineIO.DO[5]);
	SetDO(DO7,gMachineIO.DO[6]);
	SetDO(DO8,gMachineIO.DO[7]);
	SetDO(DO9,gMachineIO.DO[8]);
	SetDO(DO10,gMachineIO.DO[9]);
	SetDO(DO11,gMachineIO.DO[10]);
	SetDO(DO12,gMachineIO.DO[11]);
					

	SetDO(DOLED,gMachineIO.LED);
	///////////////////////////////
	gMachineIO.Encoder = TIM5 -> CNT + 4294967296 * gMachineIO.exeedTimes;

	gMachineIO.oldEncoder = TIM5->CNT;
	
	///////////////////////////////
}
__weak void IdleApp(void)
{
	///////////////////////////////////
	PVMap();
	///////////////////////////////
	/////////
}
__weak void cyc2ms(){}
__weak void cyc4ms(){}
__weak void cyc20ms(){}
__weak void cyc100ms(){}
__weak void cyc500ms(){}
__weak void cycLongTsk(){}


