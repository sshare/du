
#include <string.h>

#include "WIFI_E103_W02.h"
#include "CommonUsart.h"

#include "led.h"
#include "delay.h"
#include "oled.h"
#include "USB_To_Usart.h"	
#include "timer.h"

////////////////////////////////////
//wifiģ�� 
//huzhikun
//2019-09-17
//
///////////////////////////////////////

char *E103_W02_AT="+++";//����ATָ��

char *E103_W02_AT_EXIT="AT+EXIT\r\n";//�˳�ATָ��
char *E103_W02_STA="AT+ROLE=STA\r\n";//����WIFIģʽAP/STA
//char *SET_="AT+STA=ysnhzk.cn,2,880115ysn\r\n";
//����STATION ����(������Ч),��Ҫ���ӵ�WIFI
//char *E103_W02_SET="AT+STA=chipcloud,2,chipcloud2017\r\n";
//char *E103_W02_SET="AT+STA=zwy_hw,2,zwyiot2017\r\n";
char *E103_W02_SET="AT+STA=scifront,2,123456789\r\n";

//����socket �ͻ���39.107.65.176 
//char *E103_W02_CLIENT="AT+SOCK=TCP,CLIENT,192.168.43.21,3003,8887\r\n";
//char *E103_W02_CLIENT="AT+SOCK=TCP,CLIENT,123.57.255.14,3003,8887\r\n";
char *E103_W02_CLIENT="AT+SOCK=TCP,CLIENT,39.107.65.176,3003,8887\r\n";
//��λָ��
char *E103_W02_RST="AT+RST\r\n";
//����ģ���ip��Ϣ
char *E103_W02_APIP="AT+APIP=192.168.1.12,255.255.255.0,192.168.1.12,192.168.1.1\r\n";
//�ָ��������ã���λ��Ч��
char *E103_W02_RESTORE="AT+RESTORE\r\n";

//��ѯģ�鵱ǰsocket��Ϣ
char *E103_W02_CLIENT_="AT+SOCK=?\r\n";
//��ѯ��ǰģ��汾��Ϣ
char *E103_W02_VER="AT+VER=?\r\n";

//����ATָ��Ļ�����Ϣ
char *E103_W02_AT_RES = "Entered AT mode";
//�˳�ATָ��Ļ�����Ϣ
char *E103_W02_AT_EXIT_RES = "Exited AT mode";
//����STA�Ļ�����Ϣ
char *E103_W02_STA_RES = "Set STA mode";
//STA����wifi�Ļ�����Ϣ
char *E103_W02_SET_RES = "STA update OK";
//����socket�ͻ��˵Ļ�����Ϣ
char *E103_W02_CLIENT_RES = "Socket update OK";
//��λ������Ϣ
char *E103_W02_RST_RES = "Module Reboot";
//�ָ��������û�����Ϣ
char *E103_W02_RESTORE_RES = "Restore OK";

COMMON_USART_PARAMET E103_UsartParamet;


u8 getLinkStatus(void){

	return GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6);
}
 u8 WIFI_USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//�������ݳ���
 u16 WIFI_USART_RX_STA=0;       
