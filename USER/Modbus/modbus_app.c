
#include "bsp.h"
#include "modbus_host.h"
#include "HwCfg.h"

static USINT COMsTEP, GOHomeStep;

struct MDB_type MDB={0};

static BOOL recording=0;
static float MinMasurePos=1000.0;
static float velBackScale=1.0;
void _CYC_RW(void){

	
	uint8_t ret=0;
	
	switch(COMsTEP){
	
		case 0:
				if( MDB.ReadCmdCoils == FALSE && 
						MDB.ReadCmdRgister==FALSE &&
						MDB.WriteCmdCoils==FALSE &&
						MDB.WriteCmdRgister==FALSE  && 
						MDB.WriteCmdRgisterSect2==FALSE ){
							
					COMsTEP = COMsTEP + 1;
					MDB.ReadCmdCoils = TRUE;						
				}
			break;
				
			case 1:
				if( MDB.ReadCmdCoils == FALSE && 
						MDB.ReadCmdRgister==FALSE &&
						MDB.WriteCmdCoils==FALSE &&
						MDB.WriteCmdRgister==FALSE  && 
						MDB.WriteCmdRgisterSect2==FALSE ){
							
					COMsTEP = COMsTEP + 1;
					MDB.ReadCmdRgister = TRUE;						
				}
			break;
				
			case 2:
				if( MDB.ReadCmdCoils == FALSE && 
						MDB.ReadCmdRgister==FALSE &&
						MDB.WriteCmdCoils==FALSE &&
						MDB.WriteCmdRgister==FALSE  && 
						MDB.WriteCmdRgisterSect2==FALSE ){
							
					COMsTEP = COMsTEP + 1;
					MDB.WriteCmdCoils = TRUE;						
				}
			break;
				
			case 3:
				if( MDB.ReadCmdCoils == FALSE && 
						MDB.ReadCmdRgister==FALSE &&
						MDB.WriteCmdCoils==FALSE &&
						MDB.WriteCmdRgister==FALSE  && 
						MDB.WriteCmdRgisterSect2==FALSE ){
							
					COMsTEP = COMsTEP + 1;
					MDB.WriteCmdRgister = TRUE;						
				}
			break;
				
			case 4:
				if( MDB.ReadCmdCoils == FALSE && 
						MDB.ReadCmdRgister==FALSE &&
						MDB.WriteCmdCoils==FALSE &&
						MDB.WriteCmdRgister==FALSE  && 
						MDB.WriteCmdRgisterSect2==FALSE ){
							
					COMsTEP = 0;
					MDB.WriteCmdRgisterSect2 = TRUE;						
				}
			break;
	}
	
	
	if(MDB.ReadCmdCoils){
		
		ret = MODBUS_01H(0x01,0x00,(BOOL*)&MDB.MCGSCoils[0],39);
		
		if(ret==1){
			MDB.ReadCmdCoils = 0;
			MDB.ModbusCommOK = 1;
		}
		else if(ret == 0){
			MDB.ReadCmdCoils = 0;
			MDB.ModbusCommOK = 0;
		}
		
	}

	else if(MDB.ReadCmdRgister){
		
		ret = MODBUS_03H(0x01,0x00,(uint16_t*)&MDB.MCGSReg[0],74);
		
		if(ret==1){
			MDB.ReadCmdRgister = 0;
			MDB.ModbusCommOK = 1;
		}
		else if(ret == 0){
			MDB.ReadCmdRgister = 0;
			MDB.ModbusCommOK = 0;
		}
		
	}
	
	else if(MDB.WriteCmdCoils){
		
		ret = MODBUS_0FH(0x01,0x0A,(BOOL*)&MDB.MCGSCoilsWr[0],12);
		
		if(ret==1){
			MDB.WriteCmdCoils = 0;
			MDB.ModbusCommOK = 1;
		}
		else if(ret == 0){
			MDB.WriteCmdCoils = 0;
			MDB.ModbusCommOK = 0;
		}
		
	}

	else if(MDB.WriteCmdRgister){
		
		ret =  MODBUS_10H(0x01,0x10,(uint16_t *)&MDB.MCGSRegWr[0],6);
		
		if(ret==1){
			MDB.WriteCmdRgister = 0;
			MDB.ModbusCommOK = 1;
		}
		else if(ret == 0){
			MDB.WriteCmdRgister = 0;
			MDB.ModbusCommOK = 0;
		}
		
	}
	
	else if(MDB.WriteCmdRgisterAsyn){
		
		ret =  MODBUS_10H(0x01,0x16,(uint16_t *)&MDB.MCGSRegWr[6],2);
		
		if(ret==1){
			MDB.WriteCmdRgisterAsyn = 0;
			MDB.ModbusCommOK = 1;
		}
		else if(ret == 0){
			//MDB.WriteCmdRgisterAsyn = 0;
			MDB.ModbusCommOK = 0;
		}
		
	}
	
	else if(MDB.WriteCmdRgisterSect2){
		
		ret =  MODBUS_10H(0x01,0x4C,(uint16_t *)&MDB.MCGSRegWrSect2[2],2);
		
		if(ret==1){
			MDB.WriteCmdRgisterSect2 = 0;
			MDB.ModbusCommOK = 1;
		}
		else if(ret == 0){
			MDB.WriteCmdRgisterSect2 = 0;
			MDB.ModbusCommOK = 0;
		}
		
	}

}


