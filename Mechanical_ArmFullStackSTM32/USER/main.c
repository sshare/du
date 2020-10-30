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
//获取温湿度传感器数据
	void GetSHTDataToGlobParams()
{
	int sendDataLen=0;
	
	u8 	buffer[128];
	uint8_t 	ServerUartBufferTx[128];
	uint32_t sendData[2];
	u16 humi_val, temp_val;//原始温湿度
	u8 err = 0, checksum = 0;
	//真实的温湿度数据
	float humi_val_real = 0.0; 
	float temp_val_real = 0.0;
	//数据间隔处理
	delayCount--;
	if(delayCount>0){
		return;
	}
	delayCount=10;
	err += SHT11_Measure(&temp_val, &checksum, TEMP);                  //获取温度
	err += SHT11_Measure(&humi_val, &checksum, HUMI);                  //获取湿度
	if(err != 0){
		SHT11_ConReset();
	}else{
		SHT11_Calculate(temp_val, humi_val, &temp_val_real, &humi_val_real); //实际温度
		sprintf((char *)buffer,"temp:%2.2f hum:%2.2f", temp_val_real, humi_val_real);
		
		OLED_ShowString(0,26, buffer,12);
		sendData[0]=(int)(temp_val_real*100);
		sendData[1]=(int)(humi_val_real*100);
		//拼包获取上传数据包
		sendDataLen=getData(ServerUartBufferTx,0,sendData,2);
		
			//	SendDataToDevice_a(ServerUartBufferTx,sendDataLen);//打印指令包
			//	delay_ms(11);
		//判断连接服务器状态
		if(!getLinkStatus()){
			//发送数据到服务器
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
	
	//打印设备号
	for(i=0;i<8;i++){
		sprintf((char *)pbuf, "%02X", (unsigned int)(DeviceID>>8*(7-i)&0xFF));
	//	SendDataToDevice_a(pbuf,2);
		OLED_ShowString(20+12*i,16, pbuf,12);	
	}
			
	OLED_Refresh_Gram();//更新显示到OLED	
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
			//垂直
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
	//水平
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
		case 1:	//垂直
			vertical();
			break;
		case 2:	//水平
			level();
			break;
		case 3:	//水平
			//ad();
			break;
		}
}
int delay=500;
void link_wifi(){
	OLED_ShowString(0,0, (unsigned char *)"WIFI Set start...",12);
	OLED_Refresh_Gram();//更新显示到OLED	 	
	delay_ms(2000);		
	while(link_server()){
		OLED_ShowString(0,0, (unsigned char *)"WIFI Set Fail",12);
		OLED_Refresh_Gram();//更新显示到OLED	 		
		delay_ms(1000);
		OLED_Clear();
		OLED_ShowString(0,0, (unsigned char *)"WIFI Set start...",12);
		
		OLED_Refresh_Gram();//更新显示到OLED	 		
	}
	OLED_ShowString(0,0, (unsigned char *)"WIFI Set Success",12);
	OLED_Refresh_Gram();//更新显示到OLED	 		
		delay_ms(1000);
					
}
void rest_wifi(void){
	//delay_ms(2000);
	OLED_ShowString(0,52, (unsigned char *)"WIFI Rest start...",12);	
	OLED_Refresh_Gram();//更新显示到OLED	 	
	
	delay_ms(2000);	
	if(restE103_W02()==0){
		OLED_ShowString(0,52, (unsigned char *)"WIFI Rest Success",12);
	}else{
		OLED_ShowString(0,52, (unsigned char *)"WIFI Rest Fail",12);
	}
	OLED_Refresh_Gram();//更新显示到OLED	 
	delay_ms(1000);	
	show2();	
}
//设置机械臂关节参数，便宜量等
void setMechanical_Arm_Paramter(){
	
	ArmParameter armPar[6];
	ArmParameter arm;
	//初始化需要用到的的定时器
 TIM3_PWM_Init(20000-1,84-1);
 TIM4_PWM_Init(20000-1,84-1);
 TIM2_Int_Init(20000-1,84-1);
	//配置机械臂每个关节舵机的参数
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
	intit_SteeringEngine(armPar,6);//设置机械臂参数
				
	
	
	//校对值设置（每个关节的偏移量）
	sheCheckValue(0,0);
	sheCheckValue(1,5);
	sheCheckValue(2,5);
	sheCheckValue(3,-4);
	sheCheckValue(4,-5);
	sheCheckValue(5,70);
	
}
//检测是否有接收到服务器数据，并执行对应的指令和操作
void checkData(){
	int orderNum=-1;
	int ss=0;
	//检测是否有服务器数据
	if(WIFI_USART_RX_STA>0x8000){
		
			//SendDataToDevice_a(WIFI_USART_RX_BUF, WIFI_USART_RX_STA^0x8000);//打印指令包
			//delay_ms(11);
		//根据协议解析数据
			if(ParseServerData(WIFI_USART_RX_BUF, WIFI_USART_RX_STA^0x8000)){
				
				WIFI_USART_RX_STA = 0;//清除数据标志
			/*	for(ss=0;ss<32;ss++){//打印所有参数值到串口
					sprintf((char *)buffer,"val%d数值:%d %d %d %d\r\n", ss+1,dataFrame.mOrderConnect[ss]&0xFF,dataFrame.mOrderConnect[ss]>>8&0xFF,dataFrame.mOrderConnect[ss]>>16&0xFF,dataFrame.mOrderConnect[ss]>>24&0xFF);
					SendDataToDevice_a(buffer, 40);
					delay_ms(10);
					
				}*/
				for(ss=0;ss<dataFrame.mOrderDataCount;ss++){
					if((dataFrame.mOrderConnect[ss]>>24&0xFF)>=0x10){//舵机操控指令
						
							//OLED_ShowNum(80,0, dataFrame.mOrderConnect[ss]>>16&0xFF,4,12);
						sheAngle((dataFrame.mOrderConnect[ss]>>24&0xFF)-0x10,dataFrame.mOrderConnect[ss]>>16&0xFF,dataFrame.mOrderConnect[ss]>>8&0xFF);
						
						if((dataFrame.mOrderConnect[ss]&0xFF)==1){//等待此舵机执行完旋转动作
							
							while(checkPar[(dataFrame.mOrderConnect[ss]>>24&0xFF)-0x10].armParameter.mTraget>0){
								
					//sprintf((char *)buffer,"数值:%d ", checkPar[ss].armParameter.mTraget);
					//SendDataToDevice_a(buffer, 40);
							}
						}else if((dataFrame.mOrderConnect[ss]&0xFF)==2){ //延时执行后各个续舵机动作
							delay_ms(delay);
						}
						
					}else {
						if(ss==0){
							//操作指令
							//OLED_ShowNum(80,0, dataFrame.mOrderConnect[ss],4,12);
							orderNum=dataFrame.mOrderConnect[ss];
						}
					}
					
				}
			}/**/
	}
	execute_order(orderNum);//执行操作指令
	orderNum=-1;
}
int main(void)
{ 
	u8 key;
	delay_init(168);		  //初始化延时函数
	uart_init(115200);//调试串口初始化
	LED_Init();		        //初始化LED端口
 	OLED_Init();				//初始化OLED 
	KEY_Init();					//	按键初始化
	delay_ms(11);
	SHT11_Config();//温湿度初始化
	init_E103_w02();//WIFI初始化
	initDataFrame(0xDC2010016615FAAC,0x123456);//初始化数据协议，并设置协议中用到的设备id和登录
	link_wifi();//连接wifi热点
	setMechanical_Arm_Paramter();//设置机械臂参数
	show2(); 
	while(1)
	{		
		checkData();//检测是否接收到服务器数据
		if(getLinkStatus()){//wifi联网状态
			
			OLED_ShowString(6*11,0, (unsigned char *)"NO",12);
		}else{
			OLED_ShowString(6*11,0, (unsigned char *)"OK",12);
		}
	OLED_Refresh_Gram();//更新显示到OLED	
	//////////////////////////////////////////////////////////////////////
		key=KEY_Scan(0);		//得到键值
		LED1=!LED1;
				if(key)
			{						
				switch(key)
				{				 
					case WKUP_PRES:	//初始化姿态
						start();
						break;
					case KEY0_PRES:	//初始化wifi
						rest_wifi();
						break;
					case KEY1_PRES:	//垂直姿态
						vertical();
						break;
					case KEY2_PRES:	//水平姿态
						level();
						break;
				}
			}else delay_ms(100); 
		
			TIM4->CCR4=1500;//安装用的调试输出（舵机归中信号）
			GetSHTDataToGlobParams();//获取温湿度数据，并发送到服务器
			
	}
}




