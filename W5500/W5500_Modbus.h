#ifndef __W5500_MODBUS_H
#define __W5500_MODBUS_H
#include "mb.h"
#include <stdint.h>
#include <stdbool.h>


/****************************** GLOBAL VARIABLES ******************************/
extern uint16_t RxLength;
/****************************** GLOBAL FUNCTIONS ******************************/
extern bool TCP_PortEventPost(void);
extern uint16_t TCP_xMBReceiveRxFrame(uint8_t);
extern BOOL TCP_xMBPortGetRequest( UCHAR ** , USHORT *  );
extern BOOL TCP_xMBPortSendResponse(const UCHAR * , USHORT  );

extern void W5500_ModbusTCP_Poll(void);

#endif
