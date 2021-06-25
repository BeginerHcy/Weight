#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "string.h"
#include "stm32f4xx_spi.h"


void ReadDAC7760(void);
void EnableDaisyChain(uint8_t * OutputRage);
void SetDAC7760(uint16_t * ui16DACOut);
