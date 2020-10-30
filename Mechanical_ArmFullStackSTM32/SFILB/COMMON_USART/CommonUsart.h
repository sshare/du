
#ifndef COMMONUSART_H_
#define COMMONUSART_H_

//#include "sys.h" 


#include "stm32f4xx_conf.h"
#include "stdio.h"
#include "stm32f4xx_conf.h"

  typedef struct 
	{
	//���Ŷ���
USART_TypeDef* COMMON_USART;//Ҫʹ�ܵĴ���    eg:USART1;
	__IO uint32_t COMMON_USART_CLK;// Ҫʹ�ܵĴ���ʱ�� eg:RCC_APB2Periph_USART1;
__IO  uint32_t COMMON_USART_BAUDRATE ;// 115200;  //���ڲ�����

GPIO_TypeDef* COMMON_USART_RX_GPIO_PORT ;//Ҫʹ�ܵ�IO  GPIOX X A~G
__IO  uint32_t COMMON_USART_RX_GPIO_CLK ;//IO���ڵ�ʱ��                RCC_AHB1Periph_GPIOA;
__IO  uint32_t COMMON_USART_RX_PIN ;//����IO RX
 __IO uint16_t COMMON_USART_RX_SOURCE ;//  ���ô�����Դ�����ǽ������ڸ��õ��Ǹ�IO�£�                GPIO_PinSource10;

GPIO_TypeDef* COMMON_USART_TX_GPIO_PORT;//Ҫʹ�ܵ�IO GPIOX X A~G
__IO  uint32_t COMMON_USART_TX_GPIO_CLK ;//IO���ڵ�ʱ��                 RCC_AHB1Periph_GPIOA;
 __IO uint32_t COMMON_USART_TX_PIN ;//����IO TX
 __IO uint16_t COMMON_USART_TX_SOURCE ;//  ���ô�����Դ�����ǽ������ڸ��õ��Ǹ�IO�£�                  GPIO_PinSource9;

 //GPRS_USART_IRQHandler                  USART1_IRQHandler;
	__IO uint8_t COMMON_USART_IRQ ;//�ж��õ���IRQͨ��  eg:USART1_IRQn;
__IO uint8_t  COMMON_USART_AF;        //GPIO_AF_USART1
}COMMON_USART_PARAMET;
 //extern GPRS_USART_PARAMET paramet;//�����ʼ��GPRS����ز���
void COMMON_USART_OUT(USART_TypeDef* USARTx,uint8_t *Data, int len);

void initialCOMMON_USART( COMMON_USART_PARAMET paramet_);


#endif






