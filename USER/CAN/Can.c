
#include "mInclude.h"

static void CAN_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_GPIOB_CLK_ENABLE();
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_CAN1);
	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void CAN_Configuration(uint8_t Bauderate)
{
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	CAN_GPIO_Configuration();
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);	
	NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	CAN_InitStructure.CAN_TTCM = DISABLE; 
	CAN_InitStructure.CAN_ABOM = DISABLE; 
	CAN_InitStructure.CAN_AWUM = DISABLE; 
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS2_6tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS1_7tq; 
	switch(Bauderate){
		case 0://125k
			CAN_InitStructure.CAN_Prescaler = 24;
			break;
		case 1://250k
			CAN_InitStructure.CAN_Prescaler = 12;
			break;
		case 2://500k
			CAN_InitStructure.CAN_Prescaler = 6;
			break;
		case 3://1000k
			CAN_InitStructure.CAN_Prescaler = 3;
			break;
		default:
			CAN_InitStructure.CAN_Prescaler = 12;
			break;			
	}
	CAN_Init(CAN1,&CAN_InitStructure);	
	CAN_FilterInitStructure.CAN_FilterNumber=0;
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);	
}

void CanWriteData(uint32_t ID,uint8_t *databuf,uint8_t datalen)
{
	CanTxMsg TxMessage;
	TxMessage.StdId = ID;
	TxMessage.RTR 	= CAN_RTR_DATA;
	TxMessage.IDE 	= CAN_ID_STD;
	TxMessage.DLC 	= datalen;
	for(uint8_t i = 0;i < datalen;i++)
	{
		TxMessage.Data[i] = databuf[i]; 
	}
	CAN_Transmit(CAN1,&TxMessage);
}

uint8_t CAN2_Send_Msg(uint32_t canid,uint8_t frame_type,uint8_t* msg,uint8_t len)
{	
  uint8_t mbox;
  uint16_t i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=canid;	 // 标准标识符为0
  TxMessage.ExtId=canid;	 // 设置扩展标示符（29位）
  TxMessage.IDE=frame_type;		  // 使用扩展标识符
  TxMessage.RTR=0;		  // 消息类型为数据帧，一帧8位
  TxMessage.DLC=len;							 // 发送两帧信息
  for(i=0;i<len;i++)
		TxMessage.Data[i]=msg[i];				 // 第一帧信息          
  mbox = CAN_Transmit(CAN1, &TxMessage); 
	//return mbox;
	if(mbox==CAN_TxStatus_NoMailBox) return CAN_TxStatus_NoMailBox;
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
  return mbox;		

}

