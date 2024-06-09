#ifndef __MODBUS_COMM_H
#define __MODBUS_COMM_H
#include <stdint.h>


typedef enum
{
    MB_ENOERR=0x00,             /*!< no error. */
    MB_EINFUN=0x01,             //非法功能码
    MB_ENOREG=0x02,             //非法寄存器地址
    MB_EINVAL=0x03,             //非法数据(包括数据包格式)
    MB_EIO=0x04,                //从设备失败
    MB_EILLCRC=0x08,            //CRC校验错误
}eMBErrorCode;

typedef struct
{
    uint16_t        ucFunctionCode;
    eMBErrorCode    ( *pxMBFunctionHandler ) ( uint8_t* , uint16_t*);
}xMBFunctionHandler;



#define MB_FUNC_HANDLERS_MAX    7
extern const xMBFunctionHandler xFuncHandlers[MB_FUNC_HANDLERS_MAX];

#endif