void UART4_IRQHandler(void){
	
	u8 res;	    
	if(USART_GetITStatus(E103_UsartParamet.COMMON_USART, USART_IT_RXNE) != RESET)//���յ�����
	{	 
 
				
	res =USART_ReceiveData(E103_UsartParamet.COMMON_USART);		
	if((WIFI_USART_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
	{ 
		if(WIFI_USART_RX_STA<200)		//�����Խ�������
		{
			TIM_SetCounter(TIM5,0);//���������        				 
			if(WIFI_USART_RX_STA==0)		
				TIM_Cmd(TIM5, ENABLE);  //ʹ�ܶ�ʱ��3 
			WIFI_USART_RX_BUF[WIFI_USART_RX_STA++]=res;		//��¼���յ���ֵ	 
		}else 
		{
			WIFI_USART_RX_STA|=1<<15;					//ǿ�Ʊ�ǽ������
		} 
	}  	
 }
}
 //wifi ��������
void E103_W02_SendData( uint8_t *Data,int l){
	//int l = sizeof(Data) ;
	//int l=strlen((const char *)Data);
	COMMON_USART_OUT(E103_UsartParamet.COMMON_USART,Data,l);
}

 //wifi ��������
void E103_W02_SendAT( uint8_t *Data){
	//int l = sizeof(Data) ;
	int l=strlen((const char *)Data);
	 E103_W02_SendData(Data,l);
}
void init_E103_w02(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	//wifi �Ĵ��ڲ������ǲο��ܹ��� COMMON_USART_PARAMET
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
	//ʹ��GPIODʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 	
	//����GPIOD7�Ĳ���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOD7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	//��ʼ��GPIOD7 
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	//����ߵ�ƽ��ʹ��I/O�ŵĹ���ʧЧ
	GPIO_SetBits(GPIOD,GPIO_Pin_7);//GPIOD7���ø�,
		
	//����GPIOD7�Ĳ���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;           //GPIOD7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	//��ʼ��GPIOD7 
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//����GPIOC9�Ĳ���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOD6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;        //����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	//��ʼ��GPIOD6
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����30��Ϊ3ms 
	TIM5_Int_Init(30-1,8400-1);		//100ms�ж�
	WIFI_USART_RX_STA=0;		//����
	TIM_Cmd(TIM5, DISABLE); //�رն�ʱ��3
}
void rest(){
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_7);//GPIOD7���ø�,
	delay_ms(500);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);//GPIOD7���ø�,
	delay_ms(500);
	GPIO_SetBits(GPIOB,GPIO_Pin_1);//GPIOD7���ø�,
	delay_ms(6000);
	
	GPIO_SetBits(GPIOD,GPIO_Pin_7);//GPIOD7���ø�,
}
//�ȶ������ַ����Ƿ�һ��������string1����string2
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
//�˳�ATָ��ģʽ
void AT_Exit(){
	E103_W02_SendAT((uint8_t*)E103_W02_AT_EXIT);
	delay_ms(500);
	if(WIFI_USART_RX_STA!=0){
	//	LCD_ShowString(30,240,210,48,12,USART_RX_BUF);	
	}
	WIFI_USART_RX_STA=0;
}
//���ӷ�����������WIFIģʽ��Ҫ���ӵ�wifi�˺����롢������IP���˿ںţ�����λ����Ч
char link_server(){
	int AT_id=0;
	char AT_res=0;
	while(AT_res==0){
		switch(AT_id){
			case 0://���Ϳ���ָ�������
				E103_W02_SendAT((uint8_t*)E103_W02_AT);
				delay_ms(500);//���͵����ջ�Ӧ�и����
			SendDataToDevice_a(WIFI_USART_RX_BUF,WIFI_USART_RX_STA);
				//LCD_ShowString(30,140,210,48,12,USART_RX_BUF);//��ʾ��Ӧ��Ϣ
				//�ж��Ƿ���յ����ݣ��ͻ�Ӧ����Ϣ�Ƿ���������Ҫ�ģ���Ҫ���ǽ��бȶԣ�
				AT_res=WIFI_USART_RX_STA=0?1:my_strncmp(WIFI_USART_RX_BUF,(unsigned char *)E103_W02_AT_RES,strlen(E103_W02_AT_RES));
				break;
			case 1://��������wifi STAģʽ����
				E103_W02_SendAT((uint8_t*)E103_W02_STA);
				delay_ms(500);
			SendDataToDevice_a(WIFI_USART_RX_BUF,WIFI_USART_RX_STA^0x8000);
				AT_res=WIFI_USART_RX_STA=0?1:my_strncmp(WIFI_USART_RX_BUF,(unsigned char *)E103_W02_STA_RES,strlen(E103_W02_STA_RES));
				break;
			case 2://���� ����Ҫ����wifi����Ϣ�����֡����ܷ�ʽ�����룩
				E103_W02_SendAT((uint8_t*)E103_W02_SET);
				delay_ms(3000);
			SendDataToDevice_a(WIFI_USART_RX_BUF,WIFI_USART_RX_STA^0x8000);
				AT_res=WIFI_USART_RX_STA=0?1:my_strncmp(WIFI_USART_RX_BUF,(unsigned char *)E103_W02_SET_RES,strlen(E103_W02_SET_RES));
				break;
			case 3://���� ����socket��Ϣ���ģʽ��Э�顢IP���˿ںţ�
				E103_W02_SendAT((uint8_t*)E103_W02_CLIENT);
				delay_ms(500);
			SendDataToDevice_a(WIFI_USART_RX_BUF,WIFI_USART_RX_STA^0x8000);
				AT_res=WIFI_USART_RX_STA=0?1:my_strncmp(WIFI_USART_RX_BUF,(unsigned char *)E103_W02_CLIENT_RES,strlen(E103_W02_CLIENT_RES));
				break;
			case 4://���͸�λ����
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
	AT_Exit();//�����˳�ATָ��ģʽ������
	return AT_res;
}
char restE103_W02(void){
	
	int AT_id=0;
	char AT_res=0;
	
	while(AT_res==0){
		switch(AT_id){
			case 0://���Ϳ���ָ�������
				E103_W02_SendAT((uint8_t*)E103_W02_AT);
				delay_ms(1000);
			
			SendDataToDevice_a(WIFI_USART_RX_BUF,WIFI_USART_RX_STA^0x8000);
				//OLED_ShowString(0,50, USART_RX_BUF,12);OLED_Refresh_Gram();//������ʾ��OLED
				AT_res=WIFI_USART_RX_STA=0?1:my_strncmp(WIFI_USART_RX_BUF,(unsigned char *)E103_W02_AT_RES,strlen(E103_W02_AT_RES));
		
				break;
			case 1://���ͻָ��������õ�����
				E103_W02_SendAT((uint8_t*)E103_W02_RESTORE);
				delay_ms(2000);
			SendDataToDevice_a(WIFI_USART_RX_BUF,WIFI_USART_RX_STA^0x8000);
				//OLED_ShowString(0,50, USART_RX_BUF,12);OLED_Refresh_Gram();//������ʾ��OLED
				AT_res=WIFI_USART_RX_STA=0?1:my_strncmp(WIFI_USART_RX_BUF,(unsigned char *)E103_W02_RESTORE_RES,strlen(E103_W02_RESTORE_RES));
			
				break;
			case 2://���͸�λ����
				E103_W02_SendAT((uint8_t*)E103_W02_RST);
				delay_ms(1000);
			SendDataToDevice_a(WIFI_USART_RX_BUF,WIFI_USART_RX_STA^0x8000);
				//OLED_ShowString(0,50, USART_RX_BUF,12);OLED_Refresh_Gram();//������ʾ��OLED
				AT_res=WIFI_USART_RX_STA=0?1:my_strncmp(WIFI_USART_RX_BUF,(unsigned char *)E103_W02_RST_RES,strlen(E103_W02_RST_RES));
				
			break;
		}
		AT_id++;
		WIFI_USART_RX_STA=0;
		if(AT_id==3){
			break;
		}
	}
	AT_Exit();//�����˳�ָ��ģʽ������
	return AT_res;
}


