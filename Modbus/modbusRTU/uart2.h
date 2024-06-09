#ifndef __UART2_H
#define __UART2_H
#include "stm32f4xx_hal.h"
#include <stdbool.h>

typedef struct
{
    volatile bool vbWholeFrame;         //���ڻ�ȡ��һ������֡
    uint8_t u8RtuBuf[256];              /*!< Maximum size of a Modbus RTU frame. */
    
    volatile uint16_t vu16RcvLen;       //�Ѿ��������ݵĳ���
	volatile uint16_t vu16TransmitLen;  //�����Ѿ������ͳ�ȥ�ֽڼ���
	uint16_t u16SndLen;				    //��Ҫ�������ݵĳ���
    
    uint8_t u8Addr;
    void (*uartSend)(void);
    
}struct_mdRtu_t;


extern UART_HandleTypeDef huart2;
extern struct_mdRtu_t mdRtu_uart2;

void uart2_init(void);
void mdRtu_uart2Init(void);

#endif
