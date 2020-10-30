
#include <string.h>

#include "WIFI_E103_W02.h"
#include "CommonUsart.h"

#include "led.h"
#include "delay.h"
#include "oled.h"
#include "USB_To_Usart.h"	
#include "timer.h"

////////////////////////////////////
//wifi模块 
//huzhikun
//2019-09-17
//
///////////////////////////////////////

char *E103_W02_AT="+++";//开启AT指令

char *E103_W02_AT_EXIT="AT+EXIT\r\n";//退出AT指令
char *E103_W02_STA="AT+ROLE=STA\r\n";//设置WIFI模式AP/STA
//char *SET_="AT+STA=ysnhzk.cn,2,880115ysn\r\n";
//设置STATION 参数(重启生效),即要连接的WIFI
//char *E103_W02_SET="AT+STA=chipcloud,2,chipcloud2017\r\n";
//char *E103_W02_SET="AT+STA=zwy_hw,2,zwyiot2017\r\n";
char *E103_W02_SET="AT+STA=scifront,2,123456789\r\n";

//创建socket 客户端39.107.65.176 
//char *E103_W02_CLIENT="AT+SOCK=TCP,CLIENT,192.168.43.21,3003,8887\r\n";
//char *E103_W02_CLIENT="AT+SOCK=TCP,CLIENT,123.57.255.14,3003,8887\r\n";
char *E103_W02_CLIENT="AT+SOCK=TCP,CLIENT,39.107.65.176,3003,8887\r\n";
//复位指令
char *E103_W02_RST="AT+RST\r\n";
//设置模块的ip信息
char *E103_W02_APIP="AT+APIP=192.168.1.12,255.255.255.0,192.168.1.12,192.168.1.1\r\n";
//恢复出厂设置（复位有效）
char *E103_W02_RESTORE="AT+RESTORE\r\n";

//查询模块当前socket信息
char *E103_W02_CLIENT_="AT+SOCK=?\r\n";
//查询当前模块版本信息
char *E103_W02_VER="AT+VER=?\r\n";

//开启AT指令的回显信息
char *E103_W02_AT_RES = "Entered AT mode";
//退出AT指令的回显信息
char *E103_W02_AT_EXIT_RES = "Exited AT mode";
//设置STA的回显信息
char *E103_W02_STA_RES = "Set STA mode";
//STA链接wifi的回显信息
char *E103_W02_SET_RES = "STA update OK";
//设置socket客户端的回显信息
char *E103_W02_CLIENT_RES = "Socket update OK";
//复位回显信息
char *E103_W02_RST_RES = "Module Reboot";
//恢复出厂设置回显信息
char *E103_W02_RESTORE_RES = "Restore OK";

COMMON_USART_PARAMET E103_UsartParamet;


u8 getLinkStatus(void){

	return GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6);
}
 u8 WIFI_USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收数据长度
 u16 WIFI_USART_RX_STA=0;       
void UART4_IRQHandler(void){
	
	u8 res;	    
	if(USART_GetITStatus(E103_UsartParamet.COMMON_USART, USART_IT_RXNE) != RESET)//接收到数据
	{	 
 
				
	res =USART_ReceiveData(E103_UsartParamet.COMMON_USART);		
	if((WIFI_USART_RX_STA&(1<<15))==0)//接收完的一批数据,还没有被处理,则不再接收其他数据
	{ 
		if(WIFI_USART_RX_STA<200)		//还可以接收数据
		{
			TIM_SetCounter(TIM5,0);//计数器清空        				 
			if(WIFI_USART_RX_STA==0)		
				TIM_Cmd(TIM5, ENABLE);  //使能定时器3 
			WIFI_USART_RX_BUF[WIFI_USART_RX_STA++]=res;		//记录接收到的值	 
		}else 
		{
			WIFI_USART_RX_STA|=1<<15;					//强制标记接收完成
		} 
	}  	
 }
}
 //wifi 发送数据
void E103_W02_SendData( uint8_t *Data,int l){
	//int l = sizeof(Data) ;
	//int l=strlen((const char *)Data);
	COMMON_USART_OUT(E103_UsartParamet.COMMON_USART,Data,l);
}

 //wifi 发送数据
