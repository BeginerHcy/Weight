#ifndef __CAN_MASTER_HEADER_INCLUDE__
#define __CAN_MASTER_HEADER_INCLUDE__
#include <stdint.h>
/* stdbool.h standard header */
#ifndef _STDBOOL
#define _STDBOOL

#define __bool_true_false_are_defined        1

#ifndef __cplusplus
                /* TYPES */

#if 199901L <= __STDC_VERSION__


#else /* 199901L <= __STDC_VERSION__ */
#if __TI_STRICT_ANSI_MODE__
typedef unsigned char _Bool;
#endif
#endif /* 199901L <= __STDC_VERSION__ */

                /* MACROS */
#define bool        _Bool
#define false        0
#define true        1
#endif /* __cplusplus */


#endif /* _STDBOOL */

// Needed defines by Canfestival lib
#define MAX_CAN_BUS_ID 2
#define SDO_MAX_LENGTH_TRANSFER 32
#define SDO_MAX_SIMULTANEOUS_TRANSFERS 1
#define NMT_MAX_NODE_ID 128
#define SDO_TIMEOUT_MS 0XFFFF
#define MAX_NB_TIMER 8
#define SDO_BLOCK_SIZE 16

/*  Function Codes 
   ---------------
  defined in the canopen DS301 
*/
#define NMT	   		 0x0
#define SYNC       0x1
#define TIME_STAMP 0x2
#define PDO1tx     0x3
#define PDO1rx     0x4
#define PDO2tx     0x5
#define PDO2rx     0x6
#define PDO3tx     0x7
#define PDO3rx     0x8
#define PDO4tx     0x9
#define PDO4rx     0xA
#define SDOtx      0xB
#define SDOrx      0xC
#define NODE_GUARD 0xE
#define LSS 	   0xF


/* constantes used in the different state machines */
/* ----------------------------------------------- */
/* Must not be modified */
#define state1  0x01
#define state2  0x02
#define state3  0x03
#define state4  0x04
#define state5  0x05
#define state6  0x06
#define state7  0x07
#define state8  0x08
#define state9  0x09
#define state10 0x0A
#define state11 0x0B


#define CAN_SLAVE_NUM 2
/* The nodes states 
 * -----------------
 * values are choosen so, that they can be sent directly
 * for heartbeat messages...
 * Must be coded on 7 bits only
 * */
/* Should not be modified */
enum enum_nodeState {
  Initialisation  = 0x00, 
  Disconnected    = 0x01,
  Connecting      = 0x02,
  Preparing       = 0x02,
  Stopped         = 0x04,
  Operational     = 0x05,
  Pre_operational = 0x7F,//127
  Unknown_state   = 0x0F
};

enum enum_nmtcmd {
	StartNode = 0x01,
	StopNode = 0x02,
	Pre_oper = 0x80,//128
	ResetNode = 0x81,
	ResetCommunicate = 0x82,
	
};

typedef enum enum_nodeState e_nodeState;
typedef enum enum_nmtcmd e_nodeCmd;


/////////////////////////////////////////////////////////
typedef struct pdo_pv_type{
	bool mouduleOK;
	uint32_t rpdo[4];
	uint32_t tpdo[4];
}pdo_pv_type;
/////////////////////////////////////////////////////////
typedef struct can_master_type{
	
	uint8_t SlaveNumbers;
	uint16_t MstHeartBeatTimeInterval;
	uint16_t SlaveHeartBeatTimeInterval;
	uint16_t synTimeInterval;
	uint16_t lifeGuardTimeoutsetting;
	uint32_t nodeTimeOut[NMT_MAX_NODE_ID];
	uint8_t MstNodeID;
	uint8_t nodes[NMT_MAX_NODE_ID];
	e_nodeState NMTable[NMT_MAX_NODE_ID];
	bool networkInitialed[NMT_MAX_NODE_ID];
	bool parInitialed[NMT_MAX_NODE_ID];
	bool resetNeeded[NMT_MAX_NODE_ID];
	uint8_t pardwnstep[NMT_MAX_NODE_ID];
	e_nodeState MstNodeState;
	pdo_pv_type pdomapping[NMT_MAX_NODE_ID];
	bool msterReset;
	bool synNeeded;
	uint32_t tpdoCnt[NMT_MAX_NODE_ID];
	
}can_master_type;



void _canMasterBoot_up();
void _canMaster_Cyclic();
void _canMaster_PVMapping(int j);
extern can_master_type canMaster;

#endif

