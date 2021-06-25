#include "stm32f4xx.h"
#include "mInclude.h"

agv_mode_App AppTask[]={NullApp,BMSApp,CallerApp,ChargeStaionApp,SonarApp};
//gMachine_type gMachine;


int main(void)
{
	uint8_t Parameter2[800] = {0};
	///////////////////
	SystemConfig();////
	///////////////////
	IAPInit();
	///////////////////
  while(1){
		IAPAPP();
		IdleApp();
		AppTask[0]();	
		/////////////////////////////////////////////////////////
		if(gSystemPara.SaveParameter){
			gSystemPara.SaveParameter = 0;
			memcpy(&Parameter2,&gSystemPara,sizeof(gSystemPara));
			Parameter2[799] = 0xCD;
			STMFLASH_Write(FLASH_SAVE_ADDR,( uint32_t * )Parameter2,800);
		}	
	}
}
__weak void NullApp(){;}
__weak void BMSApp(){;}
__weak void CallerApp(){;}
__weak void ChargeStaionApp(){;}
__weak void SonarApp(){;}
	
	
