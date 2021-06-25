#ifndef __CAN_H
#define __CAN_H

#include <stdio.h>
#include "stm32f4xx.h"

extern void CanWriteData(uint32_t ID,uint8_t *databuf,uint8_t datalen);
extern void CAN_Configuration(uint8_t Bauderate);
extern uint8_t ReadCanAddr(void);
extern void CAN_Send_Digital_Value(void);
extern uint8_t CAN2_Send_Msg(uint32_t canid,uint8_t frame_type,uint8_t* msg,uint8_t len);
extern void  xMotor_dataSend(uint32_t motor_id,uint8_t* data, uint8_t len);
#endif
