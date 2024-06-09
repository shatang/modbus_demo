#ifndef __TICK_TASK_H
#define __TICK_TASK_H
#include <stdint.h>
#include <stdbool.h>


typedef void timTaskCall(void);//�ص��������Ͷ���
/*******************************************************************************
��ע�ͣ��ṹ�嶨������
*******************************************************************************/
typedef struct{
	bool vbTask_Exist;	     	//�����Ƿ����
	uint32_t vu32Time_Out;		//�����趨ʱ��
	volatile uint32_t vu32Time_Count;		//�����ʱ����
	timTaskCall *pCallbackFunction;	        //�����趨ʱ�䵽ִ�иûص�����
}struct_TaskData_t;
/*******************************************************************************
��ע�ͣ���ʱ������ӿ�
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


extern bool timerTask_add(enum_TimTask_t fe_Name, uint32_t fu32_Timeout, timTaskCall* pf_CallbackFunction); //��Ӷ�ʱ������
extern bool timerTask_clearTask(enum_TimTask_t fe_Name);    											    //�����ʱ������
extern bool timerTask_clearCount(enum_TimTask_t fe_Name);
extern uint32_t sysTimeGet(void);
extern void sysTim_Init(void);


#endif