void E103_W02_SendAT( uint8_t *Data){
	//int l = sizeof(Data) ;
	int l=strlen((const char *)Data);
	 E103_W02_SendData(Data,l);
}
void init_E103_w02(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	//wifi 的串口参数这是参考架构体 COMMON_USART_PARAMET
	E103_UsartParamet.COMMON_USART=UART4;
	E103_UsartParamet.COMMON_USART_CLK=RCC_APB1Periph_UART4;
	E103_UsartParamet.COMMON_USART_BAUDRATE=115200;
	
	E103_UsartParamet.COMMON_USART_RX_GPIO_CLK=RCC_AHB1Periph_GPIOC;
	E103_UsartParamet.COMMON_USART_RX_GPIO_PORT=GPIOC;
	E103_UsartParamet.COMMON_USART_RX_PIN=GPIO_Pin_11;
	E103_UsartParamet.COMMON_USART_RX_SOURCE=GPIO_PinSource11;
	
	E103_UsartParamet.COMMON_USART_TX_GPIO_CLK=RCC_AHB1Periph_GPIOC;
	E103_UsartParamet.COMMON_USART_TX_GPIO_PORT=GPIOC;
	E103_UsartParamet.COMMON_USART_TX_PIN=GPIO_Pin_10;
	E103_UsartParamet.COMMON_USART_TX_SOURCE=GPIO_PinSource10;
	
	E103_UsartParamet.COMMON_USART_AF=GPIO_AF_UART4;
	E103_UsartParamet.COMMON_USART_IRQ=UART4_IRQn;
	initialCOMMON_USART(E103_UsartParamet);
	
	//SendDataToDevice("Test486", 7);
	//使能GPIOD时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 	
	//配置GPIOD7的参数
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOD7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上啦
	//初始化GPIOD7 
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	//输出高电平，使此I/O脚的功能失效
	GPIO_SetBits(GPIOD,GPIO_Pin_7);//GPIOD7设置高,
		
	//配置GPIOD7的参数
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;           //GPIOD7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	//初始化GPIOD7 
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//配置GPIOC9的参数
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOD6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;        //输入
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上啦
	//初始化GPIOD6
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数30次为3ms 
	TIM5_Int_Init(30-1,8400-1);		//100ms中断
	WIFI_USART_RX_STA=0;		//清零
	TIM_Cmd(TIM5, DISABLE); //关闭定时器3
}
void rest(){
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_7);//GPIOD7设置高,
	delay_ms(500);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);//GPIOD7设置高,
	delay_ms(500);
	GPIO_SetBits(GPIOB,GPIO_Pin_1);//GPIOD7设置高,
	delay_ms(6000);
	
	GPIO_SetBits(GPIOD,GPIO_Pin_7);//GPIOD7设置高,
}
//比对两个字符串是否一样，或者string1包含string2
unsigned char my_strncmp(unsigned char *string1,unsigned char *string2,unsigned int count)
{
	unsigned char res=0,k=1;
	while(*string1!='\0'&&*string2!='\0'&&k<=count)
	{
		k++;
		if(*string1==*string2)
		{ 
			string1++;
			string2++;     
		}
		else{    
			res = *string1-*string2;
			break;
		}
	}
	return res;
}
//退出AT指令模式
void AT_Exit(){
	E103_W02_SendAT((uint8_t*)E103_W02_AT_EXIT);
	delay_ms(500);
	if(WIFI_USART_RX_STA!=0){
	//	LCD_ShowString(30,240,210,48,12,USART_RX_BUF);	
	}
	WIFI_USART_RX_STA=0;
}
//连接服务器（配置WIFI模式、要连接的wifi账号密码、服务器IP，端口号），复位后生效
char link_server(){
	int AT_id=0;
	char AT_res=0;
	while(AT_res==0){
		switch(AT_id){
			case 0://发送开启指令的命令
				E103_W02_SendAT((uint8_t*)E103_W02_AT);
				delay_ms(500);//发送到接收回应有个间隔
			SendDataToDevice_a(WIFI_USART_RX_BUF,WIFI_USART_RX_STA);
				//LCD_ShowString(30,140,210,48,12,USART_RX_BUF);//显示回应信息
				//判断是否接收到数据，和回应的信息是否是我们需要的（需要我们进行比对）
				AT_res=WIFI_USART_RX_STA=0?1:my_strncmp(WIFI_USART_RX_BUF,(unsigned char *)E103_W02_AT_RES,strlen(E103_W02_AT_RES));
				break;
			case 1://发送设置wifi STA模式命令
				E103_W02_SendAT((uint8_t*)E103_W02_STA);
				delay_ms(500);
			SendDataToDevice_a(WIFI_USART_RX_BUF,WIFI_USART_RX_STA^0x8000);
				AT_res=WIFI_USART_RX_STA=0?1:my_strncmp(WIFI_USART_RX_BUF,(unsigned char *)E103_W02_STA_RES,strlen(E103_W02_STA_RES));
				break;
			case 2://发送 设置要连接wifi的信息（名字、加密方式、密码）
				E103_W02_SendAT((uint8_t*)E103_W02_SET);
				delay_ms(3000);
			SendDataToDevice_a(WIFI_USART_RX_BUF,WIFI_USART_RX_STA^0x8000);
				AT_res=WIFI_USART_RX_STA=0?1:my_strncmp(WIFI_USART_RX_BUF,(unsigned char *)E103_W02_SET_RES,strlen(E103_W02_SET_RES));
				break;
			case 3://发送 设置socket信息命令（模式、协议、IP、端口号）
				E103_W02_SendAT((uint8_t*)E103_W02_CLIENT);
				delay_ms(500);
			SendDataToDevice_a(WIFI_USART_RX_BUF,WIFI_USART_RX_STA^0x8000);
				AT_res=WIFI_USART_RX_STA=0?1:my_strncmp(WIFI_USART_RX_BUF,(unsigned char *)E103_W02_CLIENT_RES,strlen(E103_W02_CLIENT_RES));
				break;
			case 4://发送复位命令
				E103_W02_SendAT((uint8_t*)E103_W02_RST);
				delay_ms(500);
			SendDataToDevice_a(WIFI_USART_RX_BUF,WIFI_USART_RX_STA^0x8000);
				AT_res=WIFI_USART_RX_STA=0?1:my_strncmp(WIFI_USART_RX_BUF,(unsigned char *)E103_W02_RST_RES,strlen(E103_W02_RST_RES));
				break;
		}
		
		AT_id++;
		WIFI_USART_RX_STA=0;
		if(AT_id==5){
			break;
		}
	}
	AT_Exit();//发送退出AT指令模式的命令
	return AT_res;
}
char restE103_W02(void){
	
	int AT_id=0;
	char AT_res=0;
	
	while(AT_res==0){
		switch(AT_id){
			case 0://发送开启指令的命令
				E103_W02_SendAT((uint8_t*)E103_W02_AT);
				delay_ms(1000);
			
			SendDataToDevice_a(WIFI_USART_RX_BUF,WIFI_USART_RX_STA^0x8000);
				//OLED_ShowString(0,50, USART_RX_BUF,12);OLED_Refresh_Gram();//更新显示到OLED
				AT_res=WIFI_USART_RX_STA=0?1:my_strncmp(WIFI_USART_RX_BUF,(unsigned char *)E103_W02_AT_RES,strlen(E103_W02_AT_RES));
		
				break;
			case 1://发送恢复出厂设置的命令
				E103_W02_SendAT((uint8_t*)E103_W02_RESTORE);
				delay_ms(2000);
			SendDataToDevice_a(WIFI_USART_RX_BUF,WIFI_USART_RX_STA^0x8000);
				//OLED_ShowString(0,50, USART_RX_BUF,12);OLED_Refresh_Gram();//更新显示到OLED
				AT_res=WIFI_USART_RX_STA=0?1:my_strncmp(WIFI_USART_RX_BUF,(unsigned char *)E103_W02_RESTORE_RES,strlen(E103_W02_RESTORE_RES));
			
				break;
			case 2://发送复位命令
				E103_W02_SendAT((uint8_t*)E103_W02_RST);
				delay_ms(1000);
			SendDataToDevice_a(WIFI_USART_RX_BUF,WIFI_USART_RX_STA^0x8000);
				//OLED_ShowString(0,50, USART_RX_BUF,12);OLED_Refresh_Gram();//更新显示到OLED
				AT_res=WIFI_USART_RX_STA=0?1:my_strncmp(WIFI_USART_RX_BUF,(unsigned char *)E103_W02_RST_RES,strlen(E103_W02_RST_RES));
				
			break;
		}
		AT_id++;
		WIFI_USART_RX_STA=0;
		if(AT_id==3){
			break;
		}
	}
	AT_Exit();//发送退出指令模式的命令
	return AT_res;
}


