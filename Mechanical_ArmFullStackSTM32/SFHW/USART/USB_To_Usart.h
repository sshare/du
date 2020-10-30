#ifndef __USB_TO_USART_H
#define __USB_TO_USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
//////////////////////////////////////////////////////////////////////////////////	 

////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_UART5_RX 			1		//ʹ�ܣ�1��/��ֹ��0������5����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
void SendDataToDevice_a(unsigned char * src, char len);
#endif


