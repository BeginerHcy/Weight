#ifndef _RTM_H_
#define _RTM_H_

#include <plctypes.h>
#include "HC2500B.h"
#include "motorky.h"
#include <math.h>




#define AX_BEND 1
#define AX_DRAW 0
#define AX_VIRTRUAL 2
typedef struct MchineEif_typ
{	plcbit Estop;
} MchineEif_typ;

typedef struct SimulateEncoder_type
{	float BeginPos;
	float X1Pos;
	float VelocityuUpDown;
	unsigned short StepSim;
	float OutPos;
	float VelocityBend;
	float stepIncNormal;
	float stepIncBend;
	struct TON_10ms_typ delaySim;
	float EndPos;
} SimulateEncoder_type;


typedef struct BendMachinePara_cmd_type
{	plcbit HomeServo_B;
	plcbit HomeServo;
	plcbit RasterHomeJiajin;
	plcbit RasetReset;
	plcbit JogPos_B;
	plcbit JogNeg_B;
	plcbit JogPos;
	plcbit EnableSim;
	plcbit JogNeg;
	plcbit GoHome;
	plcbit Stepper;
	plcbit Reset;
	plcbit VacnumOper;
	plcbit Manual;
	plcbit oEstop;
	plcbit oHoldStick;
	plcbit oServoReady;
	plcbit oHomeOK;
	plcbit oPulse;
	plcbit oDirection;
	plcbit oEnable;
	plcbit Teach_StartPos;
	plcbit Teach_Mode;
	plcbit MachineType;
	plcbit Teach_EndPos;
	plcbit oZeroBegin;
	plcbit oldJogPos;
	plcbit oldJogNeg;
	plcbit oldJogPos_B;
	plcbit oldJogNeg_B;
	plcbit oldGoHome;
	plcbit oldRasetReset;
	plcbit oldTeach_StartPos;
	plcbit oldTeach_EndPos;
	plcbit oldVacnumOper;
	plcbit oldHomeServo;
	plcbit oldHomeServo_B;
	
} BendMachinePara_cmd_type;

typedef struct BendMachinePara_MCGS_type
{	signed long MaualVelocity_B;
	signed long MaualVelocity;
	signed long BackVelocity_B;
	signed long BackVelocity;
	signed long FollowVelocity_B;
	signed long FollowVelocity;
	signed long RasterOffset;
	signed long BackPosition_B;
	signed long BackPosition;
	signed long DEC_B;
	signed long ACC_B;
	signed long SuckDist;
	signed long DEC;
	signed long ACC;
	signed long EndPos;
	signed long StartPos;
	signed long RepeatTime;
	signed long StepSum;
	signed long PosLimitSw_B;
	signed long PlatThick;
	signed long Distance90Degree;
	signed long NegLimitSw_B;
	signed long PosLimitSw;
	signed long NegLimitSw;
	signed long ActStep;
	signed long ServalPos;
	signed long AutoBackPos;
	signed long RasterPos;
	signed long oServalPos;
	signed long oRasterPos;
	signed long HoldBendTime;
	signed long BendAngle;
	signed long VDistMachine;
	signed long Bend2TimeOfset;
	signed long MachineZeroOfset;
	signed long VMoldD;
	signed long SpringH;
	signed long MoldH;
	signed long oActStep;
} BendMachinePara_MCGS_type;

typedef struct QuitInfo_type{
	
	plcbit CoupleEnable;
	unsigned short statusA;
	unsigned short statusB;
	unsigned short statusVirtual;
	unsigned long CoupleStep;

}QuitInfo_type;

typedef struct BendMachine_type
{	signed long MeasurePosRaw;
	unsigned char Mode;
	plcbit CoupleEnable;
	plcbit MachineType;
	float TargetPosition_B;
	float TargetPosition;
	float MagPosCal;
	float MeasurePos;
	unsigned long CoupleStep;
	struct BendMachinePara_cmd_type McgsCmd;
	struct BendMachinePara_MCGS_type McgsPara;
	plcbit doHomeBend_B;
	plcbit doHomeBend;
	plcbit Bend2TimeOfst;
	plcbit CompensateNeeded;
	plcbit doVacnum;
	plcbit HomeBend_B;
	plcbit HomeBend;
	plcbit HomeOK_B;
	plcbit HomeOK;
	plcbit ResetButtom;
	plcbit GotoButtom;
	plcbit ResetRaster;
	unsigned char HomeStep;
	plcbit HoldStick;
	unsigned char ZeroofsetMode;
	plcbit cmdHomeofset[2];
	QuitInfo_type quitInfo;
	float BendPosMaxA;
	float BendPosMaxB;
	float speedscale;
	float MaxB33Percent;
} BendMachine_type;




void _CYCLIC_ProgramCyclic(void);
void _INIT_ProgramInit(void);


extern struct BendMachine_type BendMachine;
extern struct MchineEif_typ gMchineEif;
#endif


