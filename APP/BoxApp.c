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
int16_t inputReg[10]={0};
static float timeLoad=0,timeIdle=0,timePer=0;
bool Rs485Request04hCap(UrtBuf_type * pSrcBuf);
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
		data_temp=weight_count;
		weight=data_temp*1000/weight_proportion;
		//timeLoad += 1;
		if(timeLoad>0){
			timePer = 1000.0*(timeIdle - timeLoad);
		}
		timeLoad = timeIdle;
	}
	inputReg[0] = weight_count >> 16;
	inputReg[1] = weight_count & 0xFFFF;
	
	inputReg[2] = (int32_t)(weight*100.0) >> 16;
	inputReg[3] = (int32_t)(weight*100.0) & 0xFFFF;
	
	inputReg[4] = (int32_t)(timeIdle*100.0) >> 16;
	inputReg[5] = (int32_t)(timeIdle*100.0) & 0xFFFF;
	
	Rs485Request04hCap(&gMachineIO.Uart1Data);
	Rs485Request04hCap(&gMachineIO.Uart6Data);
	
	//////////////////////////pass the value to the pdo;		
}
	
	if(readCmd==1){///	
		readCmd = 0;
	}
	gMachineIO.IdleTime++;
}

static bool Rs485Request04hCap(UrtBuf_type * pSrcBuf)
{
	uint8_t filled = pSrcBuf->pRfil;
	uint8_t degred = pSrcBuf->pRder;
	uint8_t iFunCode=5,iLenCode=4,iDstSta=3,iSrcSta=2;
	uint8_t chnOKBfLen = 8;
	bool result=0;
	
	unsigned char i=0,cnt;
	unsigned int crc,regAdr,regNum;
	unsigned char crch,crcl;
	static u8 len;
	static u8 buf[60];
	///
	result = 0;
	///
	if(filled >= degred+ chnOKBfLen)//1. Len enough.
	{ 
		for(;degred<=filled-1;degred++)
		{
			if(filled >= degred+ chnOKBfLen){
				
				crc=CRC_Verify(&pSrcBuf->rBuffer[degred],6);
				crch=crc>>8;                                    //crcH
				crcl=crc&0xFF;                                  //crcL
				
				regNum = (u16)(pSrcBuf->rBuffer[degred+5]) | (u16) (pSrcBuf->rBuffer[degred+4] << 8);
				regAdr = (u16)(pSrcBuf->rBuffer[degred+3]) | (u16) (pSrcBuf->rBuffer[degred+2] << 8);
				
				if(pSrcBuf->rBuffer[degred]==gSystemPara.RS485Node &&
					pSrcBuf->rBuffer[degred+6] == crcl &&
					pSrcBuf->rBuffer[degred+7] == crch)
				{					
					switch(pSrcBuf->rBuffer[degred+1]){
						
						case 0x04:
							if(regAdr>=1000 && regAdr<=1010 && (regAdr + regNum) <= 1011){
								
								i 			= regAdr-1000;
								cnt 		= regNum;								
								pSrcBuf->sBuffer[0] 	= gSystemPara.RS485Node;
								pSrcBuf->sBuffer[1] 	= 0x04;
								pSrcBuf->sBuffer[2] 	= cnt*2; 								
								len=3;   								
								while(cnt--)
								{
									pSrcBuf->sBuffer[len++] = inputReg[i] >> 8 & 0xFF;
									pSrcBuf->sBuffer[len++] = inputReg[i] >> 0 & 0xFF;
									i++;
								}											
							}
							else{
								pSrcBuf->sBuffer[0] 	= gSystemPara.RS485Node;
								pSrcBuf->sBuffer[1]		= 0x83;
								pSrcBuf->sBuffer[2]		= 0x02;
								len=3;
							}								
							break;
							
						default:
							break;
					
					}
					
					crc=CRC_Verify((uint8_t*)&pSrcBuf->sBuffer,len);					
					pSrcBuf->sBuffer[len++]=crc&0xff;
					pSrcBuf->sBuffer[len++]=crc>>8;
					
					pSrcBuf->sLen = len;
					SendUrtBuf(pSrcBuf);		
					
					pSrcBuf->pRder = degred+chnOKBfLen-1;
					result = 1;
				}
			}
		}
	}
	return result;
}

uint16_t CRC_Verify(uint8_t *CRC_Buf,uint8_t Buf_Length)
{
	uint8_t i,j;
	uint16_t wCrc = 0xffff;
	uint16_t wPolyNum = 0xA001;
	for(i = 0;i < Buf_Length;i++)
	{
		wCrc ^= CRC_Buf[i];
		
		for(j=0;j<8;j++)
		{
			if(wCrc & 0x0001)
			{
				wCrc = (wCrc >> 1)^wPolyNum;
			}
			else
			{
				wCrc = wCrc>>1;
			}
		}
	}
	return wCrc;
}
