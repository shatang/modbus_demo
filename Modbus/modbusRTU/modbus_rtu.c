#include "modbus_rtu.h"
#include "modbus_comm.h"
#include "modbus_crc.h"
#include "uart2.h"


/* ----------------------- Defines ------------------------------------------*/
#define MB_SER_PDU_SIZE_MIN     4       /*!< Minimum size of a Modbus RTU frame. */
#define MB_SER_PDU_ADDR_OFF     0       /*!< Offset of slave address in Ser-PDU. */
#define MB_SER_PDU_PDU_OFF      1       /*!< Offset of Modbus-PDU in Ser-PDU. */
#define MB_FUNC_ERROR           ( 0x80 )


static void modbusRTU_handle( struct_mdRtu_t* fs_mdRtu )
{
    if(fs_mdRtu->vbWholeFrame == false)return;  //����δ��ȡ��һ������֡
    
    fs_mdRtu->vbWholeFrame = false;
    uint16_t lu16_bufLen = fs_mdRtu->vu16RcvLen;
    fs_mdRtu->vu16RcvLen = 0;
    
    if(lu16_bufLen < MB_SER_PDU_SIZE_MIN)return;    //����֡������modbusЭ�鲻��
    if(fs_mdRtu->u8Addr != fs_mdRtu->u8RtuBuf[MB_SER_PDU_ADDR_OFF])return;    //����֡��ַ�ͱ�����ַ��ͬ
    
    eMBErrorCode    eStatus = MB_ENOERR;
    uint8_t lu8_FuncCode = fs_mdRtu->u8RtuBuf[MB_SER_PDU_PDU_OFF];//��ȡ������
    
    if( u16Get_mbCRC16(fs_mdRtu->u8RtuBuf, lu16_bufLen )==0 ){
        lu16_bufLen -= 3;   //-2 ȥ�� addr + crc16
        for( uint8_t i=0; i<MB_FUNC_HANDLERS_MAX; i++ ){
            if( xFuncHandlers[i].ucFunctionCode == lu8_FuncCode ){
                eStatus = xFuncHandlers[i].pxMBFunctionHandler( fs_mdRtu->u8RtuBuf+1, &lu16_bufLen ); 
                break;
            }
        }
        lu16_bufLen++;      //+1 ���� addr
    }
    else
        eStatus = MB_EILLCRC;
    //���������
    if( eStatus != MB_ENOERR ){
        fs_mdRtu->u8RtuBuf[MB_SER_PDU_PDU_OFF] = ( uint8_t )( lu8_FuncCode | MB_FUNC_ERROR );
        fs_mdRtu->u8RtuBuf[MB_SER_PDU_PDU_OFF+1] = eStatus;
        lu16_bufLen = 3;
    }
    //������Ӧ����
    if( fs_mdRtu->vu16RcvLen == 0){
        /* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
        uint16_t lu16_CRC16 = u16Get_mbCRC16( fs_mdRtu->u8RtuBuf, lu16_bufLen );
        fs_mdRtu->u8RtuBuf[lu16_bufLen++] = ( uint8_t )( lu16_CRC16 & 0xFF );  //crc������ǰ�������ں�
        fs_mdRtu->u8RtuBuf[lu16_bufLen++] = ( uint8_t )( lu16_CRC16 >> 8 ); 
        fs_mdRtu->u16SndLen = lu16_bufLen;
        /* Activate the transmitter. */
        fs_mdRtu->uartSend();
    }
    else
        eStatus = MB_EIO;
}

void modbusRTU_init(void)
{
    uart2_init();
    mdRtu_uart2Init();
}
void modbusRTU_poll(void)
{
	modbusRTU_handle(&mdRtu_uart2);
}
