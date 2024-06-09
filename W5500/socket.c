#include "socket.h"
#include "w5500_reg.h"

/*******************************************************************************
* 函数名  : Socket_Connect
* 描述    : 设置指定Socket(0~7)为客户端与远程服务器连接
* 输入    : s:待设定的端口
* 输出    : 无
* 返回值  : 成功返回TRUE(0xFF),失败返回FALSE(0x00)
* 说明    : 当本机Socket工作在客户端模式时,引用该程序,与远程服务器建立连接
*			如果启动连接后出现超时中断，则与服务器连接失败,需要重新调用该程序连接
*			该程序每调用一次,就与服务器产生一次连接
*******************************************************************************/
/*********************** Set Socket n in TCP Client mode ************************/
bool socket_tcpConnect(SOCKET s)
{
    for( uint8_t i=0; i<3; i++ ){
        Write_SOCK_1Byte(s, Sn_MR, MR_TCP);
        Write_SOCK_1Byte(s, Sn_CR, OPEN);

        HAL_Delay(5);	/* Wait for a moment */
        
        if(Read_SOCK_1Byte(s,Sn_SR) != SOCK_INIT){
            if( i>1 ){
                Write_SOCK_1Byte(s, Sn_CR, CLOSE);		/* Close Socket n */
                return false;
            }
        }
        else break;
    }
    
	Write_SOCK_1Byte(s, Sn_CR, CONNECT);
	return true;
}
/*******************************************************************************
* 函数名  : Socket_Listen
* 描述    : 设置指定Socket(0~7)作为服务器等待远程主机的连接
* 输入    : s:待设定的端口
* 输出    : 无
* 返回值  : 成功返回TRUE(0xFF),失败返回FALSE(0x00)
* 说明    : 当本机Socket工作在服务器模式时,引用该程序,等等远程主机的连接
*			该程序只调用一次,就使W5500设置为服务器模式
*******************************************************************************/
/*********************** Set Socket n in TCP Server mode ************************/
bool socket_tcpListen(SOCKET s)
{
	uint8_t i=0;
    
    for( i=0; i<3; i++ ){
        Write_SOCK_1Byte(s,Sn_CR,CLOSE);
        Write_SOCK_1Byte(s,Sn_MR,MR_TCP);
        Write_SOCK_1Byte(s,Sn_CR,OPEN);

        HAL_Delay(5);
        if(Read_SOCK_1Byte(s,Sn_SR)!=SOCK_INIT){
            if( i>1 ){
                Write_SOCK_1Byte(s,Sn_CR,CLOSE);
                return false;
            }
        }
        else break;
    }
    while( 1 ){
        Write_SOCK_1Byte(s,Sn_CR,LISTEN);
        HAL_Delay(5);	/* Wait for a moment */
        if(Read_SOCK_1Byte(s,Sn_SR)!=SOCK_LISTEN){
            if( i>1 ){
                Write_SOCK_1Byte(s,Sn_CR,CLOSE);		/* Close Socket n */
                return false;
            }
        }
        else break;
    }
	
	return true;
}
//至此完成了Socket的打开和设置侦听工作.至于远程客户端是否与它建立连接,
//则需要检查Socket中断，以判断Socket的连接是否成功。参考W5500数据手册的Socket中断状态
//在服务器侦听模式不需要设置目的IP和目的端口号






void socket_config(SOCKET s, struct_socket_t* ps_socket)
{
    Write_SOCK_1Byte(s, Sn_CR, CLOSE);//关闭对应socket
    Write_SOCK_2Byte(s, Sn_PORT, ps_socket->localPort);

    Write_SOCK_1Byte(s, Sn_KPALVTR, 1); //Keeplive时基为5秒
    
	Write_SOCK_2Byte(s, Sn_DPORTR, ps_socket->remotePort);
    Write_SOCK_4Byte(s, Sn_DIPR, ps_socket->remoteIP);
    
    Write_SOCK_2Byte(s, Sn_MSSR, 1460); //设置分片长度，参考W5500数据手册，该值可以不修改，最大分片字节数=1460(0x5b4)
    Write_SOCK_1Byte(s, Sn_IMR, IMR_SENDOK | IMR_TIMEOUT | IMR_RECV | IMR_DISCON | IMR_CON);//设置中断触发方式
    Write_SOCK_1Byte(s, Sn_RXBUF_SIZE, 0x02);//设置发送缓冲区大小，Set RX Buffer Size as 2K 
    Write_SOCK_1Byte(s, Sn_TXBUF_SIZE, 0x02);//设置接收缓冲区大小，Set TX Buffer Size as 2K
}

