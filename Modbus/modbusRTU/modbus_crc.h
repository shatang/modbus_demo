#ifndef __MODBUS_CRC_H
#define __MODBUS_CRC_H
#include <stdint.h>

uint16_t u16Get_mbCRC16( uint8_t* fp8_frame, uint16_t fu16_len );
int8_t s8Check_mbCRC16( uint8_t* fp8_frame, uint16_t fu16_len );
#endif
