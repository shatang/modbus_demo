#ifndef __UART2_H
#define __UART2_H
#include "stm32f4xx_hal.h"
#include <stdbool.h>

typedef struct
{
    volatile bool vbWholeFrame;         //串口获取到一个完整帧
    uint8_t u8RtuBuf[256];              /*!< Maximum size of a Modbus RTU frame. */
    
    volatile uint16_t vu16RcvLen;       //已经接收数据的长度
	volatile uint16_t vu16TransmitLen;  //数据已经被发送出去字节计数
	uint16_t u16SndLen;				    //将要发送数据的长度
    
    uint8_t u8Addr;
    void (*uartSend)(void);
    
}struct_mdRtu_t;


extern UART_HandleTypeDef huart2;
extern struct_mdRtu_t mdRtu_uart2;

void uart2_init(void);
void mdRtu_uart2Init(void);

#endif
