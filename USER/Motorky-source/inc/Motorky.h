/* Automation Studio generated header file */
/* Do not edit ! */

#ifndef _MOTORKY_
#define _MOTORKY_
#ifdef __cplusplus
extern "C" 
{
#endif

#include <plctypes.h>

/* Constants */

#define MOVE_COMMAND_STOP 0U
#define HOMING_DIRECT 0U
#define MOTION_STATE_STANDSTILL 0U
#define DIRECTION_POSITIVE 0U
#define MOTION_STATE_ACCELERATING 1U
#define HOMING_END_SWITCH 1U
#define MOVE_COMMAND_ABSOLUTE 1U
#define DIRECTION_NEGATIVE 1U
#define MOVE_COMMAND_VELOCITY 2U
#define MOTION_STATE_CONSTANT_SPEED 2U
#define HOMING_REF_SWITCH 2U
#define MOTION_STATE_DECELERATING 3U
#define ERR_CONTROLLER_OFF 40001U
#define ERR_WRONG_UNITS 40002U
#define ERR_NEGATIVE_LIMIT_SWITCH_CLOSED 40003U
#define ERR_NEGATIVE_SW_LIMIT_REACHED 40004U
#define ERR_OVER_CURRENT 40006U
#define ERR_OVER_TEMPERATURE 40007U
#define ERR_POSITIVE_LIMIT_SWITCH_CLOSED 40008U
#define ERR_POSITIVE_SW_LIMIT_REACHED 40009U
#define ERR_STALL 40011U
#define ERR_DRIVE_NOT_READY 40012U
#define ERR_POWER_SUPPLY_ERROR 40013U
#define ERR_UNDER_VOLTAGE 40014U
#define ERR_OVER_VOLTAGE 40015U
#define ERR_LAG_ERROR 40016U
#define ERR_ENCODER_SUPPLY 40017U
#define ERR_WRONG_TASK_TIME 40018U
#define ERR_LOW_DC_BUS_VOLTAGE 40019U
#define ERR_WRONG_ENCODER_INCREMENTS 40020U
#define ERR_DRV_NOT_OK 40021U
#define SPEED_FIFO_UBOUND 19





/* Datatypes and datatypes of function blocks */
typedef struct Motorky_ActualSpeed
{	signed long actPosition;
	signed long actSpeed;
	signed long taskTime;
	signed long filterTime;
	signed long positionDifference;
	signed long FIFOsize;
	signed long FIFOindex;
	signed long positionFIFO[20];
} Motorky_ActualSpeed;

typedef struct Motorky_Cmd_typ
{	plcbit power;
	plcbit homing;
	plcbit absolute;
	plcbit additive;
	plcbit cyclicPosition;
	plcbit velocity;
	plcbit acknowledge;
	plcbit stop;
} Motorky_Cmd_typ;

typedef struct Motorky_ConvertAcceleration
{	signed long inputScale;
	signed long outputScale;
	signed long maxAcceleration;
	unsigned short status;
} Motorky_ConvertAcceleration;

typedef struct Motorky_ConvertPosition
{	signed long inputScale;
	signed long outputScale;
	plcbit reverseDirection;
	signed short input;
	signed long output;
	signed short dInput;
	signed long integral;
} Motorky_ConvertPosition;

typedef struct Motorky_ConvertSpeed
{	signed long inputScale;
	signed long outputScale;
	signed long maxSpeed;
	unsigned short status;
} Motorky_ConvertSpeed;

typedef struct Motorky_SpeedRamping
{	signed long setSpeed;
	signed long acceleration;
	signed long deceleration;
	signed long actualPosition;
	signed long actualSpeed;
	unsigned short motionState;
	signed long divisor;
	signed long sIntegral;
	signed long vIntegral;
} Motorky_SpeedRamping;

typedef struct Motorky_PathGenerator
{	unsigned char moveCommand;
	signed long setPosition;
	signed long setSpeed;
	signed long acceleration;
	signed long deceleration;
	signed long positiveSwLimit;
	signed long negativeSwLimit;
	signed long outputPosition;
	signed long outputSpeed;
	unsigned short motionState;
	unsigned short status;
	unsigned short step;
	struct Motorky_SpeedRamping speedRamping;
} Motorky_PathGenerator;

typedef struct Motorky_DS_Intern_typ
{	unsigned short step;
	unsigned short oldStep;
	signed long oldPosition;
	plcbit oldReferenceSwitch;
	signed long movTargetTimeOffset;
	struct Motorky_PathGenerator pathGenerator;
	struct Motorky_ConvertPosition convertPosition;
	struct Motorky_ActualSpeed actualSpeed;
} Motorky_DS_Intern_typ;

typedef struct Motorky_DS_IOMapping
{	plcbit DriveReady;
	signed long SystemTime;
	plcbit DriveEnable;
	signed long MovTargetTime;
	signed long MovTargetPosition;
	plcbit positiveLimitSwitch;
	plcbit negativeLimitSwitch;
	plcbit referenceSwitch;
} Motorky_DS_IOMapping;

typedef struct Motorky_FI_IOMapping
{	plcbit DriveReady;
	signed short Encoder;
	plcbit DriveEnable;
	signed short AnalogOutput;
	plcbit positiveLimitSwitch;
	plcbit negativeLimitSwitch;
	plcbit referenceSwitch;
} Motorky_FI_IOMapping;

typedef struct Motorky_Info_typ
{	plcbit initialized;
	plcbit powerOn;
	plcbit homingOk;
	signed long position;
	signed long speed;
	plcbit moveActive;
	signed long lagError;
	unsigned short status;
	plcstring errorText[101];
} Motorky_Info_typ;

typedef struct Motorky_Init_MM_typ
{	float taskTime;
	signed long units;
	signed long encoderIncrements;
	plcbit reverseDirection;
	signed long maxLagError;
	float Kp;
	float Tn;
	float speedKp;
	float speedTn;
	float speedFilter;
	float settleTime;
	signed long positiveSwLimit;
	signed long negativeSwLimit;
} Motorky_Init_MM_typ;

typedef struct Motorky_Init_Enc_typ
{	float taskTime;
	signed long units;
	signed long encoderIncrements;
	plcbit reverseDirection;
	signed long maxLagError;
	float Kp;
	float Tn;
	float speedFilter;
	float settleTime;
	signed long positiveSwLimit;
	signed long negativeSwLimit;
} Motorky_Init_Enc_typ;

typedef struct Motorky_Init_SM_typ
{	float taskTime;
	signed long units;
	signed long microsteps;
	plcbit reverseDirection;
	float settleTime;
	signed long positiveSwLimit;
	signed long negativeSwLimit;
	float outputFilter;
} Motorky_Init_SM_typ;

typedef struct Motorky_Init_NoEnc_typ
{	float taskTime;
	signed long units;
	signed long microsteps;
	plcbit reverseDirection;
	float settleTime;
	signed long positiveSwLimit;
	signed long negativeSwLimit;
} Motorky_Init_NoEnc_typ;

typedef struct Motorky_MoveActive
{	unsigned short motionState;
	signed long taskTime;
	signed long settleTime;
	signed long settleTimer;
	plcbit moveActive;
} Motorky_MoveActive;

typedef struct Motorky_Intern_typ
{	unsigned short step;
	unsigned short oldStep;
	signed long oldPosition;
	plcbit oldReferenceSwitch;
	struct Motorky_PathGenerator pathGenerator;
	struct Motorky_MoveActive moveActive;
} Motorky_Intern_typ;

typedef struct Motorky_PID
{	plcbit enable;
	signed long W;
	signed long X;
	signed long Y;
	signed long Kp;
	signed long Y_max;
	signed long Y_min;
	signed long e;
	signed long Yp;
	signed long Yi;
	signed long Yi_sum;
	signed long Ki;
	signed long Ytotal;
	signed long Ylim;
	signed long Ywindup;
} Motorky_PID;

typedef struct Motorky_InternFI_typ
{	struct Motorky_ConvertPosition encoder2units;
	struct Motorky_PID pid;
	struct Motorky_ActualSpeed actualSpeed;
} Motorky_InternFI_typ;

typedef struct Motorky_InternMM_typ
{	struct Motorky_ConvertPosition encoder2units;
	struct Motorky_PID pid;
	struct Motorky_ActualSpeed actualSpeed;
	struct Motorky_PID speedPID;
} Motorky_InternMM_typ;

typedef struct Motorky_MM_Intern_typ
{	unsigned short step;
	unsigned short oldStep;
	signed short encoderPosition;
	signed long oldPosition;
	plcbit oldReferenceSwitch;
	struct Motorky_PathGenerator pathGenerator;
	struct Motorky_ConvertPosition convertPosition;
	struct Motorky_ActualSpeed actualSpeed;
	struct Motorky_MoveActive moveActive;
	struct Motorky_PID pid;
} Motorky_MM_Intern_typ;

typedef struct Motorky_MM_IOMapping
{	plcbit DriveReady;
	plcbit DriveEnable;
	unsigned short PeriodDuration;
	signed short PulseWidth;
	plcbit ClearError;
	signed short Counter;
	plcbit UnderVoltageError;
	plcbit OverVoltageError;
	plcbit OvertemperatureError;
	plcbit OvercurrentError;
	plcbit positiveLimitSwitch;
	plcbit negativeLimitSwitch;
	plcbit referenceSwitch;
} Motorky_MM_IOMapping;

typedef struct Motorky_OutputGenerator
{	plcbit enable;
	plcbit encoderUsed;
	plcbit reverseDirection;
	signed long setPosition;
	signed long actPosition;
	signed long setSpeed;
	unsigned short motionState;
	signed long settleTime;
	signed long Y;
	plcbit moveActive;
	signed long settleTimer;
	signed long taskTime;
	signed long e;
	signed long Kp;
	signed long Ki;
	signed long Yp;
	signed long Yi;
	signed long Yiintegral;
} Motorky_OutputGenerator;

typedef struct Motorky_Param_Homing
{	unsigned char mode;
	plcbit startDirection;
	plcbit edgeSwitch;
	plcbit triggerDirection;
	signed long speed2;
} Motorky_Param_Homing;

typedef struct Motorky_Param_typ
{	signed long position;
	signed long speed;
	signed long acceleration;
	signed long deceleration;
	struct Motorky_Param_Homing homing;
} Motorky_Param_typ;

typedef struct Motorky_TOF
{	plcbit IN;
	signed long PT;
	plcbit Q;
	signed long ET;
} Motorky_TOF;

typedef struct Motorky_SM_Intern_typ
{	unsigned short step;
	unsigned short oldStep;
	signed long setPosition;
	signed long oldPosition;
	signed long dPosition;
	plcbit oldReferenceSwitch;
	struct Motorky_PathGenerator pathGenerator;
	struct Motorky_ConvertPosition outputConversion;
	struct Motorky_ActualSpeed actualSpeed;
	struct Motorky_TOF tofBoostCurrent;
	struct Motorky_MoveActive moveActive;
} Motorky_SM_Intern_typ;

typedef struct Motorky_SM_IOMapping
{	plcbit DriveReady;
	signed short MotorStep0;
	plcbit DriveEnable;
	plcbit BoostCurrent;
	plcbit StandstillCurrent;
	plcbit StallError;
	plcbit OvertemperatureError;
	plcbit OvercurrentError;
	plcbit ModulePowerSupplyError;
	plcbit ClearError;
	plcbit UZKStatus;
	plcbit positiveLimitSwitch;
	plcbit negativeLimitSwitch;
	plcbit referenceSwitch;
} Motorky_SM_IOMapping;

typedef struct FI
{
	/* VAR_INPUT (analog) */
	struct Motorky_Init_Enc_typ init;
	struct Motorky_Cmd_typ cmd;
	struct Motorky_Param_typ param;
	/* VAR_OUTPUT (analog) */
	struct Motorky_Info_typ info;
	struct Motorky_FI_IOMapping IOMapping;
	/* VAR (analog) */
	struct Motorky_Intern_typ intern;
	struct Motorky_InternFI_typ internFI;
} FI_typ;

typedef struct MM
{
	/* VAR_INPUT (analog) */
	struct Motorky_Init_MM_typ init;
	struct Motorky_Cmd_typ cmd;
	struct Motorky_Param_typ param;
	/* VAR_OUTPUT (analog) */
	struct Motorky_Info_typ info;
	struct Motorky_MM_IOMapping IOMapping;
	/* VAR (analog) */
	struct Motorky_Intern_typ intern;
	struct Motorky_InternMM_typ internMM;
} MM_typ;

typedef struct DS
{
	/* VAR_INPUT (analog) */
	struct Motorky_Init_NoEnc_typ init;
	struct Motorky_Cmd_typ cmd;
	struct Motorky_Param_typ param;
	/* VAR_OUTPUT (analog) */
	struct Motorky_Info_typ info;
	struct Motorky_DS_IOMapping IOMapping;
	/* VAR (analog) */
	struct Motorky_Intern_typ intern;
	signed long movTargetTimeOffset;
	struct Motorky_ConvertPosition units2output;
} DS_typ;

typedef struct SM
{
	/* VAR_INPUT (analog) */
	struct Motorky_Init_SM_typ init;
	struct Motorky_Cmd_typ cmd;
	struct Motorky_Param_typ param;
	/* VAR_OUTPUT (analog) */
	struct Motorky_Info_typ info;
	struct Motorky_SM_IOMapping IOMapping;
	/* VAR (analog) */
	struct Motorky_Intern_typ intern;
	struct Motorky_TOF tofBoostCurrent;
	signed long dPosition;
	signed long oldPosition;
	struct Motorky_ConvertPosition units2output;
	signed short filterLevel;
} SM_typ;



/* Prototyping of functions and function blocks */
void FI(struct FI* inst);
void MM(struct MM* inst);
void DS(struct DS* inst);
void SM(struct SM* inst);


#ifdef __cplusplus
};
#endif
#endif /* _MOTORKY_ */

