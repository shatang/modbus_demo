#include "w5500_app.h"
#include "spi1.h"
#include "w5500_reg.h"
#include "socket.h"

#include "modbus_tcp.h"

/*******************************************************************************
* 函数名  : w5500_paramInit
* 描述    : 初始化W5500寄存器函数
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : 在使用w5500之前，检查并对w5500初始化
*******************************************************************************/
static void w5500_paramInit(void)
{
	uint8_t Check = Read_W5500_1Byte(VERR);//检查W5500版本
    if(0x04!=Check)return;
	
    //软件复位W5500,置1有效,复位后自动清0
    Write_W5500_1Byte(MR, RST);
	HAL_Delay(100);
	
    //设置重试时间，默认为2000(200ms) 
	//每一单位数值为100微秒,初始化时值设为2000(0x07D0),等于200毫秒
	Write_W5500_2Byte(RTR, 0x07d0);
	//设置重试次数，默认为5次 
	//如果重发的次数超过设定值,则产生超时中断(相关的端口中断寄存器中的Sn_IR 超时位(TIMEOUT)置“1”)
    Write_W5500_1Byte(RCR, 5);
    //低电平中断定时器寄存器
    //该寄存器用于设置中断生效等待时间。当下一个中断触发，中断引脚将会在INTLEVEL时间后
    //拉低中断引脚INTn。
    Write_W5500_1Byte(INTLEVEL, 10);

    Write_W5500_1Byte(IMR, IM_IR7 | IM_IR6);//中断屏蔽寄存器。启用ip冲突中断，启用目的地址不能抵达产生中断
    Write_W5500_1Byte(SIMR, S0_IMR);	    //启用socket0中断
}

uint8_t tcpBuf[256] = {0};
struct_socket_t socket0 = {{192,168,1,115}, 502, 8080, 0, tcpBuf, 0, tcpBuf};


void w5500_sysInit(void)
{
    uint8_t Gateway_IP[4] ={192, 168, 1, 1};                //网关IP地址
    uint8_t Sub_Mask[4] ={255, 255, 255, 0};	            //子网掩码
    uint8_t Phy_Addr[6] ={0x00,0x08,0xdc,0x11,0x11,0x11};   //物理地址(MAC)
    uint8_t IP_Addr[4] ={192,168,1,230};                    //本机IP地址
	
    
	spi1_init();		//初始化W5500管脚驱动
	w5500_paramInit();	//初始化W5500寄存器配置

    Write_W5500_nByte(GAR, Gateway_IP, 4);  //加载网关参数
    Write_W5500_nByte(SUBR, Sub_Mask, 4);   //加载子网掩码
    Write_W5500_nByte(SHAR, Phy_Addr, 6);   //加载物理地址
    Write_W5500_nByte(SIPR, IP_Addr, 4);    //加载本机IP地址
    
    socket_config(0, &socket0);	//socket0基本配置
    socket_tcpListen(0);	    //socket0配置为服务器，tcp监听
}

//案例
void modbusTcp_poll(void)
{
	socketCheck( 0 );
	if( socket_recvData( 0, &socket0 ) )
	{
        socket0.sendLen = modbusTCP_handele( socket0.recvBuf, socket0.recvLen );
        if( socket0.sendLen != 0 )
            socket_sendData( 0,  &socket0 );
	}
}
