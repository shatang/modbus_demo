#include "modbus_tcp.h"
#include "modbus_comm.h"

/* ----------------------- MBAP Header --------------------------------------*/
/*
 *
 * <------------------------ MODBUS TCP/IP ADU(1) ------------------------->
 *              <----------- MODBUS PDU (1') ---------------->
 *  +-----------+---------------+------------------------------------------+
 *  | TID | PID | Length | UID  |Code | Data                               |
 *  +-----------+---------------+------------------------------------------+
 *  |     |     |        |      |                                           
 * (2)   (3)   (4)      (5)    (6)                                          
 *
 * (2)  ... MB_TCP_TID          = 0 (Transaction Identifier - 2 Byte) 
 * (3)  ... MB_TCP_PID          = 2 (Protocol Identifier - 2 Byte)
 * (4)  ... MB_TCP_LEN          = 4 (Number of bytes - 2 Byte)
 * (5)  ... MB_TCP_UID          = 6 (Unit Identifier - 1 Byte)
 * (6)  ... MB_TCP_FUNC         = 7 (Modbus Function Code)
 *
 * (1)  ... Modbus TCP/IP Application Data Unit
 * (1') ... Modbus Protocol Data Unit
 */
/* ----------------------- Defines ------------------------------------------*/
#define MB_TCP_PID          2
#define MB_TCP_LEN          4
#define MB_TCP_FUNC         7

#define MB_TCP_PROTOCOL_ID  0   /* 0 = Modbus Protocol */
#define MB_FUNC_ERROR       ( 0x80 )

uint16_t modbusTCP_handele( uint8_t* fpu8_mem, uint16_t fu16_length )
{
	uint16_t lu16_PID = 0;
	uint16_t lu16_mbLen = 0;
	
	lu16_PID = fpu8_mem[MB_TCP_PID] << 8U;
	lu16_PID |= fpu8_mem[MB_TCP_PID + 1];
	if( lu16_PID == MB_TCP_PROTOCOL_ID ){
		uint8_t* lpu8_mbFrame = &fpu8_mem[MB_TCP_FUNC];
		uint16_t lu16_funcCode = fpu8_mem[MB_TCP_FUNC];
		eMBErrorCode    eStatus = MB_ENOERR;
		
		lu16_mbLen = fu16_length - MB_TCP_FUNC;;
		
		for(uint8_t i=0; i<MB_FUNC_HANDLERS_MAX; i++ ){
			if( xFuncHandlers[i].ucFunctionCode == lu16_funcCode ){
				eStatus = xFuncHandlers[i].pxMBFunctionHandler( lpu8_mbFrame, &lu16_mbLen );
				break;
			}
		}

		if( eStatus != MB_ENOERR ){
			/* An exception occured. Build an error frame. */
			lpu8_mbFrame[lu16_mbLen++] = ( uint8_t )( lu16_funcCode | MB_FUNC_ERROR );
			lpu8_mbFrame[lu16_mbLen++] = eStatus;
			lu16_mbLen = 0;
		}
		fpu8_mem[MB_TCP_LEN] = ( lu16_mbLen + 1 ) >> 8U;
		fpu8_mem[MB_TCP_LEN + 1] = ( lu16_mbLen + 1 ) & 0xFF;
		lu16_mbLen += MB_TCP_FUNC;
	}
	return lu16_mbLen;
}
