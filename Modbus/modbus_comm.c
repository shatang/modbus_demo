#include "modbus_comm.h"

extern eMBErrorCode eMBRegHolding_read( uint8_t* fp8_regBuf, uint16_t fu16_addr, uint16_t fu16_nRegs );
extern eMBErrorCode eMBRegHolding_write( uint8_t* fp8_regBuf, uint16_t fu16_addr, uint16_t fu16_nRegs );
extern eMBErrorCode eMBRegCoils_read( uint8_t* fp8_regBuf, uint16_t fu16_addr, uint16_t fu16_nRegs );
extern eMBErrorCode eMBRegCoils_write( uint8_t* fp8_regBuf, uint16_t fu16_addr, uint16_t fu16_nRegs );
extern eMBErrorCode eMBRegInputCB( uint8_t* fp8_regBuf, uint16_t fu16_addr, uint16_t fu16_nRegs );


static eMBErrorCode eMBFuncReadHoldingRegister( uint8_t* fp8_frame, uint16_t* fp16_Len );
static eMBErrorCode eMBFuncWriteHoldingRegister( uint8_t* fp8_frame, uint16_t* fp16_Len );
static eMBErrorCode eMBFuncWriteMultipleHoldingRegister( uint8_t* fp8_frame, uint16_t* fp16_Len );
static eMBErrorCode eMBFuncReadCoils( uint8_t* fp8_frame, uint16_t* fp16_Len );
static eMBErrorCode eMBFuncWriteCoil( uint8_t* fp8_frame, uint16_t* fp16_Len );
static eMBErrorCode eMBFuncWriteMultipleCoils( uint8_t* fp8_frame, uint16_t* fp16_Len );
static eMBErrorCode eMBFuncReadInputRegister( uint8_t* fp8_frame, uint16_t* fp16_Len );


const xMBFunctionHandler xFuncHandlers[MB_FUNC_HANDLERS_MAX] = {
    {0x03, eMBFuncReadHoldingRegister},             //READ_HOLDING_REGISTER
    {0x04, eMBFuncReadInputRegister},               //REG_INPUT_CB

    {0x06, eMBFuncWriteHoldingRegister},            //WRITE_REGISTER
    {0x10, eMBFuncWriteMultipleHoldingRegister},    //WRITE_MULTIPLE_REGISTERS

    {0x01, eMBFuncReadCoils},                       //READ_COILS
    {0x05, eMBFuncWriteCoil},                       //WRITE_COILS
    {0x0f, eMBFuncWriteMultipleCoils},              //WRITE_MULTIPLE_COILS
};

static eMBErrorCode eMBFuncReadHoldingRegister( uint8_t* fp8_frame, uint16_t* fp16_Len )
{
    uint16_t        lu16_regAddr;
    uint16_t        lu16_regCnt;
    uint8_t*        lp8_frameCur;
    
    eMBErrorCode    eStatus = MB_ENOERR;

    if( *fp16_Len == 5 )   //funCode + regAddr + regCount = 1+2+2
    {
        lu16_regAddr = ( uint16_t )( fp8_frame[1] << 8 );
        lu16_regAddr |= ( uint16_t )( fp8_frame[2] );
        lu16_regCnt = ( uint16_t )( fp8_frame[3] << 8 );
        lu16_regCnt = ( uint16_t )( fp8_frame[4] );

        if( ( lu16_regCnt >= 1 ) && ( lu16_regCnt <= 0x7d ) ) //MB_PDU_FUNC_READ_REGCNT_MAX   125
        {
            /* First byte contains the function code. */
            lp8_frameCur = &fp8_frame[0]; 
            *lp8_frameCur++ = 0x03;
            *fp16_Len = 1;
            /* Second byte in the response contain the number of bytes. */
            *lp8_frameCur++ = ( uint16_t ) ( lu16_regCnt * 2 );
            *fp16_Len += 1;
            /* Make callback to fill the buffer. */
            eStatus = eMBRegHolding_read( lp8_frameCur, lu16_regAddr, lu16_regCnt);
            if( eStatus == MB_ENOERR )
                *fp16_Len += lu16_regCnt * 2;
        }
        else
            eStatus = MB_EINVAL;
    }
    else
        eStatus = MB_EINVAL;

    return eStatus;
}

