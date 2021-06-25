/* Automation Studio generated header file */
/* Do not edit ! */

#ifndef _HC2500B_
#define _HC2500B_
#ifdef __cplusplus
extern "C" 
{
#endif

#include <plctypes.h>
#include "string.h"
#include "motorky.h"
/* Constants */

#define PUNUM 4U
#define PUNUM_MIN 3U
#define PI 3.141592653
#define ABS(c)		(((c) >= 0) ? (c) : -(c))
/* Datatypes and datatypes of function blocks */
typedef enum nAxisHomeModeEnum
{	nAXIS_HOME_MODE_DEFAULT = 0,
	nAXIS_HOME_MODE_ABS_SWITCH = 2,
	nAXIS_HOME_MODE_SWITCH_GATE = 8,
	nAXIS_HOME_MODE_LIMIT_SWITCH = 3,
	nAXIS_HOME_MODE_REF_PULSE = 5,
	nAXIS_HOME_MODE_DIRECT = 1,
	nAXIS_HOME_MODE_ABSOLUTE = 4,
	nAXIS_HOME_MODE_ABSOLUTE_CORR = 6,
	nAXIS_HOME_MODE_DCM = 9,
	nAXIS_HOME_MODE_DCM_CORR = 10,
	nAXIS_HOME_MODE_RESTORE_POS = 11,
	nAXIS_HOME_MODE_AXIS_REF = 12,
	nAXIS_HOME_MODE_BLOCK_TORQUE = 13,
	nAXIS_HOME_MODE_BLOCK_DS = 14,
	nAXIS_HOME_MODE_CAM = 15,
	nAXIS_HOME_MODE_LINEAR = 15
} nAxisHomeModeEnum;

typedef enum nAxisHomeMoveDirectionEnum
{	nAXIS_HOME_DIR_POSITIVE = 0,
	nAXIS_HOME_DIR_NEGATIVE = 1
} nAxisHomeMoveDirectionEnum;

typedef enum nAxisMotorDirectionEnum
{	nAXIS_DIR_CLOCKWISE = 0,
	nAXIS_DIR_COUNTERCLOCKWISE = 255
} nAxisMotorDirectionEnum;

typedef enum nAxisLoadTypeEnum
{	nAXIS_LINEAR = 0,
	nAXIS_CAM = 1,
	nAXIS_SPLINE = 2
} nAxisLoadTypeEnum;

typedef enum nAxisHomeOptionEnum
{	nAXIS_HOME_OPTION_OFF = 0,
	nAXIS_HOME_OPTION_ON = 1
} nAxisHomeOptionEnum;

typedef enum nAxisAccStateTypeEmu
{	AccNull = 0,
	Accing = 1,
	Decing = 2
} nAxisAccStateTypeEmu;

typedef enum BoxPressCmdTypEmu
{	AllClose = 0,
	NegpOpen = 1,
	PospOpenH = 2,
	PospOpenL = 3
} BoxPressCmdTypEmu;

typedef enum nDriverType
{	nServo = 0,
	nRelay = 1,
	nInverter = 2
} nDriverType;

typedef enum nDirectionType
{	Positive = 0,
	Negative = 1
} nDirectionType;










typedef struct nNegpEif_typ
{	plcbit oMoveNeg;
	plcbit oMovePos;
	plcbit iLimited;
	unsigned char oPIDOut;
	plcbit oSwitch;
	unsigned char reverse1;
	unsigned char reverse2;
	unsigned char reverse3;
	unsigned char reverse4;
} nNegpEif_typ;

typedef struct nPospEif_typ
{	plcbit iLimited;
	plcbit oSwitchHL;
	plcbit oSwitch;
	plcbit oRelease;
	unsigned char reverse1;
	unsigned char reverse2;
	unsigned char reverse3;
	unsigned char reverse4;
} nPospEif_typ;


typedef struct nEncoderType
{	unsigned char nBits;
	unsigned char nRem;
} nEncoderType;

typedef struct nAxisGearboxType
{	unsigned long Input;
	unsigned long Output;
	float DeadV;
	float NormVelocity;
	float Scaling;
	float CamDistance;
	enum nAxisLoadTypeEnum LoadMech;
	enum nAxisMotorDirectionEnum Direction;
} nAxisGearboxType;

typedef struct nMountType
{	struct nEncoderType EncoderInfo;
	enum nAxisMotorDirectionEnum EncoderDir;
	struct nAxisGearboxType GearBox;
} nMountType;

typedef struct nAxisHomingType
{	enum nAxisHomeModeEnum Mode;
	double Position;
	signed long EncoderOffset;
	float StartVelocity;
	float HomingVelocity;
	double SensorOffset;
	float Acceleration;
	enum nAxisHomeMoveDirectionEnum StartDirection;
	enum nAxisHomeMoveDirectionEnum HomingDirection;
	enum nAxisHomeOptionEnum NoDirectionChange;
	enum nAxisHomeMoveDirectionEnum SwitchEdge;
	enum nAxisHomeOptionEnum ReferencePulse;
	double ReferencePulseBlockingDistance;
	float TorqueLimit;
	double BlockDetectionPositionError;
	double PositionErrorStopLimit;
	unsigned long EndlessPositionDataRef;
} nAxisHomingType;

typedef struct nAxisJogType
{	float Acceleration;
	float Deceleration;
	float Velocity;
	double LowerLimit;
	double UpperLimit;
} nAxisJogType;

typedef struct nAxisBasicPar_type
{	double Position;
	double Distance;
	float Velocity;
	float Collision;
	float Acceleration;
	float Deceleration;
	struct nAxisHomingType Home;
	struct nAxisJogType Jog;
	enum nDriverType Drivertype;
	struct nMountType MountInfo;
	enum nAxisMotorDirectionEnum Direction;
} nAxisBasicPar_type;

typedef struct nAxisEif_typ
{	signed long dEncoderRaw;
	plcbit iLimit_p;
	plcbit iLimit_n;
	plcbit iRef;
	plcbit iEstop;
	plcbit oMoveNeg;
	plcbit oMovePos;
	float oVOverride;
	unsigned char reverse1;
	unsigned char reverse2;
	unsigned char reverse3;
} nAxisEif_typ;


typedef struct nAxisMON_typ
{	signed long s;
	float v;
} nAxisMON_typ;

typedef struct nAxisObj_type
{	struct nAxisMON_typ Monitor;
	struct nAxisEif_typ Interface;
} nAxisObj_type;

typedef struct nAxisRT_Param_Homing
{	unsigned char mode;
	plcbit startDirection;
	plcbit edgeSwitch;
	plcbit triggerDirection;
	float position;
	float speedSearch;
	float speed2;
} nAxisRT_Param_Homing;

typedef struct nAxisRT_Info_typ
{	plcbit initialized;
	plcbit netWorkinitialized;
	plcbit powerOn;
	plcbit homingOk;
	float position;
	float position_REAL;
	float speed;
	plcbit moveActive;
	signed long lagError;
	unsigned short status;
	enum nAxisAccStateTypeEmu SpeedState;
	plcstring errorText[101];
} nAxisRT_Info_typ;

typedef struct CanOpenEif_type
{	signed char iModesOfOperation6061;
	unsigned short oControlword6040;
	signed long oTagetPosition607A;
	signed long oTargetVelocity60FF;
	signed long iVelocity606C;
	signed long iActualPosition6064;
	signed char oModesOfOperation;
	unsigned long oProfileVelocity6081;
	unsigned long oProfileDeceleration6084;
	unsigned long oProfileAcceleration6083;
	unsigned short iStatusword6041;
} CanOpenEif_type;

typedef struct HC2500B_MECH_type
{	float OutLet;
	float SensorGV;
	float SensorNM;
	float Drawer;
	float RearRoller;
	float PrintHeader;
	float FrontRoller;
	float WheelGirf;
	float WheelGirfEfc;
} HC2500B_MECH_type;

typedef struct RT_SM_Para_type
{	unsigned char Direction;
	float unitFactor;
	float Distance;
	float Position;
	float MaxSpeed;
	float Speed;
	float Acceleration;
	float Deceleration;
} RT_SM_Para_type;

typedef struct RT_SM_Cmd_type
{	plcbit StopMove;
	plcbit Homing;
	plcbit NegMove;
	plcbit PosMove;
	plcbit RelMove;
	plcbit AbsMove;
} RT_SM_Cmd_type;

typedef struct nAxise_IOMap_typ
{	signed short MotorStep0;
	plcbit DoDirection;
	plcbit DoDriveBrake;
	plcbit DriveEnable;
	plcbit BoostCurrent;
	plcbit StandstillCurrent;
	plcbit StallError;
	plcbit OvertemperatureError;
	plcbit OvercurrentError;
	plcbit ModulePowerSupplyError;
	plcbit ClearError;
	plcbit UZKStatus;
	plcbit Enable;
	plcbit positiveLimitSwitch;
	plcbit negativeLimitSwitch;
	plcbit referenceSwitch;
	plcbit iStandillExt;
	plcbit iErrorExt;
	plcbit iReadyExt;
} nAxise_IOMap_typ;

typedef struct RT_SM_Monitor_type
{	plcbit moveActive;
	plcbit tempDoDirection;
	plcbit DoDirection;
	signed short MotorStep0;
	unsigned char Step;
	unsigned short status;
	float position;
	float speed;
} RT_SM_Monitor_type;

typedef struct RT_SM_type
{	struct RT_SM_Cmd_type Cmd;
	struct RT_SM_Para_type Para;
	struct RT_SM_Monitor_type Monitor;
} RT_SM_type;

typedef struct nAxisRT_Param_SWLimit_typ
{	plcbit Ignore;
	float Upper;
	float Lower;
} nAxisRT_Param_SWLimit_typ;

typedef struct nAxisRT_Param_typ
{	float position;
	float JogSpeed;
	float speed;
	float acceleration;
	float deceleration;
	struct nAxisRT_Param_Homing homing;
	struct nAxisRT_Param_SWLimit_typ SWLimits;
} nAxisRT_Param_typ;

typedef struct TON_10ms_typ
{	
	plcbit IN;
	unsigned long PT;
	plcbit Q;
	unsigned long ET;

} TON_10ms_typ;


typedef struct nAxiseRT
{
	/* VAR_INPUT (analog) */
	plcstring Tag[11];
	float Unifactor;
	float Distance;
	struct SM* nLink;
	struct nAxisRT_Param_typ Parameter;
	struct Motorky_Init_SM_typ SystemCfg;
	/* VAR_OUTPUT (analog) */
	struct nAxisRT_Info_typ info;
	struct nAxise_IOMap_typ IOMapping;
	unsigned long StatusJOG;
	unsigned long StatusID;
	/* VAR (analog) */
	unsigned long CycDireStep;
	float oldCycPosition;
	float oldSpeed;
	unsigned long internalStatus;
	struct TON_10ms_typ statuCycDelay;
	struct TON_10ms_typ statuDelay;
	/* VAR_INPUT (digital) */
	plcbit Enable;
	plcbit Power;
	plcbit MountDirection;
	plcbit Homing;
	plcbit MoveAbsolute;
	plcbit HWIgnore;
	plcbit MoveAdditive;
	plcbit JogPos;
	plcbit JogNeg;
	plcbit MoveCyclicPosition;
	plcbit MoveVelocity;
	plcbit Stop;
	plcbit Acknowledge;
	/* VAR_OUTPUT (digital) */
	plcbit SWLimitPos;
	plcbit SWLimitNeg;
	/* VAR (digital) */
	plcbit oldcmdMoveAbs;
	plcbit oldcmdMoveRef;
} nAxiseRT_typ;


extern nAxiseRT_typ AxisRTs[3];
extern SM_typ AxisObjects[3];
/* Prototyping of functions and function blocks */
void nAxiseRT(struct nAxiseRT* inst);
float MAXReal(float AA, float BB);
float MINReal(float AA, float BB);
signed long swap2CombREAL(float* adr);
signed long swap32b(signed long ABCD);
signed long swap2Comb(unsigned short* adr);
void TON_10ms(TON_10ms_typ *inst);

#ifdef __cplusplus
};
#endif
#endif /* _HC2500B_ */

