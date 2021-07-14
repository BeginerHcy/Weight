#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

#include "stm32f4xx_spi.h"
#include "HwCfg.h"
#include "delay.h"

uint8_t SPI1_ReadByte(void);
void SPI1_WriteByte(uint8_t TxData);
uint8_t SPI1_ReadWriteByte(uint8_t TxData);
void SpiRawWrite(uint8_t *pbuf, uint8_t length);
void SpiReadCont(uint8_t *pbuf, uint8_t length);
void SpiReadSingle(uint8_t *pbuf, uint8_t number);
void SpiWriteCont(uint8_t *pbuf, uint8_t length);
void SpiWriteSingle(uint8_t *pbuf, uint8_t length);
#endif

