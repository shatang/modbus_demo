#include "W5500_Device.h"
#include "socket.h"
#include "mb.h"
#include "NetParam.h"
#include "W5500_Driver.h"
//#include "IO_Device.h"

/***************----- ���������������(�ڲ�����) -----***************/
static uint8_t Gateway_IP[4];   //����IP��ַ
static uint8_t Sub_Mask[4];	    //��������
static uint8_t Phy_Addr[6];	    //�����ַ(MAC)
static uint8_t IP_Addr[4];      //����IP��ַ

static uint8_t UDP_DIPR[4];     //UDP(�㲥)ģʽ,Ŀ������IP��ַ
static uint16_t UDP_DPORT;	    //UDP(�㲥)ģʽ,Ŀ�������˿ں�
/***************----- �˿ڵ�����ģʽ -----***************/
unsigned char S0_Mode =3;	//0:TCP������ģʽ,1:TCP�ͻ���ģʽ,2:UDP(�㲥)ģʽ

#define TCP_SERVER		0x00	//TCP������ģʽ
#define TCP_CLIENT		0x01	//TCP�ͻ���ģʽ
#define UDP_MODE		0x02	//UDP(�㲥)ģʽ 

/*******************************************************************************
* ������  : W5500_Init
* ����    : ��ʼ��W5500�Ĵ�������
* ����    : ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��ʹ��W5500֮ǰ���ȶ�W5500��ʼ��
*******************************************************************************/
void W5500_ParamInit(void)
{
    //�����λW5500,��1��Ч,��λ���Զ���0
    Write_W5500_1Byte(MR, RST);
	delay_ms(100);
    //��������ʱ�䣬Ĭ��Ϊ2000(200ms) 
	//ÿһ��λ��ֵΪ100΢��,��ʼ��ʱֵ��Ϊ2000(0x07D0),����200����
	Write_W5500_2Byte(RTR, 0x07d0);
	//�������Դ�����Ĭ��Ϊ5�� 
	//����ط��Ĵ��������趨ֵ,�������ʱ�ж�(��صĶ˿��жϼĴ����е�Sn_IR ��ʱλ(TIMEOUT)�á�1��)
    Write_W5500_1Byte(RCR,5);
    //�͵�ƽ�ж϶�ʱ���Ĵ���
    //�üĴ������������ж���Ч�ȴ�ʱ�䡣����һ���жϴ������ж����Ž�����INTLEVELʱ���
    //�����ж�����INTn��
    Write_W5500_1Byte(INTLEVEL,10);
        
    Write_W5500_1Byte(IMR, IM_IR7 | IM_IR6);    //����IP��ͻ�жϡ�����Ŀ�ĵ�ַ���ִܵ�
    Write_W5500_1Byte(SIMR, S0_IMR);            //����socket0�ж�

    Modify_Gateway_IP();    //�������ز���
    Modify_Sub_Mask();      //������������
    Modify_Sub_MacAddr();   //���������ַ
    Modify_LocalIP();       //���ر���IP��ַ
}


void W5500_SystemInit(void)
{
    uint8_t Check=0;
	
    W5500_PortInit();       //��ʼ��W5500�ܽ�����
    Check = Read_W5500_1Byte(VERR);//���W5500�汾
    if(0x04!=Check)return;

	W5500_ParamInit();      //��ʼ��W5500�Ĵ�������

    Socket_Config(0);//����Socket0
    
    Socket_Listen(0);
    
    eMBTCPInit(0);
}


//�޸�����
void Modify_Gateway_IP(void)
{
	uint8_t i;
    //�������ز���
    for(i=0;i<4;i++)
        Gateway_IP[i] = gs_NetBasicParam.u8Gateway_IP[i];
    
//    Gateway_IP[0]=192;
//    Gateway_IP[1]=168;
//    Gateway_IP[2]=101;
//    Gateway_IP[3]=254;
    
    Write_W5500_nByte(GAR, Gateway_IP, 4);
}
//�޸���������
void Modify_Sub_Mask(void)
{
    uint8_t i;
    //������������
    for(i=0;i<4;i++)
        Sub_Mask[i] = gs_NetBasicParam.u8Sub_Mask[i];
    
//    Sub_Mask[0]=255;
//    Sub_Mask[1]=255;
//    Sub_Mask[2]=255;
//    Sub_Mask[3]=0;
    
    
    Write_W5500_nByte(SUBR, Sub_Mask, 4);    
}
//�޸�����Mac��ַ
void Modify_Sub_MacAddr(void)
{
    uint8_t i;
    //��������MAC��ַ
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
//�޸ı���IP��ַ
void Modify_LocalIP(void)
{
    uint8_t i;
    //���ر���IP��ַ
    for(i=0;i<4;i++)
        IP_Addr[i] = gs_NetBasicParam.u8Local_IP[i];
    
//    IP_Addr[0]=192;
//    IP_Addr[1]=168;
//    IP_Addr[2]=101;
//    IP_Addr[3]=246; 
    
    Write_W5500_nByte(SIPR, IP_Addr, 4);
}

