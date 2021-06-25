/**
 * @brief 机器人事件管理模块,用于机器人内部状态的事件通知
 * @anthor 谢龙华
 * @date 2019.5.11
 */
#ifndef __MAG_ROBOT_EVENT_HEADER_INCLUDE__
#define __MAG_ROBOT_EVENT_HEADER_INCLUDE__
#include <stdint.h>
#include "config.h"

 
//========================事件相关定义===========================
#define ROBOT_EVENT_FUNC_HANDLERS_MAX  10  //每个事件最多关联10个回调函数
/**
 * @brief  事件类型定义
 */
typedef enum{
    ROBOT_EVENT_CHANGE_POSITION,  //机器人位置改变
    ROBOT_EVENT_CHANGE_TARGET,	 //机器人导航目标改变	
    ROBOT_EVENT_CHANGE_SYS_STATE,	//机器人状态改变	
    ROBOT_EVENT_CHANGE_SYS_MODE,	//机器人控制模式改变	
	ROBOT_EVENT_MOTION_STATE_CHANGE, //运动状态改变
	ROBOT_EVENT_OBSTACLE_STATE_CHANGE, //障碍物状态改变	
	ROBOT_EVENT_TURN_MODE_CHANGE, 	  //拐弯状态改变		
	ROBOT_EVENT_BATTERY_ENERGY_CHANGE, 	  //电池电量改变	 	
	//地图相关事件
	ROBOT_EVENT_MAP_INITED,  	   //初始化完成事件	
	ROBOT_EVENT_MAP_NODE_ENTER,  //地图节点进入事件
	ROBOT_EVENT_MAP_EDGE_ENTER,  //地图边进入事件
	ROBOT_EVENT_MAP_PATH_SEARCH_EXCEPTION_NODE,  //地图路径搜索异常事件
	ROBOT_EVENT_MAP_NEXT_NODE_MISS_MATCH,  //地图导航下一个节点和规划不匹配事件	
 
	//导航事件
	ROBOT_EVENT_GOAL_REACHED,  //导航目标达到事件

	//硬件事件
	ROBOT_EVENT_START_BUTTON,	//启动按钮按下事件,参数无
	ROBOT_EVENT_STOP_BUTTON,	//停止按钮按下事件,参数无
	ROBOT_EVENT_RESET_BUTTON,	//复位按钮按下事件,参数无
	ROBOT_EVENT_ESTOP_BUTTON_PRESS,	//急停按钮按下事件,参数无	
	ROBOT_EVENT_ESTOP_BUTTON_RELEASE,	//急停按钮释放事件,参数无		
	ROBOT_EVENT_CAN1,  	 //CAN1数据(传感器反馈)
	ROBOT_EVENT_CAN2,    //CAN2数据(电机反馈)	
	ROBOT_EVENT_USART5,  //串口5数据进入事件(对应控制器的串口1)
	ROBOT_EVENT_USART6,  //串口6数据进入事件(对应控制器的串口2)	
	ROBOT_EVENT_MAX_NULL_ID
} eRobotEventID;

/**
 * @brief  事件回调函数定义
 * @param  eRobotEventID 事件ID
 * @param  arg 参数指针
 */
typedef   void( *exRobotFunctionHandler ) (eRobotEventID,void*arg);

/**
 * @brief  事件管理结构体定义
 */
typedef struct
{
    eRobotEventID    e_id;
    exRobotFunctionHandler eHandlers[ROBOT_EVENT_FUNC_HANDLERS_MAX];
} eRobotFunctionHandler;
 
 
//========================操作函数=========================== 
/**
 * @brief  事件系统初始化
 * @param  void
 * @return  void
 */
void RobotEventInit(void); 


/**
 * @brief  注册事件回调函数到系统中
 * @param  e_id 事件ID
 * @param  eHandler 回调函数指针
 */
void RobotRegisterEventHandler( eRobotEventID e_id, exRobotFunctionHandler eHandler ); 


/**
 * @brief  从系统中移除事件回调函数
 * @param  e_id 事件ID
 * @param  eHandler 回调函数指针
 */
void RobotRemoveEventHandler( eRobotEventID e_id, exRobotFunctionHandler eHandler ); 

/**
 * @brief  发送事件
 * @param  e_id 事件ID
 */
void RobotPostEvent(eRobotEventID e_id,void* arg); 

#endif
