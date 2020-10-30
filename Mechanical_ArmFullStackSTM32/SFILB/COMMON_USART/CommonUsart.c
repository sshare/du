
#include "CommonUsart.h"
#include <string.h>


//#include "sys.h"
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOSʹ��	  
#endif

////////////////////////////////////////////////////////////////////////////////// 	 

//////////////////////////////////////////////////////////////////////////////////	 
//���õĴ��ڹ��ߣ�F407��
//2019-09-14
//huzhikun
//����initialCOMMON_USART ����
//�������òο�COMMON_USART_PARAMET paramet
////////////////////////////////////////////////////////////////////////////////// 	  
 


//���� ���ڲ���
 void COMMON_USART_Config(USART_TypeDef* USARTx, unsigned int baud)
{
	//   //USART1 ��ʼ������
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = baud;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USARTx, &USART_InitStructure); //��ʼ������
	USART_Cmd(USARTx, ENABLE);  //ʹ�ܴ��� 	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);//��������ж�	
	//USARTx->SR &= ~(1 << 6);		
}
//ʹ�� ���ڡ����ڶ���д��ʱ��
 void COMMON_RCC_Config(COMMON_USART_PARAMET paramet){
	RCC_AHB1PeriphClockCmd(paramet.COMMON_USART_TX_GPIO_CLK,ENABLE); //ʹ��GPIOXʱ��
	RCC_AHB1PeriphClockCmd(paramet.COMMON_USART_RX_GPIO_CLK,ENABLE); //ʹ��GPIOXʱ��
	if(RCC_APB2Periph_USART6!=paramet.COMMON_USART_CLK&&RCC_APB2Periph_USART1!=paramet.COMMON_USART_CLK){
			RCC_APB1PeriphClockCmd(paramet.COMMON_USART_CLK,ENABLE);//ʹ��USARTXʱ��
		 
	 }else{
			RCC_APB2PeriphClockCmd(paramet.COMMON_USART_CLK,ENABLE);//ʹ��USARTXʱ��
	 }
}

//���ڵĸ��ú�io���ŵĳ�ʼ��
 void COMMON_GPIO_Config(COMMON_USART_PARAMET paramet)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//���ڶ� �ĸ���
	GPIO_PinAFConfig(paramet.COMMON_USART_RX_GPIO_PORT,
	paramet.COMMON_USART_RX_SOURCE,
	paramet.COMMON_USART_AF);
	//����д�ĸ���
	GPIO_PinAFConfig(paramet.COMMON_USART_TX_GPIO_PORT,
	paramet.COMMON_USART_TX_SOURCE,
	paramet.COMMON_USART_AF); 
	//��ʼ�����ڶ���io����
	GPIO_InitStructure.GPIO_Pin = paramet.COMMON_USART_RX_PIN; //0-15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(paramet.COMMON_USART_RX_GPIO_PORT,&GPIO_InitStructure); 
	
	//��ʼ������д ��io����
	GPIO_InitStructure.GPIO_Pin = paramet.COMMON_USART_TX_PIN; //0-15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(paramet.COMMON_USART_TX_GPIO_PORT,&GPIO_InitStructure); 	
}
//���� �ж� ����
 void COMMON_NVIC_Config(COMMON_USART_PARAMET paramet)
{
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = paramet.COMMON_USART_IRQ;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//��ʼ�� ���ڹ��� ���ݲ��� paramet
void initialCOMMON_USART(COMMON_USART_PARAMET paramet)
{
	
	COMMON_RCC_Config(paramet);
	COMMON_GPIO_Config(paramet);
	COMMON_NVIC_Config(paramet);
	COMMON_USART_Config( paramet.COMMON_USART, paramet.COMMON_USART_BAUDRATE);
}
//�������ݵ� USARTx ���� �ɲο�paramet.COMMON_USART
//USARTx ����id
//Data ���������
//len ���ݳ���
void COMMON_USART_OUT(USART_TypeDef* USARTx,uint8_t *Data, int len){
	if(USARTx==NULL){
			
		//USB_To_Usart_SendData("USARTx=null", 11);
	//	SendDataToDevice_b("USARTx=null", 11);
		return;
	}
	USARTx->SR;
	while(len--){
			USART_SendData(USARTx, *Data++);
			while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	}
}


 



