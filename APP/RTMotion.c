#include "mInclude.h"

#include <math.h>
#define PULSE_VER 1
#define DEC_VIRTUAL 900.0
/* Variables */
float BackPos_A;
float BackPos_B;
SimulateEncoder_type SimulateEncoder={0};
float MaxMasurePos;
plcbit goBackFlag;
unsigned char FirstBoot;
float TargetPosition_B;
float TargetPosition;
float FlagMeasureOrigin;
signed long RepeatTimes;
struct TON_10ms_typ dlyHmeStep;
struct TON_10ms_typ dlyCouple;
struct TON_10ms_typ dlyFlagBack;
unsigned short CycT;
struct BendMachine_type BendMachine={0};
struct MchineEif_typ gMchineEif={1};
static BOOL flagd4,flagd1;
static REAL oldMeasurePos=0;
void AxisRTsParInit();
void simBendMachinePar();

void SimmulateEnPos(void);

void _INIT_ProgramInit(void)
{	
	
	CycT = 4;
	
	memset(&SimulateEncoder,0,sizeof(SimulateEncoder));
	
	memset(&BendMachine,0,sizeof(BendMachine));
	
	memset(&MDB,0,sizeof(MDB));
	
	////////////////////////
	AxisRTsParInit();
	////////////////////////
}
void AxisRTsParInit()
{	
	FirstBoot = 1;
	simBendMachinePar();
}
REAL BendXPositionRef(REAL h ,REAL d){
	if(pow(h,2)+pow(d,2)==0) return 0;
	return sqrt( 1472699.25 * ( 1 - 0.2319 * d / sqrt(pow(h,2)+pow(d,2)) + 0.9727 * h / sqrt(pow(h,2)+pow(d,2)) )) - 1063.62;
}
REAL ShortL1(REAL x ){
	LREAL D0 = gSystemPara.VDistMachine ; 
	LREAL D3 = gSystemPara.VmoldeD; 
	LREAL b = gSystemPara.AngleB;
	LREAL D4 = gSystemPara.PlatThick;
	LREAL BendH = gSystemPara.BendH;
	return (D0+77.8)* x / D3 + D4 - (D4 * sqrt(pow(x,2)+pow(D3,2)))/D3;
}
REAL XCalOutXfromL1(REAL pos){
	LREAL D0 = gSystemPara.VDistMachine ; 
	LREAL D3 = gSystemPara.VmoldeD; 
	LREAL b  = gSystemPara.AngleB;
	LREAL D4 = gSystemPara.PlatThick;
	LREAL BendH = gSystemPara.BendH;
	LREAL K1 = D0 + 77.8, K2 = -pos + D4,K3 = K1 / D3 ,x,x1,x2,resultx1,resultx2;
	LREAL A = pow(K3,2) - D4*D4/D3/D3 , B = 2*K2*K1 / D3 , C = pow(pos,2) - D4*2*pos;
	
	if (B*B - 4 * A*C < 0 || (A==0 && B==0))
		x = 1000;
	else if(A==0 && B!=0)
		x = -C / B;
	else
	{
		x2 = (-B - sqrt(B * B - 4 * A*C)) / (2 * A);
		x1 = (-B + sqrt(B * B - 4 * A*C)) / (2 * A);
		resultx1 = fabs(D4 * sqrt(pow(D3,2)+pow(x1,2)) / D3 - K3 * x1 - K2);
		resultx2 = fabs(D4 * sqrt(pow(D3,2)+pow(x2,2)) / D3 - K3 * x2 - K2);
		x = resultx1<resultx2?x1:x2;
	}
	return x*gSystemPara.AngleScaleK;
}

void RecordQuitInfo(void){
	BendMachine.quitInfo.CoupleEnable = BendMachine.CoupleEnable;
	BendMachine.quitInfo.statusA = AxisRTs[AX_DRAW].info.status;
	BendMachine.quitInfo.statusB = AxisRTs[AX_BEND].info.status;
	BendMachine.quitInfo.statusVirtual = AxisRTs[AX_VIRTRUAL].info.status;
	BendMachine.quitInfo.CoupleStep = BendMachine.CoupleStep;
}

void simBendMachinePar(){
	
	gSystemPara.SynVelocity = 1250.0;
	gSystemPara.MaualVelocity = 200.0;
	gSystemPara.BackVel = 1250.0;
	gSystemPara.ACC = 4000.0;
	gSystemPara.DEC = 4000.0;
	gSystemPara.StartPos = 280.0;
	gSystemPara.EndPos = 270.0;
	gSystemPara.BackPosition = 10;
	gSystemPara.OffsetPos = 320.0;
	gSystemPara.AutoBackPos = 0;
	gSystemPara.PosLimitSw = 4000.0;
	gSystemPara.NegLimitSw = -2.0;
	gSystemPara.BendAngle = 90.0;
	
	gSystemPara.SynVelocity_B = 500.0;
	gSystemPara.BackVel_B = 200.0;
	gSystemPara.BackPosition_B = 10.0;
	gSystemPara.MaualVelocity_B = 200.0;
	gSystemPara.ACC_B = 4000.0;
	gSystemPara.DEC_B = 4000.0;
	gSystemPara.PosLimitSw_B = 4000.0;
	gSystemPara.NegLimitSw_B = -2.0;
	
	gSystemPara.PlatThick = 75.0;
	gSystemPara.MachineZeroOfset = 0;
	gSystemPara.HoldBendTime = 1.0;
	gSystemPara.VDistMachine = 120.0;
	gSystemPara.Vmolde2D = 10.0;
	gSystemPara.SuckDist = 0.0;
	gSystemPara.SpringH = 0;
	
	gSystemPara.MoldH = 10.0;
	gSystemPara.Bend2TimeOfset = 2.0;
	
	gSystemPara.BendX1 = gSystemPara.StartPos;
	gSystemPara.BendH = gSystemPara.StartPos - gSystemPara.EndPos;;
	
	//gSystemPara.EnableSim = 1;
	
	gSystemPara.VmoldeD = gSystemPara.Vmolde2D / 2.0;
	
	
	AxisRTs[AX_DRAW].Parameter.SWLimits.Upper  		= gSystemPara.PosLimitSw;
	AxisRTs[AX_DRAW].Parameter.SWLimits.Lower  		= gSystemPara.NegLimitSw;	
	AxisRTs[AX_BEND].Parameter.SWLimits.Upper  		= gSystemPara.PosLimitSw_B;
	AxisRTs[AX_BEND].Parameter.SWLimits.Lower  		= gSystemPara.NegLimitSw_B;
	AxisRTs[AX_VIRTRUAL].Parameter.SWLimits.Upper  		= gSystemPara.PosLimitSw_B;
	AxisRTs[AX_VIRTRUAL].Parameter.SWLimits.Lower  		= gSystemPara.NegLimitSw_B;
	
	AxisRTs[AX_DRAW].Parameter.acceleration 		= gSystemPara.ACC;//10mm/s
	AxisRTs[AX_DRAW].Parameter.deceleration 		= gSystemPara.DEC;//10mm/s
	
	AxisRTs[AX_BEND].Parameter.acceleration 		= gSystemPara.ACC_B;//10mm/s
	AxisRTs[AX_BEND].Parameter.deceleration 		= gSystemPara.DEC_B;//10mm/s
		
	AxisRTs[AX_VIRTRUAL].Parameter.acceleration 		= gSystemPara.ACC_B;//10mm/s
	AxisRTs[AX_VIRTRUAL].Parameter.deceleration 		= DEC_VIRTUAL;//10mm/s
	

	gSystemPara.AngleB = 90 - gSystemPara.BendAngle / 2;
	gSystemPara.AngleScaleK = 1;
	gSystemPara.TargetExchScale = (gSystemPara.VDistMachine/1) / gSystemPara.VmoldeD;
	
	BendMachine.CoupleEnable = TRUE;

}

