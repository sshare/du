
#ifndef COMMONUSART_H_
#define COMMONUSART_H_

//#include "sys.h" 


#include "stm32f4xx_conf.h"
#include "stdio.h"
#include "stm32f4xx_conf.h"

  typedef struct 
	{
	//引脚定义
USART_TypeDef* COMMON_USART;//要使能的串口    eg:USART1;
	__IO uint32_t COMMON_USART_CLK;// 要使能的串口时钟 eg:RCC_APB2Periph_USART1;
__IO  uint32_t COMMON_USART_BAUDRATE ;// 115200;  //串口波特率

GPIO_TypeDef* COMMON_USART_RX_GPIO_PORT ;//要使能的IO  GPIOX X A~G
__IO  uint32_t COMMON_USART_RX_GPIO_CLK ;//IO所在的时钟                RCC_AHB1Periph_GPIOA;
__IO  uint32_t COMMON_USART_RX_PIN ;//串口IO RX
 __IO uint16_t COMMON_USART_RX_SOURCE ;//  复用串口来源（就是将读串口复用到那个IO下）                GPIO_PinSource10;

GPIO_TypeDef* COMMON_USART_TX_GPIO_PORT;//要使能的IO GPIOX X A~G
__IO  uint32_t COMMON_USART_TX_GPIO_CLK ;//IO所在的时钟                 RCC_AHB1Periph_GPIOA;
 __IO uint32_t COMMON_USART_TX_PIN ;//串口IO TX
 __IO uint16_t COMMON_USART_TX_SOURCE ;//  复用串口来源（就是将读串口复用到那个IO下）                  GPIO_PinSource9;

 //GPRS_USART_IRQHandler                  USART1_IRQHandler;
	__IO uint8_t COMMON_USART_IRQ ;//中断用到的IRQ通道  eg:USART1_IRQn;
__IO uint8_t  COMMON_USART_AF;        //GPIO_AF_USART1
}COMMON_USART_PARAMET;
 //extern GPRS_USART_PARAMET paramet;//储存初始化GPRS的相关参数
void COMMON_USART_OUT(USART_TypeDef* USARTx,uint8_t *Data, int len);

void initialCOMMON_USART( COMMON_USART_PARAMET paramet_);


#endif






