#include "tick_task.h"
#include "main.h"


static struct_TaskData_t timer_task[TIM_TASK_NUM];  	// 定时器任务的事件队列
static volatile uint32_t vu32_timerTick = 0;        	// 系统时间(不可修改)
//================================ 任务定时器部分 =================================
/********************************************************************************
* 函数名 : timerTask_add
* 功  能 : 定时器任务添加
* 说  明 : none
* 入  参 : fe_Name:任务ID号
            timeout:定时时间
            pf_CallbackFunction:回调函数指针
* 返  回 : none
* 设  计 : shatang                    时  间 : 2020.10.20
* 修  改 : none                       时  间 : none
********************************************************************************/
bool timerTask_add(enum_TimTask_t fe_Name, uint32_t fu32_Timeout, timTaskCall* pf_CallbackFunction)
{
	if((fe_Name == No_TaskID)||(fu32_Timeout == 0))return false;//传入参数异常
	timerTask_clearTask(fe_Name);//防止重复添加任务，添加任务前函数先清除同一名字的任务
    if(timer_task[fe_Name].vbTask_Exist == true)return false;///存在该定时器任务，则不添加
    
    timer_task[fe_Name].vbTask_Exist = true;
    timer_task[fe_Name].vu32Time_Out = fu32_Timeout;
    timer_task[fe_Name].vu32Time_Count = 0;
    timer_task[fe_Name].pCallbackFunction = pf_CallbackFunction;
    
    return true;
}
/********************************************************************************
* 函数名 : timerTask_clearTask
* 功  能 : 定时器任务清除
* 说  明 : none
* 入  参 : fe_Name:任务ID号
* 返  回 : none
* 设  计 : shatang                    时  间 : 2020.10.20
* 修  改 : none                       时  间 : none
********************************************************************************/
bool timerTask_clearTask(enum_TimTask_t fe_Name)
{
	if(fe_Name == No_TaskID)return false;//无该任务
    
    //根据任务名称查找相应任务并清除
    timer_task[fe_Name].vbTask_Exist = false;
    timer_task[fe_Name].vu32Time_Out = 0;
    timer_task[fe_Name].vu32Time_Count = 0;
    timer_task[fe_Name].pCallbackFunction = NULL;

	return true;
}
/********************************************************************************
* 函数名 : timerTask_clearCount
* 功  能 : 定时器计数清除
* 说  明 : none
* 入  参 : fe_Name:任务ID号
* 返  回 : none
* 设  计 : shatang                    时  间 : 2020.10.20
* 修  改 : none                       时  间 : none
********************************************************************************/
bool timerTask_clearCount(enum_TimTask_t fe_Name)
{
	if(fe_Name == No_TaskID)return false;//无该任务
    timer_task[fe_Name].vu32Time_Count = 0;

	return true;
}
/********************************************************************************
* 函数名 : timerTask_clearCount
* 功  能 : 定时器计数清除
* 说  明 : none
* 入  参 : fe_Name:任务ID号
* 返  回 : none
* 设  计 : shatang                    时  间 : 2020.10.20
* 修  改 : none                       时  间 : none
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
	//定时器3,200us
    __HAL_RCC_TIM3_CLK_ENABLE();            //使能TIM3时钟
    HAL_NVIC_SetPriority(TIM3_IRQn, 3, 3);  //设置中断优先级，抢占优先级3，子优先级3
    HAL_NVIC_EnableIRQ(TIM3_IRQn);          //开启ITM3中断
	
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 9000-1;          //10khz
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 2-1;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_Base_Init(&htim3);
	
	__HAL_TIM_ENABLE(&htim3);                       //使能定时器3
    __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);     //使能定时器3更新中断：TIM_IT_UPDATE

	
	//定时器4,1ms
    __HAL_RCC_TIM4_CLK_ENABLE();            //使能TIM4时钟
    HAL_NVIC_SetPriority(TIM4_IRQn, 3, 3);  //设置中断优先级，抢占优先级3，子优先级3
    HAL_NVIC_EnableIRQ(TIM4_IRQn);          //开启ITM4中断
	
	htim4.Instance = TIM4;
	htim4.Init.Prescaler = 9000-1;       	//10khz
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 10-1;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_Base_Init(&htim4);
	
	__HAL_TIM_ENABLE(&htim4);                       //使能定时器4
    __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);     //使能定时器4更新中断：TIM_IT_UPDATE
	
		
	//定时器5,1s
    __HAL_RCC_TIM5_CLK_ENABLE();            //使能TIM5时钟
    HAL_NVIC_SetPriority(TIM5_IRQn, 3, 3);  //设置中断优先级，抢占优先级3，子优先级3
    HAL_NVIC_EnableIRQ(TIM5_IRQn);          //开启ITM5中断
	
	htim5.Instance = TIM5;
	htim5.Init.Prescaler = 9000-1;       	//10khz
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim5.Init.Period = 10000-1;
	htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_Base_Init(&htim5);
	
	__HAL_TIM_ENABLE(&htim5);                       //使能定时器5
    __HAL_TIM_ENABLE_IT(&htim5, TIM_IT_UPDATE);     //使能定时器5更新中断：TIM_IT_UPDATE
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
    if(__HAL_TIM_GET_FLAG(&htim3, TIM_FLAG_UPDATE))
    {
		for(uint8_t i=1; i<LED_COMM_TASK; i++) 	//任务检索
		{
			if(timer_task[i].vu32Time_Out != 0)	//如果任务存在
			{
				timer_task[i].vu32Time_Count++;	//对应任务时间计数
				//任务所设定的时间点到了，执行回调函数
				if(timer_task[i].vu32Time_Out <= timer_task[i].vu32Time_Count)
				{
					timer_task[i].vu32Time_Count = 0;           //复位计时时间
					if(timer_task[i].pCallbackFunction != NULL) //回调函数存在
						timer_task[i].pCallbackFunction();      //执行回调函数
				}
			}
		}
        __HAL_TIM_CLEAR_FLAG(&htim3, TIM_FLAG_UPDATE);
    }
}

//定时器4中断服务函数
void TIM4_IRQHandler(void)
{
    if(__HAL_TIM_GET_FLAG(&htim4, TIM_FLAG_UPDATE))
    {
		vu32_timerTick++;
		
		for(uint8_t i=LED_COMM_TASK; i<IWDG_FEED_TASK; i++) 	//任务检索
		{
			if(timer_task[i].vu32Time_Out != 0)	//如果任务存在
			{
				timer_task[i].vu32Time_Count++;	//对应任务时间计数
				//任务所设定的时间点到了，执行回调函数
				if(timer_task[i].vu32Time_Out <= timer_task[i].vu32Time_Count)
				{
					timer_task[i].vu32Time_Count = 0;           //复位计时时间
					if(timer_task[i].pCallbackFunction != NULL) //回调函数存在
						timer_task[i].pCallbackFunction();      //执行回调函数
				}
			}
		}
        __HAL_TIM_CLEAR_FLAG(&htim4, TIM_FLAG_UPDATE);
    }
}

//定时器5中断服务函数
void TIM5_IRQHandler(void)
{
    if(__HAL_TIM_GET_FLAG(&htim5, TIM_FLAG_UPDATE))
    {
		for(uint8_t i=IWDG_FEED_TASK; i<TIM_TASK_NUM; i++) 	//任务检索
		{
			if(timer_task[i].vu32Time_Out != 0)	//如果任务存在
			{
				timer_task[i].vu32Time_Count++;	//对应任务时间计数
				//任务所设定的时间点到了，执行回调函数
				if(timer_task[i].vu32Time_Out <= timer_task[i].vu32Time_Count)
				{
					timer_task[i].vu32Time_Count = 0;           //复位计时时间
					if(timer_task[i].pCallbackFunction != NULL) //回调函数存在
						timer_task[i].pCallbackFunction();      //执行回调函数
				}
			}
		}
        __HAL_TIM_CLEAR_FLAG(&htim5, TIM_FLAG_UPDATE);
    }
}
