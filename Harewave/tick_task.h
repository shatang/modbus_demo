#ifndef __TICK_TASK_H
#define __TICK_TASK_H
#include <stdint.h>
#include <stdbool.h>


typedef void timTaskCall(void);//回调函数类型定义
/*******************************************************************************
块注释：结构体定义声明
*******************************************************************************/
typedef struct{
	bool vbTask_Exist;	     	//任务是否存在
	uint32_t vu32Time_Out;		//任务设定时间
	volatile uint32_t vu32Time_Count;		//任务计时缓存
	timTaskCall *pCallbackFunction;	        //任务设定时间到执行该回调函数
}struct_TaskData_t;
/*******************************************************************************
块注释：定时器任务接口
*******************************************************************************/
typedef enum
{
    No_TaskID = 0,
	//200us
    UART1_RECV_TASK,
    UART2_RECV_TASK,
    UART3_RECV_TASK,
	UART6_RECV_TASK,
	//1ms
//	LED_RUN_TASK,
    LED_COMM_TASK,
    
	//1s
    IWDG_FEED_TASK,
	DLT645_ERR_TASK,
    MODBUS_HOST_TASK,
	UART6_TEST_TASK,
    LED_RUN_TASK,
	CAN_CHECK_TASK,
    
    TIM_TASK_NUM,
}enum_TimTask_t;


extern bool timerTask_add(enum_TimTask_t fe_Name, uint32_t fu32_Timeout, timTaskCall* pf_CallbackFunction); //添加定时器任务
extern bool timerTask_clearTask(enum_TimTask_t fe_Name);    											    //清除定时器任务
extern bool timerTask_clearCount(enum_TimTask_t fe_Name);
extern uint32_t sysTimeGet(void);
extern void sysTim_Init(void);


#endif
