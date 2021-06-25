#include "mInclude.h"

uint16_t NodenDis[10][3],pNowNode=0,NodeSum=2,TimeRecInterval=0;

uint32_t oldTimeStampForAlive,TimeAlive,timesExit[4],oldTimeStampForCnt=0,oldTimeStampFor4msCnt=0,oldTimeStampFor1sCnt;

static uint8_t readCmd=0;

///////////////////////////////////////////////////////////////
//agv_mode_App AppTask[]={NullApp,BMSApp,CallerApp,ChargeStaionApp,SonarApp};
//cyc_task_fun cycTask[]={cyc2ms,cyc4ms,cyc20ms,cyc100ms,cyc500ms,cycLongTsk};
///////////////////////////////////////////////////////////////



void cyc4ms(){	
	gMachineIO.cyc4msUpdate = 1;	
}

void cyc20ms(){	readCmd=1;};

void cyc500ms(){
		gMachineIO.LED = 1 - gMachineIO.LED;
}

void NullApp(){
	
	gMachineIO.timeSec = gMachineIO.TimeStamp / 1000.0f;
	
	if(1==gMachineIO.cyc4msUpdate){

		gMachineIO.cyc4msUpdate = 0;
		
		//////////////////////////pass the value to the pdo;		
	}
	
	if(readCmd==1){///	
		readCmd = 0;
	}
	gMachineIO.IdleTime++;
}

