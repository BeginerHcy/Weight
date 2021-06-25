#include "math.h"
#define Pos2D AGV_pos_TYPE
#define BOOL _Bool
typedef struct MoveStrutType
{
	double x;
	double Ntime;
	double buffer[200];
	u32    Nfilled;
	double y;
}MoveStrutType;

BOOL XORD(unsigned char* pBuffer, unsigned short BufferSize);
double AXDR610Scale(double DiVoltage);
double AI2Vol(signed short DiVoltage);
double VoltoAI(double Voltage);
double Rate2Angle(double cycT, double AngleRate, double pAngle);
double AXDR610TempScale(signed short DiVoltage);
BOOL Transtlate2D(struct Pos2D* pPos2D, double X, double Y);
BOOL Rotat2D(struct Pos2D* pPos2D, double deltaX, double Yaw);
double STDeviate(unsigned long pDataArray, unsigned char DataLen);
double MoveAvgFilter(MoveStrutType* pFunData);
double MIN(double a,double b);
u8 XorCheckSum(u8* buf,u8 bufLen);
void ArryMLO(double* buf,u32 bufByte);
uint16_t CRC_Verify(uint8_t *CRC_Buf,uint8_t Buf_Length);