void _CYC_MDB_TASK(void){


	
	_CYC_RW();
	
	if(MDB.ModbusCommOK == 0) return;
	
	BendMachine.McgsPara.MaualVelocity= swap2Comb((UINT *)&(MDB.MCGSReg[0]));
	BendMachine.McgsPara.BackVelocity= swap2Comb((UINT *)&(MDB.MCGSReg[2]));
	BendMachine.McgsPara.FollowVelocity= swap2Comb((UINT *)&(MDB.MCGSReg[4]));
	BendMachine.McgsPara.BackPosition= swap2Comb((UINT *)&(MDB.MCGSReg[6]));	
	BendMachine.McgsPara.DEC= swap2Comb((UINT *)&(MDB.MCGSReg[10]));
	BendMachine.McgsPara.StartPos= swap2Comb((UINT *)&(MDB.MCGSReg[12]));
	BendMachine.McgsPara.EndPos= swap2Comb((UINT *)&(MDB.MCGSReg[14]));
	BendMachine.McgsPara.ServalPos= swap2Comb((UINT *)&(MDB.MCGSReg[18]));
	BendMachine.McgsPara.RasterPos= swap2Comb((UINT *)&(MDB.MCGSReg[20]));
	BendMachine.McgsPara.ActStep= swap2Comb((UINT *)&(MDB.MCGSReg[22]));
	BendMachine.McgsPara.StepSum= swap2Comb((UINT *)&(MDB.MCGSReg[24]));
	BendMachine.McgsPara.RasterOffset= swap2Comb((UINT *)&(MDB.MCGSReg[28]));
	BendMachine.McgsPara.RepeatTime= swap2Comb((UINT *)&(MDB.MCGSReg[30]));
	BendMachine.McgsPara.PosLimitSw= swap2Comb((UINT *)&(MDB.MCGSReg[32]));
	BendMachine.McgsPara.NegLimitSw= swap2Comb((UINT *)&(MDB.MCGSReg[34]));
	BendMachine.McgsPara.BendAngle = swap2Comb((UINT *)&(MDB.MCGSReg[36]));
	BendMachine.McgsPara.MaualVelocity_B = swap2Comb((UINT *)&(MDB.MCGSReg[38]));
	BendMachine.McgsPara.BackVelocity_B= swap2Comb((UINT *)&(MDB.MCGSReg[40]));
	BendMachine.McgsPara.FollowVelocity_B= swap2Comb((UINT *)&(MDB.MCGSReg[42]));
	BendMachine.McgsPara.BackPosition_B= swap2Comb((UINT *)&(MDB.MCGSReg[44]));
	BendMachine.McgsPara.ACC_B= swap2Comb((UINT *)&(MDB.MCGSReg[46]));
	BendMachine.McgsPara.DEC_B= swap2Comb((UINT *)&(MDB.MCGSReg[48]));
	BendMachine.McgsPara.PosLimitSw_B= swap2Comb((UINT *)&(MDB.MCGSReg[50]));
	BendMachine.McgsPara.NegLimitSw_B= swap2Comb((UINT *)&(MDB.MCGSReg[52]));
	BendMachine.McgsPara.PlatThick = swap2Comb((UINT *)&(MDB.MCGSReg[54]));
	BendMachine.McgsPara.ACC= swap2Comb((UINT *)&(MDB.MCGSReg[56]));
	BendMachine.McgsPara.AutoBackPos= swap2Comb((UINT *)&(MDB.MCGSReg[58]));// change frome 27 to 59
	BendMachine.McgsPara.VDistMachine= swap2Comb((UINT *)&(MDB.MCGSReg[60]));// change frome 55 to 61
	BendMachine.McgsPara.VMoldD= swap2Comb((UINT *)&(MDB.MCGSReg[62])); // change frome 59 to 63
	BendMachine.McgsPara.MachineZeroOfset= swap2Comb((UINT *)&(MDB.MCGSReg[64]));
	BendMachine.McgsPara.HoldBendTime= swap2Comb((UINT *)&(MDB.MCGSReg[66]));	
	BendMachine.McgsPara.SpringH= swap2Comb((UINT *)&(MDB.MCGSReg[68]));	// change frome 61 to 69
	BendMachine.McgsPara.MoldH= swap2Comb((UINT *)&(MDB.MCGSReg[70]));// change frome 63 to 71	
	BendMachine.McgsPara.Bend2TimeOfset = swap2Comb((UINT *)&(MDB.MCGSReg[72]));// second bend
	BendMachine.McgsPara.SuckDist = swap2Comb((UINT *)&(MDB.MCGSReg[78]));
	
	
	BendMachine.McgsCmd.HomeServo= MDB.MCGSCoils[0];
	BendMachine.McgsCmd.RasetReset= MDB.MCGSCoils[1];
	BendMachine.McgsCmd.JogPos= MDB.MCGSCoils[2];
	BendMachine.McgsCmd.JogNeg= MDB.MCGSCoils[3];
	BendMachine.McgsCmd.GoHome= MDB.MCGSCoils[4];
	BendMachine.McgsCmd.Stepper= MDB.MCGSCoils[5];
	BendMachine.McgsCmd.Manual= MDB.MCGSCoils[6];
	BendMachine.McgsCmd.Reset= MDB.MCGSCoils[7];
	BendMachine.McgsCmd.JogPos_B= MDB.MCGSCoils[8];
	BendMachine.McgsCmd.JogNeg_B= MDB.MCGSCoils[9];
	
	BendMachine.McgsCmd.HomeServo_B= MDB.MCGSCoils[23];
	BendMachine.McgsCmd.Teach_Mode= MDB.MCGSCoils[25];
	BendMachine.McgsCmd.Teach_StartPos= MDB.MCGSCoils[26];
	BendMachine.McgsCmd.Teach_EndPos= MDB.MCGSCoils[27];
	BendMachine.McgsCmd.MachineType = MDB.MCGSCoils[28];
	BendMachine.McgsCmd.VacnumOper = MDB.MCGSCoils[33];
	BendMachine.McgsCmd.EnableSim = MDB.MCGSCoils[35];

	////////////////
	MDB.MCGSCoilsWr[0]= !gMchineEif.Estop;
	MDB.MCGSCoilsWr[1]= gMachineIO.DI[5];
	MDB.MCGSCoilsWr[2]= gMachineIO.DI[6];
	MDB.MCGSCoilsWr[3]= AxisRTs[AX_DRAW].info.status==0;
	MDB.MCGSCoilsWr[4]= AxisRTs[AX_DRAW].info.status>0;
	MDB.MCGSCoilsWr[5]= gMachineIO.flagLed[0];
	MDB.MCGSCoilsWr[6]= AxisRTs[AX_BEND].info.status==0;
	MDB.MCGSCoilsWr[7]= AxisRTs[AX_BEND].info.status>0;
	MDB.MCGSCoilsWr[8]= AxisRTs[AX_DRAW].IOMapping.positiveLimitSwitch;
	MDB.MCGSCoilsWr[9]= AxisRTs[AX_DRAW].IOMapping.negativeLimitSwitch;
	MDB.MCGSCoilsWr[10]= AxisRTs[AX_BEND].IOMapping.positiveLimitSwitch;
	MDB.MCGSCoilsWr[11]= AxisRTs[AX_BEND].IOMapping.negativeLimitSwitch;
	
	////////////////	
	
	REAL TESTREAL;
	DINT TESTDint;
	
	TESTREAL = (AxisRTs[AX_BEND].info.position);
	TESTDint = swap2Comb((UINT *)&(TESTREAL));
	memcpy(&(MDB.MCGSRegWr[0]),&(TESTDint),sizeof(TESTREAL));
	TESTREAL = (AxisRTs[AX_DRAW].info.position);
	TESTDint = swap2Comb((UINT *)&(TESTREAL));
	memcpy(&(MDB.MCGSRegWr[2]),&(TESTDint),sizeof(TESTREAL));
	TESTDint = swap2Comb((UINT *)&(BendMachine.MeasurePos));
	memcpy(&(MDB.MCGSRegWr[4]),&(TESTDint),sizeof(AxisRTs[0].info.position));
	TESTDint = swap2Comb((UINT *)&(BendMachine.McgsPara.oActStep));
	memcpy(&(MDB.MCGSRegWr[6]),&(TESTDint),sizeof(TESTDint));
	
	TESTDint = swap2Comb((UINT *)&(gSystemPara.TeachAngle));
	memcpy(&(MDB.MCGSRegWrSect2[0]),&(TESTDint),sizeof(AxisRTs[1].info.position));
	TESTDint = swap2Comb((UINT *)&(gSystemPara.EndPos_Teach));
	memcpy(&(MDB.MCGSRegWrSect2[2]),&(TESTDint),sizeof(AxisRTs[1].info.position));

	////////////////
	BendMachine.Mode = BendMachine.McgsCmd.Manual;
	///////////////
	memcpy(&(gSystemPara.SynVelocity),&(BendMachine.McgsPara.FollowVelocity),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.MaualVelocity),&(BendMachine.McgsPara.MaualVelocity),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.BackVel),&(BendMachine.McgsPara.BackVelocity),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.ACC),&(BendMachine.McgsPara.ACC),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.DEC),&(BendMachine.McgsPara.DEC),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.StartPos),&(BendMachine.McgsPara.StartPos),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.EndPos),&(BendMachine.McgsPara.EndPos),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.BackPosition),&(BendMachine.McgsPara.BackPosition),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.OffsetPos),&(BendMachine.McgsPara.RasterOffset),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.AutoBackPos),&(BendMachine.McgsPara.AutoBackPos),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.PosLimitSw),&(BendMachine.McgsPara.PosLimitSw),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.NegLimitSw),&(BendMachine.McgsPara.NegLimitSw),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.BendAngle),&(BendMachine.McgsPara.BendAngle),sizeof(gSystemPara.MaualVelocity));
	
	memcpy(&(gSystemPara.SynVelocity_B),&(BendMachine.McgsPara.FollowVelocity_B),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.BackVel_B),&(BendMachine.McgsPara.BackVelocity_B),sizeof(gSystemPara.MaualVelocity));	
	memcpy(&(gSystemPara.BackPosition_B),&(BendMachine.McgsPara.BackPosition_B),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.MaualVelocity_B),&(BendMachine.McgsPara.MaualVelocity_B),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.ACC_B),&(BendMachine.McgsPara.ACC_B),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.DEC_B),&(BendMachine.McgsPara.DEC_B),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.PosLimitSw_B),&(BendMachine.McgsPara.PosLimitSw_B),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.NegLimitSw_B),&(BendMachine.McgsPara.NegLimitSw_B),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.PlatThick),&(BendMachine.McgsPara.PlatThick),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.MachineZeroOfsetTemp),&(BendMachine.McgsPara.MachineZeroOfset),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.HoldBendTime),&(BendMachine.McgsPara.HoldBendTime),sizeof(gSystemPara.MaualVelocity));	
	memcpy(&(gSystemPara.VDistMachine),&(BendMachine.McgsPara.VDistMachine),sizeof(gSystemPara.MaualVelocity));		
	memcpy(&(gSystemPara.Vmolde2D),&(BendMachine.McgsPara.VMoldD),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.SuckDist),&(BendMachine.McgsPara.SuckDist),sizeof(gSystemPara.MaualVelocity));				///
	memcpy(&(gSystemPara.SpringH),&(BendMachine.McgsPara.SpringH),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.MoldH),&(BendMachine.McgsPara.MoldH),sizeof(gSystemPara.MaualVelocity));
	memcpy(&(gSystemPara.Bend2TimeOfset),&(BendMachine.McgsPara.Bend2TimeOfset),sizeof(gSystemPara.MaualVelocity));
	gSystemPara.EnableSim = BendMachine.McgsCmd.EnableSim;
	
	
	if( gSystemPara.Vmolde2D<=0 ){
		gSystemPara.Vmolde2D = 4;
	};
	gSystemPara.VmoldeD = gSystemPara.Vmolde2D / 2.0;
	if( gSystemPara.VmoldeD<=0 ){
		gSystemPara.VmoldeD = 3;
	};
	if( gSystemPara.PosLimitSw<=0 ){
		gSystemPara.PosLimitSw = 350;
	};
	
	if( gSystemPara.PosLimitSw_B<=0 ){
		gSystemPara.PosLimitSw_B = 100;
	};
	
	if( gSystemPara.SpringH<0 ){
		gSystemPara.SpringH = 0;
	};
	if( gSystemPara.MoldH<0 ){
		gSystemPara.MoldH = 0;
	};
	
	
	AxisRTs[AX_DRAW].Parameter.SWLimits.Upper  		= gSystemPara.PosLimitSw;
	AxisRTs[AX_DRAW].Parameter.SWLimits.Lower  		= gSystemPara.NegLimitSw;	
	AxisRTs[AX_BEND].Parameter.SWLimits.Upper  		= gSystemPara.PosLimitSw_B;
	AxisRTs[AX_BEND].Parameter.SWLimits.Lower  		= gSystemPara.NegLimitSw_B;
	
	
	if( gSystemPara.MaualVelocity==0 ){
		gSystemPara.MaualVelocity = 10;
	};
	
	if( gSystemPara.ACC==0 ){
		gSystemPara.ACC = 500;
	};
	
	if( gSystemPara.DEC==0 ){
		gSystemPara.DEC = 500;
	};
	
	if( gSystemPara.ACC_B==0 ){
		gSystemPara.ACC_B = 500;
	};
	
	if( gSystemPara.DEC_B==0 ){
		gSystemPara.DEC_B = 500;
	};	
	
	if( gSystemPara.BackVel_B <= 0 ){
		gSystemPara.BackVel_B = 10;
	};
	
	if( gSystemPara.BackVel <= 0 ){
		gSystemPara.BackVel = 10;
	};
	
	if( gSystemPara.MaualVelocity_B <= 0 ){
		gSystemPara.MaualVelocity_B = 10;
	};

	if( gSystemPara.MaualVelocity <= 0 ){
		gSystemPara.MaualVelocity = 10;
	};	
	//
	AxisRTs[AX_DRAW].Parameter.acceleration 		= gSystemPara.ACC;//10mm/s
	AxisRTs[AX_DRAW].Parameter.deceleration 		= gSystemPara.DEC;//10mm/s
	
	AxisRTs[AX_BEND].Parameter.acceleration 		= gSystemPara.ACC_B;//10mm/s
	AxisRTs[AX_BEND].Parameter.deceleration 		= gSystemPara.DEC_B;//10mm/s
