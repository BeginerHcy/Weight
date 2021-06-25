#include "mInclude.h"


#define BSWAP_32(x) \
    (uint32_t)((((uint32_t)(x) & 0xff000000) >> 24) | \
                    (((uint32_t)(x) & 0x00ff0000) >> 8) | \
                    (((uint32_t)(x) & 0x0000ff00) << 8) | \
                    (((uint32_t)(x) & 0x000000ff) << 24) \
             ) 

/**
 * @brief 配置TPDO
 * @param id 站号
 * @param TPDO通道号[1-4]
 * @param map_obj 映射的对象值数组 
 * @param count 对象数量  
 * @return RET_SUCCESS or RET_FAILD
 */ 
bool  CanOpenConfigTPDO(uint8_t id,uint8_t ch,uint32_t* map_obj,uint8_t count){
/****
TPDO配置
2f 00 18 03 02 00 00 00   禁止时间设置
2f 00 1A 00 00 00 00 00   清空条目
23 00 1A 01 10 00 41 60   0x60410010
23 00 1A 02 08 00 61 60   0x60610008
23 00 1A 03 10 00 3F 60   0x603f0010
2f 00 1A 00 03 00 00 00   3个条目
23 10 10 00 65 76 61 73   保存数据
*/	
	uint8_t value[4] = {0x64,0x00,0x00,0x00};
	
	uint32_t tpdo_ba[4] = {0x180,0x280,0x380,0x480};
	
	uint32_t pdochCls = 0xC0000000 + tpdo_ba[ch-1] + id;
	uint32_t pdochEn = 0x40000000 + tpdo_ba[ch-1] + id;
	
	uint8_t	i = 0;
	BOOL ret = RET_SUCCESS;
	

	value[0] = (uint8_t)(pdochCls>>0 & 0x000000FF);
	value[1] = (uint8_t)(pdochCls>>8 & 0x000000FF);
	value[2] = (uint8_t)(pdochCls>>16 & 0x000000FF);
	value[3] = (uint8_t)(pdochCls>>24 & 0x000000FF);

	ret = ret && CanOpen_writeSDO(id,0x1800+ ch - 1,1,value,4); //disable cob-id
	
	value[0] = 1;
	ret = ret && CanOpen_writeSDO(id,0x1800+ ch - 1,2,value,1); //disable send type = 1

	value[0] = 0;
	ret = ret && CanOpen_writeSDO(id,0x1A00+ ch - 1,0,value,1); //clear mapping
	for(i = 0;i < count;i++){
		ret = ret && CanOpen_writeSDO(id,0x1A00+ ch - 1, i + 1 ,(uint8_t*)(map_obj + i),4); 
	}
	ret = ret && CanOpen_writeSDO(id,0x1A00+ ch - 1,0,&count,1); //mapping obj	
	
	
	value[0] = (uint8_t)(pdochEn>>0 & 0x000000FF);
	value[1] = (uint8_t)(pdochEn>>8 & 0x000000FF);
	value[2] = (uint8_t)(pdochEn>>16 & 0x000000FF);
	value[3] = (uint8_t)(pdochEn>>24 & 0x000000FF);
	ret = ret && CanOpen_writeSDO(id,0x1800+ ch - 1,1,value,4); //enable cob-id
	
	return ret;
}


/**
 * @brief 配置RPDO
 * @param id 站号
 * @param RPDO通道号[1-4]
 * @param map_obj 映射的对象值数组 
 * @param count 对象数量  
 * @return RET_SUCCESS or RET_FAILD
 */ 
bool  CanOpenConfigRPDO(uint8_t id,uint8_t ch,uint32_t* map_obj,uint8_t count){
	uint8_t i = 0;
	uint8_t value[4] = {0x64,0x00,0x00,0x00};
	uint32_t rpdo_ba[4] = {0x200,0x300,0x400,0x500};
	uint32_t pdochCls = 0x80000000 + rpdo_ba[ch-1] + id;
	uint32_t pdochEn = 0x00000000 + rpdo_ba[ch-1] + id;
	
	BOOL ret = RET_SUCCESS;	
	
	value[0] = (uint8_t)(pdochCls>>0 & 0x000000FF);
	value[1] = (uint8_t)(pdochCls>>8 & 0x000000FF);
	value[2] = (uint8_t)(pdochCls>>16 & 0x000000FF);
	value[3] = (uint8_t)(pdochCls>>24 & 0x000000FF);
	ret = ret && CanOpen_writeSDO(id,0x1400+ ch - 1,1,value,4); //disable cob-id
	value[0] = 1;
	ret = ret && CanOpen_writeSDO(id,0x1400+ ch - 1,2,value,1); //disable send type = 1
	value[0] = 0;
	ret = CanOpen_writeSDO(id,0x1600+ ch - 1,0,value,1); //有效条数清空
	//配置对象
	for(i = 0;i < count;i++){
		ret = ret &&CanOpen_writeSDO(id,0x1600+ ch - 1, i + 1 ,(uint8_t*)(map_obj + i),4); 
	}
	ret = ret && CanOpen_writeSDO(id,0x1600+ ch - 1,0,&count,1); //设置有效条数
	
	value[0] = (uint8_t)(pdochEn>>0 & 0x000000FF);
	value[1] = (uint8_t)(pdochEn>>8 & 0x000000FF);
	value[2] = (uint8_t)(pdochEn>>16 & 0x000000FF);
	value[3] = (uint8_t)(pdochEn>>24 & 0x000000FF);
	
	ret = ret && CanOpen_writeSDO(id,0x1400+ ch - 1,1,value,4); //disable cob-id
	
	return ret;
}
