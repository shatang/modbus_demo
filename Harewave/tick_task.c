#include "tick_task.h"
#include "main.h"


static struct_TaskData_t timer_task[TIM_TASK_NUM];  	// ��ʱ��������¼�����
static volatile uint32_t vu32_timerTick = 0;        	// ϵͳʱ��(�����޸�)
//================================ ����ʱ������ =================================
/********************************************************************************
* ������ : timerTask_add
* ��  �� : ��ʱ���������
* ˵  �� : none
* ��  �� : fe_Name:����ID��
            timeout:��ʱʱ��
            pf_CallbackFunction:�ص�����ָ��
* ��  �� : none
* ��  �� : shatang                    ʱ  �� : 2020.10.20
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
bool timerTask_add(enum_TimTask_t fe_Name, uint32_t fu32_Timeout, timTaskCall* pf_CallbackFunction)
{
	if((fe_Name == No_TaskID)||(fu32_Timeout == 0))return false;//��������쳣
	timerTask_clearTask(fe_Name);//��ֹ�ظ���������������ǰ���������ͬһ���ֵ�����
    if(timer_task[fe_Name].vbTask_Exist == true)return false;///���ڸö�ʱ�����������
    
    timer_task[fe_Name].vbTask_Exist = true;
    timer_task[fe_Name].vu32Time_Out = fu32_Timeout;
    timer_task[fe_Name].vu32Time_Count = 0;
    timer_task[fe_Name].pCallbackFunction = pf_CallbackFunction;
    
    return true;
}
/********************************************************************************
* ������ : timerTask_clearTask
* ��  �� : ��ʱ���������
* ˵  �� : none
* ��  �� : fe_Name:����ID��
* ��  �� : none
* ��  �� : shatang                    ʱ  �� : 2020.10.20
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
bool timerTask_clearTask(enum_TimTask_t fe_Name)
{
	if(fe_Name == No_TaskID)return false;//�޸�����
    
    //�����������Ʋ�����Ӧ�������
    timer_task[fe_Name].vbTask_Exist = false;
    timer_task[fe_Name].vu32Time_Out = 0;
    timer_task[fe_Name].vu32Time_Count = 0;
    timer_task[fe_Name].pCallbackFunction = NULL;

	return true;
}
/********************************************************************************
* ������ : timerTask_clearCount
* ��  �� : ��ʱ���������
* ˵  �� : none
* ��  �� : fe_Name:����ID��
* ��  �� : none
* ��  �� : shatang                    ʱ  �� : 2020.10.20
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
bool timerTask_clearCount(enum_TimTask_t fe_Name)
{
	if(fe_Name == No_TaskID)return false;//�޸�����
    timer_task[fe_Name].vu32Time_Count = 0;

	return true;
}
/********************************************************************************
* ������ : timerTask_clearCount
* ��  �� : ��ʱ���������
* ˵  �� : none
* ��  �� : fe_Name:����ID��
* ��  �� : none
* ��  �� : shatang                    ʱ  �� : 2020.10.20
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
uint32_t sysTimeGet(void)
{
    return vu32_timerTick;
}



TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;

void sysTim_Init(void)
{
	//��ʱ��3,200us
    __HAL_RCC_TIM3_CLK_ENABLE();            //ʹ��TIM3ʱ��
    HAL_NVIC_SetPriority(TIM3_IRQn, 3, 3);  //�����ж����ȼ�����ռ���ȼ�3�������ȼ�3
    HAL_NVIC_EnableIRQ(TIM3_IRQn);          //����ITM3�ж�
	
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 9000-1;          //10khz
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 2-1;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_Base_Init(&htim3);
	
	__HAL_TIM_ENABLE(&htim3);                       //ʹ�ܶ�ʱ��3
    __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);     //ʹ�ܶ�ʱ��3�����жϣ�TIM_IT_UPDATE

	
	//��ʱ��4,1ms
    __HAL_RCC_TIM4_CLK_ENABLE();            //ʹ��TIM4ʱ��
    HAL_NVIC_SetPriority(TIM4_IRQn, 3, 3);  //�����ж����ȼ�����ռ���ȼ�3�������ȼ�3
    HAL_NVIC_EnableIRQ(TIM4_IRQn);          //����ITM4�ж�
	
	htim4.Instance = TIM4;
	htim4.Init.Prescaler = 9000-1;       	//10khz
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 10-1;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_Base_Init(&htim4);
	
	__HAL_TIM_ENABLE(&htim4);                       //ʹ�ܶ�ʱ��4
    __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);     //ʹ�ܶ�ʱ��4�����жϣ�TIM_IT_UPDATE
	
		
	//��ʱ��5,1s
    __HAL_RCC_TIM5_CLK_ENABLE();            //ʹ��TIM5ʱ��
    HAL_NVIC_SetPriority(TIM5_IRQn, 3, 3);  //�����ж����ȼ�����ռ���ȼ�3�������ȼ�3
    HAL_NVIC_EnableIRQ(TIM5_IRQn);          //����ITM5�ж�
	
	htim5.Instance = TIM5;
	htim5.Init.Prescaler = 9000-1;       	//10khz
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim5.Init.Period = 10000-1;
	htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_Base_Init(&htim5);
	
	__HAL_TIM_ENABLE(&htim5);                       //ʹ�ܶ�ʱ��5
    __HAL_TIM_ENABLE_IT(&htim5, TIM_IT_UPDATE);     //ʹ�ܶ�ʱ��5�����жϣ�TIM_IT_UPDATE
}

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
    if(__HAL_TIM_GET_FLAG(&htim3, TIM_FLAG_UPDATE))
    {
		for(uint8_t i=1; i<LED_COMM_TASK; i++) 	//�������
		{
			if(timer_task[i].vu32Time_Out != 0)	//����������
			{
				timer_task[i].vu32Time_Count++;	//��Ӧ����ʱ�����
				//�������趨��ʱ��㵽�ˣ�ִ�лص�����
				if(timer_task[i].vu32Time_Out <= timer_task[i].vu32Time_Count)
				{
					timer_task[i].vu32Time_Count = 0;           //��λ��ʱʱ��
					if(timer_task[i].pCallbackFunction != NULL) //�ص���������
						timer_task[i].pCallbackFunction();      //ִ�лص�����
				}
			}
		}
        __HAL_TIM_CLEAR_FLAG(&htim3, TIM_FLAG_UPDATE);
    }
}

//��ʱ��4�жϷ�����
void TIM4_IRQHandler(void)
{
    if(__HAL_TIM_GET_FLAG(&htim4, TIM_FLAG_UPDATE))
    {
		vu32_timerTick++;
		
		for(uint8_t i=LED_COMM_TASK; i<IWDG_FEED_TASK; i++) 	//�������
		{
			if(timer_task[i].vu32Time_Out != 0)	//����������
			{
				timer_task[i].vu32Time_Count++;	//��Ӧ����ʱ�����
				//�������趨��ʱ��㵽�ˣ�ִ�лص�����
				if(timer_task[i].vu32Time_Out <= timer_task[i].vu32Time_Count)
				{
					timer_task[i].vu32Time_Count = 0;           //��λ��ʱʱ��
					if(timer_task[i].pCallbackFunction != NULL) //�ص���������
						timer_task[i].pCallbackFunction();      //ִ�лص�����
				}
			}
		}
        __HAL_TIM_CLEAR_FLAG(&htim4, TIM_FLAG_UPDATE);
    }
}

//��ʱ��5�жϷ�����
void TIM5_IRQHandler(void)
{
    if(__HAL_TIM_GET_FLAG(&htim5, TIM_FLAG_UPDATE))
    {
		for(uint8_t i=IWDG_FEED_TASK; i<TIM_TASK_NUM; i++) 	//�������
		{
			if(timer_task[i].vu32Time_Out != 0)	//����������
			{
				timer_task[i].vu32Time_Count++;	//��Ӧ����ʱ�����
				//�������趨��ʱ��㵽�ˣ�ִ�лص�����
				if(timer_task[i].vu32Time_Out <= timer_task[i].vu32Time_Count)
				{
					timer_task[i].vu32Time_Count = 0;           //��λ��ʱʱ��
					if(timer_task[i].pCallbackFunction != NULL) //�ص���������
						timer_task[i].pCallbackFunction();      //ִ�лص�����
				}
			}
		}
        __HAL_TIM_CLEAR_FLAG(&htim5, TIM_FLAG_UPDATE);
    }
}