void BendMchineLogic()
{
	if (FirstBoot)
	{
		if(canMaster.parInitialed[canMaster.nodes[AX_DRAW]]==1&&canMaster.tpdoCnt[canMaster.nodes[AX_DRAW]]>100){
			AxisRTs[AX_DRAW].Power = 1;
		}
		if(canMaster.parInitialed[canMaster.nodes[AX_BEND]]==1&&canMaster.tpdoCnt[canMaster.nodes[AX_BEND]]>100){
			AxisRTs[AX_BEND].Power = 1;
			AxisRTs[AX_VIRTRUAL].Power = 1;
		}
		
		if(AxisRTs[AX_BEND].info.powerOn && 
		   AxisRTs[AX_DRAW].info.powerOn && 
		   AxisRTs[AX_BEND].info.netWorkinitialized && 
			 canMaster.tpdoCnt[canMaster.nodes[AX_BEND]]>100&&
			 canMaster.tpdoCnt[canMaster.nodes[AX_DRAW]]>100&&
			 AxisRTs[AX_DRAW].info.status==0 &&
		   AxisRTs[AX_BEND].info.status==0 &&
		   AxisRTs[AX_DRAW].info.netWorkinitialized && 
			 gMchineEif.Estop == 0){
			////////////////////////////////
			AxisRTs[AX_BEND].Homing = 1;
			AxisRTs[AX_DRAW].Homing = 1;
			AxisRTs[AX_VIRTRUAL].Homing = 1;
			AxisRTs[AX_BEND].Parameter.homing.position 			= AxisRTs[AX_BEND].info.position_REAL;
			AxisRTs[AX_DRAW].Parameter.homing.position 			= AxisRTs[AX_DRAW].info.position_REAL;
			AxisRTs[AX_VIRTRUAL].Parameter.homing.position 	= AxisRTs[AX_BEND].info.position_REAL;
			FirstBoot = 0;
		}
	}
	if (gSystemPara.MeasurePosScale<=0.00001)
		gSystemPara.MeasurePosScale = 5.0 / 1000.0;
	if (gSystemPara.SynVelocity<=0)
		gSystemPara.SynVelocity = 300;
	if (gSystemPara.SynVelocity_B<=0)
		gSystemPara.SynVelocity_B = 300;	
	if (gSystemPara.AxisBScale<=0)
		gSystemPara.AxisBScale = 3;
	
	AxisRTs[AX_DRAW].Parameter.SWLimits.Ignore		= 0;//方向
	AxisRTs[AX_BEND].Parameter.SWLimits.Ignore		= 0;//方向
	AxisRTs[AX_VIRTRUAL].Parameter.SWLimits.Ignore		= 0;//方向	
	
	if (gSystemPara.TargetExchScale<=0)
		gSystemPara.TargetExchScale = 1;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	SimmulateEnPos();
	///////////////////////////////////////////////////////////////////////////////// IO 部分的 程序
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////	
	//////io MAP
	if(BendMachine.ResetRaster == TRUE){
		TIM5->CNT = 0;
		gMachineIO.exeedTimes = 0;
	}
	
	BendMachine.MeasurePosRaw = gMachineIO.Encoder;
	gMchineEif.Estop					= !gMachineIO.DI[0];
	
	AxisRTs[AX_DRAW].Stop 		= gMchineEif.Estop;
	AxisRTs[AX_BEND].Stop 		= gMchineEif.Estop;
	AxisRTs[AX_VIRTRUAL].Stop = gMchineEif.Estop;
	
/*	AxisRTs[AX_DRAW].IOMapping.positiveLimitSwitch 			= !gMachineIO.DI[1];//
	AxisRTs[AX_DRAW].IOMapping.negativeLimitSwitch 			= !gMachineIO.DI[2];//
	AxisRTs[AX_BEND].IOMapping.positiveLimitSwitch 			= !gMachineIO.DI[3];//
	AxisRTs[AX_BEND].IOMapping.negativeLimitSwitch 			= !gMachineIO.DI[4];//
	AxisRTs[AX_VIRTRUAL].IOMapping.positiveLimitSwitch 	= !gMachineIO.DI[3];//
	AxisRTs[AX_VIRTRUAL].IOMapping.negativeLimitSwitch 	= !gMachineIO.DI[4];//
*/	
	////////////////////////////////////
	if (gMachineIO.DI[5]==1 && flagd1==0 && (BendMachine.CoupleStep==51 || BendMachine.CoupleStep==50)  ){
			BendMachine.ResetButtom 		= 1;
			BendMachine.ZeroofsetMode 	= 1;
		}
	if (gMachineIO.DI[6]==1 && flagd4==0 && BendMachine.CoupleStep==50){
		BendMachine.GotoButtom 			= 1;
		BendMachine.ZeroofsetMode 	= 0;
	}
	flagd1 = gMachineIO.DI[5];
	flagd4 = gMachineIO.DI[6];
	/////////////////////////////////
	///////////////////////////////
	/*	
	if (gMchineEif.Estop==0 && AxisRTs[AX_DRAW].info.powerOn && AxisRTs[AX_DRAW].info.status>0 && !AxisRTs[AX_DRAW].info.moveActive)
		AxisRTs[AX_DRAW].Acknowledge = 1;
	if (gMchineEif.Estop==0 && AxisRTs[AX_BEND].info.powerOn && AxisRTs[AX_BEND].info.status>0 && !AxisRTs[AX_BEND].info.moveActive)
		AxisRTs[AX_BEND].Acknowledge = 1;
	*/
	AxisRTs[AX_DRAW].IOMapping.iReadyExt 		= 1; /* 伺服准备好  */
	AxisRTs[AX_BEND].IOMapping.iReadyExt 		= 1; /* 伺服准备好  */
	////////////////////////////////////////////////////////////////////////////////Digital Output mapping
	gMachineIO.DO[0] = BendMachine.doVacnum;
	/////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////// IO 部分的 程序	
	BendMachine.MeasurePos = -BendMachine.MeasurePosRaw * gSystemPara.MeasurePosScale + gSystemPara.OffsetPos;	//	
	/////////////////////////////////////////////////////////////////////////////////
	if (gSystemPara.EnableSim)//仿真开关
		BendMachine.MeasurePos = SimulateEncoder.OutPos;
	/////////////////////////////////////////////////////////////////////////////////
	BendMachine.TargetPosition = BendXPositionRef((fabs(BendMachine.MagPosCal - gSystemPara.BendX1)/gSystemPara.AngleScaleK),gSystemPara.VmoldeD);
	BendMachine.TargetPosition_B = ShortL1(fabs(BendMachine.MeasurePos - gSystemPara.BendX1)/gSystemPara.AngleScaleK);
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	if(/*BendMachine.ZeroofsetMode == 1 &&*/ BendMachine.CompensateNeeded == 0)
		gSystemPara.MachineZeroOfset = gSystemPara.MachineZeroOfsetTemp;
	else
		gSystemPara.MachineZeroOfset = 0;
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	if (BendMachine.MachineType==0){//上动式 // 升降轴
		/////////////////////////////////////////////////////////////////////////////////			
		BendMachine.BendPosMaxB = ShortL1(fabs(gSystemPara.BendH)/gSystemPara.AngleScaleK) - gSystemPara.SpringH + gSystemPara.MachineZeroOfset;
		BendMachine.MaxB33Percent = ShortL1(fabs(gSystemPara.BendH)/gSystemPara.AngleScaleK)*0.33;
		
		if(AxisRTs[AX_VIRTRUAL].info.position > BendMachine.BendPosMaxB - BendMachine.MaxB33Percent && BendMachine.MaxB33Percent > 0){		
			BendMachine.speedscale = (AxisRTs[AX_VIRTRUAL].info.position - (BendMachine.BendPosMaxB - BendMachine.MaxB33Percent)) / BendMachine.MaxB33Percent;
		}
		else BendMachine.speedscale = 0.0;
		BendMachine.speedscale = 0.0;
		/////////////////////////////////////////////////////////////////////////////////
		if (BendMachine.CoupleStep>0){
			if (AxisRTs[AX_VIRTRUAL].info.position>=( -gSystemPara.SpringH + gSystemPara.MachineZeroOfset) &&
				BendMachine.CoupleStep>50)//这里要极细推敲一下
			//if (BendMachine.MeasurePos <= gSystemPara.BendX1)//这里要极细推敲一下	
				BendMachine.MagPosCal =gSystemPara.BendX1 - XCalOutXfromL1( AxisRTs[AX_VIRTRUAL].info.position + gSystemPara.SpringH - gSystemPara.MachineZeroOfset);
			else
				BendMachine.MagPosCal =gSystemPara.BendX1;
			////////////////////////////////////////////////////////////////////////////
			if (BendMachine.MeasurePos>=(gSystemPara.BendX1+gSystemPara.SpringH))
			{
				TargetPosition_B = 0;
				AxisRTs[AX_DRAW].Parameter.speed = gSystemPara.SynVelocity;
				AxisRTs[AX_VIRTRUAL].Parameter.speed = 0.5 * gSystemPara.SynVelocity_B + (1.0 - BendMachine.speedscale) * 0.5 * gSystemPara.SynVelocity_B;		
			}			
			else if (BendMachine.MeasurePos>=gSystemPara.BendX1){//在夹紧点下面的时候，都不动
				if (BendMachine.CompensateNeeded)
					TargetPosition_B = BendMachine.MeasurePos - (gSystemPara.BendX1+gSystemPara.SpringH);				
				AxisRTs[AX_DRAW].Parameter.speed = gSystemPara.SynVelocity;
				AxisRTs[AX_VIRTRUAL].Parameter.speed = 0.5 * gSystemPara.SynVelocity_B + (1.0 - BendMachine.speedscale) * 0.5 * gSystemPara.SynVelocity_B;
			}
			else if (BendMachine.MeasurePos<=(gSystemPara.BendX1 - gSystemPara.BendH)){//超过行程后，直接到终点
				TargetPosition_B = ShortL1(fabs(gSystemPara.BendH)/gSystemPara.AngleScaleK) - gSystemPara.SpringH;//ok,
				AxisRTs[AX_DRAW].Parameter.speed = gSystemPara.SynVelocity;
				AxisRTs[AX_VIRTRUAL].Parameter.speed = 0.5 * gSystemPara.SynVelocity_B + (1.0 - BendMachine.speedscale) * 0.5 * gSystemPara.SynVelocity_B;
			}
			else{//这是在折弯范围内
				TargetPosition_B = BendMachine.TargetPosition_B - gSystemPara.SpringH;// 
				AxisRTs[AX_DRAW].Parameter.speed = gSystemPara.SynVelocity;
				AxisRTs[AX_VIRTRUAL].Parameter.speed = 0.5 * gSystemPara.SynVelocity_B + (1.0 - BendMachine.speedscale) * 0.5 * gSystemPara.SynVelocity_B;
			}
		}
		
	}
	else if(BendMachine.MachineType==1){//下动式
		/////////////////////////////////////////////////////////////////////////////////
		BendMachine.BendPosMaxB = fabs(gSystemPara.BendX1 - gSystemPara.OffsetPos) + ShortL1(fabs(gSystemPara.BendH)/gSystemPara.AngleScaleK) + gSystemPara.MachineZeroOfset;
		BendMachine.MaxB33Percent = ShortL1(fabs(gSystemPara.BendH)/gSystemPara.AngleScaleK)*0.33;
		
		if(AxisRTs[AX_VIRTRUAL].info.position > BendMachine.BendPosMaxB - BendMachine.MaxB33Percent && BendMachine.MaxB33Percent > 0){		
			BendMachine.speedscale = (AxisRTs[AX_VIRTRUAL].info.position - (BendMachine.BendPosMaxB - BendMachine.MaxB33Percent)) / BendMachine.MaxB33Percent;
		}
		else BendMachine.speedscale = 0.0;
		BendMachine.speedscale = 0.0;
		/////////////////////////////////////////////////////////////////////////////////
		if (BendMachine.CoupleStep>0){
			
			if ((AxisRTs[AX_VIRTRUAL].info.position - gSystemPara.MachineZeroOfset)<= (gSystemPara.BendX1 - gSystemPara.OffsetPos)){
				BendMachine.MagPosCal = gSystemPara.OffsetPos - AxisRTs[AX_VIRTRUAL].info.position;
			}
			else{
				BendMachine.MagPosCal = gSystemPara.BendX1 - XCalOutXfromL1(AxisRTs[AX_VIRTRUAL].info.position - gSystemPara.MachineZeroOfset - fabs(gSystemPara.BendX1 - gSystemPara.OffsetPos));
			}			
			if (BendMachine.MeasurePos>=gSystemPara.BendX1){//在夹紧点下面的时候，都不动
				TargetPosition_B = fabs(BendMachine.MeasurePos - gSystemPara.OffsetPos);//ok
				AxisRTs[AX_DRAW].Parameter.speed = gSystemPara.SynVelocity;
				AxisRTs[AX_VIRTRUAL].Parameter.speed = 0.5 * gSystemPara.SynVelocity_B + (1.0 - BendMachine.speedscale) * 0.5 * gSystemPara.SynVelocity_B;
			}
			else if (BendMachine.MeasurePos<=(gSystemPara.BendX1 - gSystemPara.BendH)){//超过行程后，直接到终点
				TargetPosition_B = fabs(gSystemPara.BendX1 - gSystemPara.OffsetPos) + ShortL1(fabs(gSystemPara.BendH)/gSystemPara.AngleScaleK);//
				AxisRTs[AX_DRAW].Parameter.speed = gSystemPara.SynVelocity;
				AxisRTs[AX_VIRTRUAL].Parameter.speed = 0.5 * gSystemPara.SynVelocity_B + (1.0 - BendMachine.speedscale) * 0.5 * gSystemPara.SynVelocity_B;
			}
			else{//这是在折弯范围内
				TargetPosition_B = fabs(gSystemPara.BendX1 - gSystemPara.OffsetPos) + BendMachine.TargetPosition_B;
				AxisRTs[AX_DRAW].Parameter.speed = gSystemPara.SynVelocity;
				AxisRTs[AX_VIRTRUAL].Parameter.speed = 0.5 * gSystemPara.SynVelocity_B + (1.0 - BendMachine.speedscale) * 0.5 * gSystemPara.SynVelocity_B;
			}
		}
	}	
	if (BendMachine.MachineType==0){//上动式  //推缸
		/////////////////////////////////////////////////////////////////////////////////
		BendMachine.BendPosMaxA = BendXPositionRef(fabs(gSystemPara.BendH/gSystemPara.AngleScaleK),gSystemPara.VmoldeD);
		/////////////////////////////////////////////////////////////////////////////////	
		if (BendMachine.CoupleStep>0){
			if (BendMachine.MagPosCal>=gSystemPara.BendX1){//在夹紧点下面的时候，都不动
				TargetPosition = 0;
				AxisRTs[AX_DRAW].Parameter.speed = gSystemPara.SynVelocity;
				AxisRTs[AX_VIRTRUAL].Parameter.speed = gSystemPara.SynVelocity_B;
			}
			else if (BendMachine.MagPosCal<=(gSystemPara.BendX1 - gSystemPara.BendH)){//超过行程后，直接到终点
				TargetPosition = BendXPositionRef(fabs(gSystemPara.BendH/gSystemPara.AngleScaleK),gSystemPara.VmoldeD);// need confirm.
				AxisRTs[AX_DRAW].Parameter.speed = gSystemPara.SynVelocity;
				AxisRTs[AX_VIRTRUAL].Parameter.speed = gSystemPara.SynVelocity_B;
			}
			else{//这是在折弯范围内
				TargetPosition = BendMachine.TargetPosition;// need confirm.
				AxisRTs[AX_DRAW].Parameter.speed = gSystemPara.SynVelocity;
				AxisRTs[AX_VIRTRUAL].Parameter.speed = gSystemPara.SynVelocity_B;
			}			
		}
	}
	else if(BendMachine.MachineType==1){//下动式
		/////////////////////////////////////////////////////////////////////////////////
		BendMachine.BendPosMaxA = BendXPositionRef(fabs(gSystemPara.BendH/gSystemPara.AngleScaleK),gSystemPara.VmoldeD);
		/////////////////////////////////////////////////////////////////////////////////		
		if (BendMachine.CoupleStep>0){
			if (BendMachine.MagPosCal>=gSystemPara.BendX1){//在夹紧点下面的时候，都不动
				TargetPosition = 0;
				AxisRTs[AX_DRAW].Parameter.speed = gSystemPara.SynVelocity;
				AxisRTs[AX_VIRTRUAL].Parameter.speed = gSystemPara.SynVelocity_B;
			}
			else if (BendMachine.MagPosCal<=(gSystemPara.BendX1 - gSystemPara.BendH)){//超过行程后，直接到终点
				TargetPosition = BendXPositionRef(fabs(gSystemPara.BendH/gSystemPara.AngleScaleK),gSystemPara.VmoldeD);
				AxisRTs[AX_DRAW].Parameter.speed = gSystemPara.SynVelocity;
				AxisRTs[AX_VIRTRUAL].Parameter.speed = gSystemPara.SynVelocity_B;
			}
			else{//这是在折弯范围内
				TargetPosition = BendMachine.TargetPosition;
				AxisRTs[AX_DRAW].Parameter.speed = gSystemPara.SynVelocity;
				AxisRTs[AX_VIRTRUAL].Parameter.speed = gSystemPara.SynVelocity_B;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	switch (BendMachine.CoupleStep)
	{
		case 0://等待耦合的信号
			if (BendMachine.CoupleEnable==1 && 
				AxisRTs[AX_DRAW].info.homingOk && 
				AxisRTs[AX_VIRTRUAL].info.homingOk && 
				fabs(AxisRTs[AX_DRAW].info.position_REAL) <=5 && 
				fabs(AxisRTs[AX_VIRTRUAL].info.position_REAL) <=5 &&
				!gMchineEif.Estop &&
				AxisRTs[AX_DRAW].info.status==0 &&
				AxisRTs[AX_VIRTRUAL].info.status==0 &&
				BendMachine.MeasurePos >= gSystemPara.BendX1)
			{
				BendMachine.CoupleStep = 10;
				BackPos_A = 0;
				BackPos_B = 0;
			}
			break;

		case 10:
			AxisRTs[AX_DRAW].Power = 1;
			AxisRTs[AX_VIRTRUAL].Power = 1;
			BendMachine.CoupleStep = 20;
			BendMachine.McgsPara.oActStep = BendMachine.McgsPara.ActStep;
			break;
		
		case 20:
			if (AxisRTs[AX_DRAW].info.powerOn && 
				AxisRTs[AX_VIRTRUAL].info.powerOn)
			{
				BendMachine.CoupleStep = 30;
			}
			break;
		
		case 30:
			AxisRTs[AX_DRAW].Parameter.homing.position =  AxisRTs[AX_DRAW].info.position_REAL+0.1;
			AxisRTs[AX_DRAW].Homing = 1;
			AxisRTs[AX_VIRTRUAL].Parameter.homing.position =  AxisRTs[AX_VIRTRUAL].info.position_REAL+0.1;
			AxisRTs[AX_VIRTRUAL].Homing = 1;
			BendMachine.CoupleStep = 40;
			break;
		
		case 40:
			if (!AxisRTs[AX_DRAW].Homing && AxisRTs[AX_DRAW].info.homingOk  &&
				!AxisRTs[AX_VIRTRUAL].Homing && AxisRTs[AX_VIRTRUAL].info.homingOk ){
				BendMachine.CoupleStep = 42;
			}
			break;
		
		case 42:
			if (fabs(AxisRTs[AX_DRAW].info.position - AxisRTs[AX_DRAW].info.position_REAL)<1.0 && 
				fabs(AxisRTs[AX_VIRTRUAL].info.position - AxisRTs[AX_VIRTRUAL].info.position_REAL)<1.0)
			/////////////
				BendMachine.CoupleStep = 44;
			/////////////
			if (gMchineEif.Estop||!BendMachine.CoupleEnable){
				BendMachine.CoupleStep = 0;
			}
			break;
		
		case 44:
			AxisRTs[AX_BEND].MoveCyclicPosition = 1;			
			BendMachine.CoupleStep = 46;
			if (gMchineEif.Estop||!BendMachine.CoupleEnable){
				BendMachine.CoupleStep = 0;
			}
			break;
		
		case 46:
			BendMachine.CoupleStep = 47;
			if (gMchineEif.Estop||!BendMachine.CoupleEnable){
				BendMachine.CoupleStep = 0;
			}
			break;
			
		case 47:
			AxisRTs[AX_VIRTRUAL].Parameter.position = gSystemPara.MachineZeroOfset;
			AxisRTs[AX_VIRTRUAL].MoveAbsolute = 1;
			BendMachine.CoupleStep = 48;
			break;
		
		case 48:
			if (AxisRTs[AX_VIRTRUAL].MoveAbsolute==0 &&
			fabs(AxisRTs[AX_VIRTRUAL].info.position-gSystemPara.MachineZeroOfset)<10){
				BendMachine.CoupleStep = 50;
			}
			if (BendMachine.CoupleEnable==0 || AxisRTs[AX_DRAW].Stop || AxisRTs[AX_DRAW].info.status>0 || AxisRTs[AX_VIRTRUAL].info.status>0){
				AxisRTs[AX_DRAW].MoveCyclicPosition = 0;
				AxisRTs[AX_VIRTRUAL].MoveCyclicPosition = 0;
				AxisRTs[AX_BEND].MoveCyclicPosition = 0;
				RecordQuitInfo();
				BendMachine.CoupleStep = 0;
			}////////////////////////
			break;
	
	
	
	
		case 50:
			AxisRTs[AX_DRAW].MoveCyclicPosition = 1;
			AxisRTs[AX_VIRTRUAL].MoveCyclicPosition = 1;	
			AxisRTs[AX_VIRTRUAL].Parameter.deceleration = DEC_VIRTUAL;
		
			if (BackPos_A > 10 && BackPos_B>10){
				if (TargetPosition>BackPos_A)AxisRTs[AX_DRAW].Parameter.position = TargetPosition;
				if (TargetPosition_B+ gSystemPara.MachineZeroOfset>BackPos_B)AxisRTs[AX_VIRTRUAL].Parameter.position = TargetPosition_B + gSystemPara.MachineZeroOfset;
			}
			else{
				AxisRTs[AX_DRAW].Parameter.position = TargetPosition;
				AxisRTs[AX_VIRTRUAL].Parameter.position = TargetPosition_B + gSystemPara.MachineZeroOfset;		
			}
			/////////////////////////
			if (BendMachine.MeasurePos< (gSystemPara.BendX1+gSystemPara.SuckDist ))//开始折弯
			{
				BendMachine.CoupleStep = 55;
				BendMachine.doVacnum = 1;//真空吸打开
			}	
			/////////////////////////
			if (BendMachine.GotoButtom)
			{
				BendMachine.GotoButtom = 0;
				if (BendMachine.CompensateNeeded)
					BendMachine.CoupleStep = 51;
			}			
			/////////////////////////
			if (BendMachine.CoupleEnable==0 || AxisRTs[AX_DRAW].Stop || AxisRTs[AX_DRAW].info.status>0 || AxisRTs[AX_VIRTRUAL].info.status>0){
				AxisRTs[AX_DRAW].MoveCyclicPosition = 0;
				AxisRTs[AX_VIRTRUAL].MoveCyclicPosition = 0;
				AxisRTs[AX_BEND].MoveCyclicPosition = 0;
				RecordQuitInfo();
				BendMachine.CoupleStep = 0;
			}////////////////////////
			
			break;
		
		case 51:
			/////////////////////////
			AxisRTs[AX_VIRTRUAL].Parameter.position = -(gSystemPara.SpringH + gSystemPara.MoldH);
			AxisRTs[AX_VIRTRUAL].Parameter.speed = gSystemPara.BackVel_B;
			/////////////////////////
			if (BendMachine.ResetButtom){
				BendMachine.ResetButtom = 0;
				if (BendMachine.MeasurePos >= (gSystemPara.BendX1+gSystemPara.SpringH))
					BendMachine.CoupleStep =52;	
			}
			/////////////////////////
			if (BendMachine.CoupleEnable==0 || AxisRTs[AX_DRAW].Stop || AxisRTs[AX_DRAW].info.status>0 || AxisRTs[AX_VIRTRUAL].info.status>0){
				AxisRTs[AX_DRAW].MoveCyclicPosition = 0;
				AxisRTs[AX_VIRTRUAL].MoveCyclicPosition = 0;
				AxisRTs[AX_BEND].MoveCyclicPosition = 0;
				RecordQuitInfo();
				BendMachine.CoupleStep = 0;
			}////////////////////////

			break;
		
		case 52:
			/////////////////////////
			AxisRTs[AX_VIRTRUAL].Parameter.position = 0;
			AxisRTs[AX_VIRTRUAL].Parameter.speed = gSystemPara.BackVel_B;
			/////////////////////////
			if (AxisRTs[AX_VIRTRUAL].info.position>=-2){//差不多回到0点
				BendMachine.CoupleStep =50;	
			}
			/////////////////////////
			if (BendMachine.CoupleEnable==0 || AxisRTs[AX_DRAW].Stop || AxisRTs[AX_DRAW].info.status>0 || AxisRTs[AX_VIRTRUAL].info.status>0){
				AxisRTs[AX_DRAW].MoveCyclicPosition = 0;
				AxisRTs[AX_VIRTRUAL].MoveCyclicPosition = 0;
				AxisRTs[AX_BEND].MoveCyclicPosition = 0;
				RecordQuitInfo();
				BendMachine.CoupleStep = 0;
			}////////////////////////
			break;
		
		case 55:
			AxisRTs[AX_DRAW].MoveCyclicPosition = 1;
			AxisRTs[AX_VIRTRUAL].MoveCyclicPosition = 1;			
			if (BackPos_A > 10 && BackPos_B>10){
				if (TargetPosition>BackPos_A)AxisRTs[AX_DRAW].Parameter.position = TargetPosition;
				if (TargetPosition_B + gSystemPara.MachineZeroOfset > BackPos_B)AxisRTs[AX_VIRTRUAL].Parameter.position = TargetPosition_B + gSystemPara.MachineZeroOfset;
			}
			else{
				AxisRTs[AX_DRAW].Parameter.position = TargetPosition;
				AxisRTs[AX_VIRTRUAL].Parameter.position = TargetPosition_B + gSystemPara.MachineZeroOfset;		
			}
			MaxMasurePos = BendMachine.MeasurePos;
			goBackFlag = 0;
			if (BendMachine.MeasurePos<gSystemPara.BendX1 - 0.5)//认为已经进入折弯
				BendMachine.CoupleStep = 56;

			/////////////////////////
			if (BendMachine.CoupleEnable==0 || AxisRTs[AX_DRAW].Stop || AxisRTs[AX_DRAW].info.status>0 || AxisRTs[AX_VIRTRUAL].info.status>0){
				AxisRTs[AX_DRAW].MoveCyclicPosition = 0;
				AxisRTs[AX_VIRTRUAL].MoveCyclicPosition = 0;
				AxisRTs[AX_BEND].MoveCyclicPosition = 0;
				RecordQuitInfo();
				BendMachine.CoupleStep = 0;

			}////////////////////////
			oldMeasurePos = BendMachine.MeasurePos;
			dlyFlagBack.IN = 0;
			break;
		
		case 56:
			AxisRTs[AX_DRAW].MoveCyclicPosition = 1;
			AxisRTs[AX_VIRTRUAL].MoveCyclicPosition = 1;			
		if (goBackFlag == 0 && (BendMachine.MeasurePos < MaxMasurePos || BendMachine.MeasurePos == oldMeasurePos)  )
			{		
				if (BackPos_A > 10 && BackPos_B>10){
					if (TargetPosition>BackPos_A)
						AxisRTs[AX_DRAW].Parameter.position = TargetPosition;
					if (TargetPosition_B + gSystemPara.MachineZeroOfset>BackPos_B)
						AxisRTs[AX_VIRTRUAL].Parameter.position = TargetPosition_B + gSystemPara.MachineZeroOfset;
				}
				else{
					AxisRTs[AX_DRAW].Parameter.position = TargetPosition;
					AxisRTs[AX_VIRTRUAL].Parameter.position = TargetPosition_B + gSystemPara.MachineZeroOfset;		
				}
			}
			if (BackPos_A > 10){
				if (TargetPosition>BackPos_A)
					AxisRTs[AX_DRAW].Parameter.position = TargetPosition;
			}
			else{
				AxisRTs[AX_DRAW].Parameter.position = TargetPosition;
			}
			
			/////////////////////////									
			MaxMasurePos = MINReal(MaxMasurePos,BendMachine.MeasurePos);
			oldMeasurePos = BendMachine.MeasurePos;
			/*
			if(BendMachine.MeasurePos > MaxMasurePos + 0.5)
				goBackFlag = 1;
			*/
			dlyFlagBack.IN = BendMachine.MeasurePos > MaxMasurePos + 0.5;
			dlyFlagBack.PT = (0.2) * 1000.0 / CycT;
			
			dlyCouple.IN = goBackFlag ;
			dlyCouple.PT = (gSystemPara.HoldBendTime + 0.1) * 1000.0 / CycT;
			
			if(dlyFlagBack.Q){
				goBackFlag = 1;
			}
			
			if (dlyCouple.Q)//认为已经折弯结束
			{
				BendMachine.CoupleStep = 60;
				goBackFlag = 0;
				dlyCouple.IN = 0;
				dlyFlagBack.IN = 0;
			}
				
			if (BendMachine.CoupleEnable==0 || AxisRTs[AX_DRAW].Stop || AxisRTs[AX_DRAW].info.status>0 || AxisRTs[AX_VIRTRUAL].info.status>0){
				AxisRTs[AX_DRAW].MoveCyclicPosition = 0;
				AxisRTs[AX_VIRTRUAL].MoveCyclicPosition = 0;
				AxisRTs[AX_BEND].MoveCyclicPosition = 0;
				RecordQuitInfo();
				BendMachine.CoupleStep = 0;
				dlyCouple.IN = 0;
			}////////////////////////

		
			break;	
		
		
		case 60:			
			dlyCouple.IN =(AxisRTs[AX_DRAW].info.speed==0 && AxisRTs[AX_VIRTRUAL].info.speed==0 && BendMachine.MagPosCal + 0.1 >= gSystemPara.BendX1);//如果已经返回到反回的位置，并且磁珊也返回到初始位置
			dlyCouple.PT = 0.1 * 1000 / 4;
			if (dlyCouple.Q){
				dlyCouple.IN = 0;
				BendMachine.CoupleStep = 65;
				MDB.WriteCmdRgisterAsyn = 1;//那么把下一次需要执行的步数告诉触摸屏
				RepeatTimes++;
				if (RepeatTimes>=BendMachine.McgsPara.RepeatTime)//如果次数比设定次数要多
				{
					if (BendMachine.McgsPara.oActStep>=BendMachine.McgsPara.StepSum)//那么当前步变成1，让触摸屏输出第一步的参数
						BendMachine.McgsPara.oActStep = 1;
					else
						BendMachine.McgsPara.oActStep++;//把下一次需要执行的步数告诉触摸屏
					BendMachine.McgsPara.RepeatTime = 0;//清零，等待读取的值
					RepeatTimes = 0;
				}
				else
				{
					BendMachine.CoupleStep = 50;//次数不够那就继续循环
				}
				BendMachine.doVacnum = 0;
			}		
			AxisRTs[AX_DRAW].MoveCyclicPosition = 1;
			AxisRTs[AX_VIRTRUAL].MoveCyclicPosition = 1;			

			//////////////返回的时候需要根据返回角度返回//////////////////////////
			if (gSystemPara.AutoBackPos >= 0 && gSystemPara.AutoBackPos < gSystemPara.AngleB)
			{
				if (BendMachine.MachineType==1){
					BackPos_A = BendXPositionRef((fabs(gSystemPara.BendH * gSystemPara.AutoBackPos/gSystemPara.AngleB)/gSystemPara.AngleScaleK),gSystemPara.VmoldeD);
					
					if (gSystemPara.AutoBackPos == 0)
						BackPos_B = gSystemPara.MachineZeroOfset;
					else
						BackPos_B = fabs(gSystemPara.BendX1 - gSystemPara.OffsetPos) + ShortL1(fabs(gSystemPara.BendH * gSystemPara.AutoBackPos  /gSystemPara.AngleB)) + gSystemPara.MachineZeroOfset;	
					
				}
				else{
					BackPos_A =  BendXPositionRef((fabs(gSystemPara.BendH * gSystemPara.AutoBackPos/gSystemPara.AngleB)/gSystemPara.AngleScaleK),gSystemPara.VmoldeD);
					BackPos_B =  ShortL1(fabs(gSystemPara.BendH * gSystemPara.AutoBackPos  /gSystemPara.AngleB)) - gSystemPara.SpringH + gSystemPara.MachineZeroOfset;
					
				}
				/////////////////////////////////////////////////
				if (TargetPosition>BackPos_A)
					AxisRTs[AX_DRAW].Parameter.position = TargetPosition;
				else
					AxisRTs[AX_DRAW].Parameter.position = BackPos_A;
				/////////////////////////////////////////////////
				if (TargetPosition_B + gSystemPara.MachineZeroOfset>BackPos_B)
					AxisRTs[AX_VIRTRUAL].Parameter.position = TargetPosition_B + gSystemPara.MachineZeroOfset;							
				else
					AxisRTs[AX_VIRTRUAL].Parameter.position = BackPos_B;
				/////////////////////////////////////////////////
			}
			else{
				AxisRTs[AX_DRAW].Parameter.position = TargetPosition;
				AxisRTs[AX_VIRTRUAL].Parameter.position = TargetPosition_B + gSystemPara.MachineZeroOfset;
			}
			AxisRTs[AX_DRAW].Parameter.speed = gSystemPara.BackVel;
			AxisRTs[AX_VIRTRUAL].Parameter.speed = gSystemPara.BackVel_B;
			/////////////////////////
			if (BendMachine.CoupleEnable==0 || AxisRTs[AX_DRAW].Stop || AxisRTs[AX_DRAW].info.status>0 || AxisRTs[AX_VIRTRUAL].info.status>0){
				AxisRTs[AX_DRAW].MoveCyclicPosition = 0;
				AxisRTs[AX_VIRTRUAL].MoveCyclicPosition = 0;
				AxisRTs[AX_BEND].MoveCyclicPosition = 0;
				RecordQuitInfo();
				BendMachine.CoupleStep = 0;

				dlyCouple.IN = 0;
			}////////////////////////
			break;
			
		case 65:
			
			if (BendMachine.McgsPara.oActStep==BendMachine.McgsPara.ActStep)
			{
				BendMachine.CoupleStep = 50;//等待刷新参数
			}
			else{
				MDB.WriteCmdRgisterAsyn = gMachineIO.LED;
			}
			AxisRTs[AX_DRAW].MoveCyclicPosition = 1;
			AxisRTs[AX_VIRTRUAL].MoveCyclicPosition = 1;			
			if (BackPos_A > 10 && BackPos_B>10){
				if (TargetPosition>BackPos_A)AxisRTs[AX_DRAW].Parameter.position = TargetPosition;
				if (TargetPosition_B + gSystemPara.MachineZeroOfset>BackPos_B)AxisRTs[AX_VIRTRUAL].Parameter.position = TargetPosition_B + gSystemPara.MachineZeroOfset;
			}
			else{
				AxisRTs[AX_DRAW].Parameter.position = TargetPosition;
				AxisRTs[AX_VIRTRUAL].Parameter.position = TargetPosition_B + gSystemPara.MachineZeroOfset;		
			}
			AxisRTs[AX_VIRTRUAL].Parameter.speed = gSystemPara.BackVel_B;
			/////////////////////////
			if (BendMachine.CoupleEnable==0 || AxisRTs[AX_DRAW].Stop || AxisRTs[AX_DRAW].info.status>0 || AxisRTs[AX_VIRTRUAL].info.status>0){
				AxisRTs[AX_DRAW].MoveCyclicPosition = 0;
				AxisRTs[AX_VIRTRUAL].MoveCyclicPosition = 0;
				AxisRTs[AX_BEND].MoveCyclicPosition = 0;
				RecordQuitInfo();
				BendMachine.CoupleStep = 0;
			}////////////////////////
			break;
		default:
			break;
	}
	TON_10ms(&dlyFlagBack);
	TON_10ms(&dlyCouple);
	
	if (AxisRTs[AX_DRAW].Parameter.position>=gSystemPara.PosLimitSw)
		AxisRTs[AX_DRAW].Parameter.position = gSystemPara.PosLimitSw - 0.05;
	if (AxisRTs[AX_DRAW].Parameter.position<=gSystemPara.NegLimitSw)
		AxisRTs[AX_DRAW].Parameter.position = gSystemPara.NegLimitSw + 0.05;
	
	if (AxisRTs[AX_VIRTRUAL].Parameter.position>=gSystemPara.PosLimitSw_B)
		AxisRTs[AX_VIRTRUAL].Parameter.position = gSystemPara.PosLimitSw_B - 0.05;
	if (AxisRTs[AX_VIRTRUAL].Parameter.position<=gSystemPara.NegLimitSw_B)
		AxisRTs[AX_VIRTRUAL].Parameter.position = gSystemPara.NegLimitSw_B + 0.05;

	if (BendMachine.CoupleStep>0){		
		if (AxisRTs[AX_VIRTRUAL].Parameter.position<(AxisRTs[AX_VIRTRUAL].info.position))	{	
			if(AxisRTs[AX_VIRTRUAL].info.position< BendMachine.BendPosMaxB*0.3){
				AxisRTs[AX_VIRTRUAL].Parameter.speed = gSystemPara.BackVel_B*0.366;
			}else{
				AxisRTs[AX_VIRTRUAL].Parameter.speed = gSystemPara.BackVel_B;
				AxisRTs[AX_VIRTRUAL].Parameter.deceleration = DEC_VIRTUAL * 0.05;
			}
		}
	}
	
	if(AxisRTs[AX_BEND].MoveCyclicPosition==1){
		AxisRTs[AX_BEND].Parameter.position = AxisRTs[AX_VIRTRUAL].info.position;
		AxisRTs[AX_BEND].Parameter.speed = AxisRTs[AX_VIRTRUAL].Parameter.speed;
	}
	

	
}
void SimmulateEnPos(void){
 
	SimulateEncoder.stepIncNormal = SimulateEncoder.VelocityuUpDown * CycT/1000.0;
	SimulateEncoder.stepIncBend   = SimulateEncoder.VelocityBend * CycT/1000.0;
	SimulateEncoder.BeginPos = gSystemPara.OffsetPos;
	SimulateEncoder.X1Pos = gSystemPara.BendX1;
	SimulateEncoder.EndPos = gSystemPara.BendX1 - gSystemPara.BendH;
	
	if (SimulateEncoder.VelocityuUpDown<=0)
		SimulateEncoder.VelocityuUpDown = 10;
	if (SimulateEncoder.VelocityBend<=1)
		SimulateEncoder.VelocityBend = 15;
	
	switch (SimulateEncoder.StepSim)
	{
		case 0:
			if (BendMachine.CoupleEnable)
				SimulateEncoder.StepSim = 10;
			break;

		case 10:
			SimulateEncoder.OutPos = SimulateEncoder.BeginPos;
			if (BendMachine.CoupleStep==50)
				SimulateEncoder.StepSim = 40;	
			break;
		
		case 40://自动加减
			if (SimulateEncoder.OutPos>SimulateEncoder.X1Pos)
				SimulateEncoder.OutPos = SimulateEncoder.OutPos - SimulateEncoder.stepIncNormal;
			else if (SimulateEncoder.OutPos<=SimulateEncoder.X1Pos && SimulateEncoder.OutPos>SimulateEncoder.EndPos)
				SimulateEncoder.OutPos = SimulateEncoder.OutPos - SimulateEncoder.stepIncBend;
			else
				SimulateEncoder.StepSim = 50;
			
			if (BendMachine.CoupleStep<50)
			{
				SimulateEncoder.StepSim = 0;
				SimulateEncoder.delaySim.IN = 0;
			}
			break;
		
		case 50:
			SimulateEncoder.delaySim.IN = 1;
			SimulateEncoder.delaySim.PT = 3 * 1000 / 4;		
			if (SimulateEncoder.delaySim.Q)
			{
				SimulateEncoder.delaySim.IN = 0;
				SimulateEncoder.StepSim = 60;
			}
			if (BendMachine.CoupleStep<50)
			{
				SimulateEncoder.StepSim = 0;
				SimulateEncoder.delaySim.IN = 0;
			}
			break;
		
		case 60:
			if (SimulateEncoder.OutPos<SimulateEncoder.BeginPos)
				SimulateEncoder.OutPos = SimulateEncoder.OutPos + SimulateEncoder.stepIncNormal;
			else
				SimulateEncoder.StepSim = 70;
			if (BendMachine.CoupleStep<50)
			{
				SimulateEncoder.StepSim = 0;
				SimulateEncoder.delaySim.IN = 0;
			}
			break;
		
		case 70:
			SimulateEncoder.delaySim.IN = 1;
			SimulateEncoder.delaySim.PT = 2 * 1000 / 4;
			if (SimulateEncoder.delaySim.Q)
			{
				SimulateEncoder.delaySim.IN = 0;
				SimulateEncoder.StepSim = 0;
			}
			if (BendMachine.CoupleStep<50)
			{
				SimulateEncoder.StepSim = 0;
				SimulateEncoder.delaySim.IN = 0;
			}
			break;
		
		default:

			break;
	}
     
	TON_10ms(&SimulateEncoder.delaySim);

}

void _CYCLIC_ProgramCyclic(void)
{
	BendMchineLogic();
}


