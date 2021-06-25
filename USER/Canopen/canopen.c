
#include "mInclude.h"

//dsp
#include "stm32f4xx_can.h"

/**************************
说明:
为简单，这里最多只管理4个从站
***************************/


/************************************************
define
************************************************/
#define MAX_CANOPEN_SLAVE 4
#define RSDO_ID_BASE 0x580  //SDO回复的基本地址
#define NMT_ERR_CTRL 0x700  //NMT Error Control object

/************************************************
static
************************************************/
struct SDO{
	uint8_t cmd;
	uint16_t index;
	int8_t   sub;
	int32_t value;
};

static  CanSend sender_ = 0;
static  PDOReciver pdo_reciver_ = 0;
static  NMTReciver nmt_reciver_ = 0;
static  struct SDO last_sdo[MAX_CANOPEN_SLAVE] = 
{
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0}
};	

//写SDO发送的功能码
static uint8_t write_sdo_cmd[4] = {0x2F,0x2B,0x27,0x23};
//读SDO接收的功能码
static uint8_t read_sdo_cmd[4] = {0x4F,0x4B,0x47,0x43};
//TPDO基本地址表
static uint32_t tpdo_base[4] = {0x180,0x280,0x380,0x480};
//RPDO基本地址表
static uint32_t rpdo_base[4] = {0x200,0x300,0x400,0x500};

//比较SDO,属于同一个SDO返回1
static uint8_t __compareSDO(uint8_t cmd_code,uint16_t index, uint8_t sub,struct SDO* sdo){
	 return (cmd_code==sdo->cmd) && (index == sdo->index) && (sub == sdo->sub);
}

//====================================È«¾ÖÄÚ´æ²Ù×÷º¯Êý===================================
/**
 * @brief  ÉèÖÃÁ¬ÐøÄÚ´æµÄÖµ
 * @param  data Êý¾ÝÖ¸Õë 
 * @param  val Êý¾ÝÖµ
 * @param  count ÐèÒªÉèÖÃµÄÄÚ´æ³ß´ç[µ¥Î»Byte]
 * @return void
 */   
void MM_memset(void *data,uint8_t val,uint32_t count){
   uint8_t *xs = data;  
   while (count--)*xs++ = val;
}	


/**
 * @brief  ¸´ÖÆÄÚ´æ
 * @param  des Ä¿±êÊý¾ÝÖ¸Õë 
 * @param  src Ô´Êý¾ÝÖ¸Õë
 * @param  n ÐèÒª¿½±´µÄÄÚ´æÊý¾Ý³ß´ç[µ¥Î»Byte]
 * @return void
 */  
void MM_memcpy(void *des,void *src,uint32_t n){
	//TODO ´ËÖÖÐ´·¨ÆäÊµÊÇÓÐÎÊÌâµÄ.....
    uint8_t *xdes=des;
	uint8_t *xsrc=src; 
    while(n--)*xdes++=*xsrc++; 
}

//CAN接收
static void __CanDataRecv(eRobotEventID evt,void*arg){
	uint8_t id = 0;
	uint8_t i = 0;
	CanRxMsg* RxMessage = (CanRxMsg*)arg;
	
	uint32_t cob_fun= RxMessage->StdId>>0 & 0xFFFFFF80;
	uint32_t cob_id= RxMessage->StdId>>0  & 0x7F;
	//	uint32_t base_id = (RxMessage->StdId) & 0xFFF0;
	//SDO回复的报文
 	if(cob_fun == RSDO_ID_BASE ){
		id = cob_id; //从站地址
		if(id>MAX_CANOPEN_SLAVE) return;
		last_sdo[id-1].cmd = RxMessage->Data[0]; //功能码
		last_sdo[id-1].index = ( ((uint16_t)RxMessage->Data[2]) << 8 ) | (RxMessage->Data[1]); //索引
		last_sdo[id-1].sub = RxMessage->Data[3]; //子索引	
		last_sdo[id-1].value = (*(int32_t*)(&RxMessage->Data[4]));
		return;
	}
	else if(cob_fun==NMT_ERR_CTRL){//NMT Error报文
		id = cob_id; //从站地址
		if(id>MAX_CANOPEN_SLAVE) return;
		
		if(nmt_reciver_){
			nmt_reciver_(id,RxMessage->Data);
		}	
	}else{//PDO报文
		for(i = 0 ; i < 4 ; i++){
			if(cob_fun == tpdo_base[i]){
				id = cob_id; //从站地址
				if(id>MAX_CANOPEN_SLAVE) break;			
				if(pdo_reciver_){
					pdo_reciver_(id,(PDO_CH)i,RxMessage->Data,RxMessage->DLC);
				}	
				break;
			}
		}	
	}
}

 
/**
 * @brief Canopen 初始化
 * @return void
 */ 
void CanOpen_init(CanSend sender,PDOReciver pdo_reciver){
	 sender_ = sender;
	 pdo_reciver_ = pdo_reciver;
	
   //注册驱动器回传事件
   RobotRegisterEventHandler(ROBOT_EVENT_CAN1,__CanDataRecv);	
}

/**
 * @brief Canopen NMT服务
 * @param cmd 命令
 * @param slave id
 * @return void
 */ 
void CanOpen_nmt(e_nodeCmd cmd,uint8_t id){
	uint8_t data[2] = {0x00,0x00};
	data[0] = cmd;
	data[1] = id;
	if(sender_){
		sender_(0,data,sizeof(data));
	}
}

/**
 * @brief Canopen NMT服务
 * @param cmd 命令
 * @param slave id
 * @return void
 */ 
void CanOpen_syn(){
	uint8_t data[2] = {0x00,0x00};
	data[0] = 00;
	data[1] = 00;
	if(sender_){
		sender_(0x80,data,0);
	}
}


