#include "w5500_app.h"
#include "spi1.h"
#include "w5500_reg.h"
#include "socket.h"

#include "modbus_tcp.h"

/*******************************************************************************
* ������  : w5500_paramInit
* ����    : ��ʼ��W5500�Ĵ�������
* ����    : ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��ʹ��w5500֮ǰ����鲢��w5500��ʼ��
*******************************************************************************/
static void w5500_paramInit(void)
{
	uint8_t Check = Read_W5500_1Byte(VERR);//���W5500�汾
    if(0x04!=Check)return;
	
    //�����λW5500,��1��Ч,��λ���Զ���0
    Write_W5500_1Byte(MR, RST);
	HAL_Delay(100);
	
    //��������ʱ�䣬Ĭ��Ϊ2000(200ms) 
	//ÿһ��λ��ֵΪ100΢��,��ʼ��ʱֵ��Ϊ2000(0x07D0),����200����
	Write_W5500_2Byte(RTR, 0x07d0);
	//�������Դ�����Ĭ��Ϊ5�� 
	//����ط��Ĵ��������趨ֵ,�������ʱ�ж�(��صĶ˿��жϼĴ����е�Sn_IR ��ʱλ(TIMEOUT)�á�1��)
    Write_W5500_1Byte(RCR, 5);
    //�͵�ƽ�ж϶�ʱ���Ĵ���
    //�üĴ������������ж���Ч�ȴ�ʱ�䡣����һ���жϴ������ж����Ž�����INTLEVELʱ���
    //�����ж�����INTn��
    Write_W5500_1Byte(INTLEVEL, 10);

    Write_W5500_1Byte(IMR, IM_IR7 | IM_IR6);//�ж����μĴ���������ip��ͻ�жϣ�����Ŀ�ĵ�ַ���ִܵ�����ж�
    Write_W5500_1Byte(SIMR, S0_IMR);	    //����socket0�ж�
}

uint8_t tcpBuf[256] = {0};
struct_socket_t socket0 = {{192,168,1,115}, 502, 8080, 0, tcpBuf, 0, tcpBuf};


void w5500_sysInit(void)
{
    uint8_t Gateway_IP[4] ={192, 168, 1, 1};                //����IP��ַ
    uint8_t Sub_Mask[4] ={255, 255, 255, 0};	            //��������
    uint8_t Phy_Addr[6] ={0x00,0x08,0xdc,0x11,0x11,0x11};   //�����ַ(MAC)
    uint8_t IP_Addr[4] ={192,168,1,230};                    //����IP��ַ
	
    
	spi1_init();		//��ʼ��W5500�ܽ�����
	w5500_paramInit();	//��ʼ��W5500�Ĵ�������

    Write_W5500_nByte(GAR, Gateway_IP, 4);  //�������ز���
    Write_W5500_nByte(SUBR, Sub_Mask, 4);   //������������
    Write_W5500_nByte(SHAR, Phy_Addr, 6);   //���������ַ
    Write_W5500_nByte(SIPR, IP_Addr, 4);    //���ر���IP��ַ
    
    socket_config(0, &socket0);	//socket0��������
    socket_tcpListen(0);	    //socket0����Ϊ��������tcp����
}

//����
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
