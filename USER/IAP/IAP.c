
#include "mInclude.h"
#include "bsp_user_lib.h"
float timeBoot = 0;
dwnCtrl_type dwnCtrl={0};
uint8_t IAPFirstBoot = 0;

void IAPInit(void){
	IAPFirstBoot = 1;
}

void IAPAPP(void){
	
	uint16_t crc;
	
	if(IAPFirstBoot==1){
		delay_ms(100);
		gMachineIO.Uart1Data.sBuffer[0] = 0xDB;
		gMachineIO.Uart1Data.sBuffer[1] = 0x01;
		crc=CRC16_Modbus(&gMachineIO.Uart1Data.sBuffer[0],2);
		gMachineIO.Uart1Data.sBuffer[2] = crc>>8;
		gMachineIO.Uart1Data.sBuffer[3] = crc>>0 & 0xFF; 
		gMachineIO.Uart1Data.sLen = 4;	
		SendUrtBuf(&gMachineIO.Uart1Data);
		IAPFirstBoot = 0;
		dwnCtrl.iArry = 0;
		dwnCtrl.demandIndex = 0;
		memset(&dwnCtrl,0,sizeof(dwnCtrl));
	}
	
	switch (dwnCtrl.status){
	
		case 0:
			
			break;
		
		case 10:
			
			break;
		
		case 20:
			
			break;
		
		case 30:
			
			break;		
		
		default:
			
			break;
	
	}
	
	if(capStartDwn(&gMachineIO.Uart1Data)==1){	
		delay_ms(10);
		
		gMachineIO.Uart1Data.sBuffer[0] = 0xBB;
		gMachineIO.Uart1Data.sBuffer[1] = 0x01;
		crc=CRC16_Modbus(&gMachineIO.Uart1Data.sBuffer[0],2);
		gMachineIO.Uart1Data.sBuffer[2] = crc>>8;
		gMachineIO.Uart1Data.sBuffer[3] = crc>>0 & 0xFF; 
		gMachineIO.Uart1Data.sLen = 4;	
		SendUrtBuf(&gMachineIO.Uart1Data);
		
		delay_ms(100);
		
		NVIC_SystemReset();
	}
	
}

bool capStartDwn(UrtBuf_type * pSrcBuf){
	uint8_t filled = pSrcBuf->pRfil;
	uint8_t degred = pSrcBuf->pRder;
	uint16_t crcResult=0;
	uint8_t chnOKBfLen = 4;
	
	bool result=0;
	///
	result = 0;
	///
	if(filled >= degred+ chnOKBfLen)//1. Len enough.
	{
		for(;degred<=filled-1;degred++)
		{
			if(filled >= degred+ chnOKBfLen){
				/////////////////////////////////////
				if(pSrcBuf->rBuffer[degred]==0xAB && 
					pSrcBuf->rBuffer[degred+1]==0x01)
				{
					crcResult = CRC16_Modbus(&pSrcBuf->rBuffer[degred],2);
					if(crcResult == pSrcBuf->rBuffer[degred+2]*256+pSrcBuf->rBuffer[degred+3])
					{
						pSrcBuf->pRder = degred+chnOKBfLen-1;
						result = 1;
					}
				}
			}
		}
	}
	return result;
}

bool capEndDwn(UrtBuf_type * pSrcBuf){
	uint8_t filled = pSrcBuf->pRfil;
	uint8_t degred = pSrcBuf->pRder;
	uint16_t crcResult=0;
	uint8_t chnOKBfLen = 4;
	
	bool result=0;
	///
	result = 0;
	///
	if(filled >= degred+ chnOKBfLen)//1. Len enough.
	{
		for(;degred<=filled-1;degred++)
		{
			if(filled >= degred+ chnOKBfLen){
				/////////////////////////////////////
				if(pSrcBuf->rBuffer[degred]==0xAC && 
					pSrcBuf->rBuffer[degred+1]==0x01)
				{
					crcResult = CRC16_Modbus(&pSrcBuf->rBuffer[degred],2);
					if(crcResult == pSrcBuf->rBuffer[degred+2]*256+pSrcBuf->rBuffer[degred+3])
					{
						pSrcBuf->pRder = degred+chnOKBfLen-1;
						result = 1;
					}
				}
			}
		}
	}
	return result;
}

bool capBin(UrtBuf_type * pSrcBuf){
	uint8_t filled = pSrcBuf->pRfil;
	uint8_t degred = pSrcBuf->pRder;
	uint16_t crcResult=0;
	uint8_t chnOKBfLen = 10;
//	uint32_t index;
	bool result=0;
	///
	result = 0;
	///
	if(filled >= degred+ chnOKBfLen)//1. Len enough.
	{
		for(;degred<=filled-1;degred++)
		{
			if(filled >= degred+ chnOKBfLen){
				/////////////////////////////////////
				if(pSrcBuf->rBuffer[degred]==0xAA && pSrcBuf->rBuffer[degred+7]<=255)
				{
					if(filled>=degred+chnOKBfLen+pSrcBuf->rBuffer[degred+7]){
						crcResult = CRC16_Modbus(&pSrcBuf->rBuffer[degred],8+pSrcBuf->rBuffer[degred+7]);
						if(crcResult == pSrcBuf->rBuffer[degred+8+pSrcBuf->rBuffer[degred+7]]*256 + pSrcBuf->rBuffer[degred+8+pSrcBuf->rBuffer[degred+7]+1])
						{
							dwnCtrl.index = pSrcBuf->rBuffer[degred+1] * 65536 + pSrcBuf->rBuffer[degred+2] * 256 + pSrcBuf->rBuffer[degred+3];
							if(dwnCtrl.index==dwnCtrl.demandIndex){
								dwnCtrl.demandIndex = dwnCtrl.index + pSrcBuf->rBuffer[degred+7];
								dwnCtrl.numSend = pSrcBuf->rBuffer[degred+7];
								dwnCtrl.bufAddr = (uint32_t)&(pSrcBuf->rBuffer[degred+8]);
								pSrcBuf->pRder = degred+chnOKBfLen-1;
								result = 1;
							}
						}
					}
				}
			}
		}
	}
	return result;
}


