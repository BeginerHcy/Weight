#include "mInclude.h"
#include "stm32f4xx.h" 

/****************************************
static
*****************************************/
static uint8_t can_frame_type = CAN_Id_Standard; 
 


/**
 * @brief  默认硬件接口初始化函数[平台实现]
 * @return void 
 */
void  xMotor_platformDefaultInit(void){
	//默认初始化can2通道
	//CAN2_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,12,CAN_Mode_Normal);//波特率250Kbps	
}

/**
 * @brief  控制数据发送接口[平台实现]
 * @param  motor_id 电机ID
 * @param  data 控制数据
 * @param  len 控制数据尺寸
 * @return void 
 */
void  xMotor_dataSend(uint32_t motor_id,uint8_t* data, uint8_t len){
	while(CAN_TxStatus_NoMailBox==CAN2_Send_Msg(motor_id,can_frame_type,data,len)); 
}
