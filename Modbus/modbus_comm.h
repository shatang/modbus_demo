#ifndef __MODBUS_COMM_H
#define __MODBUS_COMM_H
#include <stdint.h>


typedef enum
{
    MB_ENOERR=0x00,             /*!< no error. */
    MB_EINFUN=0x01,             //�Ƿ�������
    MB_ENOREG=0x02,             //�Ƿ��Ĵ�����ַ
    MB_EINVAL=0x03,             //�Ƿ�����(�������ݰ���ʽ)
    MB_EIO=0x04,                //���豸ʧ��
    MB_EILLCRC=0x08,            //CRCУ�����
}eMBErrorCode;

typedef struct
{
    uint16_t        ucFunctionCode;
    eMBErrorCode    ( *pxMBFunctionHandler ) ( uint8_t* , uint16_t*);
}xMBFunctionHandler;



#define MB_FUNC_HANDLERS_MAX    7
extern const xMBFunctionHandler xFuncHandlers[MB_FUNC_HANDLERS_MAX];

#endif
