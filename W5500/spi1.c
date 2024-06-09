#include "spi1.h"


SPI_HandleTypeDef       hspi1;
    
/* SPI Initialization  */
//STM32F429的SPI1最大速率为42Mhz
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
    hspi1.Init.Mode = SPI_MODE_MASTER;                         /* 设置SPI工作模式，设置为主模式 */
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;               /* 设置SPI单向或者双向的数据模式:SPI设置为双线模式 */
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;                   /* 设置SPI的数据大小:SPI发送接收8位帧结构 */
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;                 /* 串行同步时钟的空闲状态为低电平 */
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;                     /* 串行同步时钟的第一个跳变沿（上升或下降）数据被采样 */
    hspi1.Init.NSS = SPI_NSS_SOFT;                             /* NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制 */
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;    /* 定义波特率预分频的值:波特率预分频值为4 */
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;                    /* 指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始 */
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;                    /* 关闭TI模式 */
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;    /* 关闭硬件CRC校验 */
    hspi1.Init.CRCPolynomial = 7;                              /* CRC值计算的多项式 */
    HAL_SPI_Init(&hspi1);                                      /* 初始化 */
    
    __HAL_SPI_ENABLE(&hspi1); /* 使能SPI1 */
}


uint8_t spi1_readByte(void)
{
    uint8_t ReadBuf = hspi1.Instance->DR;
    hspi1.Instance->DR = 0x00;
    while(__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_TXE) == RESET);
    ReadBuf = hspi1.Instance->DR;  //读从机数据
    
    return ReadBuf;
}

void spi1_writeByte(uint8_t fu8_buf)
{
    hspi1.Instance->DR = fu8_buf;  //写从机数据
    while(__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_TXE) == RESET);
}

//上面为未测试写法，下面为已测试可用的常规写法
//void spi1_readByte(uint8_t* fu8_mem, uint8_t fu8_length)
//{
//	HAL_SPI_Receive(&hspi1, fu8_mem, fu8_length, 1000);
//}

//void spi1_writeByte(uint8_t* fu8_mem, uint8_t fu8_length)
//{
//	HAL_SPI_Transmit(&hspi1, fu8_mem, fu8_length, 1000);
//}


