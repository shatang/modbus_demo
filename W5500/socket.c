#include "socket.h"
#include "w5500_reg.h"

/*******************************************************************************
* ������  : Socket_Connect
* ����    : ����ָ��Socket(0~7)Ϊ�ͻ�����Զ�̷���������
* ����    : s:���趨�Ķ˿�
* ���    : ��
* ����ֵ  : �ɹ�����TRUE(0xFF),ʧ�ܷ���FALSE(0x00)
* ˵��    : ������Socket�����ڿͻ���ģʽʱ,���øó���,��Զ�̷�������������
*			����������Ӻ���ֳ�ʱ�жϣ��������������ʧ��,��Ҫ���µ��øó�������
*			�ó���ÿ����һ��,�������������һ������
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
* ������  : Socket_Listen
* ����    : ����ָ��Socket(0~7)��Ϊ�������ȴ�Զ������������
* ����    : s:���趨�Ķ˿�
* ���    : ��
* ����ֵ  : �ɹ�����TRUE(0xFF),ʧ�ܷ���FALSE(0x00)
* ˵��    : ������Socket�����ڷ�����ģʽʱ,���øó���,�ȵ�Զ������������
*			�ó���ֻ����һ��,��ʹW5500����Ϊ������ģʽ
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
//���������Socket�Ĵ򿪺�������������.����Զ�̿ͻ����Ƿ�������������,
//����Ҫ���Socket�жϣ����ж�Socket�������Ƿ�ɹ����ο�W5500�����ֲ��Socket�ж�״̬
//�ڷ���������ģʽ����Ҫ����Ŀ��IP��Ŀ�Ķ˿ں�






void socket_config(SOCKET s, struct_socket_t* ps_socket)
{
    Write_SOCK_1Byte(s, Sn_CR, CLOSE);//�رն�Ӧsocket
    Write_SOCK_2Byte(s, Sn_PORT, ps_socket->localPort);

    Write_SOCK_1Byte(s, Sn_KPALVTR, 1); //Keepliveʱ��Ϊ5��
    
	Write_SOCK_2Byte(s, Sn_DPORTR, ps_socket->remotePort);
    Write_SOCK_4Byte(s, Sn_DIPR, ps_socket->remoteIP);
    
    Write_SOCK_2Byte(s, Sn_MSSR, 1460); //���÷�Ƭ���ȣ��ο�W5500�����ֲᣬ��ֵ���Բ��޸ģ�����Ƭ�ֽ���=1460(0x5b4)
    Write_SOCK_1Byte(s, Sn_IMR, IMR_SENDOK | IMR_TIMEOUT | IMR_RECV | IMR_DISCON | IMR_CON);//�����жϴ�����ʽ
    Write_SOCK_1Byte(s, Sn_RXBUF_SIZE, 0x02);//���÷��ͻ�������С��Set RX Buffer Size as 2K 
    Write_SOCK_1Byte(s, Sn_TXBUF_SIZE, 0x02);//���ý��ջ�������С��Set TX Buffer Size as 2K
}

//Socket���óɹ���ֱ�Ӽ���

//ע��IP���ò���������
//����ĳ�ʼ��
//1. Ӳ�� 2. �汾�� 3. �������ã�socket�˿ں����ã�����������ʼ��modbustcp

//��ѯ����
//1. �ɹر�TCP����(�и�����)���Ҷ�������ܹر�(ִ�к�������Ӧ������λ)
//2. ��ʼ���ɹ��;���ѯ����ʼ�����ɹ�������(��ȫ)��ʼ��

//�и��������Ʋ�������ѯ�н����ʼ������


///********* Process IR Register ********/
//void W5500TCP_ServerPoll(void)
//{
//    socketCheck(1000);//���ڼ��socket״̬
    
//    if(gs_NetUserParam.u32W5500_WorkMode)
//        W5500_AutoRespond_Cont_Poll();
//    else
//    {
//        if(Read_W5500_1Byte(SIR) & 0x01)//Socket0���ж�
//        {
//            uint8_t lu8_status;
//            
//            lu8_status = Read_SOCK_1Byte(0, Sn_IR); //��ȡ�ж���Ϣ
//            Write_SOCK_1Byte(0, Sn_IR, lu8_status);	//����ж���Ϣ

//            RxLength = TCP_xMBReceiveRxFrame(0);
//            if(RxLength)TCP_PortEventPost();
//            W5500_ModbusTCP_Poll();
//        }
//    }
//}

//�޸Ķ˿ں�Ҫ�ȹرռ�������ʼ�����ã��ٴ򿪼���





bool socketCheck(SOCKET s)
{
    static uint32_t LinkTime = 0;//�ж��socket����Ҫ�Ż���tick�����С
    
    if(HAL_GetTick() - LinkTime > 1000)//���ڼ���socket�����Ƿ�����
    {
        LinkTime = HAL_GetTick();

        switch(Read_SOCK_1Byte(s, Sn_SR))
        {
            case SOCK_INIT:
                socket_tcpListen(s);break;//��Socket����
			case SOCK_ESTABLISHED:break;
            case SOCK_LISTEN:break;
            
            default:
            {
                Write_SOCK_1Byte(s, Sn_CR, CLOSE);
                socket_tcpListen(s);//���´�Socket����
            }break;
        }
    }
}

bool socket_recvData(SOCKET s, struct_socket_t* ps_socket)
{
    uint8_t lu8_test = Read_W5500_1Byte(SIR);
    
	if(Read_W5500_1Byte(SIR) & (0x01<<s))//����Ӧsocket �Ƿ�����ж�
	{
		uint8_t lu8_status = Read_SOCK_1Byte(s, Sn_IR); //��ȡ�ж���Ϣ
		Write_SOCK_1Byte(s, Sn_IR, lu8_status);			//����ж���Ϣ
		switch(lu8_status)
		{
			case IR_RECV:
			{
				ps_socket->recvLen = Read_SOCK_DataBuffer(s, ps_socket->recvBuf);	//��ȡ�յ���Ϣ
				return true;
			}break;
			
			case IR_SEND_OK:	//socketX ���ݷ�������ж�
			case IR_CON:		//socketX �����ж�
				break;
			case IR_TIMEOUT:	//socketX ABP��TCP��ʱ�ж�
			case IR_DISCON:		//socketX �����ж�
			{
				Write_SOCK_1Byte(s, Sn_CR, CLOSE);	//�ر�socket
			}break;	
		}
	}

	return false;
}

void socket_sendData(SOCKET s, struct_socket_t* ps_socket)
{
	Write_SOCK_DataBuffer(s, ps_socket->sendBuf, ps_socket->sendLen);
}