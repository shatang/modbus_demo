#include "W5500_Modbus.h"
#include "W5500_Driver.h"
#include "FreeModbus_User.h"

#define MB_TCP_DEFAULT_PORT     502          	// TCP listening port. 
#define MB_TCP_BUF_SIZE     	( 256 + 7 ) 	// Must hold a complete Modbus TCP frame. 

static uint8_t TCPBuf[MB_TCP_BUF_SIZE];
uint16_t RxLength;
/********************************************************************************
* 函数名 : TCP_PortEventPost
* 功  能 : 
* 说  明 : none
* 入  参 : none
* 返  回 : none
* 设  计 : Shatang                    时  间 : 2020.09.11
* 修  改 : none                       时  间 : none
********************************************************************************/
bool TCP_PortEventPost(void)
{
    return TCP_xMBPortEventPost( EV_FRAME_RECEIVED );    
}
/********************************************************************************
* 函数名 : 
* 功  能 : 
* 说  明 : none
* 入  参 : none
* 返  回 : none
* 设  计 : Shatang                    时  间 : 2020.09.11
* 修  改 : none                       时  间 : none
********************************************************************************/
uint16_t TCP_xMBReceiveRxFrame(uint8_t socket)
{
	return Read_SOCK_DataBuffer(socket, TCPBuf);
}
/********************************************************************************
* 函数名 : TCP_xMBPortGetRequest
* 功  能 : 
* 说  明 : none
* 入  参 : none
* 返  回 : none
* 设  计 : Shatang                    时  间 : 2020.09.11
* 修  改 : none                       时  间 : none
********************************************************************************/
BOOL
TCP_xMBPortGetRequest( UCHAR ** ppucMBTCPFrame, USHORT * usTCPLength )
{
	*ppucMBTCPFrame = &TCPBuf[0];
    *usTCPLength = RxLength;

    return TRUE;  
}
/********************************************************************************
* 函数名 : TCP_xMBPortSendResponse
* 功  能 : 
* 说  明 : none
* 入  参 : none
* 返  回 : none
* 设  计 : Shatang                    时  间 : 2020.09.11
* 修  改 : none                       时  间 : none
********************************************************************************/
BOOL
TCP_xMBPortSendResponse(const UCHAR * pucMBTCPFrame, USHORT TCPLength )
{
	Write_SOCK_DataBuffer(0 ,(UCHAR*)pucMBTCPFrame, TCPLength);//采用Socket0

    return TRUE;
}

void W5500_ModbusTCP_Poll(void)
{
    ge_ModbusPort = TCPW5500;
    TCP_eMBPoll();//处理接收
    TCP_eMBPoll();//处理发送
}
