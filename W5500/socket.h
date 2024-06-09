#ifndef _SOCKET_H
#define _SOCKET_H
#include <stdint.h>
#include <stdbool.h>

typedef  uint8_t SOCKET;
typedef struct
{
	uint8_t remoteIP[4];	        //socket 目的IP地址
	uint16_t localPort;	        	//socket 本地端口号
	uint16_t remotePort;	        //socket 目的端口号
	uint16_t recvLen;				//data接收长度
	uint8_t* recvBuf;				//data接收缓存指针
	uint16_t sendLen;				//data发送长度
	uint8_t* sendBuf;				//data发送缓存指针 
}struct_socket_t;

/*********************** Set Socket n in TCP Client mode ************************/
bool socket_tcpConnect(SOCKET s);
/*********************** Set Socket n in TCP Server mode ************************/
bool socket_tcpListen(SOCKET s);

void socket_config(SOCKET s, struct_socket_t* ps_socket);
bool socket_recvData(SOCKET s, struct_socket_t* ps_socket);
void socket_sendData(SOCKET s, struct_socket_t* ps_socket);
bool socketCheck(SOCKET s);


//void W5500TCP_ServerPoll(void);


#endif
