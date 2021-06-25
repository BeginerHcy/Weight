
#ifndef __CANOPEN_STEPO_AXIS_BJECT_HEADER_INCLUDE__
#define __CANOPEN_STEPO_AXIS_BJECT_HEADER_INCLUDE__
#include <stdint.h>
#include <plctypes.h>
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

////////////////////////
#define ncSDC_ENC16 100U
#define ncSDC_ENC32 101U
#define ncSDC_DRVSM16 110U
#define ncSDC_DRVSM32 111U
#define ncSDC_DRVSM16_CTRL 112U
#define ncSDC_DRVSM32_CTRL 113U
#define ncSDC_DRVSERVO16 120U
#define ncSDC_TRIG 130U
#define ncSDC_TRIGDIGin 131U
#define ncSDC_DIDO 140U

/* Variables */


/* Datatypes and datatypes of function blocks */
typedef struct SdcHwCfg_typ
{	unsigned short EncIf1_Typ;
	unsigned short EncIf2_Typ;
	unsigned short DrvIf_Typ;
	unsigned short TrigIf1_Typ;
	unsigned short TrigIf2_Typ;
	unsigned short DiDoIf_Typ;
	plcstring EncIf1_Name[34];
	plcstring EncIf2_Name[34];
	plcstring DrvIf_Name[34];
	plcstring TrigIf1_Name[34];
	plcstring TrigIf2_Name[34];
	plcstring DiDoIf_Name[34];
	unsigned long NOT_USE[10];
} SdcHwCfg_typ;

typedef struct SdcEncIf16_typ
{	signed char iLifeCnt;
	bool iEncOK;
	signed short iActTime;
	signed short iActPos;
	signed short iRefPulsePos;
	signed char iRefPulseCnt;
	bool reserve[3];
} SdcEncIf16_typ;

typedef struct SdcEncIf32_typ
{	signed char iLifeCnt;
	bool iEncOK;
	signed short iActTime;
	signed long iActPos;
	signed long iRefPulsePos;
	signed char iRefPulseCnt;
	bool reserve[3];
} SdcEncIf32_typ;

typedef struct SdcDrvIf16_typ
{	signed char iLifeCnt;
	bool iDrvOK;
	signed short oSetTime;
	signed short oSetPos;
	bool oBoostCurrent;
	bool oStandStillCurrent;
	bool iStatusEnable;
	bool oBrake;
	bool reserve[2];
} SdcDrvIf16_typ;

typedef struct SdcDrvIf32_typ
{	signed char iLifeCnt;
	bool iDrvOK;
	signed short oSetTime;
	signed long oSetPos;
	bool oBoostCurrent;
	bool oStandStillCurrent;
	bool iStatusEnable;
	bool oBrake;
} SdcDrvIf32_typ;

typedef struct SdcTrigIf_typ
{	signed char iLifeCnt;
	signed char iTriggerCntRise;
	signed char iTriggerCntFall;
	bool iTriggerInput;
	signed short iTriggerTimeRise;
	signed short iTriggerTimeFall;
} SdcTrigIf_typ;

typedef struct SdcTrigIfDIGin_typ
{	signed char iLifeCnt;
	bool iTriggerInput;
	bool reserve[2];
} SdcTrigIfDIGin_typ;

typedef struct SdcDiDoIf_typ
{	signed char iLifeCntDriveReady;
	signed char iLifeCntPosHwEnd;
	signed char iLifeCntNegHwEnd;
	signed char iLifeCntReference;
	signed char iLifeCntDriveEnable;
	bool iDriveReady;
	bool iPosHwEnd;
	bool iNegHwEnd;
	bool iReference;
	bool oDriveEnable;
	bool reserve[2];
} SdcDiDoIf_typ;



/* Prototyping of functions and function blocks */


////////////////////////


/* Datatypes and datatypes of function blocks */
typedef struct DigitalOutput402
{	unsigned long BitMask;
	unsigned long Output;
} DigitalOutput402;

typedef struct Common_entries
{	unsigned short ErrorCode;
	unsigned long DigitalInputs;
	struct DigitalOutput402 DigitalOutputs;
	unsigned long MotorData[6];
	unsigned long DriveData[6];
} Common_entries;

typedef struct Cyclic_sync_mode
{	signed long Position_offset;
	signed long Velocity_offset;
	unsigned long Torque_offset;
} Cyclic_sync_mode;

typedef struct Device_control
{	unsigned char DeviceType;
	bool  ModuleOK;
	bool bootup;
	unsigned short ControlWord;
	unsigned short StatusWord;
	unsigned short Shutdown_option_code;
	unsigned short Disable_operation_option_code;
	unsigned short Halt_option_code;
	signed char Modes_of_operation;
	signed char Modes_of_operation_display;
	unsigned short ControlWord_oms;
	signed short Quick_stop_option_code;
} Device_control;

typedef struct Position_encoder_resolution
{	unsigned long Encoder_increments;
	unsigned long Motor_revolutions;
} Position_encoder_resolution;

typedef struct Factor_group
{	struct Position_encoder_resolution PositionEncoderResolution;
} Factor_group;

typedef struct HomingSpeeds
{	unsigned long SpeedForSwitch;
	unsigned long SpeedForZero;
} HomingSpeeds;

