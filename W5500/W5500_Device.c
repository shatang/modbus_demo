#include "W5500_Device.h"
#include "socket.h"
#include "mb.h"
#include "NetParam.h"
#include "W5500_Driver.h"
//#include "IO_Device.h"

/***************----- 网络参数变量定义(内部调用) -----***************/
static uint8_t Gateway_IP[4];   //网关IP地址
static uint8_t Sub_Mask[4];	    //子网掩码
static uint8_t Phy_Addr[6];	    //物理地址(MAC)
static uint8_t IP_Addr[4];      //本机IP地址

static uint8_t UDP_DIPR[4];     //UDP(广播)模式,目的主机IP地址
static uint16_t UDP_DPORT;	    //UDP(广播)模式,目的主机端口号
/***************----- 端口的运行模式 -----***************/
unsigned char S0_Mode =3;	//0:TCP服务器模式,1:TCP客户端模式,2:UDP(广播)模式

#define TCP_SERVER		0x00	//TCP服务器模式
#define TCP_CLIENT		0x01	//TCP客户端模式
#define UDP_MODE		0x02	//UDP(广播)模式 

/*******************************************************************************
* 函数名  : W5500_Init
* 描述    : 初始化W5500寄存器函数
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : 在使用W5500之前，先对W5500初始化
*******************************************************************************/
void W5500_ParamInit(void)
{
    //软件复位W5500,置1有效,复位后自动清0
    Write_W5500_1Byte(MR, RST);
	delay_ms(100);
    //设置重试时间，默认为2000(200ms) 
	//每一单位数值为100微秒,初始化时值设为2000(0x07D0),等于200毫秒
	Write_W5500_2Byte(RTR, 0x07d0);
	//设置重试次数，默认为5次 
	//如果重发的次数超过设定值,则产生超时中断(相关的端口中断寄存器中的Sn_IR 超时位(TIMEOUT)置“1”)
    Write_W5500_1Byte(RCR,5);
    //低电平中断定时器寄存器
    //该寄存器用于设置中断生效等待时间。当下一个中断触发，中断引脚将会在INTLEVEL时间后
    //拉低中断引脚INTn。
    Write_W5500_1Byte(INTLEVEL,10);
        
    Write_W5500_1Byte(IMR, IM_IR7 | IM_IR6);    //启动IP冲突中断、开启目的地址不能抵达
    Write_W5500_1Byte(SIMR, S0_IMR);            //启用socket0中断

    Modify_Gateway_IP();    //加载网关参数
    Modify_Sub_Mask();      //加载子网掩码
    Modify_Sub_MacAddr();   //加载物理地址
    Modify_LocalIP();       //加载本机IP地址
}


void W5500_SystemInit(void)
{
    uint8_t Check=0;
	
    W5500_PortInit();       //初始化W5500管脚驱动
    Check = Read_W5500_1Byte(VERR);//检查W5500版本
    if(0x04!=Check)return;

	W5500_ParamInit();      //初始化W5500寄存器配置

    Socket_Config(0);//配置Socket0
    
    Socket_Listen(0);
    
    eMBTCPInit(0);
}


//修改网关
void Modify_Gateway_IP(void)
{
	uint8_t i;
    //加载网关参数
    for(i=0;i<4;i++)
        Gateway_IP[i] = gs_NetBasicParam.u8Gateway_IP[i];
    
//    Gateway_IP[0]=192;
//    Gateway_IP[1]=168;
//    Gateway_IP[2]=101;
//    Gateway_IP[3]=254;
    
    Write_W5500_nByte(GAR, Gateway_IP, 4);
}
//修改子网掩码
void Modify_Sub_Mask(void)
{
    uint8_t i;
    //加载子网掩码
    for(i=0;i<4;i++)
        Sub_Mask[i] = gs_NetBasicParam.u8Sub_Mask[i];
    
//    Sub_Mask[0]=255;
//    Sub_Mask[1]=255;
//    Sub_Mask[2]=255;
//    Sub_Mask[3]=0;
    
    
    Write_W5500_nByte(SUBR, Sub_Mask, 4);    
}
//修改物理Mac地址
void Modify_Sub_MacAddr(void)
{
    uint8_t i;
    //加载物理MAC地址
    Phy_Addr[0] = 0xBC;
    Phy_Addr[1] = 0x66;
    Phy_Addr[2] = 0x41;
    for(i=3;i<6;i++)
        Phy_Addr[i] = gs_NetBasicParam.u8MAC_Addr[i];
    
//    Phy_Addr[3] = 0x01;
//    Phy_Addr[4] = 0x02;
//    Phy_Addr[5] = 0x03;
    
    
    Write_W5500_nByte(SHAR, Phy_Addr, 6);
}
//修改本地IP地址
void Modify_LocalIP(void)
{
    uint8_t i;
    //加载本机IP地址
    for(i=0;i<4;i++)
        IP_Addr[i] = gs_NetBasicParam.u8Local_IP[i];
    
//    IP_Addr[0]=192;
//    IP_Addr[1]=168;
//    IP_Addr[2]=101;
//    IP_Addr[3]=246; 
    
    Write_W5500_nByte(SIPR, IP_Addr, 4);
}

