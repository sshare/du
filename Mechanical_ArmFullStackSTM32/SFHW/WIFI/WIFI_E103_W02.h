#ifndef __WIFI_E103_W02_H
#define __WIFI_E103_W02_H

#include "stm32f4xx.h" 

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
	  	
extern u8  WIFI_USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 WIFI_USART_RX_STA;         		//接收状态标记	

void E103_W02_SendData( uint8_t *Data,int l);//通过串口操控WIFI模组发送数据到服务器
void init_E103_w02(void);//初始化WIFI模块（相关引脚和通信的串口）
char link_server(void);//执行链接服务器功能（设置STA模式、设置路由器或热点账号密码、设置远程IP）
char restE103_W02(void);//WIFI模块恢复出厂化设置功能
u8 getLinkStatus(void);//获取WIFI模组连接服务器状态
void rest();
#endif

