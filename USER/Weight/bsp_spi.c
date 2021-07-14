
#include "bsp_spi.h"


uint8_t SPI1_ReadByte(void){//SPI×ÜÏß¶ÁÒ»¸ö×Ö½Ú 
	uint8_t retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:½ÓÊÜ»º´æ·Ç¿Õ±êÖ¾Î»
	{
		retry++;
	}	  						    
	return SPI_I2S_ReceiveData(SPI1); //·µ»ØÍ¨¹ýSPIx×î½ü½ÓÊÕµÄÊý¾Ý
}

void SPI1_WriteByte(uint8_t TxData){//SPI×ÜÏßÐ´Ò»¸ö×Ö½Ú 
	uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET) //¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:·¢ËÍ»º´æ¿Õ±êÖ¾Î»
	{
		retry++;
	}			  
	SPI_I2S_SendData(SPI1, TxData); //Í¨¹ýÍâÉèSPIx·¢ËÍÒ»¸öÊý¾Ý
}

uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{		
	uint8_t ret = 0x00;
	uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET) 			//¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:·¢ËÍ»º´æ¿Õ±êÖ¾Î»
	{;}			  
	SPI_I2S_SendData(SPI1, TxData); 																	//Í¨¹ýÍâÉèSPIx·¢ËÍÒ»¸öÊý¾Ý
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) 	//¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:½ÓÊÜ»º´æ·Ç¿Õ±êÖ¾Î»
	{;}	  						    
	ret = SPI_I2S_ReceiveData(SPI1); 																	//·µ»ØÍ¨¹ýSPIx×î½ü½ÓÊÕµÄÊý¾Ý		
	return ret;
}

//===============================================================
// NAME: void SpiRawWrite (uint8_t *pbuf, uint8_t length)
//
// BRIEF: Is used in SPI mode to write direct to the reader chip.
//
// INPUTS:
//	Parameters:
//		uint8_t		*pbuf		raw data
//		uint8_t		length		number of data bytes
//
// OUTPUTS:
//
// PROCESS:	[1] send raw data to reader chip
//
// CHANGE:
// DATE  		WHO	DETAIL
// 24Nov2010	RP	Original Code
// 07Dec2010	RP	integrated wait while busy loops
//===============================================================

void 
SpiRawWrite(uint8_t *pbuf, uint8_t length)
{	
	uint8_t temp=0;
	//SLAVE_SELECT_LOW; 						//Start SPI Mode
	while(length > 0)
	{	
		SPI1_ReadWriteByte(*pbuf);
		pbuf++;
		length--;
	}
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	//SLAVE_SELECT_HIGH; 						// Stop SPI Mode		
}

//===============================================================
// NAME: void SpiReadCont (uint8_t *pbuf, uint8_t length)
//
// BRIEF: Is used in SPI mode to read a specified number of
// reader chip registers from a specified address upwards.
//
// INPUTS:
//	Parameters:
//		uint8_t		*pbuf		address of first register
//		uint8_t		length		number of registers
//
// OUTPUTS:
//
// PROCESS:	[1] read registers
//			[2] write contents to *pbuf
//
// CHANGE:
// DATE  		WHO	DETAIL
// 24Nov2010	RP	Original Code
// 07Dec2010	RP	integrated wait while busy loops
//===============================================================

void
SpiReadCont(uint8_t *pbuf, uint8_t length)
{	
	//uint8_t	j = 0;
	uint8_t temp=0;
	//SLAVE_SELECT_LOW; 							//Start SPI Mode
	// Address/Command Word Bit Distribution
	//*pbuf = (0x60 | *pbuf); 					// address, read, continuous
	//*pbuf = (0x7f &*pbuf);						// register address
	//SPI1_WriteByte(*pbuf);
	while(length > 0)
	{	
		*pbuf=SPI1_ReadWriteByte(0x00);
		pbuf++;
		length--;
	}
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	//SLAVE_SELECT_HIGH; 						// Stop SPI Mode

}

//===============================================================
// NAME: void SpiReadSingle (uint8_t *pbuf, uint8_t number)
//
// BRIEF: Is used in SPI mode to read specified reader chip
// registers.
//
// INPUTS:
//	Parameters:
//		uint8_t		*pbuf		addresses of the registers
//		uint8_t		number		number of the registers
//
// OUTPUTS:
//
// PROCESS:	[1] read registers
//			[2] write contents to *pbuf
//
// CHANGE:
// DATE  		WHO	DETAIL
// 24Nov2010	RP	Original Code
// 07Dec2010	RP	integrated wait while busy loops
//===============================================================

