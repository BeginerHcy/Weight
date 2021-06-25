#ifndef __M4_IAP_H__
#define __M4_IAP_H__

#include "HwCfg.h"

typedef struct dwnCtrl_type{

	uint32_t fwlen;
	uint32_t status;
	uint32_t index;
	uint8_t numSend;
	uint32_t indexArry[100];
	uint8_t iArry;
	uint32_t demandIndex;
	uint32_t bufAddr;
	
	
}dwnCtrl_type;


extern dwnCtrl_type dwnCtrl;
extern float timeBoot;
extern uint8_t IAPFirstBoot;

void IAPAPP(void);
void IAPInit(void);
bool capStartDwn(UrtBuf_type * pSrcBuf);
bool capEndDwn(UrtBuf_type * pSrcBuf);
bool capBin(UrtBuf_type * pSrcBuf);


#endif
