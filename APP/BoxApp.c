#include "mInclude.h"

uint16_t NodenDis[10][3],pNowNode=0,NodeSum=2,TimeRecInterval=0;

uint32_t oldTimeStampForAlive,TimeAlive,timesExit[4],oldTimeStampForCnt=0,oldTimeStampFor4msCnt=0,oldTimeStampFor1sCnt;

static uint8_t readCmd=0,firstBootWeight=1;
static int32_t weight_Zero_Data=0; 
static int32_t weight_count=0;  
static float weight;
static int32_t weight_proportion=31114;  // 电压值与重量变换比例，这个需要实际测试计算才能得到
static float data_temp=0;  
///////////////////////////////////////////////////////////////
//agv_mode_App AppTask[]={NullApp,BMSApp,CallerApp,ChargeStaionApp,SonarApp};
//cyc_task_fun cycTask[]={cyc2ms,cyc4ms,cyc20ms,cyc100ms,cyc500ms,cycLongTsk};
///////////////////////////////////////////////////////////////

static float timeLoad=0,timeIdle=0,timePer=0;

void cyc4ms(){	
	gMachineIO.cyc4msUpdate = 1;	
	timeIdle += 0.004;
}

void cyc20ms(){	readCmd=1;};

void cyc500ms(){
		gMachineIO.LED = 1 - gMachineIO.LED;
}


void NullApp()
{
	static uint32_t readBack[10];

	gMachineIO.timeSec = gMachineIO.TimeStamp / 1000.0f;

	if(1==gMachineIO.cyc4msUpdate){

		gMachineIO.cyc4msUpdate = 0;
		if(firstBootWeight==1){
		if(AD7190_Init()==0){
			while(1){
				delay_ms(1000);
				if(AD7190_Init())
					break;
			}
		}
		delay_ms(1000);
		firstBootWeight = 0;
		weight_ad7190_conf();
		delay_ms(1000);
		weight_Zero_Data = weight_ad7190_ReadAvg(3);
	}
	else{
		SLAVE_SELECT_LOW;
		weight_count=weight_ad7190_ReadAvg(1);
		data_temp=weight_count-weight_Zero_Data;
		weight=data_temp*1000/weight_proportion;
		//timeLoad += 1;
		if(timeLoad>0){
			timePer = 1000.0*(timeIdle - timeLoad);
		}
		timeLoad = timeIdle;
	}
	
	//////////////////////////pass the value to the pdo;		
}
	
	

	
	if(readCmd==1){///	
		readCmd = 0;
	}
	gMachineIO.IdleTime++;
}

