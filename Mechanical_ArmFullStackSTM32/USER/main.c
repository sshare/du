#include "sys.h"
#include "delay.h"
#include "led.h"

#include "PWM_PB67.h"
#include "key.h"
#include "oled.h"
#include "USB_To_Usart.h"	
#include "arm.h"
#include "WIFI_E103_W02.h"
#include "Data_.h" 
#include "lcd.h" 
#include "sht11.h" 
#include <string.h>

extern uint64_t DeviceID;

extern uint32_t  UserPwd;
int delayCount=10;
void rest_wifi(void);
//��ȡ��ʪ�ȴ���������
	void GetSHTDataToGlobParams()
{
	int sendDataLen=0;
	
	u8 	buffer[128];
	uint8_t 	ServerUartBufferTx[128];
	uint32_t sendData[2];
	u16 humi_val, temp_val;//ԭʼ��ʪ��
	u8 err = 0, checksum = 0;
	//��ʵ����ʪ������
	float humi_val_real = 0.0; 
	float temp_val_real = 0.0;
	//���ݼ������
	delayCount--;
	if(delayCount>0){
		return;
	}
	delayCount=10;
	err += SHT11_Measure(&temp_val, &checksum, TEMP);                  //��ȡ�¶�
	err += SHT11_Measure(&humi_val, &checksum, HUMI);                  //��ȡʪ��
	if(err != 0){
		SHT11_ConReset();
	}else{
		SHT11_Calculate(temp_val, humi_val, &temp_val_real, &humi_val_real); //ʵ���¶�
		sprintf((char *)buffer,"temp:%2.2f hum:%2.2f", temp_val_real, humi_val_real);
		
		OLED_ShowString(0,26, buffer,12);
		sendData[0]=(int)(temp_val_real*100);
		sendData[1]=(int)(humi_val_real*100);
		//ƴ����ȡ�ϴ����ݰ�
		sendDataLen=getData(ServerUartBufferTx,0,sendData,2);
		
			//	SendDataToDevice_a(ServerUartBufferTx,sendDataLen);//��ӡָ���
			//	delay_ms(11);
		//�ж����ӷ�����״̬
		if(!getLinkStatus()){
			//�������ݵ�������
			E103_W02_SendData(ServerUartBufferTx,sendDataLen);
		}
	} 
	
}
void show2(){
	int i;
	u8 pbuf[2];
	OLED_Clear();
	OLED_ShowString(0,0, (unsigned char *)"Link State:",12);
	OLED_ShowString(0,10, "---------------------",12);//
	OLED_ShowString(0,16, "ID: ",12);//
		OLED_ShowString(0,26, "temp:0     hum:0    ",12);
	
	OLED_ShowString(0,36, "---------------------",12);//
	//OLED_ShowString(0,42, "key_up  key1    key2",12);//
//	OLED_ShowString(0,52, "start vertical level",12);//
	
	//��ӡ�豸��
	for(i=0;i<8;i++){
		sprintf((char *)pbuf, "%02X", (unsigned int)(DeviceID>>8*(7-i)&0xFF));
	//	SendDataToDevice_a(pbuf,2);
		OLED_ShowString(20+12*i,16, pbuf,12);	
	}
			
	OLED_Refresh_Gram();//������ʾ��OLED	
}
void start(){
	sheAngle(0,90,10);
	sheAngle(1,90,10);
	sheAngle(2,90,10);
	sheAngle(3,180,5);
	sheAngle(4,0,10);
	sheAngle(5,140,5);
	while(checkPar[5].armParameter.mTraget){};
}
void ad(){
		sheAngle(0,70,5);
		sheAngle(4,180,20);
		while(checkPar[4].armParameter.mTraget){};
		
		sheAngle(3,120,6);
		sheAngle(5,140,15);
		while(checkPar[5].armParameter.mTraget){};
		sheAngle(1,105,4);
		while(checkPar[1].armParameter.mTraget){};
		
		sheAngle(5,90,15);
		while(checkPar[5].armParameter.mTraget){};
		
		
		sheAngle(1,90,6);
		sheAngle(3,110,6);
		while(checkPar[3].armParameter.mTraget){};
		
		sheAngle(0,110,10);
		while(checkPar[0].armParameter.mTraget){};
		
		sheAngle(1,103,6);
		sheAngle(3,120,6);
		while(checkPar[3].armParameter.mTraget){};
		
		sheAngle(5,140,15);
		while(checkPar[5].armParameter.mTraget){};
		
		sheAngle(1,90,5);
		while(checkPar[1].armParameter.mTraget){};
		sheAngle(3,180,5);
		sheAngle(4,0,15);
		sheAngle(0,90,5);
		sheAngle(5,140,5);
		while(checkPar[5].armParameter.mTraget){};
}
void vertical(){
			//��ֱ
	sheAngle(0,90,5);
	while(checkPar[0].armParameter.mTraget){};
	sheAngle(1,90,5);
	sheAngle(2,180,5);
	sheAngle(3,90,5);
	sheAngle(4,0,5);
	sheAngle(5,140,5);
	while(checkPar[5].armParameter.mTraget){};
}

