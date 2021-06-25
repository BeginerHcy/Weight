/******************** (C) COPYRIGHT 2013 PNI Sensor Corp *****************
* File Name: ThreeD_magIC.c
* Sample Code for 3D MagIC ASIC
*************************************************************************/
#include "RM3100.h"
#include "math.h"
#include <stdio.h> 
#include <stdlib.h> 
#include "arm_math.h"
#include "24cxx.h"
#include "stmflash.h"
uint8_t Parameter1[800] = {0};
uint8_t ReadTemp1[800] = {0};
extern u8 dir;
extern u16 led0pwmval; 
bool flashLED[10];
uint32_t TimeStamp,oldTimeStamp,deltaStamp,OldStamp[10],TimeStamp500ms;

	

/*************************************************************************
* Function Name : DataReady()
* Description : Check DRDY pin, return true if high, otherwise false.
* Input : None
* Output : None
* Return : true or false
*************************************************************************/
