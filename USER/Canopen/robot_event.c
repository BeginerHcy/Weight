
#include "mInclude.h"



/* 为所有事件开辟内存
 */
static eRobotFunctionHandler eFuncHandlers[ROBOT_EVENT_MAX_NULL_ID];
  
/**
 * @brief  事件系统初始化
 * @param  void
 * @return  void
 */
void RobotEventInit(void){
	MM_memset((void*)eFuncHandlers,0,sizeof(eFuncHandlers));
}

	
/**
 * @brief  注册事件回调函数到系统中
 * @param  e_id 事件ID
 * @param  eHandler 回调函数指针
 */
void RobotRegisterEventHandler( eRobotEventID e_id, exRobotFunctionHandler eHandler ){
	uint8_t i;
    if( ( 0 <= e_id ) && ( e_id < ROBOT_EVENT_MAX_NULL_ID ) )
    {
      //  ENTER_CRITICAL_SECTION(  );
	    exRobotFunctionHandler*  eHandlers = eFuncHandlers[e_id].eHandlers;
        if( eHandler != 0 )
        {			
            for( i = 0; i < ROBOT_EVENT_FUNC_HANDLERS_MAX; i++ )
            {
								if(eHandlers[i]==eHandler){ //已经存在不重复添加
									break;
								}
				
                if(eHandlers[i] == 0) //找出尾部添加之
                {
                    eHandlers[i] = eHandler;
                    break;
                }
            } 
        } 
	}
}

/**
 * @brief  从系统中移除事件回调函数
 * @param  e_id 事件ID
 * @param  eHandler 回调函数指针
 */
void RobotRemoveEventHandler( eRobotEventID e_id, exRobotFunctionHandler eHandler ){
	uint8_t i,j;
    if( ( 0 <= e_id ) && ( e_id < ROBOT_EVENT_MAX_NULL_ID ) )
    {
      //  ENTER_CRITICAL_SECTION(  );
				exRobotFunctionHandler*  eHandlers = eFuncHandlers[e_id].eHandlers;
        if( eHandler != 0 )
        {			
            for( i = 0; i < ROBOT_EVENT_FUNC_HANDLERS_MAX; i++ )
            {
								if(eHandlers[i]==eHandler){ //已找到该指针 
							 //清空当前指针,后面数据前移
							 for( j = i; j < ROBOT_EVENT_FUNC_HANDLERS_MAX-1; j++ )
							 {
								 eHandlers[j] = eHandlers[j+1];
							 }
							 
				}
 
            } 
        }
 
    }	
}

/**
 * @brief  发送事件
 * @param  e_id 事件ID
 */
void RobotPostEvent(eRobotEventID e_id,void* arg){
		uint8_t i;
    if( ( 0 <= e_id ) && ( e_id < ROBOT_EVENT_MAX_NULL_ID ) )
    {
      //  ENTER_CRITICAL_SECTION(  );
				exRobotFunctionHandler*  eHandlers = eFuncHandlers[e_id].eHandlers;
        for( i = 0; i < ROBOT_EVENT_FUNC_HANDLERS_MAX; i++ ){
					if(eHandlers[i]==0){  //结尾符
	 
						 break;
					}
					eHandlers[i](e_id,arg); //call
        }
 
    }		
}
