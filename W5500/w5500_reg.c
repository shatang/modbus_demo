#include "w5500_reg.h"


#define S_RX_SIZE	2048
#define S_TX_SIZE	2048

static uint8_t lu8_wrMem[25];//写缓存
static uint8_t lu8_rdMem[25];//读缓存


static void write_nByte( uint8_t* fp8_buf, uint8_t fu8_len )
{
    for( uint8_t i=0; i<fu8_len; i++ )
        spi1_writeByte( fp8_buf[i] );
}

static void read_nByte( uint8_t fu8_len )
{
    for( uint8_t i=0; i<fu8_len; i++ )
        lu8_rdMem[i] = spi1_readByte();    
}
/******************************* W5500 Write Operation *******************************/
void Write_W5500_1Byte(uint16_t reg, uint8_t dat)
{
    /* Set W5500 SCS Low */
    W5500_CS_LOW;
    
	lu8_wrMem[0] = reg>>8;
    lu8_wrMem[1] = reg&0xff;
    lu8_wrMem[2] = FDM1|RWB_WRITE|COMMON_R;
    lu8_wrMem[3] = dat;
    write_nByte( lu8_wrMem, 4 );
    
    /* Set W5500 SCS High */
	W5500_CS_HIGH;
}
void Write_W5500_2Byte(uint16_t reg, uint16_t dat)
{
    /* Set W5500 SCS Low */
    W5500_CS_LOW;
    
	lu8_wrMem[0] = reg>>8;
    lu8_wrMem[1] = reg&0xff;
    lu8_wrMem[2] = FDM2|RWB_WRITE|COMMON_R;
    lu8_wrMem[3] = dat>>8;
	lu8_wrMem[4] = dat&0xff;
    write_nByte( lu8_wrMem, 5 );
    
    /* Set W5500 SCS High */
	W5500_CS_HIGH;
}
void Write_W5500_nByte(uint16_t reg, uint8_t *dat_ptr, uint8_t size)
{
	uint8_t i;

 	/* Set W5500 SCS Low */
	W5500_CS_LOW;
    
    lu8_wrMem[0] = reg>>8;
    lu8_wrMem[1] = reg&0xff;
    lu8_wrMem[2] = VDM|RWB_WRITE|COMMON_R;
    for(i=0;i<size;i++)
    {
        lu8_wrMem[3+i] = *dat_ptr;
        dat_ptr++;
    }
    write_nByte( lu8_wrMem, size+3 );

    /* Set W5500 SCS High*/
	W5500_CS_HIGH;
}
/******************************* W5500 Read Operation *******************************/
uint8_t Read_W5500_1Byte(uint16_t reg)
{
	/* Set W5500 SCS Low */
	W5500_CS_LOW;

    lu8_wrMem[0] = reg>>8;
    lu8_wrMem[1] = reg&0xff;
    lu8_wrMem[2] = FDM1|RWB_READ|COMMON_R;    
    write_nByte( lu8_wrMem, 3 );
    read_nByte( 1 );

	/* Set W5500 SCS High*/
	W5500_CS_HIGH;

	return lu8_rdMem[0];
}
/******************************* W5500 Write Socket Operation ***********************/
void Write_SOCK_1Byte(uint8_t socket, uint16_t reg, uint8_t dat)
{
	/* Set W5500 SCS Low */
	W5500_CS_LOW;

    lu8_wrMem[0] = reg>>8;
    lu8_wrMem[1] = reg&0xff;
    lu8_wrMem[2] = FDM1|RWB_WRITE|(socket*0x20+0x08);
    lu8_wrMem[3] = dat;
    write_nByte( lu8_wrMem, 4 );

	/* Set W5500 SCS High */
	W5500_CS_HIGH;
}
void Write_SOCK_2Byte(uint8_t socket, uint16_t reg, uint16_t dat)
{
	/* Set W5500 SCS Low */
	W5500_CS_LOW;

    lu8_wrMem[0] = reg>>8;
    lu8_wrMem[1] = reg&0xff;
    lu8_wrMem[2] = FDM2|RWB_WRITE|(socket*0x20+0x08);
    lu8_wrMem[3] = dat>>8;
	lu8_wrMem[4] = dat&0xff;
    write_nByte( lu8_wrMem, 5 );	

	/* Set W5500 SCS High */
	W5500_CS_HIGH;
}
void Write_SOCK_4Byte(uint8_t socket, uint16_t reg, uint8_t *dat_ptr)
{
	/* Set W5500 SCS Low */
	W5500_CS_LOW;

    lu8_wrMem[0] = reg>>8;
    lu8_wrMem[1] = reg&0xff;
	lu8_wrMem[2] = FDM4|RWB_WRITE|(socket*0x20+0x08);
    lu8_wrMem[3] = (*dat_ptr++);
    lu8_wrMem[4] = (*dat_ptr++);
    lu8_wrMem[5] = (*dat_ptr++);
    lu8_wrMem[6] = (*dat_ptr++);
    write_nByte( lu8_wrMem, 7 ); 

	/* Set W5500 SCS High */
	W5500_CS_HIGH;
}
/******************************* W5500 Read Socket Operation ************************/
uint8_t Read_SOCK_1Byte(uint8_t socket, uint16_t reg)
{
	/* Set W5500 SCS Low */
	W5500_CS_LOW;

    lu8_wrMem[0] = reg>>8;
    lu8_wrMem[1] = reg&0xff;
	lu8_wrMem[2] = FDM1|RWB_READ|(socket*0x20+0x08);
    write_nByte( lu8_wrMem, 3 );
    read_nByte( 1 );

	/* Set W5500 SCS High*/
	W5500_CS_HIGH;
    
	return lu8_rdMem[0];
}
uint16_t Read_SOCK_2Byte(uint8_t socket, uint16_t reg)
{
	uint16_t ReadWord=0;

    uint16_t i=0;
    
	/* Set W5500 SCS Low */
	W5500_CS_LOW;

    lu8_wrMem[0] = reg>>8;
    lu8_wrMem[1] = reg&0xff;
	lu8_wrMem[2] = FDM2|RWB_READ|(socket*0x20+0x08);
	write_nByte( lu8_wrMem, 3 );
	read_nByte( 2 );

    ReadWord = lu8_rdMem[0];
    ReadWord <<= 8;
    ReadWord |= lu8_rdMem[1];

	/* Set W5500 SCS High*/
	W5500_CS_HIGH;

	return ReadWord;
}
/******************** Read data from W5500 Socket data RX Buffer *******************/
uint16_t Read_SOCK_DataBuffer(uint8_t socket, uint8_t *dat_ptr)
{
	uint16_t rx_size;
	uint16_t Offset_H=0;
    uint16_t Offset_L=0;
	uint16_t i;
	rx_size=Read_SOCK_2Byte(socket,Sn_RX_RSR);
    
    rx_size=Read_SOCK_2Byte(0,Sn_RX_RSR);
	if(rx_size==0)		/* if no data received, return */
		return 0;
	if(rx_size>1460)
		rx_size=1460;

	Offset_H = Read_SOCK_2Byte(socket,Sn_RX_RD);
	Offset_L = Offset_H;
	Offset_H &= (S_RX_SIZE-1);		/* Calculate the real physical address */

	/* Set W5500 SCS Low */
	W5500_CS_LOW;

    lu8_wrMem[0] = Offset_H>>8;
    lu8_wrMem[1] = Offset_H&0xff;
	lu8_wrMem[2] = VDM|RWB_READ|(socket*0x20+0x18);
	write_nByte( lu8_wrMem, 3 );

	
	if((Offset_H+rx_size)<S_RX_SIZE)
	{
		/* Read Data */
		for(i=0;i<rx_size;i++)
		{
            read_nByte( 1 );
			*dat_ptr=lu8_rdMem[0];
			dat_ptr++;
		}
	}
	else
	{
		Offset_H = S_RX_SIZE - Offset_H;
		for(i=0;i<Offset_H;i++)
		{
            read_nByte( 1 );
			*dat_ptr=lu8_rdMem[0];
			dat_ptr++;
		}
		/* Set W5500 SCS High*/
		W5500_CS_HIGH;

		/* Set W5500 SCS Low */
		W5500_CS_LOW;
        
		lu8_wrMem[0] = 0x00;
        lu8_wrMem[1] = 0x00;
        lu8_wrMem[2] = VDM|RWB_READ|(socket*0x20+0x18);
        write_nByte( lu8_wrMem, 3 );
        
		for(;i<rx_size;i++)
		{
			read_nByte( 1 );
			*dat_ptr=lu8_rdMem[0];
			dat_ptr++;
		}
	}
	/* Set W5500 SCS High*/
	W5500_CS_HIGH;

	/* Update offset*/
	Offset_L += rx_size;
	Write_SOCK_2Byte(socket, Sn_RX_RD, Offset_L);
	Write_SOCK_1Byte(socket, Sn_CR, RECV);					/* Write RECV Command */
	
    return rx_size;
}
/******************** Write data to W5500 Socket data TX Buffer *******************/
void Write_SOCK_DataBuffer(uint8_t socket, uint8_t *dat_ptr, uint16_t size)
{
	uint16_t Offset_H=0;
    uint16_t Offset_L=0;
	uint16_t i;

	Offset_H=Read_SOCK_2Byte(socket,Sn_TX_WR);
	Offset_L=Offset_H;
	Offset_H&=(S_TX_SIZE-1);		/* Calculate the real physical address */

	/* Set W5500 SCS Low */
	W5500_CS_LOW;

    lu8_wrMem[0] = Offset_H>>8;
    lu8_wrMem[1] = Offset_H&0xff;
    lu8_wrMem[2] = VDM|RWB_WRITE|(socket*0x20+0x10);
	write_nByte( lu8_wrMem, 3 );

	if((Offset_H+size)<S_TX_SIZE)
	{
		/* Write Data */
		for(i=0;i<size;i++)
		{
            lu8_wrMem[0] = *dat_ptr;
            write_nByte( lu8_wrMem, 1 );
			dat_ptr++;
		}
	}
	else
	{
        //写入缓存(指针偏移)太大(未发送清空)，分为两段
        //第一段
		Offset_H = S_TX_SIZE - Offset_H;
		for(i=0;i<Offset_H;i++)
		{
			lu8_wrMem[0] = *dat_ptr;
            write_nByte( lu8_wrMem, 1 );
			dat_ptr++;
		}
		/* Set W5500 SCS High*/
		W5500_CS_HIGH;

		/* Set W5500 SCS Low */
		W5500_CS_LOW;
 
		lu8_wrMem[0] = 0x00;
        lu8_wrMem[1] = 0x00;
        lu8_wrMem[2] = VDM|RWB_WRITE|(socket*0x20+0x10);
        write_nByte( lu8_wrMem, 3 );
        //第二段
		for(;i<size;i++)
		{
            lu8_wrMem[0] = *dat_ptr;
            write_nByte( lu8_wrMem, 1 );
			dat_ptr++;
		}
	}
	/* Set W5500 SCS High*/
	W5500_CS_HIGH;

	/* Updata offset */
	Offset_L+=size;//计算得出的实际值可能超过2048，但实际只发送该最大缓存数组
	Write_SOCK_2Byte(socket, Sn_TX_WR, Offset_L);
	Write_SOCK_1Byte(socket, Sn_CR, SEND);					/* Write SEND Command */
}