static eMBErrorCode eMBFuncWriteHoldingRegister( uint8_t * fp8_frame, uint16_t * fp16_Len )
{
    uint16_t        lu16_regAddr;
    eMBErrorCode    eStatus = MB_ENOERR;

    if( *fp16_Len == 5 )   //funCode + regAddr + writeVal = 1+2+2
    {
        lu16_regAddr = ( uint16_t )( fp8_frame[1] << 8 );
        lu16_regAddr |= ( uint16_t )( fp8_frame[2] );
        /* Make callback to update the value. */
        eStatus = eMBRegHolding_write( &fp8_frame[3], lu16_regAddr, 1);
    }
    else
        eStatus = MB_EINVAL;

    return eStatus;
}

static eMBErrorCode eMBFuncWriteMultipleHoldingRegister( uint8_t * fp8_frame, uint16_t * fp16_Len )
{
    uint16_t        lu16_regAddr;
    uint16_t        lu16_regCnt;
    uint8_t         lu8_regByteCnt;

    eMBErrorCode    eStatus = MB_ENOERR;

    if( *fp16_Len >= 6 )   //funCode + regAddr + regCount + regbyteNum = 1+2+2+1
    {
        lu16_regAddr = ( uint16_t )( fp8_frame[1] << 8 );
        lu16_regAddr |= ( uint16_t )( fp8_frame[2] );
        lu16_regCnt = ( uint16_t )( fp8_frame[3] << 8 );
        lu16_regCnt |= ( uint16_t )( fp8_frame[4] );
        lu8_regByteCnt = fp8_frame[5];
        
        //MB_PDU_FUNC_WRITE_MUL_REGCNT_MAX  120
        if(( lu16_regCnt >= 1 ) && ( lu16_regCnt <= 0x78 ) && ( lu8_regByteCnt == ( uint8_t )( 2 * lu16_regCnt )))
        {
            /* Make callback to update the register values. */
            eStatus = eMBRegHolding_write( &fp8_frame[6], lu16_regAddr, lu16_regCnt);
            /* If an error occured convert it into a Modbus exception. */
            if( eStatus == MB_ENOERR )
                *fp16_Len = 5;
        }
        else
            eStatus = MB_EINVAL;
    }
    else
        eStatus = MB_EINVAL;
    
    return eStatus;
}

static eMBErrorCode eMBFuncReadCoils( uint8_t * fp8_frame, uint16_t * fp16_Len )
{
    uint16_t        lu16_regAddr;
    uint16_t        lu16_coilCnt;
    uint8_t         lu8_nBytes;
    uint8_t*        lp8_frameCur;

    eMBErrorCode    eStatus = MB_ENOERR;

    if( *fp16_Len == 5 )   //funCode + coilAddr + coilCount = 1+2+2
    {
        lu16_regAddr = ( uint16_t )( fp8_frame[1] << 8 );
        lu16_regAddr |= ( uint16_t )( fp8_frame[2] );
        lu16_coilCnt = ( uint16_t )( fp8_frame[3] << 8 );
        lu16_coilCnt |= ( uint16_t )( fp8_frame[4] );

        if( ( lu16_coilCnt >= 1 ) && ( lu16_coilCnt < 0x7d0 ) )    //MB_PDU_FUNC_READ_COILCNT_MAX  2000
        {
            /* First byte contains the function code. */
            lp8_frameCur = &fp8_frame[0];
            *lp8_frameCur++ = 1;
            *fp16_Len = 1;
            /* Test if the quantity of coils is a multiple of 8. If not last
             * byte is only partially field with unused coils set to zero. */
            if( ( lu16_coilCnt & 0x0007 ) != 0 )
                lu8_nBytes = ( uint8_t )( lu16_coilCnt / 8 + 1 );
            else
                lu8_nBytes = ( uint8_t )( lu16_coilCnt / 8 );

            *lp8_frameCur++ = lu8_nBytes;
            *fp16_Len += 1;

            eStatus = eMBRegCoils_read( lp8_frameCur, lu16_regAddr, lu16_coilCnt);
            if( eStatus == MB_ENOERR )
                *fp16_Len += lu8_nBytes;
        }
        else
            eStatus = MB_EINVAL;
    }
    else
        eStatus = MB_EINVAL;
    
    return eStatus;
}