void CanOpen_hrtbeat(e_nodeState state,uint8_t id){
	uint8_t data[1] = {0x00};
	data[0] = state;
	if(sender_){
		sender_(0x700+id,data,sizeof(data));
	}
}

/**
 * @brief Canopen 设置工作模式
 * @param id 站号
 * @param mode 模式定义
 * @return RET_SUCCESS or RET_FAILD
 */ 
bool CanOpen_setMode(uint8_t id,uint8_t mode){
	return CanOpen_writeSDO(id,0x6060,0x00,&mode,1);
}


/**
 * @brief Canopen 使能操作
 * @param id 站号
 * @return RET_SUCCESS or RET_FAILD
 */ 
bool CanOpen_enable(uint8_t id){
	uint16_t data1 = 0x0006;
	uint16_t data2 = 0x0007;
	uint16_t data3 = 0x000F;	
	return CanOpen_writeSDO(id,0x6040,0x00,(uint8_t*)&data1,2) && 
				 CanOpen_writeSDO(id,0x6040,0x00,(uint8_t*)&data2,2) &&
				 CanOpen_writeSDO(id,0x6040,0x00,(uint8_t*)&data3,2);
}

/**
 * @brief Canopen 失能操作
 * @param id 站号
 * @return RET_SUCCESS or RET_FAILD
 */ 
bool CanOpen_disable(uint8_t id){
	uint16_t data = 0x0007;
	return CanOpen_writeSDO(id,0x6040,0x00,(uint8_t*)&data,2);
}

/**
 * @brief Canopen 写SDO
 * @param id 站号
 * @param index 索引
 * @param sub 子索引 
 * @param value 值  
 * @param size 尺寸   
 * @return RET_SUCCESS or RET_FAILD
 */ 
bool CanOpen_writeSDO(uint8_t id,uint16_t index,uint8_t sub,uint8_t *value,uint8_t size){
	uint8_t i = 0;
	uint8_t data[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	struct SDO *recv_sdo = 0;
	if(size<=0 || size>4) THROW_EXCEPTION;
	if(id==0  || id>MAX_CANOPEN_SLAVE) THROW_EXCEPTION;
	recv_sdo = last_sdo +id - 1;
	
	//命令字
	data[0] = write_sdo_cmd[size - 1 ];
	data[1] = index &
	0xFF;
	data[2] = (index>>8) & 0xFF;	
	data[3] = sub;
	//拷贝数据
	MM_memcpy(data+4,value,size);
	
	if(sender_){
		MM_memset(recv_sdo,0,sizeof(struct SDO));
		//最多10帧&等待返回
		for(i = 0 ; (i < 10) && (!__compareSDO(0x60,index,sub,recv_sdo));i++){
			if(recv_sdo->cmd == 0x80) return RET_FAILD; //从站反馈异常
			sender_(0x600 + id,data,sizeof(data));
			delay_ms(2);
		}
		//收到返回
		if(__compareSDO(0x60,index,sub,recv_sdo)){
			return RET_SUCCESS;
		}
	}	
	return RET_FAILD;	
}

/**
 * @brief Canopen 写PDO1
 * @param id 站号
 * @param pdo_ch pdo通道 
 * @param data 数据地址
 * @param count 数据长度,必须<=8
 */ 
void CanOpen_writePDO(uint8_t id,PDO_CH pdo_ch,uint32_t *data,uint8_t count){
	uint8_t dataArry[4];
	if(count>8) THROW_EXCEPTION;
	if(id==0 || id>MAX_CANOPEN_SLAVE) THROW_EXCEPTION;
	
	dataArry[0] = (uint8_t)(*data>>0 & 0x000000FF);
	dataArry[1] = (uint8_t)(*data>>8 & 0x000000FF);
	dataArry[2] = (uint8_t)(*data>>16 & 0x000000FF);
	dataArry[3] = (uint8_t)(*data>>24 & 0x000000FF);
	
	if(sender_){
		sender_(rpdo_base[pdo_ch-1] + id,dataArry,count);
	}		
}

/**
 * @brief Canopen 读SDO
 * @param id 站号
 * @param index 索引
 * @param sub 子索引 
 * @param value 返回的数据存储区 
 * @param size 需要读取的尺寸  
 * @return RET_SUCCESS or RET_FAILD
 */ 
bool CanOpen_readSDO(uint8_t id,uint16_t index,uint8_t sub,uint8_t* value,uint8_t size){
	uint8_t i = 0;
	uint8_t data[8] = {0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	struct SDO *recv_sdo = 0;	
	if(size<=0 || size>4) THROW_EXCEPTION;
	if(id==0 || id>MAX_CANOPEN_SLAVE) THROW_EXCEPTION;	
	recv_sdo = last_sdo + id - 1;	
	
	data[1] = index & 0xFF;
	data[2] = (index>>8) & 0xFF;	
	data[3] = sub;
	//拷贝数据
	MM_memcpy(data+4,value,size);
	
	if(sender_){
		MM_memset(recv_sdo,0,sizeof(struct SDO));
		//最多10帧&等待返回
		for(i = 0 ; (i < 10) && (!__compareSDO(read_sdo_cmd[size-1],index,sub,recv_sdo));i++){
			if(recv_sdo->cmd == 0x80) return RET_FAILD; //从站反馈异常
			sender_(0x600 + id,data,sizeof(data));
			delay_ms(2); 
		}
		//收到返回
		if(__compareSDO(read_sdo_cmd[size-1],index,sub,recv_sdo)){
			//提取返回值
			MM_memcpy(value,&last_sdo[id].value,size);
			return RET_SUCCESS;
		}
	}	
	return RET_FAILD;		
}
