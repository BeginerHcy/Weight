
#include <plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
#include "HC2500B.h"
#include "motorky.h"
#include <math.h>
#ifdef __cplusplus
	};
#endif

void nAxiseRT( nAxiseRT_typ * inst)
{
	/////
	USINT HOMEENDSWITCHMODE,HOMEREFSWITCHMODE;
	/////Obain the config or the parameter from the Object
	memcpy(&inst->nLink->init,&inst->SystemCfg,sizeof(inst->nLink->init));
	memcpy(&inst->info.errorText,&inst->nLink->info.errorText,sizeof(inst->nLink->info.errorText));
	inst->info.homingOk = inst->nLink->info.homingOk;
	inst->info.initialized = inst->nLink->info.initialized ;
	inst->info.lagError = inst->nLink->info.lagError ;
	inst->info.moveActive = inst->nLink->info.moveActive ;
	inst->info.position = inst->nLink->info.position / inst->Unifactor ;	
	inst->info.powerOn = inst->nLink->info.powerOn ;
	inst->info.speed = inst->nLink->info.speed / inst->Unifactor;	
	inst->info.status = inst->nLink->info.status ;
	///////////////
	if (inst->Parameter.SWLimits.Ignore)
	{
		inst->SWLimitNeg = 0;
		inst->SWLimitPos = 0;
	}
	else if (inst->Parameter.SWLimits.Upper < inst->info.position)
	{
		inst->SWLimitPos = 1;
		inst->SWLimitNeg = 0;
	}
	else if (inst->Parameter.SWLimits.Lower > inst->info.position)
	{
		inst->SWLimitNeg = 1;
		inst->SWLimitPos = 0;
	}
	else
	{
		inst->SWLimitNeg = 0;
		inst->SWLimitPos = 0;
	}
	///////////////
	if (inst->internalStatus>0)
	{
		inst->info.status = inst->internalStatus;
	}
	if (!inst->info.powerOn)
		inst->nLink->info.homingOk = 0;
	//
	if (inst->StatusID!=7000)
	{
		inst->nLink->param.acceleration = inst->Parameter.acceleration * inst->Unifactor;
		inst->nLink->param.deceleration = inst->Parameter.deceleration * inst->Unifactor;
	}
	//
	inst->nLink->param.homing.mode  = HOMING_DIRECT;
	//inst->nLink->param.homing.mode = inst->Parameter.homing.mode;
	inst->nLink->param.homing.edgeSwitch 		= inst->Parameter.homing.edgeSwitch;
	inst->nLink->param.homing.speed2 			= inst->Parameter.homing.speed2 * inst->Unifactor;
	inst->nLink->param.homing.startDirection 	= inst->Parameter.homing.startDirection;
	inst->nLink->param.homing.triggerDirection 	= inst->Parameter.homing.triggerDirection;
	///
	inst->nLink->cmd.power = inst->Power;
	///IOMAPING///////
	/////////////////
	if (inst->oldSpeed>fabs(inst->info.speed))
		inst->info.SpeedState = Decing;
	else if (inst->oldSpeed<fabs(inst->info.speed))
		inst->info.SpeedState = Accing;
	else
		inst->info.SpeedState = AccNull;
	/////////////////
	inst->oldSpeed = fabs(inst->info.speed);
	/////////////////
	//////INPUT/////
	inst->nLink->IOMapping.OvercurrentError 		= inst->IOMapping.OvertemperatureError;
	inst->nLink->IOMapping.OvertemperatureError 	= inst->IOMapping.OvertemperatureError;
	inst->nLink->IOMapping.ModulePowerSupplyError 	= inst->IOMapping.ModulePowerSupplyError;
	inst->nLink->IOMapping.UZKStatus 				= inst->IOMapping.UZKStatus;
//	inst->nLink->IOMapping.Enable 					= inst->IOMapping.Enable;
	inst->nLink->IOMapping.DriveReady				= inst->IOMapping.iReadyExt;
	
	///OUTPUT///////
	inst->IOMapping.MotorStep0						= ABS(inst->nLink->IOMapping.MotorStep0);
	inst->IOMapping.DriveEnable						= inst->nLink->IOMapping.DriveEnable;
	inst->IOMapping.BoostCurrent					= inst->nLink->IOMapping.BoostCurrent;
	inst->IOMapping.StandstillCurrent				= inst->nLink->IOMapping.StandstillCurrent;
	inst->IOMapping.ClearError						= inst->nLink->IOMapping.ClearError;
	///
	HOMEENDSWITCHMODE = inst->Parameter.homing.edgeSwitch*2 + inst->Parameter.homing.triggerDirection*1;
	HOMEREFSWITCHMODE = inst->Parameter.homing.startDirection*4 + inst->Parameter.homing.edgeSwitch*2 + inst->Parameter.homing.triggerDirection*1;
	///
	if (inst->Stop)
	{
		inst->nLink->cmd.stop = 1;
		inst->MoveCyclicPosition = 0;
		inst->nLink->cmd.velocity = 0;
		inst->nLink->cmd.additive = 0;
		inst->nLink->cmd.cyclicPosition = 0;
		inst->nLink->cmd.homing = 0;
		inst->nLink->cmd.absolute = 0;
		inst->StatusID	= 0;
		inst->Homing = 0;
		inst->MoveAbsolute = 0;
		inst->MoveAdditive = 0;
		inst->MoveVelocity = 0;
		inst->JogNeg = 0;
		inst->JogPos = 0;
		inst->StatusJOG = 0;
	}
	//当轴已经停下来的时候，停止发命令
	if (!inst->info.moveActive && inst->info.speed==0 && !inst->Stop)
		inst->nLink->cmd.stop = 0;

	if (inst->Stop && !inst->info.moveActive)
		inst->Stop = 0;
	
	////
	if (inst->Acknowledge && (inst->nLink->info.status>0 || inst->internalStatus>0))
	{
		inst->nLink->cmd.acknowledge = 1;
		inst->internalStatus = 0;
	}
	else
	{
		inst->nLink->cmd.acknowledge = 0;
		inst->Acknowledge = 0;
	}
	
	if (inst->info.status>0)//如果伺服发生故障,那么
	{
		inst->nLink->cmd.velocity = 0;
		inst->JogNeg = 0;
		inst->JogPos = 0;
		inst->nLink->cmd.additive = 0;
		inst->nLink->cmd.homing = 0;
		inst->nLink->cmd.absolute = 0;
		inst->nLink->cmd.cyclicPosition = 0;
		inst->StatusID	= 0;
		inst->Homing = 0;
		inst->MoveAbsolute = 0;
		inst->MoveAdditive = 0;
		inst->MoveVelocity = 0;
		inst->MoveCyclicPosition = 0;
		inst->StatusJOG = 0;
	}

	inst->IOMapping.DoDriveBrake = inst->StatusID==0 && !inst->MoveVelocity && !inst->JogPos && !inst->JogNeg;
	
	switch (inst->StatusID)
	{
		case 0:			
			//
			if (!inst->info.initialized)
				break;
			//////
			if (inst->Homing)
				inst->StatusID = 1000;
			else if (inst->MoveAbsolute && !inst->oldcmdMoveAbs)
				inst->StatusID = 5000;
			else if (inst->MoveAdditive && !inst->oldcmdMoveRef)
				inst->StatusID = 6000;
			else if (inst->MoveCyclicPosition)
				inst->StatusID = 7000;
			else if (inst->MoveVelocity)
			{
				if (((inst->Parameter.speed>0&&((inst->IOMapping.positiveLimitSwitch&&!inst->HWIgnore)||inst->SWLimitPos))||
					(inst->Parameter.speed<0&&((inst->IOMapping.negativeLimitSwitch&&!inst->HWIgnore)||inst->SWLimitNeg)))&&
				inst->Homing == 0){
					inst->nLink->cmd.velocity=0;
					inst->MoveVelocity		 =0;
				}
				else{
					inst->nLink->cmd.velocity=1;
					inst->nLink->param.speed = inst->Parameter.speed * inst->Unifactor;
					if (inst->Parameter.speed>0)
						inst->IOMapping.DoDirection = inst->MountDirection;
					else
						inst->IOMapping.DoDirection = !inst->MountDirection;
				}
			}
			else if (inst->JogPos)
			{
				if (((inst->Parameter.JogSpeed>0&&((inst->IOMapping.positiveLimitSwitch&&!inst->HWIgnore)||inst->SWLimitPos))
				)&&inst->Homing == 0){
					inst->nLink->cmd.velocity=0;
					inst->MoveVelocity		 =0;
				}
				else{
					if (inst->Parameter.SWLimits.Ignore)
					{
						inst->nLink->cmd.velocity=(inst->IOMapping.DoDirection == inst->MountDirection);
						inst->nLink->param.speed = inst->Parameter.JogSpeed * inst->Unifactor;
						inst->IOMapping.DoDirection = inst->MountDirection;					
					}
					else
					{
						switch (inst->StatusJOG)
						{
							case 0:
								if (inst->info.position<inst->Parameter.SWLimits.Upper){
									inst->StatusJOG			= 100;
									inst->Parameter.position = inst->Parameter.SWLimits.Upper;
								}
								else if (inst->info.position>=inst->Parameter.SWLimits.Upper)
									inst->StatusJOG			= 900;
								break;
			
							case 100://		
								inst->nLink->cmd.absolute	= !((inst->IOMapping.positiveLimitSwitch&&!inst->HWIgnore)||inst->SWLimitPos);
								inst->IOMapping.DoDirection = inst->MountDirection;
								inst->nLink->param.position = inst->Parameter.position * inst->Unifactor;
								//定义D=0时是正向;MountDirection=0表示正向
								inst->nLink->cmd.stop = (inst->IOMapping.positiveLimitSwitch&&!inst->HWIgnore)||inst->SWLimitPos;
								inst->nLink->param.speed = inst->Parameter.JogSpeed * inst->Unifactor;
								if ((inst->IOMapping.positiveLimitSwitch&&!inst->HWIgnore)||inst->SWLimitPos||!inst->JogPos)
									inst->StatusJOG 			= 900;
								else
									inst->StatusJOG 			= 110;
								break;
			
							case 110://判断指令已经执行
								inst->statuDelay.IN=1;
								inst->statuDelay.PT=0.1f / inst->SystemCfg.taskTime;//wait commd
								if (inst->info.moveActive || inst->statuDelay.Q)
								{
									inst->StatusJOG 			= 111;
								}
								break;
			
							case 111://判断已经完成，或者切换其他指令	
								inst->statuDelay.IN = 0;
								/////
								inst->nLink->cmd.stop = (inst->IOMapping.positiveLimitSwitch&&!inst->HWIgnore)||inst->SWLimitPos||!inst->JogPos;
								if ((inst->IOMapping.positiveLimitSwitch&&!inst->HWIgnore)||inst->SWLimitPos)
									inst->StatusJOG 			= 900;
								else if (!inst->info.moveActive && inst->info.speed==0)
									inst->StatusJOG 				= 900;//Finish
								else if(!inst->JogPos){
									inst->StatusJOG 			= 900;
								}
									
								break;
			
							case 900:
								inst->JogPos = 0;
								inst->StatusJOG = 0;
								inst->nLink->cmd.absolute = 0;
								break;
	
							default:
								break;
						}
     				}
				}
			}
			else if (inst->JogNeg)
			{
				if (((inst->Parameter.JogSpeed>0&&((inst->IOMapping.negativeLimitSwitch&&!inst->HWIgnore)||inst->SWLimitNeg)))&&
				inst->Homing == 0){
					inst->nLink->cmd.velocity=0;
					inst->MoveVelocity		 =0;
				}
				else{
					if (inst->Parameter.SWLimits.Ignore)
					{
						inst->nLink->cmd.velocity=(inst->IOMapping.DoDirection == !inst->MountDirection);
						inst->nLink->param.speed = -inst->Parameter.JogSpeed * inst->Unifactor;
						inst->IOMapping.DoDirection = !inst->MountDirection;
					}
					else
					{
						switch (inst->StatusJOG)
						{
							case 0:
								if (inst->info.position>inst->Parameter.SWLimits.Lower){
									inst->StatusJOG			= 100;
									inst->Parameter.position = inst->Parameter.SWLimits.Lower;
								}
								else if (inst->info.position<=inst->Parameter.SWLimits.Lower)
									inst->StatusJOG			= 900;
								break;
							
							case 100://		
								inst->nLink->cmd.absolute	= !((inst->IOMapping.negativeLimitSwitch&&!inst->HWIgnore)||inst->SWLimitNeg);
								inst->IOMapping.DoDirection = !inst->MountDirection;
								inst->nLink->param.position = inst->Parameter.position * inst->Unifactor;
								//定义D=0时是正向;MountDirection=0表示正向
								inst->nLink->cmd.stop = (inst->IOMapping.negativeLimitSwitch&&!inst->HWIgnore)||inst->SWLimitNeg;
								inst->nLink->param.speed = inst->Parameter.JogSpeed * inst->Unifactor;
								if ((inst->IOMapping.negativeLimitSwitch&&!inst->HWIgnore)||inst->SWLimitNeg||!inst->JogNeg)
									inst->StatusJOG 			= 900;
								else
									inst->StatusJOG 			= 110;
								break;
							
							case 110://判断指令已经执行
								inst->statuDelay.IN=1;
								inst->statuDelay.PT= 0.1f / inst->SystemCfg.taskTime ;//wait commd 0.1s , 0.1s/taskTime(unit sec) =  times
								if (inst->info.moveActive || inst->statuDelay.Q)
								{
									inst->StatusJOG 			= 111;
								}
								break;
							
							case 111://判断已经完成，或者切换其他指令	
								inst->statuDelay.IN = 0;
								/////
								inst->nLink->cmd.stop = (inst->IOMapping.negativeLimitSwitch&&!inst->HWIgnore)||inst->SWLimitNeg||!inst->JogNeg;
								if ((inst->IOMapping.negativeLimitSwitch&&!inst->HWIgnore)||inst->SWLimitNeg)
									inst->StatusJOG 			= 900;
								else if (!inst->info.moveActive && inst->info.speed==0)
									inst->StatusJOG 				= 900;//Finish
								else if(!inst->JogNeg){
									inst->StatusJOG 			= 900;
								}
														
								break;
							
							case 900:
								inst->JogNeg = 0;
								inst->StatusJOG = 0;
								inst->nLink->cmd.absolute = 0;
			
								break;
					
							default:
							break;
						}
					}
				}
			}
			else
			{	inst->nLink->cmd.velocity=0;
				inst->statuDelay.IN = 0;
				inst->nLink->cmd.absolute = 0;
				inst->StatusJOG = 0;
				if (inst->info.moveActive)
				{
					inst->nLink->cmd.stop 			= 1;
				}
			}
			inst->oldcmdMoveRef  = inst->MoveAdditive;
			inst->oldcmdMoveAbs  = inst->MoveAbsolute;
			
			break;
		
		case 1000:
			switch (inst->Parameter.homing.mode)
			///寻参需要注意几个参数(位置=最后寻参的位置，寻参时候的速度)
			{
				case HOMING_DIRECT:
					inst->StatusID 				= 3900;
					inst->nLink->info.homingOk 	= 0;
					inst->info.homingOk			= 0;
					break;
			
				case HOMING_END_SWITCH:
					inst->StatusID 				= 2000;
					inst->nLink->info.homingOk 	= 0;
					inst->info.homingOk			= 0;

					break;
			
				case HOMING_REF_SWITCH:
					inst->StatusID 				= 3000;
					inst->nLink->info.homingOk 	= 0;
					inst->info.homingOk			= 0;

					break;
			
				default:
					break;
			}
			break;
		
		case 7000:
			inst->nLink->cmd.cyclicPosition = 1;
			inst->nLink->param.speed = inst->Parameter.speed * inst->Unifactor;			
			inst->nLink->param.acceleration = inst->Parameter.acceleration * inst->Unifactor;
			inst->nLink->param.deceleration = inst->Parameter.deceleration * inst->Unifactor;
			if (!inst->MoveCyclicPosition)
			{
				inst->StatusID 					= 0;
				inst->nLink->cmd.cyclicPosition = 0;
			}
			
			switch (inst->CycDireStep)
			{
				case 0:
					if (inst->info.position>(inst->Parameter.position+0.1f))//需要往反方向走
						inst->CycDireStep = 10;
					else if (inst->info.position<(inst->Parameter.position-0.1f))//需要往正方向走
						inst->CycDireStep = 20;
					break;
			
				case 10:
					if (inst->info.position>inst->Parameter.position)//反方向走，只有
					{
						inst->nLink->param.position = inst->Parameter.position  * inst->Unifactor;
						inst->IOMapping.DoDirection = !inst->MountDirection;
					}
					inst->statuCycDelay.PT=0.02f / inst->SystemCfg.taskTime;
					inst->statuCycDelay.IN = (inst->nLink->param.position == inst->nLink->info.position);
					if (inst->statuCycDelay.Q)
					{
						inst->statuCycDelay.IN = 0;
						inst->CycDireStep = 0;
					}
					break;
			
				case 20:
					if (inst->info.position<inst->Parameter.position)//反方向走，只有
					{
						inst->nLink->param.position = inst->Parameter.position  * inst->Unifactor;
						inst->IOMapping.DoDirection = inst->MountDirection;
					}
					inst->statuCycDelay.PT=0.02f / inst->SystemCfg.taskTime;
					inst->statuCycDelay.IN = (inst->nLink->param.position == inst->nLink->info.position);
					if (inst->statuCycDelay.Q)
					{
						inst->statuCycDelay.IN = 0;
						inst->CycDireStep = 0;
					}
					break;

				default:

					break;
			}
			break;
		
		case 2000://edgeswitch + trigger direction====>MODE			
			switch (HOMEENDSWITCHMODE)
			{
				case 0://edgeswitch:0  trigger direction:0
					inst->StatusID 				= 2100;
					break;
				case 1://edgeswitch:0  trigger direction:1
						inst->StatusID 				= 2100;//2200;
					break;
				case 2://edgeswitch:1  trigger direction:0
						inst->StatusID 				= 2300;
					break;
				case 3://edgeswitch:1  trigger direction:1
						inst->StatusID 				= 2300;//2400;
					break;
				default:
					break;
			}
			break;
		
	//↓----------------	edgeswitch:0  trigger direction:0
		case 2100:
			if (!inst->IOMapping.positiveLimitSwitch)
				inst->StatusID 				= 2101;
			else if (inst->IOMapping.positiveLimitSwitch)
				inst->StatusID 				= 2105;
			break;

		case 2101://正向走
			//inst->Parameter.speed = inst->Parameter.;
			inst->nLink->cmd.velocity	= !inst->IOMapping.positiveLimitSwitch;
			inst->IOMapping.DoDirection = inst->MountDirection;
			//定义D=0时是正向;MountDirection=0表示正向
			inst->nLink->cmd.stop = inst->IOMapping.positiveLimitSwitch;
			inst->nLink->param.speed = inst->Parameter.homing.speedSearch * inst->Unifactor;
			if (inst->IOMapping.positiveLimitSwitch)
				inst->StatusID 			= 2103;
			break;
		
		case 2103:
			if (!inst->info.moveActive && inst->info.speed==0)
				inst->StatusID 			= 2105;
			break;
		
		case 2105://反向走,取反代表反向走
			inst->nLink->cmd.velocity	= inst->IOMapping.positiveLimitSwitch;
			inst->IOMapping.DoDirection = !inst->MountDirection;
			//定义D=0时是正向;MountDirection=0表示正向
			inst->nLink->cmd.stop = !inst->IOMapping.positiveLimitSwitch;
			inst->nLink->param.speed = inst->Parameter.homing.speed2 * inst->Unifactor;
			if (!inst->IOMapping.positiveLimitSwitch)
				inst->StatusID 			= 2107;
			break;
		
		case 2107:
			if (!inst->info.moveActive && inst->info.speed==0)
				inst->StatusID 			= 2109;
			break;
		////
		case 2109://正向走
			//inst->Parameter.speed = inst->Parameter.;
			inst->nLink->cmd.velocity	= !inst->IOMapping.positiveLimitSwitch;
			inst->IOMapping.DoDirection = inst->MountDirection;
			//定义D=0时是正向;MountDirection=0表示正向
			inst->nLink->cmd.stop = inst->IOMapping.positiveLimitSwitch;
			if (inst->IOMapping.positiveLimitSwitch)
				inst->StatusID 			= 2111;
			break;
		////
		case 2111:
			if (!inst->info.moveActive && inst->info.speed==0)
				inst->StatusID 			= 3900;
			break;
	//↑----------------	edgeswitch:0  trigger direction:0
		
	//↓----------------	edgeswitch:1  trigger direction:0
		case 2300:
			if (inst->IOMapping.negativeLimitSwitch)
				inst->StatusID 				= 2301;
			else if (!inst->IOMapping.negativeLimitSwitch)
			{
				inst->StatusID 				= 2305;
				inst->nLink->param.speed = inst->Parameter.homing.speedSearch * inst->Unifactor;	
			}
			
			break;

		case 2301://正向走
			//inst->Parameter.speed = inst->Parameter.;
			inst->nLink->cmd.velocity	= inst->IOMapping.negativeLimitSwitch;
			inst->IOMapping.DoDirection = inst->MountDirection;
			//定义D=0时是正向;MountDirection=0表示正向
			inst->nLink->cmd.stop = !inst->IOMapping.negativeLimitSwitch;
			inst->nLink->param.speed = inst->Parameter.homing.speed2 * inst->Unifactor;
			if (!inst->IOMapping.negativeLimitSwitch)
				inst->StatusID 			= 2303;
			break;
		
		case 2303:
			if (!inst->info.moveActive && inst->info.speed==0)
				inst->StatusID 			= 2305;
			break;
		
		case 2305://反向走,取反代表反向走
			inst->nLink->cmd.velocity	= !inst->IOMapping.negativeLimitSwitch;
			inst->IOMapping.DoDirection = !inst->MountDirection;
			//定义D=0时是正向;MountDirection=0表示正向
			inst->nLink->cmd.stop = inst->IOMapping.negativeLimitSwitch;
			if (inst->IOMapping.negativeLimitSwitch)
				inst->StatusID 			= 2307;
			break;
		
		case 2307:
			if (!inst->info.moveActive && inst->info.speed==0)
				inst->StatusID 			= 2309;
			break;
		////
		case 2309://正向走
			//inst->Parameter.speed = inst->Parameter.;
			inst->nLink->cmd.velocity	= inst->IOMapping.negativeLimitSwitch;
			inst->IOMapping.DoDirection = inst->MountDirection;
			//定义D=0时是正向;MountDirection=0表示正向
			inst->nLink->cmd.stop = !inst->IOMapping.negativeLimitSwitch;
			inst->nLink->param.speed = inst->Parameter.homing.speed2 * inst->Unifactor;
			if (!inst->IOMapping.negativeLimitSwitch)
				inst->StatusID 			= 2311;
			break;
		////
		case 2311:
			if (!inst->info.moveActive && inst->info.speed==0)
				inst->StatusID 			= 3900;
			break;
	//↑----------------	edgeswitch:1  trigger direction:0
		case 3900:
			inst->StatusID 				= 3999;
			inst->nLink->cmd.homing 	= 1;
			inst->nLink->param.position = inst->Parameter.homing.position * inst->Unifactor;


			break;
		
		case 3999:
			if (inst->info.homingOk && fabs(inst->info.position - inst->Parameter.homing.position)<0.1)
			{
				inst->nLink->cmd.homing = 0;
				inst->Homing			= 0;
				inst->StatusID			= 8000;
			}
			break;
	//↑----------------	end Homing
		case 5000:			
		//	inst->MoveAbsolute = 0;
			if (inst->info.position>inst->Parameter.position)
				inst->StatusID			= 5100;
			else if (inst->info.position<=inst->Parameter.position)
				inst->StatusID			= 5400;
			break;
		
		case 5100://反向走,取反代表反向走			
			inst->nLink->cmd.absolute	= !((inst->IOMapping.negativeLimitSwitch&&!inst->HWIgnore)||inst->SWLimitNeg);
			inst->IOMapping.DoDirection = !inst->MountDirection;
			inst->nLink->param.position = inst->Parameter.position * inst->Unifactor;
			//定义D=0时是正向;MountDirection=0表示正向
			inst->nLink->cmd.stop = (inst->IOMapping.negativeLimitSwitch&&!inst->HWIgnore)||inst->SWLimitNeg;
			inst->nLink->param.speed = inst->Parameter.speed * inst->Unifactor;
			if ((inst->IOMapping.negativeLimitSwitch&&!inst->HWIgnore)||inst->SWLimitNeg)
				inst->StatusID 			= 9000;
			else
				inst->StatusID 			= 5110;
			break;
		
		case 5110://判断指令已经执行
			inst->statuDelay.IN=1;
			inst->statuDelay.PT=0.1f / inst->SystemCfg.taskTime;//wait commd
			if (inst->info.moveActive || inst->statuDelay.Q)
			{
				//inst->MoveAbsolute = 0;
				inst->StatusID 			= 5111;
			}
			break;
		
		case 5111://判断已经完成，或者切换其他指令	
			inst->statuDelay.IN = 0;
			/////
			inst->nLink->cmd.stop = (inst->IOMapping.negativeLimitSwitch&&!inst->HWIgnore)||inst->SWLimitNeg;
			if ((inst->IOMapping.negativeLimitSwitch&&!inst->HWIgnore)||inst->SWLimitNeg)
				inst->StatusID 			= 9000;
			else if (!inst->info.moveActive && inst->info.speed==0)
				inst->StatusID 			= 8000;
			else if (inst->MoveAbsolute)
				//inst->StatusID 			= 5120;
				;
			break;
		
		case 5120:
			if (inst->info.position>inst->Parameter.position)
				inst->StatusID			= 5800;
			else if (inst->info.position<=inst->Parameter.position)
				inst->StatusID			= 5700;		
			break;		
		
		case 5400://正向走
			//inst->Parameter.speed = inst->Parameter.;
			inst->nLink->cmd.absolute	= !((inst->IOMapping.positiveLimitSwitch&&!inst->HWIgnore)||inst->SWLimitPos);
			inst->IOMapping.DoDirection = inst->MountDirection;
			inst->nLink->param.position = inst->Parameter.position * inst->Unifactor;

			//定义D=0时是正向;MountDirection=0表示正向
			inst->nLink->cmd.stop = (inst->IOMapping.positiveLimitSwitch&&!inst->HWIgnore)||inst->SWLimitPos;
			inst->nLink->param.speed = inst->Parameter.speed * inst->Unifactor;
			if ((inst->IOMapping.positiveLimitSwitch&&!inst->HWIgnore)||inst->SWLimitPos)
				inst->StatusID 			= 9000;
			else
				inst->StatusID 			= 5410;
			break;
		
		case 5410:
			inst->statuDelay.IN=1;
			inst->statuDelay.PT=0.1f / inst->SystemCfg.taskTime;//wait commd
			if (inst->info.moveActive || inst->statuDelay.Q)
			{
				//inst->MoveAbsolute 		= 0;
				inst->StatusID 			= 5411;
			}
			break;
		
		case 5411:
			inst->statuDelay.IN = 0;
			/////
			inst->nLink->cmd.stop = (inst->IOMapping.positiveLimitSwitch&&!inst->HWIgnore)||inst->SWLimitPos;
			if ((inst->IOMapping.positiveLimitSwitch&&!inst->HWIgnore)||inst->SWLimitPos)
				inst->StatusID 			= 9000;
			else if (!inst->info.moveActive && inst->info.speed==0)
				inst->StatusID 			= 8000;
			else if (inst->MoveAbsolute)
				inst->StatusID 			= 5411;
			break;
		
		case 5420:
			if (inst->info.position>inst->Parameter.position)
				inst->StatusID			= 5700;
			else if (inst->info.position<=inst->Parameter.position)
				inst->StatusID			= 5800;		
			break;
		
		case 5700:
			inst->nLink->cmd.stop = 1;
			if (!inst->info.moveActive && inst->info.speed==0)
				inst->StatusID 			= 5800;
			break;
		
		case 5800://command resend
			inst->StatusID 				= 5000;
			break;		
	//↑----------------	end MoveAbs
		case 6000:
			if (inst->Distance<0)
				inst->StatusID			= 6100;
			else if (inst->Distance>=0)
				inst->StatusID			= 6400;
			break;
		
		case 6100://反向走,取反代表反向走			
			inst->nLink->cmd.absolute	= !((inst->IOMapping.negativeLimitSwitch&&!inst->HWIgnore)||inst->SWLimitNeg);
			inst->IOMapping.DoDirection = !inst->MountDirection;
			//////
			if (inst->info.position + inst->Distance>inst->Parameter.SWLimits.Upper && !inst->Parameter.SWLimits.Ignore)
				inst->Parameter.position 	= inst->Parameter.SWLimits.Upper;
			else if (inst->info.position + inst->Distance<inst->Parameter.SWLimits.Lower && !inst->Parameter.SWLimits.Ignore)
				inst->Parameter.position 	= inst->Parameter.SWLimits.Lower;
			else			
				inst->Parameter.position 	= inst->info.position + inst->Distance;
			
			inst->nLink->param.position = inst->Parameter.position  * inst->Unifactor;
			
			//////
			//定义D=0时是正向;MountDirection=0表示正向
			inst->nLink->cmd.stop = (inst->IOMapping.negativeLimitSwitch&&!inst->HWIgnore)||inst->SWLimitNeg;
			inst->nLink->param.speed = inst->Parameter.speed * inst->Unifactor;
			if ((inst->IOMapping.negativeLimitSwitch&&!inst->HWIgnore)||inst->SWLimitNeg)
				inst->StatusID 			= 9000;
			else
				inst->StatusID 			= 6110;
			break;
		
		case 6110://判断指令已经执行
			inst->statuDelay.IN=1;
			inst->statuDelay.PT=0.1f / inst->SystemCfg.taskTime;//wait commd
			if (inst->info.moveActive || inst->statuDelay.Q)
			{
				inst->StatusID 			= 6111;
				//inst->MoveAdditive 		= 0;

			}
			break;
		
		case 6111://判断已经完成，或者切换其他指令	
			inst->statuDelay.IN = 0;
			/////
			inst->nLink->cmd.stop = (inst->IOMapping.negativeLimitSwitch&&!inst->HWIgnore)||inst->SWLimitNeg;
			if ((inst->IOMapping.negativeLimitSwitch&&!inst->HWIgnore)||inst->SWLimitNeg)
				inst->StatusID 			= 9000;
			else if (!inst->info.moveActive && inst->info.speed==0)
				inst->StatusID 			= 8000;
			else if (inst->MoveAdditive)
				inst->StatusID 			= 6111;
			break;
		
		case 6120:
			if (inst->Distance<0)
				inst->StatusID			= 6800;
			else if (inst->Distance>=0)
				inst->StatusID			= 6700;
			break;

		
		case 6400://正向走
			//inst->Parameter.speed = inst->Parameter.;
			inst->nLink->cmd.absolute	= !((inst->IOMapping.positiveLimitSwitch&&!inst->HWIgnore)||inst->SWLimitPos);
			inst->IOMapping.DoDirection = inst->MountDirection;
			//////
			if (inst->info.position + inst->Distance>inst->Parameter.SWLimits.Upper && !inst->Parameter.SWLimits.Ignore)
				inst->Parameter.position 	= inst->Parameter.SWLimits.Upper;
			else if (inst->info.position + inst->Distance<inst->Parameter.SWLimits.Lower && !inst->Parameter.SWLimits.Ignore)
				inst->Parameter.position 	= inst->Parameter.SWLimits.Lower;
			else			
				inst->Parameter.position 	= inst->info.position + inst->Distance;
			////////
			inst->nLink->param.position = inst->Parameter.position * inst->Unifactor;
			//////
			//定义D=0时是正向;MountDirection=0表示正向
			inst->nLink->cmd.stop = (inst->IOMapping.positiveLimitSwitch&&!inst->HWIgnore)||inst->SWLimitPos;
			inst->nLink->param.speed = inst->Parameter.speed * inst->Unifactor;
			if ((inst->IOMapping.positiveLimitSwitch&&!inst->HWIgnore)||inst->SWLimitPos)
				inst->StatusID 			= 9000;
			else
				inst->StatusID 			= 6410;
			break;
		
		case 6410:
			inst->statuDelay.IN=1;
			inst->statuDelay.PT=0.1f / inst->SystemCfg.taskTime;//wait commd
			if (inst->info.moveActive || inst->statuDelay.Q)
			{
				inst->StatusID 			= 6411;
				//inst->MoveAdditive 		= 0;

			}
			break;
		
		case 6411:
			inst->statuDelay.IN = 0;
			/////
			inst->nLink->cmd.stop = (inst->IOMapping.positiveLimitSwitch && !inst->HWIgnore)||inst->SWLimitPos;
			if ((inst->IOMapping.positiveLimitSwitch && !inst->HWIgnore) || inst->SWLimitPos)
				inst->StatusID 			= 9000;
			else if (!inst->info.moveActive && inst->info.speed==0)
				inst->StatusID 			= 8000;
			else if (inst->MoveAdditive)
				inst->StatusID 			= 6411;
			break;
		
		case 6420:
			if (inst->Distance<=0)
				inst->StatusID			= 6700;
			else if (inst->Distance>0)
				inst->StatusID			= 6800;
			break;
		
		case 6700:
			inst->nLink->cmd.stop = 1;
			if (!inst->info.moveActive && inst->info.speed==0)
				inst->StatusID 			= 6800;
			break;
		
		case 6800://command resend
			inst->StatusID 				= 6000;
			break;
	//↑----------------	end MoveAddit
		case 8000://reset All command
			inst->StatusID			= 0;
			inst->MoveAdditive 		= 0;
			inst->MoveAbsolute 		= 0;
			break;
		
		case 9000://error occ
			inst->internalStatus 	= 90000;
			inst->StatusID			= 0;
			inst->MoveAdditive 		= 0;
			inst->MoveAbsolute 		= 0;
			break;
		default:

			break;
	}
	TON_10ms(&inst->statuDelay);
	TON_10ms(&inst->statuCycDelay);
}
