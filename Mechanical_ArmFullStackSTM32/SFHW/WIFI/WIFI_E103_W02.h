#ifndef __WIFI_E103_W02_H
#define __WIFI_E103_W02_H

#include "stm32f4xx.h" 

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
	  	
extern u8  WIFI_USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 WIFI_USART_RX_STA;         		//����״̬���	

void E103_W02_SendData( uint8_t *Data,int l);//ͨ�����ڲٿ�WIFIģ�鷢�����ݵ�������
void init_E103_w02(void);//��ʼ��WIFIģ�飨������ź�ͨ�ŵĴ��ڣ�
char link_server(void);//ִ�����ӷ��������ܣ�����STAģʽ������·�������ȵ��˺����롢����Զ��IP��
char restE103_W02(void);//WIFIģ��ָ����������ù���
u8 getLinkStatus(void);//��ȡWIFIģ�����ӷ�����״̬
void rest();
#endif

