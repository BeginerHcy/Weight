
#include <plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
	#include "HC2500B.h"
#ifdef __cplusplus
	};
#endif
/*<$Description$> TODO: Add your comment here </$Description$>*/
float MINReal(float AA, float BB)
{
	REAL CC = AA<BB?AA:BB;
	return CC;
}
signed long swap2Comb(UINT *adr)
{
	INT aa[2];
	DINT Result;
	aa[0] = *(adr+1);
	aa[1] = *adr;
	
	memcpy(&Result,&aa,sizeof(DINT));
	return Result;
}
signed long swap2CombREAL(REAL *adr)
{
	INT aa[2],bb[2];
	DINT Result;
	memcpy(&bb,adr,sizeof(DINT));
	aa[0] =  bb[1];
	aa[1] =  bb[0];
	
	memcpy(&Result,&aa,sizeof(DINT));
	return Result;
}
signed long swap32b(signed long ABCD){

	signed long result=0;

	result |= (ABCD>>24 & 0x000000FF);
	result |= (ABCD>>8 & 0x0000FF00);
	result |= (ABCD<<8 & 0x00FF0000);
	result |= (ABCD<<24& 0xFF000000);
	
	return result;
}
float MAXReal(float AA, float BB)
{
	REAL CC = AA>BB?AA:BB;
	return CC;
}
void TON_10ms(TON_10ms_typ *inst){
		
	if(inst->IN==1 && inst->ET < inst->PT ){
		inst->ET++;
	}
	if(inst->IN==0) inst->ET = 0;
	inst->Q = (inst->ET >= inst->PT && inst->IN) ;

}
