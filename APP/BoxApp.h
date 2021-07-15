
#ifndef __BOXAPP_H
#define __BOXAPP_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "string.h"
#include "stm32f4xx_spi.h"
#include <plctypes.h>
#include "HwCfg.h"
/* stdbool.h standard header */
#ifndef _STDBOOL
#define _STDBOOL

#define __bool_true_false_are_defined        1

#ifndef __cplusplus
                /* TYPES */

#if 199901L <= __STDC_VERSION__


#else /* 199901L <= __STDC_VERSION__ */
#if __TI_STRICT_ANSI_MODE__
typedef unsigned char _Bool;
#endif
#endif /* 199901L <= __STDC_VERSION__ */

                /* MACROS */
#define bool        _Bool
#define false        0
#define true        1
#endif /* __cplusplus */


#endif /* _STDBOOL */

#define HEXIN_TYPE

#define PITCH 25.0
#define Ratio 25.0
#define Perimeter 220.01

#ifdef HEXIN_TYPE
#define REVPULSE 131072.0
#endif
#ifdef INOVA_TYPE
#define REVPULSE 262144.0
#endif


typedef void(*cyc_task_fun)(void);
typedef void(*agv_mode_App)(void);

void cyc2ms(void);
void cyc4ms(void);
void cyc20ms(void);
void cyc100ms(void);
void cyc500ms(void);
void cycLongTsk(void);
void cycCanOpen4ms(void);
void cycCanOpen1s(void);

void NullApp(void);
void BMSApp(void);
void CallerApp(void);
void ChargeStaionApp(void);
void SonarApp(void);
///////////////////////////////////////

typedef struct SonarComVal_type
{	
	int16_t ComDisACHA;
	int16_t ComDisBCHA;
	int16_t ComDisCCHA;	
	
	int16_t ComDisACHB;
	int16_t ComDisBCHB;
	int16_t ComDisCCHB;	
}SonarComVal_type;
typedef struct SonarComInterval_type
{	
	int16_t pAreaToCom;
	bool AreaDetectSwitch;
	int16_t OutComState;//0: No Error    1: A     2: B    3: C
}SonarComInterval_type;
typedef struct SonarDualData_type
{
	////
	SonarComVal_type Area[20];
	////
	int16_t DistanCHA;
	int16_t DistanCHB;
	////
	bool bDI1;
	bool bDI2;
	bool bDI3;
	bool bDI4;
	////
	bool bDO1;
	bool bDO2;
	bool bDO3;
	bool bDO4;
	////
	SonarComInterval_type Internal;
	////
	bool AliveCHA;
	bool AliveCHB;
	////
	bool cmdSendRS232Data;
	bool cmdPCConnect;
	bool ReqConnectAllow;
	bool ReqSetOKAllow;
	////
	int8_t StatusStep;
	uint8_t SetID;
	uint8_t ActID;
}SonarDualData_type;


typedef struct SysParameter_type
{
	uint8_t	 BufferHead;
	plcbit SaveParameter;
	float MeasurePosScale;
	float TargetExchScale;
	float SynVelocity;
	float BendX1;
	float BendH;
	float BackVel;
	float FollowVelocity;
	float MaualVelocity;
	float ACC;
	float DEC;
	float StartPos;
	float EndPos;
	float BackPosition;
	float OffsetPos;
	float AutoBackPos;
	float OffsetPos2;
	float offsetBasic;
	float offsetBasic_Teach;
	float PosLimitSw;
	float NegLimitSw;
	plcbit EnableSw;
	float BendAngle;
	float BendPanX;
	float PosLimitSw_B;
	float NegLimitSw_B;
	float ACC_B;
	float DEC_B;
	float MaualVelocity_B;
	float BackPosition_B;
	float BackVel_B;
	float FollowVelocity_B;
	float SynVelocity_B;
	float Distance90Degree;
	float AxisBScale;
	float TeachAngle;
	float EndPos_Teach;
	float PositionOfstBR[10];
	float VmoldeD;
	float VDistMachine;
	float Vmolde2D;
	float SpringH;
	float MoldH;
	float AngleScaleK;
	plcbit EnableSim;
	float MachineZeroOfset;
	float Bend2TimeOfset;
	float AngleB;
	float PlatThick;
	float HoldBendTime;
	float SuckDist;
	
	uint8_t  RS485Bauderate;//0-9600 1-19200 2-38400 3-115200
	uint8_t	 RS232Bauderate;//0-9600 1-19200 2-38400 3-115200
	uint8_t	 TTLBauderate;//0-9600 1-19200 2-38400 3-115200	
	uint8_t	 CANBusBauderate;//0-125k 1-250k 2-500k 3-1000k 
	uint8_t  AppMode;
	uint8_t  AOMode;// 000:Disable 001:0~5V 002:0~10V 003:+-5V 004:+-10V 005:NULL 006:4~20mA 007:0~20mA 008:0~24mA
	bool cmdSaveParameter;
	uint8_t  RS485Node;
	uint16_t CanNode;
	uint8_t  RangeMode[4];
	/////BoardType 0 :Null  1: A-Type
	uint8_t BoardType;
	float MachineZeroOfsetTemp;
	
}SysParameter_type;
extern SysParameter_type gSystemPara;
extern SonarDualData_type SonarDual;
extern int16_t inputReg[10];
uint16_t CRC_Verify(uint8_t *CRC_Buf,uint8_t Buf_Length);

#endif
