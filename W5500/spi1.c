#include "spi1.h"


SPI_HandleTypeDef       hspi1;
    
/* SPI Initialization  */
//STM32F429��SPI1�������Ϊ42Mhz
void spi1_init(void)
{
	GPIO_InitTypeDef 		GPIO_InitStruct;

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_SPI1_CLK_ENABLE();
    
    /**SPI1 GPIO Configuration    
    PB3     ------> SPI1_SCK
    PB4     ------> SPI1_MISO
    PB5     ------> SPI1_MOSI
    */
  	GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
  	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, GPIO_PIN_SET);
    
	/* Set Chip Select pin */
	GPIO_InitStruct.Pin = GPIO_PIN_14;
	GPIO_InitStruct.Mode=  GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);

	/* Set SPI interface */
    hspi1.Instance = SPI1;                                     /* SPI1 */
    hspi1.Init.Mode = SPI_MODE_MASTER;                         /* ����SPI����ģʽ������Ϊ��ģʽ */
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;               /* ����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ */
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;                   /* ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ */
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;                 /* ����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ */
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;                     /* ����ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ����� */
    hspi1.Init.NSS = SPI_NSS_SOFT;                             /* NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ���� */
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;    /* ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ4 */
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;                    /* ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ */
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;                    /* �ر�TIģʽ */
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;    /* �ر�Ӳ��CRCУ�� */
    hspi1.Init.CRCPolynomial = 7;                              /* CRCֵ����Ķ���ʽ */
    HAL_SPI_Init(&hspi1);                                      /* ��ʼ�� */
    
    __HAL_SPI_ENABLE(&hspi1); /* ʹ��SPI1 */
}


uint8_t spi1_readByte(void)
{
    uint8_t ReadBuf = hspi1.Instance->DR;
    hspi1.Instance->DR = 0x00;
    while(__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_TXE) == RESET);
    ReadBuf = hspi1.Instance->DR;  //���ӻ�����
    
    return ReadBuf;
}

void spi1_writeByte(uint8_t fu8_buf)
{
    hspi1.Instance->DR = fu8_buf;  //д�ӻ�����
    while(__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_TXE) == RESET);
}

//����Ϊδ����д��������Ϊ�Ѳ��Կ��õĳ���д��
//void spi1_readByte(uint8_t* fu8_mem, uint8_t fu8_length)
//{
//	HAL_SPI_Receive(&hspi1, fu8_mem, fu8_length, 1000);
//}

//void spi1_writeByte(uint8_t* fu8_mem, uint8_t fu8_length)
//{
//	HAL_SPI_Transmit(&hspi1, fu8_mem, fu8_length, 1000);
//}