void
SpiReadSingle(uint8_t *pbuf, uint8_t number)
{			
	uint8_t temp=0;
//	SLAVE_SELECT_LOW; 						// Start SPI Mode
	while(number > 0)
	{	
		// Address/Command Word Bit Distribution
	//	*pbuf = (0x40 | *pbuf); 			// address, read, single
	//	*pbuf = (0x5f & *pbuf);				// register address

		temp=SPI1_ReadWriteByte(*pbuf);
		*pbuf=SPI1_ReadWriteByte(0x00);
		pbuf++;
		number--;
	}

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
//	SLAVE_SELECT_HIGH; 						// Stop SPI Mode
}

//===============================================================
// Settings for SPI Mode                                        ;
// 02DEC2010	RP	Original Code
//===============================================================

//===============================================================
// NAME: void SpiUsciExtClkSet (void)
//
// BRIEF: Is used to switch SPI data clock from DCO to more
// stabile extern clock
//
// INPUTS:
//
// OUTPUTS:
//
// PROCESS:	[1] switch SPI data clock
//
// CHANGE:
// DATE  		WHO	DETAIL
// 24Nov2010	RP	Original Code
// 07Dec2010	RP	changed SPI clock from 6.78MHz to 1.70MHz
//===============================================================

//===============================================================
// NAME: void SpiUsciSet (void)
//
// BRIEF: Is used to set USCI B0 for SPI communication
//
// INPUTS:
//
// OUTPUTS:
//
// PROCESS:	[1] make settings
//
// CHANGE:
// DATE  		WHO	DETAIL
// 24Nov2010	RP	Original Code
// 07Dec2010	RP	reduced SPI clock frequency
//===============================================================

//===============================================================
// NAME: void SpiWriteCont (uint8_t *pbuf, uint8_t length)
//
// BRIEF: Is used in SPI mode to write to a specific number of
// reader chip registers from a specific address upwards.
//
// INPUTS:
//	uint8_t	*pbuf	address of first register followed by the
//					contents to write
//	uint8_t	length	number of registers + 1
//
// OUTPUTS:
//
// PROCESS:	[1] write to the registers
//
// CHANGE:
// DATE  		WHO	DETAIL
// 24Nov2010	RP	Original Code
// 07Dec2010	RP	integrated wait while busy loops
//===============================================================

void
SpiWriteCont(uint8_t *pbuf, uint8_t length)
{	
	uint8_t temp=0;
	//SLAVE_SELECT_LOW; 						// Start SPI Mode
	// Address/Command Wort Bit Distribution
	//*pbuf = (0x20 | *pbuf); 				// address, write, continuous
	//*pbuf = (0x3f &*pbuf);					// register address
	while(length > 0)
	{	
		temp = SPI1_ReadWriteByte(*pbuf);
		pbuf++;
		length--;
	}
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	//SLAVE_SELECT_HIGH; 						// Stop SPI Mode
}

//===============================================================
// NAME: void SpiWriteSingle (uint8_t *pbuf, uint8_t length)
//
// BRIEF: Is used in SPI mode to write to a specified reader chip
// registers.
//
// INPUTS:
//	uint8_t	*pbuf	addresses of the registers followed by the
//					contends to write
//	uint8_t	length	number of registers * 2
//
// OUTPUTS:
//
// PROCESS:	[1] write to the registers
//
// CHANGE:
// DATE  		WHO	DETAIL
// 24Nov2010	RP	Original Code
// 07Dec2010	RP	integrated wait while busy loops
//===============================================================

void
SpiWriteSingle(uint8_t *pbuf, uint8_t length)
{
	uint8_t	i = 0;
	uint8_t temp=0;	
	//SLAVE_SELECT_LOW; 						// Start SPI Mode

	while(length > 0)
	{	
		*pbuf = (0x1f &*pbuf);				// register address
		for(i = 0; i < 2; i++)
		{	
			temp = SPI1_ReadWriteByte(*pbuf);
			pbuf++;
			length--;
		}
	}
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	//SLAVE_SELECT_HIGH; 						// Stop SPI Mode
}
