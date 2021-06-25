#include "mInclude.h"

can_master_type canMaster={0};


void _canMasterBoot_up(){

	canMaster.SlaveNumbers = CAN_SLAVE_NUM;//two node
	canMaster.MstNodeID = 0x04;//set master id to 0x01
	canMaster.MstHeartBeatTimeInterval = 1000;//1s interval
	canMaster.SlaveHeartBeatTimeInterval = 1000;//1s interval
	canMaster.synTimeInterval = 12;//4ms one syn frame
	canMaster.MstNodeState = Pre_operational;// master state pre-operation state
	canMaster.lifeGuardTimeoutsetting = 4000;
	//////////////////////////////////////////////////////
	canMaster.nodes[0] = 0x01;
	canMaster.nodes[1] = 0x02;
	canMaster.nodes[2] = 0x03;
	///////////////////////////
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	//////////////////////////////////////////////////////
	CanOpen_hrtbeat(canMaster.MstNodeState,canMaster.MstNodeID);
	//////////////////////////////////////////////////////
	CanOpen_nmt(ResetCommunicate,0x00);// tell all nodes
	delay_ms(1000);
	///////////////////////////
	canMaster.msterReset = 1;
}

void _canMaster_Cyclic(){
	
	uint8_t valSdo[4]={0};
	uint32_t pdo_obj[3],valuesend;
	
	
	for(int i=0;i<CAN_SLAVE_NUM;i++){		
		if(canMaster.nodeTimeOut[canMaster.nodes[i]]<(canMaster.lifeGuardTimeoutsetting)){
			canMaster.nodeTimeOut[canMaster.nodes[i]]+=canMaster.synTimeInterval;
		}
		else{
			canMaster.networkInitialed[canMaster.nodes[i]]=0;
			canMaster.resetNeeded[canMaster.nodes[i]]=1;
		}
		if(canMaster.resetNeeded[canMaster.nodes[i]]&&
			canMaster.NMTable[canMaster.nodes[i]]==Initialisation){
			canMaster.resetNeeded[canMaster.nodes[i]] = 0;
		}
	}
	
	
	if(!canMaster.msterReset) return;// for master reboot;
	
	canMaster.synNeeded = 0;
	////////////////////////////////////////////////////send heartBeat msg
	if(gMachineIO.cyc1sUpdateCanopen == 1){
		gMachineIO.cyc1sUpdateCanopen = 0;
		CanOpen_hrtbeat(Operational,canMaster.MstNodeID);
		delay_us(100);
	}
	
	for(int i=0;i<CAN_SLAVE_NUM;i++){///NMT manager and par download	
		
		if(!canMaster.networkInitialed[canMaster.nodes[i]]|| // not exit on network
			canMaster.NMTable[canMaster.nodes[i]]==Pre_operational|| // not 0x7F state
			canMaster.resetNeeded[canMaster.nodes[i]]==1){		// if timmout then reset
			CanOpen_nmt(ResetCommunicate,canMaster.nodes[i]);	// tell the nodes to resest nodes
			canMaster.parInitialed[canMaster.nodes[i]] = 0;   // tell the nodes to resest parinitial state
			delay_ms(1);
		}
		else if(!canMaster.parInitialed[canMaster.nodes[i]]){ ///  download parameters
			
			switch(canMaster.pardwnstep[canMaster.nodes[i]]){ /// download steps
				
				case 0:/// set life guard object 1016, guard time 4000ms
					valSdo[0] = (uint8_t)(canMaster.lifeGuardTimeoutsetting >> 0 & 0x00FF);
					valSdo[1] = (uint8_t)(canMaster.lifeGuardTimeoutsetting >> 8 & 0x00FF);
					valSdo[2] = canMaster.MstNodeID;
					valSdo[3] = 0;//reserve				
					if(CanOpen_writeSDO(canMaster.nodes[i],0x1016,1,valSdo,4))
						canMaster.pardwnstep[canMaster.nodes[i]]++;
					break;			
					
				case 1:///set the slave heartbeat , feedback from slave , lifeguard
					valSdo[0] = (uint8_t)(canMaster.SlaveHeartBeatTimeInterval >> 0 & 0x00FF);
					valSdo[1] = (uint8_t)(canMaster.SlaveHeartBeatTimeInterval >> 8 & 0x00FF);
					if(CanOpen_writeSDO(canMaster.nodes[i],0x1017,0,valSdo,2))
					#ifdef HEXIN_TYPE
						canMaster.pardwnstep[canMaster.nodes[i]]=4;
					#endif
					#ifdef INOVA_TYPE
						canMaster.pardwnstep[canMaster.nodes[i]]++;
					#endif
					break;
					
				case 2:///set the slave heartbeat , feedback from slave , lifeguard
					valSdo[0] = (uint8_t)(canMaster.synTimeInterval*3 >> 0 & 0x00FF);
					valSdo[1] = (uint8_t)(canMaster.synTimeInterval*3 >> 8 & 0x00FF);
					if(CanOpen_writeSDO(canMaster.nodes[i],0x2005,7,valSdo,2))
						canMaster.pardwnstep[canMaster.nodes[i]]++;
					break;					
					
				case 3:
					valSdo[0] = 0;//0 when com error , what state to act,
					if(CanOpen_writeSDO(canMaster.nodes[i],0x1029,1,valSdo,1))
						canMaster.pardwnstep[canMaster.nodes[i]]++;
					break;
					
				case 4:	//TPDO1
					pdo_obj[0] = 0x60410010;
					pdo_obj[1] = 0x60610008;
					if(CanOpenConfigTPDO(canMaster.nodes[i],1,pdo_obj,2))
						canMaster.pardwnstep[canMaster.nodes[i]]++;
					break;
					
				case 5:	//TPDO2
					pdo_obj[0] = 0x60640020;
					if(CanOpenConfigTPDO(canMaster.nodes[i],2,pdo_obj,1))
						canMaster.pardwnstep[canMaster.nodes[i]]++;
					break;	
					
				case 6:	//RPDO1
					pdo_obj[0] = 0x60400010;
					pdo_obj[1] = 0x60600008;
					if(CanOpenConfigRPDO(canMaster.nodes[i],1,pdo_obj,2))
						canMaster.pardwnstep[canMaster.nodes[i]]++;
					break;		
					
				case 7:	//RPDO2
					#ifdef HEXIN_TYPE
					pdo_obj[0] = 0x607A0020;
					if(CanOpenConfigRPDO(canMaster.nodes[i],2,pdo_obj,1))
						canMaster.pardwnstep[canMaster.nodes[i]]=10;
					#endif
					#ifdef INOVA_TYPE
					pdo_obj[0] = 0x60C10120;
					if(CanOpenConfigRPDO(canMaster.nodes[i],2,pdo_obj,1))
						canMaster.pardwnstep[canMaster.nodes[i]]++;
					#endif
					break;	
					
				case 8:
					valSdo[0] = canMaster.synTimeInterval;
					if(CanOpen_writeSDO(canMaster.nodes[i],0x60C2,1,valSdo,1))
						canMaster.pardwnstep[canMaster.nodes[i]]++;
					break;	
					
				case 9:// for interoplate
					valSdo[0] = 0xFD;//ms{FD=-3, 10^-3s = 1ms}
					if(CanOpen_writeSDO(canMaster.nodes[i],0x60C2,2,valSdo,1))
						canMaster.pardwnstep[canMaster.nodes[i]]++;
					break;			
					
				case 10:// for interoplate
					canMaster.parInitialed[canMaster.nodes[i]] = 1;
					CanOpen_nmt(StartNode,canMaster.nodes[i]);// tell nodes to start
					canMaster.pardwnstep[canMaster.nodes[i]] = 0;
					break;
				
				default:
					break;
				
			}
		}
		else{//PDO & NMT sending
			canMaster.synNeeded = 1;
			//////////////////////syn
			CanOpen_writePDO(canMaster.nodes[i],1,&canMaster.pdomapping[canMaster.nodes[i]].rpdo[0],3);
			delay_us(100);
			CanOpen_writePDO(canMaster.nodes[i],2,&canMaster.pdomapping[canMaster.nodes[i]].rpdo[1],4);
			delay_us(100);
		}
		if(i==0)delay_ms(1);
	}
	if(canMaster.synNeeded) CanOpen_syn();
}

void cycCanOpen4ms(void){
	_canMaster_Cyclic();
	;
}
void cycCanOpen1s(void){
		gMachineIO.cyc1sUpdateCanopen = 1;
}
