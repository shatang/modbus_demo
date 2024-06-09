#include "W5500_Modbus.h"
#include "W5500_Driver.h"
#include "FreeModbus_User.h"

#define MB_TCP_DEFAULT_PORT     502          	// TCP listening port. 
#define MB_TCP_BUF_SIZE     	( 256 + 7 ) 	// Must hold a complete Modbus TCP frame. 

static uint8_t TCPBuf[MB_TCP_BUF_SIZE];
uint16_t RxLength;
/********************************************************************************
* ������ : TCP_PortEventPost
* ��  �� : 
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.09.11
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
bool TCP_PortEventPost(void)
{
    return TCP_xMBPortEventPost( EV_FRAME_RECEIVED );    
}
/********************************************************************************
* ������ : 
* ��  �� : 
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.09.11
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
uint16_t TCP_xMBReceiveRxFrame(uint8_t socket)
{
	return Read_SOCK_DataBuffer(socket, TCPBuf);
}
/********************************************************************************
* ������ : TCP_xMBPortGetRequest
* ��  �� : 
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.09.11
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
BOOL
TCP_xMBPortGetRequest( UCHAR ** ppucMBTCPFrame, USHORT * usTCPLength )
{
	*ppucMBTCPFrame = &TCPBuf[0];
    *usTCPLength = RxLength;

    return TRUE;  
}
/********************************************************************************
* ������ : TCP_xMBPortSendResponse
* ��  �� : 
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.09.11
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
BOOL
TCP_xMBPortSendResponse(const UCHAR * pucMBTCPFrame, USHORT TCPLength )
{
	Write_SOCK_DataBuffer(0 ,(UCHAR*)pucMBTCPFrame, TCPLength);//����Socket0

    return TRUE;
}

void W5500_ModbusTCP_Poll(void)
{
    ge_ModbusPort = TCPW5500;
    TCP_eMBPoll();//�������
    TCP_eMBPoll();//������
}
