
#include "CommonUsart.h"
#include <string.h>


//#include "sys.h"
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOS使用	  
#endif

////////////////////////////////////////////////////////////////////////////////// 	 

//////////////////////////////////////////////////////////////////////////////////	 
//公用的串口工具（F407）
//2019-09-14
//huzhikun
//调用initialCOMMON_USART 方法
//参数配置参考COMMON_USART_PARAMET paramet
////////////////////////////////////////////////////////////////////////////////// 	  
 


//配置 串口参数
 void COMMON_USART_Config(USART_TypeDef* USARTx, unsigned int baud)
{
	//   //USART1 初始化设置
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = baud;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USARTx, &USART_InitStructure); //初始化串口
	USART_Cmd(USARTx, ENABLE);  //使能串口 	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);//开启相关中断	
	//USARTx->SR &= ~(1 << 6);		
}
//使能 串口、串口读和写的时钟
 void COMMON_RCC_Config(COMMON_USART_PARAMET paramet){
	RCC_AHB1PeriphClockCmd(paramet.COMMON_USART_TX_GPIO_CLK,ENABLE); //使能GPIOX时钟
	RCC_AHB1PeriphClockCmd(paramet.COMMON_USART_RX_GPIO_CLK,ENABLE); //使能GPIOX时钟
	if(RCC_APB2Periph_USART6!=paramet.COMMON_USART_CLK&&RCC_APB2Periph_USART1!=paramet.COMMON_USART_CLK){
			RCC_APB1PeriphClockCmd(paramet.COMMON_USART_CLK,ENABLE);//使能USARTX时钟
		 
	 }else{
			RCC_APB2PeriphClockCmd(paramet.COMMON_USART_CLK,ENABLE);//使能USARTX时钟
	 }
}

//串口的复用和io引脚的初始化
 void COMMON_GPIO_Config(COMMON_USART_PARAMET paramet)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//串口读 的复用
	GPIO_PinAFConfig(paramet.COMMON_USART_RX_GPIO_PORT,
	paramet.COMMON_USART_RX_SOURCE,
	paramet.COMMON_USART_AF);
	//串口写的复用
	GPIO_PinAFConfig(paramet.COMMON_USART_TX_GPIO_PORT,
	paramet.COMMON_USART_TX_SOURCE,
	paramet.COMMON_USART_AF); 
	//初始化串口读的io引脚
	GPIO_InitStructure.GPIO_Pin = paramet.COMMON_USART_RX_PIN; //0-15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(paramet.COMMON_USART_RX_GPIO_PORT,&GPIO_InitStructure); 
	
	//初始化串口写 的io引脚
	GPIO_InitStructure.GPIO_Pin = paramet.COMMON_USART_TX_PIN; //0-15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(paramet.COMMON_USART_TX_GPIO_PORT,&GPIO_InitStructure); 	
}
//串口 中断 配置
 void COMMON_NVIC_Config(COMMON_USART_PARAMET paramet)
{
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = paramet.COMMON_USART_IRQ;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//初始化 串口功能 根据参数 paramet
void initialCOMMON_USART(COMMON_USART_PARAMET paramet)
{
	
	COMMON_RCC_Config(paramet);
	COMMON_GPIO_Config(paramet);
	COMMON_NVIC_Config(paramet);
	COMMON_USART_Config( paramet.COMMON_USART, paramet.COMMON_USART_BAUDRATE);
}
//发送数据到 USARTx 串口 可参考paramet.COMMON_USART
//USARTx 串口id
//Data 输出的数据
//len 数据长度
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


 