//	AxisRTs[AX_VIRTRUAL].Parameter.deceleration 		= gSystemPara.DEC_B;//10mm/s
	
	
	if( BendMachine.McgsCmd.Reset && AxisRTs[AX_DRAW].info.status>0 ){
		AxisRTs[AX_DRAW].Acknowledge = TRUE;	
	};
	
	if( BendMachine.McgsCmd.Reset && AxisRTs[AX_BEND].info.status>0 ){
		AxisRTs[AX_BEND].Acknowledge = TRUE;
	};
	
	AxisRTs[AX_BEND].Acknowledge = BendMachine.McgsCmd.Reset;
	AxisRTs[AX_DRAW].Acknowledge = BendMachine.McgsCmd.Reset;
	AxisRTs[AX_VIRTRUAL].Acknowledge = BendMachine.McgsCmd.Reset;
	
	
	switch( BendMachine.Mode ){
		
		case 0://??
			
			BendMachine.MachineType = BendMachine.McgsCmd.MachineType;			
			
		//	HomeAxises[AX_DRAW].Home = BendMachine.McgsCmd.HomeServo;  //??????
		//	HomeAxises[AX_BEND].Home = BendMachine.McgsCmd.HomeServo_B;//??????				

			if(BendMachine.McgsCmd.HomeServo==1 && BendMachine.McgsCmd.oldHomeServo==0){		
				AxisRTs[AX_DRAW].Homing = 1;
				AxisRTs[AX_DRAW].Parameter.homing.position 			= 0;
				BendMachine.cmdHomeofset[AX_DRAW] = 1;
			}
			if(BendMachine.McgsCmd.HomeServo_B==1 && BendMachine.McgsCmd.oldHomeServo_B==0){			
				AxisRTs[AX_BEND].Homing = 1;
				AxisRTs[AX_VIRTRUAL].Homing = 1;
				AxisRTs[AX_BEND].Parameter.homing.position 			= 0;
				AxisRTs[AX_VIRTRUAL].Parameter.homing.position 	= 0;
				BendMachine.cmdHomeofset[AX_BEND] = 1;
			}
			if( BendMachine.McgsCmd.RasetReset==1 && BendMachine.McgsCmd.oldRasetReset==0 ){//????
				BendMachine.ResetRaster = TRUE;
			};
			if( BendMachine.McgsCmd.RasetReset==0 && BendMachine.McgsCmd.oldRasetReset==1 ){//????
				BendMachine.ResetRaster = FALSE;
			};	
			
			if( BendMachine.McgsCmd.Teach_StartPos==1 && BendMachine.McgsCmd.oldTeach_StartPos==0 ){//??????
				MinMasurePos = gSystemPara.OffsetPos;
				recording = 1;
			};	
			if(recording==1){
				MinMasurePos = MINReal(MinMasurePos,BendMachine.MeasurePos);
			}
			
			if( BendMachine.McgsCmd.Teach_StartPos==0 && BendMachine.McgsCmd.oldTeach_StartPos==1 ){//??????
				gSystemPara.EndPos_Teach = MinMasurePos;
				gSystemPara.TeachAngle = gSystemPara.BendAngle;
				gSystemPara.SaveParameter = 1;
				recording = 0;
				
			};	
			
			if( BendMachine.McgsCmd.Teach_EndPos==1 && BendMachine.McgsCmd.oldTeach_EndPos==0  ){//??????
				recording = 0;
				gSystemPara.EndPos_Teach = MinMasurePos;
			};
			
			if( BendMachine.McgsCmd.VacnumOper==1 && BendMachine.McgsCmd.oldVacnumOper==0 ){//????
				BendMachine.doVacnum = !BendMachine.doVacnum;
			};
			
			switch( GOHomeStep ){
				///////////////////////////////////////////////////////
				case 0:
					//???,??????1,
					if( !AxisRTs[AX_DRAW].info.moveActive  && 
						AxisRTs[AX_DRAW].info.homingOk && 
						AxisRTs[AX_DRAW].info.powerOn ){///??
						///////////////////////////////////////////////
						if( BendMachine.McgsCmd.JogPos==1 && BendMachine.McgsCmd.oldJogPos==0 ){
							AxisRTs[AX_DRAW].JogPos = TRUE;
						};	
						if( BendMachine.McgsCmd.JogNeg==1 && BendMachine.McgsCmd.oldJogNeg==0 ){
							AxisRTs[AX_DRAW].JogNeg = TRUE;
						};					
					};
					if( BendMachine.McgsCmd.JogPos==0 && BendMachine.McgsCmd.oldJogPos==1 ){
						AxisRTs[AX_DRAW].JogPos = FALSE;
					};
					if( BendMachine.McgsCmd.JogNeg==0 && BendMachine.McgsCmd.oldJogNeg==1 ){
						AxisRTs[AX_DRAW].JogNeg = FALSE;
					};
					///////////////////////////////////////////////////////
					//???,??????2,
					if( !AxisRTs[AX_BEND].info.moveActive  && 
						AxisRTs[AX_BEND].info.homingOk && 
						AxisRTs[AX_BEND].info.powerOn ){///??
						if( BendMachine.McgsCmd.JogPos_B==1 && BendMachine.McgsCmd.oldJogPos_B==0  ){
							AxisRTs[AX_BEND].JogPos = TRUE;
							AxisRTs[AX_VIRTRUAL].JogPos = TRUE;
						};	
						if( BendMachine.McgsCmd.JogNeg_B==1 && BendMachine.McgsCmd.oldJogNeg_B==0 ){
							AxisRTs[AX_BEND].JogNeg = TRUE;
							AxisRTs[AX_VIRTRUAL].JogNeg = TRUE;
						};	
					};
					if( BendMachine.McgsCmd.JogPos_B==0 && BendMachine.McgsCmd.oldJogPos_B==1 ){
						AxisRTs[AX_BEND].JogPos = FALSE;
						AxisRTs[AX_VIRTRUAL].JogPos = FALSE;
					};
					if( BendMachine.McgsCmd.JogNeg_B==0 && BendMachine.McgsCmd.oldJogNeg_B==1 ){
						AxisRTs[AX_BEND].JogNeg = FALSE;
						AxisRTs[AX_VIRTRUAL].JogNeg = FALSE;
					};
					if(AxisRTs[AX_DRAW].IOMapping.positiveLimitSwitch==1) AxisRTs[AX_DRAW].JogPos = FALSE;
					if(AxisRTs[AX_DRAW].IOMapping.negativeLimitSwitch==1) AxisRTs[AX_DRAW].JogNeg = FALSE;
					if(AxisRTs[AX_BEND].IOMapping.positiveLimitSwitch==1) AxisRTs[AX_BEND].JogPos = FALSE;
					if(AxisRTs[AX_BEND].IOMapping.negativeLimitSwitch==1) AxisRTs[AX_BEND].JogNeg = FALSE;
					///////////////////////////////////////////////////////
					if( BendMachine.McgsCmd.GoHome==1 && 
						BendMachine.McgsCmd.oldGoHome==0 &&
						!AxisRTs[AX_DRAW].info.moveActive  && 
						AxisRTs[AX_DRAW].info.homingOk && 
						AxisRTs[AX_DRAW].info.powerOn &&
						!AxisRTs[AX_BEND].info.moveActive  && 
						AxisRTs[AX_BEND].info.homingOk && 
						AxisRTs[AX_BEND].info.powerOn &&
						!gMchineEif.Estop){ //???
						GOHomeStep = 10;	
					};
					break;
				///////////////////////////////////////////////////////	
				case 10:
					AxisRTs[AX_BEND].MoveAbsolute = TRUE;
					AxisRTs[AX_BEND].Parameter.position = gSystemPara.BackPosition_B;
					AxisRTs[AX_BEND].Parameter.speed = gSystemPara.BackVel_B;//10mm/s
				
					
					AxisRTs[AX_VIRTRUAL].MoveAbsolute = TRUE;
					AxisRTs[AX_VIRTRUAL].Parameter.position = gSystemPara.BackPosition_B;
					AxisRTs[AX_VIRTRUAL].Parameter.speed = gSystemPara.BackVel_B;//10mm/s
				
										///////////////////////////////////////////////////////					
					AxisRTs[AX_DRAW].MoveAbsolute = TRUE;
					AxisRTs[AX_DRAW].Parameter.position = gSystemPara.BackPosition;
				
					if(AxisRTs[AX_BEND].info.position - AxisRTs[AX_BEND].Parameter.position > 1){
						velBackScale = (AxisRTs[AX_DRAW].info.position - AxisRTs[AX_DRAW].Parameter.position) / (AxisRTs[AX_BEND].info.position - AxisRTs[AX_BEND].Parameter.position);
						if(velBackScale>4)velBackScale=4;
					}
					else
						velBackScale = 1;
					

					AxisRTs[AX_DRAW].Parameter.speed = gSystemPara.BackVel_B * velBackScale;//10mm/s
					///////////////////////////////////////////////////////
					BendMachine.McgsCmd.GoHome 	= FALSE;
					GOHomeStep 					= 20;
					break;
				///////////////////////////////////////////////////////
				case 20:
					if(	!AxisRTs[AX_DRAW].info.moveActive &&	
						!AxisRTs[AX_BEND].info.moveActive ){ //???
						GOHomeStep = 0;	
					};
					break;
			///////////////////////////////////////////////////////					
			};
			
			AxisRTs[AX_BEND].Parameter.JogSpeed = gSystemPara.MaualVelocity_B;
			AxisRTs[AX_VIRTRUAL].Parameter.JogSpeed = gSystemPara.MaualVelocity_B;
			AxisRTs[AX_DRAW].Parameter.JogSpeed = gSystemPara.MaualVelocity;
		//????,????
			BendMachine.CoupleEnable = FALSE;
		//myAxCtrl[AX_BEND].MoveCyc = 0;
		//myAxCtrl[AX_DRAW].MoveCyc = 0;
			AxisRTs[AX_BEND].MoveCyclicPosition = 0;
			AxisRTs[AX_DRAW].MoveCyclicPosition = 0;
			AxisRTs[AX_VIRTRUAL].MoveCyclicPosition = 0;
			break;
			
		case 1://??
			AxisRTs[AX_DRAW].MoveAbsolute = FALSE;
			AxisRTs[AX_BEND].MoveAbsolute = FALSE;
			AxisRTs[AX_VIRTRUAL].MoveAbsolute = FALSE;
			BendMachine.ResetRaster = FALSE;
			AxisRTs[AX_DRAW].JogPos = FALSE;
			AxisRTs[AX_DRAW].JogNeg = FALSE;		
			AxisRTs[AX_BEND].JogPos = FALSE;
			AxisRTs[AX_BEND].JogNeg = FALSE;
			AxisRTs[AX_VIRTRUAL].JogPos = FALSE;
			AxisRTs[AX_VIRTRUAL].JogNeg = FALSE;
			///////////////////////////////////////////
			BendMachine.HomeBend = FALSE;
			BendMachine.HomeBend_B = FALSE;			
			///////////////////////////////////////////
			/*if( (gSystemPara.StartPos - gSystemPara.OffsetPos) >75 && BendMachine.MachineType==1 )
				gSystemPara.offsetBasic = gSystemPara.StartPos - 75;
			else*/
				gSystemPara.offsetBasic = 0;
			;
			
			/*if( (gSystemPara.StartPos_Teach - gSystemPara.OffsetPos) >75 && BendMachine.MachineType==1 )
				gSystemPara.offsetBasic_Teach = gSystemPara.StartPos_Teach - 75;
			else*/
				gSystemPara.offsetBasic_Teach = 0;
			;		
					
			//if( BendMachine.McgsCmd.Teach_Mode==0 ){
				gSystemPara.BendX1 = gSystemPara.StartPos - gSystemPara.offsetBasic;
				gSystemPara.BendH = gSystemPara.BendX1 - gSystemPara.EndPos;
			//}	
			//else{
			//	gSystemPara.BendX1 = gSystemPara.StartPos_Teach-gSystemPara.offsetBasic_Teach;
			//	gSystemPara.BendH = gSystemPara.BendX1 - gSystemPara.EndPos_Teach;				
			//};
			
			if( gSystemPara.BendAngle<30 || gSystemPara.BendAngle>180 ){
				gSystemPara.BendAngle = 90;
			};
			
			if( gSystemPara.SpringH>0 ){
				BendMachine.CompensateNeeded = 1;
			}
			else{
				BendMachine.CompensateNeeded = 0;
			};
			/*
			if( gSystemPara.BendH>0 ){
				gSystemPara.TargetExchScale = gSystemPara.Distance90Degree * gSystemPara.BendAngle / 90.0 / gSystemPara.BendH;			
			};*/
			
			REAL tempS,tempK;
			
			tempS = gSystemPara.VmoldeD * tan((90-gSystemPara.BendAngle/2.0)/180.0 * PI);
			//tempS = gSystemPara.VmoldeD * TAN(gSystemPara.BendAngle/180.0 * PI);
			/////////////
			if( tempS>0 ){
				tempK = gSystemPara.BendH / tempS;}
			else{
				tempK = 1;				
			};
			if( tempK > 2 || tempK < 0.5 ){
	 		//	tempK = 1;
				;
			};
			////////////
			gSystemPara.AngleB = 90 - gSystemPara.BendAngle / 2;
			gSystemPara.AngleScaleK = tempK;
			gSystemPara.TargetExchScale = (gSystemPara.VDistMachine/tempK) / gSystemPara.VmoldeD;
			BendMachine.CoupleEnable = TRUE;
			break;

	};
			
	BendMachine.McgsCmd.oldJogPos = BendMachine.McgsCmd.JogPos;	
	BendMachine.McgsCmd.oldJogNeg = BendMachine.McgsCmd.JogNeg;	
	BendMachine.McgsCmd.oldJogPos_B = BendMachine.McgsCmd.JogPos_B;	
	BendMachine.McgsCmd.oldJogNeg_B = BendMachine.McgsCmd.JogNeg_B;
	BendMachine.McgsCmd.oldGoHome = BendMachine.McgsCmd.GoHome;	
	BendMachine.McgsCmd.oldRasetReset = BendMachine.McgsCmd.RasetReset;	
	BendMachine.McgsCmd.oldTeach_StartPos = BendMachine.McgsCmd.Teach_StartPos;	
	BendMachine.McgsCmd.oldTeach_EndPos = BendMachine.McgsCmd.Teach_EndPos;	
	BendMachine.McgsCmd.oldVacnumOper = BendMachine.McgsCmd.VacnumOper;		
	BendMachine.McgsCmd.oldHomeServo = BendMachine.McgsCmd.HomeServo;	
	BendMachine.McgsCmd.oldHomeServo_B = BendMachine.McgsCmd.HomeServo_B;	


};

