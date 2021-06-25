#include "mInclude.h" // include


void StepAxisObj(struct StepAxisObj* inst)
{
	DINT indexaxis=0;
	STEPAxisTyp * paxis;
	
	for(indexaxis=0;indexaxis<inst->AxisNumber;indexaxis++)
	{
		/////////////////////////////////////////////////////////
		paxis = (STEPAxisTyp *)(inst->Address + indexaxis * sizeof(STEPAxisTyp));
		/////////////////////////////////////////////////////////
	/*	if(paxis->SdcInter.EncIf1.iEncOK)
		{
			paxis->SdcInter.EncIf1.iLifeCnt++;
			paxis->SdcInter.EncIf1.iActTime = paxis->SdcInter.DrvIf.oSetTime;	
		}
		/////////////////////////////////////////////////////////
		if(paxis->SdcInter.DrvIf.iDrvOK)
			paxis->SdcInter.DrvIf.iLifeCnt++;
		/////////////////////////////////////////////////////////

		paxis->SdcInter.TrigIf1.iLifeCnt++;
		/////////////////////////////////////////////////////////
		paxis->SdcInter.DiDoIf.iLifeCntDriveReady++;	
		paxis->SdcInter.DiDoIf.iLifeCntNegHwEnd++;
		paxis->SdcInter.DiDoIf.iLifeCntPosHwEnd++;
		paxis->SdcInter.DiDoIf.iLifeCntReference++;
		*/
		/////////////////////////////////////////////////////////
		paxis->SdcInter.EncIf1.iActPos = paxis->objects402.positionControlFunc.PositionActualValueInc;
		if(paxis->SdcInter.DiDoIf.iDriveReady)
		{			
			///////////////////////////////////////////////////////////
			if(paxis->SdcInter.DiDoIf.oDriveEnable)
			{
				if(paxis->objects402.deviceControl.Modes_of_operation == MODE_INTERPOLATIONPOSITION)
					paxis->objects402.profilePositionMode.Target_position = paxis->SdcInter.DrvIf.oSetPos /*+ paxis->objects402.profilePositionMode.Target_position_ofst*/;				
					paxis->SdcInter.DiDoIf.iLifeCntDriveEnable++;
			}
			else
			{
				paxis->objects402.profilePositionMode.Target_position_ofst = paxis->SdcInter.EncIf1.iActPos - paxis->SdcInter.DrvIf.oSetPos;
			}
		}
	}
	/////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////
	
	
	for(indexaxis=0;indexaxis<inst->AxisNumber;indexaxis++)
	{
		/////////////////////////////////////////////////////////
		paxis = (STEPAxisTyp *)(inst->Address + indexaxis * sizeof(STEPAxisTyp));
		/////////////////////////////////////////////////////////
		UINT ControlWord = paxis->objects402.deviceControl.ControlWord;
		UINT StatusWord = paxis->objects402.deviceControl.StatusWord;
	
		UINT tempStatus = StatusWord & STATUSWORD_STATE_MASK;
		UINT tempStatusNobit5 = StatusWord & STATUSWORD_STATE_MASK_No6;
		INT remote = (StatusWord & STATUSWORD_REMOTE) == STATUSWORD_REMOTE;
		
		paxis->internal.tempStatusNobit5 = tempStatusNobit5;
		paxis->internal.tempStatus = tempStatus;
		
#ifdef NO_CHECK_REMOTE
		remote = 1;
#endif
		paxis->internal.State_temp++;
	//	if(!remote) continue;	
		if(paxis->internal.OldState != tempStatus)
		{
			paxis->internal.OldState = tempStatus;
			paxis->internal.State = 0;
			paxis->stepError.CountWait = 0;
		}	
		if(!paxis->objects402.deviceControl.ModuleOK){	
			paxis->objects402.deviceControl.bootup = 0;
			paxis->internal.Brake = 0;
		}
		if(tempStatusNobit5 == STATUSWORD_STATE_NOTREADYTOSWITCHON)
			paxis->SdcInter.DiDoIf.iDriveReady = 0;
			
		else if((tempStatusNobit5 == STATUSWORD_STATE_SWITCHEDONDISABLED) || (paxis->objects402.deviceControl.bootup == 0))
		{
			paxis->SdcInter.DiDoIf.iDriveReady = 0;
			paxis->SdcInter.DrvIf.iDrvOK = 1;
			paxis->objects402.commonEntries.ErrorCode = 0;
			paxis->stepError.ErrorID = 0;
			paxis->internal.Brake = 0;
			//?????,???????
			if(paxis->internal.State == 0)
			{
				paxis->internal.State = 1;
			}
			else if(paxis->internal.State == 1)
			{// Motor Rated value
				paxis->internal.State = 9;
			}
			else if(paxis->internal.State == 9)
			{//
				paxis->internal.State++;
			}
			else if(paxis->internal.State == 10)
			{//
				paxis->internal.State++;
			}
			else if(paxis->internal.State == 11)
			{
				paxis->internal.State++;
			}
			else if(paxis->internal.State == 12)
			{
				//?????us
				paxis->cycleTime_us = 1000;//??1ms
				ControlWord = CONTROLWORD_COMMAND_SHUTDOWN;
				paxis->internal.State++;
			}
			else if( paxis->internal.State == 13){
			
				paxis->objects402.deviceControl.bootup = 1;
			}
			paxis->internal.StateMachine = STATUSWORD_STATE_SWITCHEDONDISABLED;
		}
		else if(tempStatus == STATUSWORD_STATE_READYTOSWITCHON)
		{
			if(paxis->SdcInter.DiDoIf.iDriveReady == 0)
				paxis->SdcInter.DiDoIf.iDriveReady = 1;

			else if(paxis->SdcInter.DiDoIf.oDriveEnable)
				ControlWord = CONTROLWORD_COMMAND_SWITCHON;
			paxis->internal.Brake = 0;
			paxis->internal.StateMachine = STATUSWORD_STATE_READYTOSWITCHON;
		}
		else if(tempStatus == STATUSWORD_STATE_SWITCHEDON)
		{		
			ControlWord = CONTROLWORD_COMMAND_SWITCHON_ENABLEOPERATION;
			paxis->internal.oldSetPos = paxis->SdcInter.DrvIf.oSetPos;;
			paxis->internal.Brake = 0;
		}
		else if(tempStatus == STATUSWORD_STATE_OPERATIONENABLED)
		{
			////////////////////////////////////////////////////////////
			if(paxis->SdcInter.DrvIf.oSetPos!=paxis->internal.oldSetPos)
			{
				paxis->internal.oldSetPos = paxis->SdcInter.DrvIf.oSetPos;
				ControlWord = CONTROLWORD_COMMAND_IPPOS;
			}
			////////////////////////////////////////////////////////////
			if(!paxis->SdcInter.DiDoIf.oDriveEnable)
			{
				ControlWord = CONTROLWORD_COMMAND_SHUTDOWN;
				paxis->SdcInter.DiDoIf.iDriveReady = 0;
			}
			paxis->internal.StateMachine = STATUSWORD_STATE_OPERATIONENABLED;
		}
		else if(tempStatus == STATUSWORD_STATE_QUICKSTOPACTIVE)
		{
			//strcpy(strMessage,"STATUSWORD_STATE_QUICKSTOPACTIVE");
		}
		else if(tempStatusNobit5 == STATUSWORD_STATE_FAULTREACTIONACTIVE)
		{//????//??F
			//pDrvIfArr[axisIndex].iDrvOK = 0;
		}
		else if(tempStatusNobit5  == STATUSWORD_STATE_FAULT)
		{
			paxis->SdcInter.DrvIf.iDrvOK = 0;
			paxis->SdcInter.DiDoIf.iDriveReady = 0;
			paxis->internal.Brake = 0;
			if(paxis->stepError.CountWait > 5)
			{
				if(paxis->stepError.ErrorAck /*&& paxis->internal.AcpErrorBuf != 0*/)
				{
					ControlWord = CONTROLWORD_COMMAND_FAULTRESET;//?????????????
					paxis->stepError.ErrorID = 0;
					paxis->stepError.ErrorAck = 0;
					paxis->stepError.CountWait = 0;
					paxis->SdcInter.EncIf1.iEncOK	= 1;
					paxis->SdcInter.DrvIf.iDrvOK = 1;
				}
				else
				{//fault?????,???
					ControlWord = CONTROLWORD_COMMAND_DISABLEVOLTAGE;
				}
			}
			else 
			{
				paxis->stepError.CountWait++;
			}			
			paxis->internal.StateMachine = STATUSWORD_STATE_FAULT;
		}
		paxis->objects402.deviceControl.ControlWord = ControlWord;
	}//for ends
}
void StepInit(struct StepInit* inst)
{
	DINT indexaxis=0;
	STEPAxisTyp * paxis;
	plcstring indexstr[2],name[80];
	
	for(indexaxis=0;indexaxis<inst->AxisNumber;indexaxis++)
	{
		/////////////////////////////////////////////////////////
		paxis = (STEPAxisTyp *)(inst->Address + indexaxis * sizeof(STEPAxisTyp));
		/////////////////////////////////////////////////////////EncIf1
		paxis->objects402.deviceControl.Modes_of_operation = MODE_INTERPOLATIONPOSITION;	
		paxis->SdcHwPv.EncIf1_Typ = ncSDC_ENC32;
		/////////////////////////////////////////////////////////DrvIf
		paxis->SdcHwPv.DrvIf_Typ 	= ncSDC_DRVSM32;
		/////////////////////////////////////////////////////////TrigIf1
		paxis->SdcHwPv.TrigIf1_Typ 	= ncSDC_TRIG;
		/////////////////////////////////////////////////////////DiDoIf
		paxis->SdcHwPv.DiDoIf_Typ 	= ncSDC_DIDO;
		/////////////////////////////////////////////////////////ready signal
		paxis->SdcInter.EncIf1.iEncOK   = 1;
		paxis->SdcInter.DrvIf.iDrvOK	= 1;	
		/////////////////////////////////////////////////////////
	}	
}