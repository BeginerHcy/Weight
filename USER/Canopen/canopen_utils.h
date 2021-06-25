/**
 * @brief CanOpen主站拓展工具
 * @anthor 谢龙华
 * @date 2020.05.12
 */
#ifndef __UTILS_CANOPEN_EXT_HEADER_INCLUDE__
#define __UTILS_CANOPEN_EXT_HEADER_INCLUDE__
#include <stdint.h>
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


/**
 * @brief 配置TPDO
 * @param id 站号
 * @param TPDO通道号[1-4]
 * @param map_obj 映射的对象值数组 
 * @param count 对象数量  
 * @return RET_SUCCESS or RET_FAILD
 */ 
bool  CanOpenConfigTPDO(uint8_t id,uint8_t ch,uint32_t* map_obj,uint8_t count);


/**
 * @brief 配置RPDO
 * @param id 站号
 * @param RPDO通道号[1-4]
 * @param map_obj 映射的对象值数组 
 * @param count 对象数量  
 * @return RET_SUCCESS or RET_FAILD
 */ 
bool  CanOpenConfigRPDO(uint8_t id,uint8_t ch,uint32_t* map_obj,uint8_t count);

#endif