typedef struct Homing_mode
{	signed long HomeOffset;
	signed char HomingMethod;
	struct HomingSpeeds HomingSpeedsMb;
	unsigned long HomingAcc;
} Homing_mode;

typedef struct Internal
{	bool Brake;
	unsigned char state103F;
	unsigned char cycCont2;
	unsigned char i;
	unsigned char cycCont;
	unsigned long DemandVelocity;
	signed long oldSetPos;
	unsigned char State_temp;
	unsigned char State;
	unsigned char TorqueExecute;
	unsigned short StateMachine;
	unsigned short V_Scale;
	unsigned char AcpErrorBuf;
	unsigned char OldState;
	unsigned short TorqueWait;
	unsigned short tempStatus;
	unsigned short tempStatusNobit5;
} Internal;

typedef struct InterpolatedPositionMode
{	signed char InterpTimeIndex;
	unsigned char InterpTimeUnits;
} InterpolatedPositionMode;

typedef struct Position_control_function
{	signed long PositionActualValueInc;
	signed long PositionActualValuePos;
	unsigned long PositionGain;
	unsigned long PositionForward;
} Position_control_function;

typedef struct Profile_position_mode
{	signed long Target_position_ofst;
	signed long Target_position_flt;
	signed long Target_position;
	unsigned long Max_motor_speed;
	unsigned long Max_acceleration;
	unsigned long Max_deceleration;
	signed long Min_position_limit;
	signed long Max_position_limit;
	unsigned long Max_profile_velocity;
	unsigned long Quick_stop_deceleration;
	unsigned char itvel;
	unsigned long maxVEver;
	unsigned long maxVel;
	unsigned long TemVel[300];
	signed long Temp[7];
	float factorVel;
	unsigned long Profile_velocity;
} Profile_position_mode;

typedef struct Profile_torque_mode
{	signed short Torque_actual_value;
	signed short Target_torque;
	unsigned long Motor_rated_torque;
	unsigned long Torque_slope;
	signed short Target_torque_buf;
	signed short Target_torque_buf2;
	signed short Torque_demand_value;
} Profile_torque_mode;

typedef struct Profile_velocity_mode
{	unsigned short Gain;
	unsigned short IntegrationTime;
	unsigned short DerivativeTime;
	unsigned short OutMax;
	unsigned short OutMin;
	signed long Target_velocity;
	signed long Velocity_actual_value;
	signed long Velocity_demand_value;
} Profile_velocity_mode;

typedef struct Obj402
{	struct Common_entries commonEntries;
	struct Device_control deviceControl;
	struct Position_control_function positionControlFunc;
	struct Profile_position_mode profilePositionMode;
	struct Homing_mode homingMode;
	struct Cyclic_sync_mode cyclicSyncMode;
	struct Factor_group factorGroup;
	struct Profile_torque_mode profileTorqueMode;
	struct Profile_velocity_mode profileVelocityMode;
	struct InterpolatedPositionMode interpolatedPositionMode;
} Obj402;

typedef struct STEPError
{	unsigned char ErrorID;
	bool ErrorAck;
	unsigned short CountWait;
} STEPError;

typedef struct STEPConfig
{	signed long powerOnOffset;
	plcstring hwNameStr[21];
	unsigned char nodeID;
	signed long encoderOffset;
} STEPConfig;

typedef struct STEPSdcInternal_typ
{	

	struct SdcEncIf32_typ EncIf1;
	struct SdcEncIf32_typ EncIf2;
	struct SdcTrigIf_typ TrigIf1;
	struct SdcTrigIf_typ TrigIf2;
	struct SdcTrigIfDIGin_typ TrigIf1Di;
	struct SdcTrigIfDIGin_typ TrigIf2Di;
	struct SdcDrvIf32_typ DrvIf;
	struct SdcDiDoIf_typ DiDoIf;
	struct SdcDrvIf16_typ DrvIfSpd;

} STEPSdcInternal_typ;

typedef struct STEPAxisTyp
{	struct Obj402 objects402;
	struct SdcHwCfg_typ SdcHwPv;
	unsigned char SdoState;
	unsigned long cycleTime_us;
	struct STEPSdcInternal_typ SdcInter;
	struct Internal internal;
	struct STEPError stepError;
	struct STEPConfig stepConfig;
} STEPAxisTyp;

typedef struct StepAxisObj
{
	/* VAR_INPUT (analog) */
	unsigned long Address;
	unsigned char AxisNumber;
	/* VAR_OUTPUT (analog) */
	plcstring ErrorText[81];
	unsigned short Status;
} StepAxisObj_typ;

typedef struct StepInit
{
	/* VAR_INPUT (analog) */
	unsigned long Address;
	char PvName[81];
	unsigned char AxisNumber;
	/* VAR_OUTPUT (analog) */
	plcstring ErrorText[81];
	unsigned short Status;
} StepInit_typ;



/* Prototyping of functions and function blocks */
void StepAxisObj(struct StepAxisObj* inst);
void StepInit(struct StepInit* inst);

extern struct StepAxisObj StepCyclic;
extern struct StepInit StepInitial;
extern STEPAxisTyp stepAxis[3];

#endif