//Socket配置成功后直接监听

//注意IP配置参数的网关
//总体的初始化
//1. 硬件 2. 版本号 3. 参数配置，socket端口号配置，并监听，初始化modbustcp

//轮询操作
//1. 可关闭TCP进程(有个参数)，且多个参数能关闭(执行函数将对应参数置位)
//2. 初始化成功就就轮询，初始化不成功就重新(完全)初始化

//有个函数打破参数，轮询中进入初始化操作


///********* Process IR Register ********/
//void W5500TCP_ServerPoll(void)
//{
//    socketCheck(1000);//定期检查socket状态
    
//    if(gs_NetUserParam.u32W5500_WorkMode)
//        W5500_AutoRespond_Cont_Poll();
//    else
//    {
//        if(Read_W5500_1Byte(SIR) & 0x01)//Socket0有中断
//        {
//            uint8_t lu8_status;
//            
//            lu8_status = Read_SOCK_1Byte(0, Sn_IR); //读取中断信息
//            Write_SOCK_1Byte(0, Sn_IR, lu8_status);	//清空中断信息

//            RxLength = TCP_xMBReceiveRxFrame(0);
//            if(RxLength)TCP_PortEventPost();
//            W5500_ModbusTCP_Poll();
//        }
//    }
//}

//修改端口号要先关闭监听，初始化配置，再打开监听





bool socketCheck(SOCKET s)
{
    static uint32_t LinkTime = 0;//有多个socket则需要优化该tick缓存大小
    
    if(HAL_GetTick() - LinkTime > 1000)//定期检索socket连接是否正常
    {
        LinkTime = HAL_GetTick();

        switch(Read_SOCK_1Byte(s, Sn_SR))
        {
            case SOCK_INIT:
                socket_tcpListen(s);break;//打开Socket监听
			case SOCK_ESTABLISHED:break;
            case SOCK_LISTEN:break;
            
            default:
            {
                Write_SOCK_1Byte(s, Sn_CR, CLOSE);
                socket_tcpListen(s);//重新打开Socket监听
            }break;
        }
    }
}

bool socket_recvData(SOCKET s, struct_socket_t* ps_socket)
{
    uint8_t lu8_test = Read_W5500_1Byte(SIR);
    
	if(Read_W5500_1Byte(SIR) & (0x01<<s))//检查对应socket 是否产生中断
	{
		uint8_t lu8_status = Read_SOCK_1Byte(s, Sn_IR); //读取中断信息
		Write_SOCK_1Byte(s, Sn_IR, lu8_status);			//清空中断信息
		switch(lu8_status)
		{
			case IR_RECV:
			{
				ps_socket->recvLen = Read_SOCK_DataBuffer(s, ps_socket->recvBuf);	//读取收到信息
				return true;
			}break;
			
			case IR_SEND_OK:	//socketX 数据发送完成中断
			case IR_CON:		//socketX 连接中断
				break;
			case IR_TIMEOUT:	//socketX ABP、TCP超时中断
			case IR_DISCON:		//socketX 断连中断
			{
				Write_SOCK_1Byte(s, Sn_CR, CLOSE);	//关闭socket
			}break;	
		}
	}

	return false;
}

void socket_sendData(SOCKET s, struct_socket_t* ps_socket)
{
	Write_SOCK_DataBuffer(s, ps_socket->sendBuf, ps_socket->sendLen);
}