static eMBErrorCode eMBFuncWriteCoil( uint8_t * fp8_frame, uint16_t * fp16_Len )
{
    uint16_t        lu16_regAddr;
    uint8_t         lu8_buf[2];

    eMBErrorCode    eStatus = MB_ENOERR;

    if( *fp16_Len == 5 )    //funCode + coilAddr + coilCount = 1+1+2+2
    {
        lu16_regAddr = ( uint16_t )( fp8_frame[1] << 8 );
        lu16_regAddr |= ( uint16_t )( fp8_frame[2] );

        if(( fp8_frame[4] == 0x00 ) && (( fp8_frame[3] == 0xFF ) || ( fp8_frame[3] == 0x00 )))
        {
            lu8_buf[1] = 0;
            if( fp8_frame[3] == 0xFF )
                lu8_buf[0] = 1;
            else
                lu8_buf[0] = 0;
            eStatus = eMBRegCoils_write( &lu8_buf[0], lu16_regAddr, 1);
        }
        else
            eStatus = MB_EINVAL;
    }
    else
        eStatus = MB_EINVAL;

    return eStatus;
}

static eMBErrorCode eMBFuncWriteMultipleCoils( uint8_t * fp8_frame, uint16_t * fp16_Len )
{
    uint16_t        lu16_regAddr;
    uint16_t        lu16_coilCnt;
    uint8_t         lu8_byteCnt;
    uint8_t         lu8_byteCntVerify;

    eMBErrorCode    eStatus = MB_ENOERR;

    if( *fp16_Len > 5 ) //funCode + coilAddr + coilCount = 1+2+2
    {
        lu16_regAddr = ( uint16_t )( fp8_frame[1] << 8 );
        lu16_regAddr |= ( uint16_t )( fp8_frame[2] );
        lu16_coilCnt = ( uint16_t )( fp8_frame[3] << 8 );
        lu16_coilCnt |= ( uint16_t )( fp8_frame[4] );
        lu8_byteCnt = fp8_frame[5];

        /* Compute the number of expected bytes in the request. */
        if( ( lu8_byteCnt & 0x0007 ) != 0 )
            lu8_byteCntVerify = ( uint8_t )( lu8_byteCnt / 8 + 1 );
        else
            lu8_byteCntVerify = ( uint8_t )( lu8_byteCnt / 8 );

        //MB_PDU_FUNC_WRITE_MUL_COILCNT_MAX     1968
        if(( lu16_coilCnt >= 1 ) && ( lu16_coilCnt <= 0x7B0 ) && ( lu8_byteCntVerify == lu8_byteCnt ))
        {
            eStatus = eMBRegCoils_write( &fp8_frame[6], lu16_regAddr, lu16_coilCnt);
            if( eStatus == MB_ENOERR )
                *fp16_Len = 5;
        }
        else
            eStatus = MB_EINVAL;
    }
    else
        eStatus = MB_EINVAL;
    
    return eStatus;
}

static eMBErrorCode eMBFuncReadInputRegister( uint8_t * fp8_frame, uint16_t * fp16_Len )
{
    uint16_t          lu16_regAddr;
    uint16_t          lu16_regCnt;
    uint8_t*          lp8_frameCur;
    
    eMBErrorCode    eStatus = MB_ENOERR;

    if( *fp16_Len == 5 )       //funCode + regAddr + regCount = 1+2+2
    {
        lu16_regAddr = ( uint16_t )( fp8_frame[1] << 8 );
        lu16_regAddr |= ( uint16_t )( fp8_frame[2] );
        lu16_regCnt = ( uint16_t )( fp8_frame[3] << 8 );
        lu16_regCnt |= ( uint16_t )( fp8_frame[4] );

        if( ( lu16_regCnt >= 1 ) && ( lu16_regCnt < 0x7d ) )   //MB_PDU_FUNC_READ_REGCNT_MAX   125
        {
            /* First byte contains the function code. */
            lp8_frameCur = &fp8_frame[0];
            *lp8_frameCur++ = 0x04;
            *fp16_Len = 1;

            /* Second byte in the response contain the number of bytes. */
            *lp8_frameCur++ = ( uint8_t )( lu16_regCnt * 2 );
            *fp16_Len += 1;

            eStatus = eMBRegInputCB( lp8_frameCur, lu16_regAddr, lu16_regCnt );
            if( eStatus == MB_ENOERR )
                *fp16_Len += lu16_regCnt * 2;
        }
        else
            eStatus = MB_EINVAL;
    }
    else
        eStatus = MB_EINVAL;
    
    return eStatus;
}
