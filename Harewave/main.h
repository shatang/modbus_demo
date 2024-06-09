#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"


#include "stdio.h"

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

#define COM_Pin GPIO_PIN_3
#define COM_GPIO_Port GPIOE
#define AL_Pin GPIO_PIN_4
#define AL_GPIO_Port GPIOE
#define ADC3_IN9_Pin GPIO_PIN_3
#define ADC3_IN9_GPIO_Port GPIOF
#define ADC3_IN14_Pin GPIO_PIN_4
#define ADC3_IN14_GPIO_Port GPIOF
#define ADC3_IN15_Pin GPIO_PIN_5
#define ADC3_IN15_GPIO_Port GPIOF
#define ADC3_IN4_Pin GPIO_PIN_6
#define ADC3_IN4_GPIO_Port GPIOF
#define ADC3_IN5_Pin GPIO_PIN_7
#define ADC3_IN5_GPIO_Port GPIOF
#define ADC3_IN6_Pin GPIO_PIN_8
#define ADC3_IN6_GPIO_Port GPIOF
#define ADC3_IN7_Pin GPIO_PIN_9
#define ADC3_IN7_GPIO_Port GPIOF
#define RCC_OSC_IN_Pin GPIO_PIN_0
#define RCC_OSC_IN_GPIO_Port GPIOH
#define RCC_OSC_OUT_Pin GPIO_PIN_1
#define RCC_OSC_OUT_GPIO_Port GPIOH
#define ETH_MDC_Pin GPIO_PIN_1
#define ETH_MDC_GPIO_Port GPIOC
#define ETH_REF_CLK_Pin GPIO_PIN_1
#define ETH_REF_CLK_GPIO_Port GPIOA
#define ETH_MDIO_Pin GPIO_PIN_2
#define ETH_MDIO_GPIO_Port GPIOA
#define ETH_CRS_DV_Pin GPIO_PIN_7
#define ETH_CRS_DV_GPIO_Port GPIOA
#define ETH_RXD0_Pin GPIO_PIN_4
#define ETH_RXD0_GPIO_Port GPIOC
#define ETH_RXD1_Pin GPIO_PIN_5
#define ETH_RXD1_GPIO_Port GPIOC
#define RTH_TX_EN_Pin GPIO_PIN_11
#define RTH_TX_EN_GPIO_Port GPIOB
#define ETH_TXD0_Pin GPIO_PIN_12
#define ETH_TXD0_GPIO_Port GPIOB
#define ETH_TXD1_Pin GPIO_PIN_13
#define ETH_TXD1_GPIO_Port GPIOB
#define USART3_DIR_Pin GPIO_PIN_15
#define USART3_DIR_GPIO_Port GPIOB
#define USART3_TX_Pin GPIO_PIN_8
#define USART3_TX_GPIO_Port GPIOD
#define USART3_RX_Pin GPIO_PIN_9
#define USART3_RX_GPIO_Port GPIOD
#define DO_S0_Pin GPIO_PIN_2
#define DO_S0_GPIO_Port GPIOG
#define DO_S1_Pin GPIO_PIN_3
#define DO_S1_GPIO_Port GPIOG
#define DO_S2_Pin GPIO_PIN_4
#define DO_S2_GPIO_Port GPIOG
#define DO_S3_Pin GPIO_PIN_5
#define DO_S3_GPIO_Port GPIOG
#define DO_S4_Pin GPIO_PIN_6
#define DO_S4_GPIO_Port GPIOG
#define DI_S0_Pin GPIO_PIN_7
#define DI_S0_GPIO_Port GPIOG
#define DI_S1_Pin GPIO_PIN_8
#define DI_S1_GPIO_Port GPIOG
#define DI_S2_Pin GPIO_PIN_6
#define DI_S2_GPIO_Port GPIOC
#define DI_S3_Pin GPIO_PIN_7
#define DI_S3_GPIO_Port GPIOC
#define DI_S4_Pin GPIO_PIN_8
#define DI_S4_GPIO_Port GPIOC
#define DI_S5_Pin GPIO_PIN_9
#define DI_S5_GPIO_Port GPIOC
#define USART1_DIR_Pin GPIO_PIN_8
#define USART1_DIR_GPIO_Port GPIOA
#define USART1_TX_Pin GPIO_PIN_9
#define USART1_TX_GPIO_Port GPIOA
#define USART1_RX_Pin GPIO_PIN_10
#define USART1_RX_GPIO_Port GPIOA
#define JTMS_SWDIO_Pin GPIO_PIN_13
#define JTMS_SWDIO_GPIO_Port GPIOA
#define JTCK_SWCLK_Pin GPIO_PIN_14
#define JTCK_SWCLK_GPIO_Port GPIOA
#define CAN1_RX_Pin GPIO_PIN_0
#define CAN1_RX_GPIO_Port GPIOD
#define CAN1_TX_Pin GPIO_PIN_1
#define CAN1_TX_GPIO_Port GPIOD
#define USART2_TX_Pin GPIO_PIN_5
#define USART2_TX_GPIO_Port GPIOD
#define USART2_RX_Pin GPIO_PIN_6
#define USART2_RX_GPIO_Port GPIOD
#define USART2_DIR_Pin GPIO_PIN_7
#define USART2_DIR_GPIO_Port GPIOD
#define CAN2_RX_Pin GPIO_PIN_5
#define CAN2_RX_GPIO_Port GPIOB
#define CAN2_TX_Pin GPIO_PIN_6
#define CAN2_TX_GPIO_Port GPIOB


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
