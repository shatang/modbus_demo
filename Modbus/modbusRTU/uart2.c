#include "uart2.h"
#include "tick_task.h"
#include <string.h>

UART_HandleTypeDef huart2;

static void uart2_txMode(void)
{
    GPIOD->ODR|=GPIO_PIN_7;     //设置为发送模式
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_TC);
    __HAL_UART_DISABLE_IT(&huart2, UART_IT_RXNE);
}

static void uart2_rxMode(void)
{
    GPIOD->ODR&=(~GPIO_PIN_7);  //设置为接收模式
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
    __HAL_UART_DISABLE_IT(&huart2, UART_IT_TC);
}

void uart2_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    /**USART2 GPIO Configuration    
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pin : rs485-control Pin */
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    HAL_UART_Init(&huart2);

    HAL_NVIC_SetPriority(USART2_IRQn, 3, 1);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
    uart2_rxMode();
}

struct_mdRtu_t mdRtu_uart2;

static void mdRtu_uart2Send(void)
{
    mdRtu_uart2.vbWholeFrame = false;
    mdRtu_uart2.vu16TransmitLen = 0;            //已发送的字节数，置0
    uart2_txMode();
    USART2->DR = mdRtu_uart2.u8RtuBuf[mdRtu_uart2.vu16TransmitLen++];//开始发送数据
}

void mdRtu_uart2Init(void)
{
    mdRtu_uart2.u8Addr = 1;
    mdRtu_uart2.uartSend = mdRtu_uart2Send;
    memset(mdRtu_uart2.u8RtuBuf, 0, sizeof(mdRtu_uart2.u8RtuBuf));
	mdRtu_uart2.vbWholeFrame = false;
    mdRtu_uart2.vu16RcvLen = 0;
    mdRtu_uart2.vu16TransmitLen = 0;
    mdRtu_uart2.u16SndLen = 0;
}

static void uart2_recv_callback(void)
{
    timerTask_clearTask(UART2_RECV_TASK);//清空定时器计数
    mdRtu_uart2.vbWholeFrame = true;
}

void USART2_IRQHandler(void)
{
    if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE))
    {
        if(mdRtu_uart2.vu16RcvLen == 0)
            timerTask_add(UART2_RECV_TASK, 4, uart2_recv_callback); //定时器基准为200us，波特率115200，限制响应为15*50=750us。现设定为(200us*4)us触发超时回调
        if(mdRtu_uart2.vu16RcvLen < 256)
            mdRtu_uart2.u8RtuBuf[mdRtu_uart2.vu16RcvLen++] = USART2->DR;
        else
            mdRtu_uart2.vu16RcvLen = 1;
        timerTask_clearCount(UART2_RECV_TASK);      //清空定时器计数
    }
    if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC))
    {
        if(mdRtu_uart2.vu16TransmitLen >= mdRtu_uart2.u16SndLen)
        {
            uart2_rxMode();
            mdRtu_uart2.vu16RcvLen = 0;
        }
        else
            USART2->DR = mdRtu_uart2.u8RtuBuf[mdRtu_uart2.vu16TransmitLen++];
        __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_TC);
    }
    if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_ORE))
    {
        USART2->DR;
        __HAL_UART_CLEAR_OREFLAG(&huart2);
    }
}
