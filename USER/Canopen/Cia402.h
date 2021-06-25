#include "BoxApp.h"

#ifndef __CANOPEN_CIA_402_HEADER_INCLUDE__
#define __CANOPEN_CIA_402_HEADER_INCLUDE__
/***************************************
ControlWord Commands Mask (IEC61800_184e)
***************************************/
#define CONTROLWORD_COMMAND_SHUTDOWN_MASK                    0x0087
#define CONTROLWORD_COMMAND_SWITCHON_MASK                    0x00C7
#define CONTROLWORD_COMMAND_SWITCHON_ENABLEOPERATION_MASK    0x008F
#define CONTROLWORD_COMMAND_DISABLEVOLTAGE_MASK                0x0082
#define CONTROLWORD_COMMAND_QUICKSTOP_MASK                    0x0086
#define CONTROLWORD_COMMAND_DISABLEOPERATION_MASK            0x008F
#define CONTROLWORD_COMMAND_ENABLEOPERATION_MASK            0x008F
#define CONTROLWORD_COMMAND_FAULTRESET_MASK                    0x0080

#define CONTROLWORD_COMMAND_MODE_MASK          0x70//
#define CONTROLWORD_COMMAND_HOME_MASK          0x10//
//Operation Mode Specific Commands bit
#define	CONTROLWORD_MOTION_HALT_TORQUE							0x0100
/***************************************
ControlWord Commands (IEC61800_184e)
***************************************/
#define CONTROLWORD_COMMAND_SHUTDOWN                        	0x0006
#define CONTROLWORD_COMMAND_SWITCHON                        	0x0007
#define CONTROLWORD_COMMAND_SWITCHON_ENABLEOPERATION        	0x000F
#define CONTROLWORD_COMMAND_DISABLEVOLTAGE                    0x0000
#define CONTROLWORD_COMMAND_QUICKSTOP                        	0x0002
#define CONTROLWORD_COMMAND_DISABLEOPERATION                	0x0007
#define CONTROLWORD_COMMAND_ENABLEOPERATION                   0x000F
#define CONTROLWORD_COMMAND_FAULTRESET                        0x0080
#define CONTROLWORD_COMMAND_SYNPOS                        		0x0003F
#define CONTROLWORD_COMMAND_IPPOS                        			0x0001F
#define CONTROLWORD_COMMAND_SYNPOS_DLL                        0x0002F

/***************************************
StatusWord Masks and Flags
***************************************/
#define STATUSWORD_STATE_MASK                                	0x006F
#define STATUSWORD_STATE_MASK_No6															0x004F
#define STATUSWORD_VOLTAGE_ENABLED                            0x0010    //indicate high voltage enabled
#define STATUSWORD_WARNING                                    0x0080
#define STATUSWORD_MANUFACTORSPECIFIC                         0x0100
#define STATUSWORD_INTERNAL_LIMIT                            	0x0800
#define STATUSWORD_REMOTE                                    	0x0200    //set if the control word is processed
#define STATUSWORD_TARGET_REACHED                            	0x0400
#define STATUSWORD_INTERNALLIMITACTIVE                        0x0800
#define STATUSWORD_DRIVE_FOLLOWS_COMMAND                    	0x1000    //used in cyclic synchronous modes

#define STATUSWORD_HOME_ATTAINED_MASK                     		0x1000 //bit 12
//#define STATUSWORD_HOME_ATTAINED_MASK
/***************************************
StatusWord
***************************************/
#define STATUSWORD_STATE_NOTREADYTOSWITCHON                   0x0000
#define STATUSWORD_STATE_SWITCHEDONDISABLED                 	0x0040//????
#define STATUSWORD_STATE_READYTOSWITCHON                     	0x0021
#define STATUSWORD_STATE_SWITCHEDON                           0x0023
#define STATUSWORD_STATE_OPERATIONENABLED                     0x0027
#define STATUSWORD_STATE_QUICKSTOPACTIVE                    	0x0007
#define STATUSWORD_STATE_FAULTREACTIONACTIVE                	0x000F
#define STATUSWORD_STATE_FAULT                                0x0008 

/***************************************
Modes
***************************************/
// aus IEC61800-7-200_22G_184e_FDIS_ETG.pdf
/*
0 No mode change/no mode assigned
+1 Profile position mode
+2 Velocity mode
+3 Profile velocity mode
+4 Torque profile mode
+5 reserved
+6 Homing mode
+7 Interpolated position mode
+8 Cyclic sync position mode
+9 Cyclic sync velocity mode
+10 Cyclic sync torque mode
*/
#define MODE_NOMODE                            0x0000
#define MODE_PROFILEPOSITION                0x0001
#define MODE_VELOCITY                        0x0002
#define MODE_PROFILEVELOCITY                0x0003
#define MODE_TORQUEPROFILE                    0x0004
#define MODE_RESERVED                        0x0005
#define MODE_HOMING                            0x0006
#ifdef HEXIN_TYPE
#define MODE_INTERPOLATIONPOSITION            0x0008
#endif
#ifdef INOVA_TYPE
#define MODE_INTERPOLATIONPOSITION            0x0007
#endif
#define MODE_CYCLICSYNCPOSITION                0x0001
#define MODE_CYCLICSYNCVELOCITY                0x0009
#define MODE_CYCLICSYNCTORQUE                0x000A
/***************************************
 Homing
 ***************************************/
#define HOME_DIRECT 35
/***************************************
 CiA402 Statemachine
 ***************************************/
#define STATE_NOT_READY_TO_SWITCH_ON         0x0001 //O
#define STATE_SWITCH_ON_DISABLED            0x0002 //O
#define STATE_READY_TO_SWITCH_ON            0x0004 //M
#define STATE_SWITCHED_ON                    0x0008 //M
#define STATE_OPERATION_ENABLED                0x0010 //M
#define STATE_QUICK_STOP_ACTIVE                0x0020 //O
#define STATE_FAULT_REACTION_ACTIVE            0x0040 //M
#define STATE_FAULT                            0x0080 //M


/***************************************
 CiA402 Modes of Operation (object 0x6060) (IEC61800_184e)
 ***************************************/
// -128 to -1 Manufacturer-specific operation modes
#define NO_MODE                    0
#define PROFILE_POSITION_MODE    1
#define VELOCITY_MODE            2
#define PROFILE_VELOCITY_MOCE    3
#define PROFILE_TORQUE_MODE        4
//5 reserved
#define HOMING_MODE                6
#define INTERPOLATION_POSITION_MODE    7
#define CYCLIC_SYNC_POSITION_MODE    8
#define CYCLIC_SYNC_VELOCITY_MODE    9
#define CYCLIC_SYNC_TORQUE_MODE        10
//+11 to +127 reserved

/***************************************
 Others
 ***************************************/
#define NO_ERROR 0

#endif
