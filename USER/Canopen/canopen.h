/**
 * @brief CanOpen主站协议库[精简库]
 * @anthor 谢龙华
 * @date 2020.04.24
 */
 #include "CanMaster.h"
#ifndef __UTILS_CANOPEN_HEADER_INCLUDE__
#define __UTILS_CANOPEN_HEADER_INCLUDE__
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



#define RET_SUCCESS 1
#define RET_FAILD  0
#define  MAX_MOTOR_COUNT 		4 //支持的最多的电机数目

//=============DEBUG========================== 
#define THROW_EXCEPTION  while(1);
#endif

//PDO通道
typedef enum{PDO1,PDO2,PDO3,PDO4} PDO_CH;

//发送函数
typedef void (*CanSend)(uint32_t stdid,uint8_t* data,uint8_t len);

/**
 * @brief PDO接收函数
 * @param id 站号
 * @param pdo_ch PDO通道
 * @param data 数据 
 * @param len 数据长度
 * @return void
 */ 
typedef void (*PDOReciver)(uint8_t id,PDO_CH pdo_ch,uint8_t* data,uint8_t len);
typedef void (*NMTReciver)(uint8_t id,uint8_t* data);
/**
 * @brief Canopen 初始化
 * @return void
 */ 
void CanOpen_init(CanSend sender,PDOReciver pdo_reciver);

/**
 * @brief Canopen NMT服务
 * @param cmd 命令
 * @param slave id
 * @return void
 */ 
void CanOpen_nmt(e_nodeCmd cmd,uint8_t id);
void CanOpen_hrtbeat(e_nodeState state,uint8_t id);

/**
 * @brief Canopen 设置工作模式
 * @param id 站号
 * @param mode 模式定义
 * @return RET_SUCCESS or RET_FAILD
 */ 
bool CanOpen_setMode(uint8_t id,uint8_t mode);


/**
 * @brief Canopen 使能操作
 * @param id 站号
 * @return RET_SUCCESS or RET_FAILD
 */ 
bool CanOpen_enable(uint8_t id);

/**
 * @brief Canopen 失能操作
 * @param id 站号
 * @return RET_SUCCESS or RET_FAILD
 */ 
bool CanOpen_disable(uint8_t id);

/**
 * @brief Canopen 写SDO
 * @param id 站号
 * @param index 索引
 * @param sub 子索引 
 * @param value 值  
 * @param size 尺寸   
 * @return RET_SUCCESS or RET_FAILD
 */ 
bool CanOpen_writeSDO(uint8_t id,uint16_t index,uint8_t sub,uint8_t *value,uint8_t size);

/**
 * @brief Canopen 写PDO1
 * @param id 站号
 * @param pdo_ch pdo通道 
 * @param data 数据地址
 * @param count 数据长度,必须<=8
 */ 
void CanOpen_writePDO(uint8_t id,PDO_CH pdo_ch,uint32_t *data,uint8_t count);

/**
 * @brief Canopen 读SDO
 * @param id 站号
 * @param index 索引
 * @param sub 子索引 
 * @param value 返回的数据存储区 
 * @param size 需要读取的尺寸  
 * @return RET_SUCCESS or RET_FAILD
 */ 
bool CanOpen_readSDO(uint8_t id,uint16_t index,uint8_t sub,uint8_t* value,uint8_t size);

//====================================È«¾ÖÄÚ´æ²Ù×÷º¯Êý===================================
/**
 * @brief  ÉèÖÃÁ¬ÐøÄÚ´æµÄÖµ
 * @param  data Êý¾ÝÖ¸Õë 
 * @param  val Êý¾ÝÖµ
 * @param  count ÐèÒªÉèÖÃµÄÄÚ´æ³ß´ç[µ¥Î»Byte]
 * @return void
 */   
void MM_memset(void *data,uint8_t val,uint32_t count);	


/**
 * @brief  ¸´ÖÆÄÚ´æ
 * @param  des Ä¿±êÊý¾ÝÖ¸Õë 
 * @param  src Ô´Êý¾ÝÖ¸Õë
 * @param  n ÐèÒª¿½±´µÄÄÚ´æÊý¾Ý³ß´ç[µ¥Î»Byte]
 * @return void
 */  
void MM_memcpy(void *des,void *src,uint32_t n);   
void CanOpen_syn();