void level(){
	//ˮƽ
	sheAngle(5,140,5);
	sheAngle(4,0,5);
	sheAngle(3,85,5);
	sheAngle(2,180,10);
	sheAngle(1,8,5);
	sheAngle(0,90,10);
}
void execute_order(int a){
	switch(a)
	{				 
		case 0:	
			start();
			break;
		case 1:	//��ֱ
			vertical();
			break;
		case 2:	//ˮƽ
			level();
			break;
		case 3:	//ˮƽ
			//ad();
			break;
		}
}
int delay=500;
void link_wifi(){
	OLED_ShowString(0,0, (unsigned char *)"WIFI Set start...",12);
	OLED_Refresh_Gram();//������ʾ��OLED	 	
	delay_ms(2000);		
	while(link_server()){
		OLED_ShowString(0,0, (unsigned char *)"WIFI Set Fail",12);
		OLED_Refresh_Gram();//������ʾ��OLED	 		
		delay_ms(1000);
		OLED_Clear();
		OLED_ShowString(0,0, (unsigned char *)"WIFI Set start...",12);
		
		OLED_Refresh_Gram();//������ʾ��OLED	 		
	}
	OLED_ShowString(0,0, (unsigned char *)"WIFI Set Success",12);
	OLED_Refresh_Gram();//������ʾ��OLED	 		
		delay_ms(1000);
					
}
void rest_wifi(void){
	//delay_ms(2000);
	OLED_ShowString(0,52, (unsigned char *)"WIFI Rest start...",12);	
	OLED_Refresh_Gram();//������ʾ��OLED	 	
	
	delay_ms(2000);	
	if(restE103_W02()==0){
		OLED_ShowString(0,52, (unsigned char *)"WIFI Rest Success",12);
	}else{
		OLED_ShowString(0,52, (unsigned char *)"WIFI Rest Fail",12);
	}
	OLED_Refresh_Gram();//������ʾ��OLED	 
	delay_ms(1000);	
	show2();	
}
//���û�е�۹ؽڲ�������������
void setMechanical_Arm_Paramter(){
	
	ArmParameter armPar[6];
	ArmParameter arm;
	//��ʼ����Ҫ�õ��ĵĶ�ʱ��
 TIM3_PWM_Init(20000-1,84-1);
 TIM4_PWM_Init(20000-1,84-1);
 TIM2_Int_Init(20000-1,84-1);
	//���û�е��ÿ���ؽڶ���Ĳ���
	arm.mSteeringEngineAngle=180;
	arm.mCurrentLocation=1500;
	arm.mTraget=0;
	arm.PWM_Max=2500;
	arm.PWM_Min=500;
	armPar[0]=arm;
	arm.mSteeringEngineAngle=180;
	arm.mCurrentLocation=1500;
	arm.PWM_Max=2500;
	arm.PWM_Min=500;
	armPar[1]=arm;
	arm.mSteeringEngineAngle=180;
	arm.mCurrentLocation=1500;
	arm.PWM_Max=2500;
	arm.PWM_Min=550;
	armPar[2]=arm;
	arm.mSteeringEngineAngle=210;
	arm.mCurrentLocation=2500;
	arm.PWM_Max=2500;
	arm.PWM_Min=500;
	armPar[3]=arm;
	arm.mSteeringEngineAngle=180;
	arm.mCurrentLocation=500;
	arm.PWM_Max=2500;
	arm.PWM_Min=550;
	armPar[4]=arm;
	arm.mSteeringEngineAngle=210;
	arm.mCurrentLocation=1500+70*(2500-550)/210;
	arm.PWM_Max=2500;
	arm.PWM_Min=550;
	armPar[5]=arm;
	intit_SteeringEngine(armPar,6);//���û�е�۲���
				
	
	
	//У��ֵ���ã�ÿ���ؽڵ�ƫ������
	sheCheckValue(0,0);
	sheCheckValue(1,5);
	sheCheckValue(2,5);
	sheCheckValue(3,-4);
	sheCheckValue(4,-5);
	sheCheckValue(5,70);
	
}
//����Ƿ��н��յ����������ݣ���ִ�ж�Ӧ��ָ��Ͳ���
void checkData(){
	int orderNum=-1;
	int ss=0;
	//����Ƿ��з���������
	if(WIFI_USART_RX_STA>0x8000){
		
			//SendDataToDevice_a(WIFI_USART_RX_BUF, WIFI_USART_RX_STA^0x8000);//��ӡָ���
			//delay_ms(11);
		//����Э���������
			if(ParseServerData(WIFI_USART_RX_BUF, WIFI_USART_RX_STA^0x8000)){
				
				WIFI_USART_RX_STA = 0;//������ݱ�־
			/*	for(ss=0;ss<32;ss++){//��ӡ���в���ֵ������
					sprintf((char *)buffer,"val%d��ֵ:%d %d %d %d\r\n", ss+1,dataFrame.mOrderConnect[ss]&0xFF,dataFrame.mOrderConnect[ss]>>8&0xFF,dataFrame.mOrderConnect[ss]>>16&0xFF,dataFrame.mOrderConnect[ss]>>24&0xFF);
					SendDataToDevice_a(buffer, 40);
					delay_ms(10);
					
				}*/
				for(ss=0;ss<dataFrame.mOrderDataCount;ss++){
					if((dataFrame.mOrderConnect[ss]>>24&0xFF)>=0x10){//����ٿ�ָ��
						
							//OLED_ShowNum(80,0, dataFrame.mOrderConnect[ss]>>16&0xFF,4,12);
						sheAngle((dataFrame.mOrderConnect[ss]>>24&0xFF)-0x10,dataFrame.mOrderConnect[ss]>>16&0xFF,dataFrame.mOrderConnect[ss]>>8&0xFF);
						
						if((dataFrame.mOrderConnect[ss]&0xFF)==1){//�ȴ��˶��ִ������ת����
							
							while(checkPar[(dataFrame.mOrderConnect[ss]>>24&0xFF)-0x10].armParameter.mTraget>0){
								
					//sprintf((char *)buffer,"��ֵ:%d ", checkPar[ss].armParameter.mTraget);
					//SendDataToDevice_a(buffer, 40);
							}
						}else if((dataFrame.mOrderConnect[ss]&0xFF)==2){ //��ʱִ�к�������������
							delay_ms(delay);
						}
						
					}else {
						if(ss==0){
							//����ָ��
							//OLED_ShowNum(80,0, dataFrame.mOrderConnect[ss],4,12);
							orderNum=dataFrame.mOrderConnect[ss];
						}
					}
					
				}
			}/**/
	}
	execute_order(orderNum);//ִ�в���ָ��
	orderNum=-1;
}
int main(void)
{ 
	u8 key;
	delay_init(168);		  //��ʼ����ʱ����
	uart_init(115200);//���Դ��ڳ�ʼ��
	LED_Init();		        //��ʼ��LED�˿�
 	OLED_Init();				//��ʼ��OLED 
	KEY_Init();					//	������ʼ��
	delay_ms(11);
	SHT11_Config();//��ʪ�ȳ�ʼ��
	init_E103_w02();//WIFI��ʼ��
	initDataFrame(0xDC2010016615FAAC,0x123456);//��ʼ������Э�飬������Э�����õ����豸id�͵�¼
	link_wifi();//����wifi�ȵ�
	setMechanical_Arm_Paramter();//���û�е�۲���
	show2(); 
	while(1)
	{		
		checkData();//����Ƿ���յ�����������
		if(getLinkStatus()){//wifi����״̬
			
			OLED_ShowString(6*11,0, (unsigned char *)"NO",12);
		}else{
			OLED_ShowString(6*11,0, (unsigned char *)"OK",12);
		}
	OLED_Refresh_Gram();//������ʾ��OLED	
	//////////////////////////////////////////////////////////////////////
		key=KEY_Scan(0);		//�õ���ֵ
		LED1=!LED1;
				if(key)
			{						
				switch(key)
				{				 
					case WKUP_PRES:	//��ʼ����̬
						start();
						break;
					case KEY0_PRES:	//��ʼ��wifi
						rest_wifi();
						break;
					case KEY1_PRES:	//��ֱ��̬
						vertical();
						break;
					case KEY2_PRES:	//ˮƽ��̬
						level();
						break;
				}
			}else delay_ms(100); 
		
			TIM4->CCR4=1500;//��װ�õĵ����������������źţ�
			GetSHTDataToGlobParams();//��ȡ��ʪ�����ݣ������͵�������
			
	}
}




