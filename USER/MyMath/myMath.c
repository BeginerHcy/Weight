#include "myMath.h"

BOOL XORD(unsigned char* pBuffer, unsigned short BufferSize)
{
	/*TODO: Add your code here*/
	return 1;
};
BOOL Rotat2D(struct Pos2D* pPos2D,double deltaX, double Yaw)
{
	pPos2D->yaw += Yaw;
	pPos2D->x 	= pPos2D->x + deltaX * cos(pPos2D->yaw);
	pPos2D->y 	= pPos2D->y + deltaX * sin(pPos2D->yaw);
	return 1;
};
double STDeviate(unsigned long pDataArray, unsigned char DataLen)
{
	int16_t data[1000],aveData,iData,iData2;
	int32_t DataSum=0,DataSum2=0;
	double  Result;
	
	mymemcpy(&(data[0]),(void*)pDataArray,DataLen*sizeof(int32_t));
	for (iData = 0; iData < DataLen; iData++)
	{
		DataSum = data[iData]+ DataSum;
	}
	aveData = DataSum/DataLen;
	for (iData2 = 0; iData2 < DataLen; iData2++)
	{
		DataSum2 = (data[iData2]-aveData)*(data[iData2]-aveData)+ DataSum2;
	}
	return aveData;
};
BOOL Transtlate2D(struct Pos2D* pPos2D, double X, double Y)
{
	pPos2D->x += X;
	pPos2D->y += Y;	
	return 1;
}
double MIN(double a,double b)
{
	double tempa=a;
	double tempb=b;
	if(tempa>tempb) return tempb;
	else return tempa;
}
void ArryMLO(double* buf,u32 bufByte)
{
	//u32 bufByte = sizeof(buf)/sizeof(buf[0]);
	u32 iBuf=0;
	while(iBuf+1<bufByte)
	{
		iBuf++;
		buf[bufByte-iBuf] = buf[bufByte-iBuf-1];
	}
};
double MoveAvgFilter(MoveStrutType * pFunData)
{
	double SumBuffer=0;
	u32 indexCal=0;
	double temN=pFunData->Ntime;//Byte
	u32 BufSizeByte=sizeof(pFunData->buffer)/sizeof(double);
	u32 BufLenMin=sizeof(pFunData->buffer)-sizeof(double);
	///////
	ArryMLO(&pFunData->buffer[0],BufSizeByte);
	///////
	pFunData->buffer[0] = pFunData->x;
	//////
	if(temN>=BufSizeByte) temN=BufSizeByte;
	
	if(temN==0) temN=1;
	//////
	pFunData->Nfilled++;
	//////
	if(pFunData->Nfilled>=temN)pFunData->Nfilled=temN;
	SumBuffer=0;
	/////
	for(indexCal=0;indexCal<pFunData->Nfilled;indexCal++)
	{
		 SumBuffer = SumBuffer + pFunData->buffer[indexCal];
	}
	/////
	pFunData->y =  SumBuffer/pFunData->Nfilled;
	return pFunData->y;
	
};
u8 XorCheckSum(u8* buf,u8 bufLen)
{
  unsigned int packet_size=bufLen;
  u8 cs=0;
	unsigned int i;
  for (i = 2; i < packet_size; i++)
  {
    cs ^= buf[i];
  }
  return cs;
}

float PolyVal4(float A[],float X)
{
	return A[0]+A[1]*X+A[2]*X*X+A[3]*X*X*X+A[4]*X*X*X*X;
}
float PolyVal6(float A[],float X)
{
	return A[0]+A[1]*X+A[2]*X*X+A[3]*X*X*X+A[4]*X*X*X*X+A[5]*X*X*X*X*X+A[6]*X*X*X*X*X*X;
}
float PolyVal7(float A[],float X)
{
	return A[0]+A[1]*X+A[2]*X*X+A[3]*X*X*X+A[4]*X*X*X*X+A[5]*X*X*X*X*X+A[6]*X*X*X*X*X*X+A[7]*X*X*X*X*X*X*X;
}

void polyfit(int n,float x[],float y[],int poly_n,float a[]) 
{ 
	int i,j; 
	float *tempx,*tempy,*sumxx,*sumxy,*ata;
	void gauss_solve(int n,float A[],float x[],float b[]);
	tempx=calloc(n,sizeof(float));
	sumxx=calloc(poly_n*2+1,sizeof(float));
	tempy=calloc(n,sizeof(float));
	sumxy=calloc(poly_n+1,sizeof(float));
	ata=calloc((poly_n+1)*(poly_n+1),sizeof(float));
	for (i=0;i<n;i++)
  {
		tempx[i]=1;
		tempy[i]=y[i];
  }
	for (i=0;i<2*poly_n+1;i++)
		for (sumxx[i]=0,j=0;j<n;j++)
		{
			sumxx[i]+=tempx[j];
			tempx[j]*=x[j];
		}
	for (i=0;i<poly_n+1;i++) 
		for (sumxy[i]=0,j=0;j<n;j++) 
		{ 
			sumxy[i]+=tempy[j]; 
			tempy[j]*=x[j]; 
		} 
	for (i=0;i<poly_n+1;i++) 
			 for (j=0;j<poly_n+1;j++) 
					ata[i*(poly_n+1)+j]=sumxx[i+j]; 
	gauss_solve(poly_n+1,ata,a,sumxy); 
	free(tempx); 
	free(sumxx); 
	free(tempy); 
	free(sumxy); 
	free(ata); 
} 
void gauss_solve(int n,float A[],float x[],float b[])
{
	int i,j,k,r;
	float max;
	for (k=0;k<n-1;k++)
	{
		max=fabs(A[k*n+k]);/*find maxmum*/
		r=k;
		for (i=k+1;i<n-1;i++)
		if (max<fabs(A[i*n+i]))
		{
			max=fabs(A[i*n+i]);
			r=i;
		}
		if (r!=k)
			for (i=0;i<n;i++)/*change array:A[k] & A[r]*/
			{
				max=A[k*n+i];
				A[k*n+i]=A[r*n+i];
				A[r*n+i]=max;
			} 
		max=b[k];/*change array:b[k]&b[r]*/
		b[k]=b[r];
		b[r]=max;
		for (i=k+1;i<n;i++)
		{
			for (j=k+1;j<n;j++)
				A[i*n+j]-=A[i*n+k]*A[k*n+j]/A[k*n+k];
			b[i]-=A[i*n+k]*b[k]/A[k*n+k];
		}
	}
	for (i=n-1;i>=0;x[i]/=A[i*n+i],i--)
			 for (j=i+1,x[i]=b[i];j<n;j++)
	x[i]-=A[i*n+j]*x[j]; 
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