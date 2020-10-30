#include "arm.h"
#include "sys.h"
#include "USB_To_Usart.h"
#include "oled.h"
#include "led.h"
#include "delay.h"


//舵机本身的参数值
int origin=1500;//由于回中基本没有误差因此我们设置这个为起点0
//int mCurrentLocation=1500;//当前位置（500-2500）
/*
int mSteeringEngineAngle=180;//舵机角度（实际可控角度，需要编程测试）
int angleToData=0;//1度对应的占空比的值（500-2500）
int PWM_Min=500;
int PWM_Max=2500;
int mOffsetValue=0;//产生1度的误差所需积累的值
//实际校对需要的参数值
int mCheckAngle=0;//校对角度，（就是某个角度相对正确角度的偏移量）*/

int mArmParSize=0;//舵机个数
CheckParameter checkPar[16];

//初始化机械臂，其中占空比的操作范围、实际操控的角度，需要自行测试获取，这个涉及机械臂的操作精准度，务必仔细测量
void intit_SteeringEngine(ArmParameter armPar[],int armParSize){
	 u8 pbuf[100];
	 float count;
	CheckParameter check;
	int i=0;
	mArmParSize=armParSize;
	
	//添加每个舵机的参数和校对参数到数组checkPar
	for(i=0;i<armParSize;i++){
		check.mCheckAngle=0;//安装偏差角度
		check.armParameter=armPar[i];//舵机参数
		check.actualValue=(float)(armPar[i].PWM_Max-armPar[i].PWM_Min)/armPar[i].mSteeringEngineAngle;//1度对应的占空比值
		//设置1度代表的PWM值，是整数值，在check.actualValue的基础上多余的小数部分组整数位加1处理，等到一个整数值
		if((int)check.actualValue==check.actualValue){
			check.angleToData=(int)check.actualValue;
		}else{
			check.angleToData=(int)check.actualValue+1;
		}
		
		check.armParameter.mSeep=10;//记录移动速度
		checkPar[i]=check;
	}
	 
}
//绝对值方法，去掉数值的符号
  float Abs(float value)
{
    if (value >= 0)
    {
        return value;
    }
    return -1*value;
}
 
 //需自行操作将当前舵机显示的位置调整到规定位置，以此来计算安装偏移量，
//从而规范角度（便宜大小会影响之后的可操作范围，有可能会导致可操作范围不足180）
 int sheCheckValue(int armID,int angle){
		int count;
		if(mArmParSize<armID&&armID<0) return 0;
		if(angle<0&&angle>180)return 0;
		checkPar[armID].mCheckAngle=angle;//设置对应舵机的安装偏差
		return 1;
 }

//用于校对，以中间值为90度调整，由于安装导致中间值的位置可能相对规定的有所偏差
void checkInit(){
	int i=0;
	//添加每个舵机的参数和校对参数到数组checkPar
	for(;i<mArmParSize;i++){
		checkPar[i].armParameter.mTraget=1500;
		checkPar[i].armParameter.mCurrentLocation=1500;
		
		delay_ms(500); 
	}
	 
}

	
float Position_KP=5,Position_KI=0,Position_KD=5;
int Position_PID (int Encoder,int Target)
{ 	
	  float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Abs(Target-Encoder);                                  //计算偏差
	if(Bias<10) return (int)Bias;
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 Pwm=Position_KP*Bias/100+Position_KI*Integral_bias/100+Position_KD*(Bias-Last_Bias)/100;       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出
}
//通过角度设置舵机的位置
int sheAngle(int armID,int angle,int seep){
	u8 pbuf[100];
	float count;//过度用参数
	if(mArmParSize<armID&&armID<0) return 0;//判断id有效性
	if(angle<0&&angle>180)return 0;//判断角度有效性
	if(seep<0)return 0;//判断速度有效性
	
	//OLED_Clear();
	count=(angle-90)*checkPar[armID].actualValue;//计算移动的角度距离90度距离（角度差*每度代表的PWM值）
	
	//sprintf((char *)pbuf, " %02f %d %02f", count,checkPar[armID].mCheckAngle,checkPar[armID].actualValue);
	//OLED_ShowString(0,0, pbuf,12);
	count=Abs(origin+count+checkPar[armID].mCheckAngle*checkPar[armID].actualValue);//获取实际的占空比值（中间PWM值+移动的距离+安装偏差）
	
	//控制占空比的值不能超出范围PWM_Min--PWM_Max（）
	if(count<checkPar[armID].armParameter.PWM_Min){
		count=checkPar[armID].armParameter.PWM_Min;
	}else if(count>checkPar[armID].armParameter.PWM_Max){
		count=checkPar[armID].armParameter.PWM_Max;
	}
	checkPar[armID].armParameter.mTraget=count;//记录目标位置
	checkPar[armID].armParameter.mSeep=seep;//记录移动速度
	
	//sprintf((char *)pbuf, " %02f  %02f ", count,checkPar[armID].mCheckAngle*checkPar[armID].actualValue);
	//OLED_ShowString(0,24, pbuf,12);
	//OLED_Refresh_Gram();//更新显示到OLED	 
}
//int d=0;
//根据当前位置和目标位置来获取运动偏移量，从而设置当前的PWM
void handle_PWM(){
	//int d=0;
	int i;
	u8 pbuf[100];
	int count;
	for(i=0;i<mArmParSize;i++){
		count=0;
		
		if(checkPar[i].armParameter.mCurrentLocation==0||checkPar[i].armParameter.mTraget==0) 
		{
		//sprintf((char *)pbuf, " %d",d++);
	//OLED_ShowString(0,36, pbuf,12);
	//OLED_Refresh_Gram();//更新显示到OLED	 
			continue;
		}
		//count =Position_PID(checkPar[i].armParameter.mCurrentLocation,checkPar[i].armParameter.mTraget);
		
		//count=10;
		count=checkPar[i].armParameter.mSeep;
		//由于减速运动算法只是求出一段距离的运动量，没有正负之分，所以我们要根据当前位置和目标位置来判断是加是减
		if(checkPar[i].armParameter.mCurrentLocation-checkPar[i].armParameter.mTraget>0){
	//OLED_ShowString(80,36, "1",12);
			checkPar[i].armParameter.mCurrentLocation-=count;
		}else if(checkPar[i].armParameter.mCurrentLocation-checkPar[i].armParameter.mTraget<0){
			checkPar[i].armParameter.mCurrentLocation+=count;
	//OLED_ShowString(80,36, "0",12);
		}else{
		checkPar[i].armParameter.mTraget=0;
	//OLED_ShowString(80,36, "2",12);
		}
		//防止导致移动位置超越目标位置（当移动距离不能整除速度，导致当前位置会超出 目标位置一定距离，这个距离在一定小于速度）
		if(Abs(checkPar[i].armParameter.mCurrentLocation-checkPar[i].armParameter.mTraget)<checkPar[i].armParameter.mSeep){
			checkPar[i].armParameter.mCurrentLocation=checkPar[i].armParameter.mTraget;//强制当前位置到目标位置
		}
		sprintf((char *)pbuf, " %d ", count);
	//OLED_ShowString(80,36, pbuf,12);
	//OLED_Refresh_Gram();//更新显示到OLED	 
		shePWM_Value(i,checkPar[i].armParameter.mCurrentLocation);//设置舵机PWM值，
	}
}

//根据舵机ID设置对应定时器通道的PWM值
	void shePWM_Value(int armID,int value){
		
//	OLED_ShowNum(80,0, armID,1,12);
//控制占空比的值不能超出范围PWM_Min--PWM_Max（）
	if(value<checkPar[armID].armParameter.PWM_Min){
		value=checkPar[armID].armParameter.PWM_Min;
	}else if(value>checkPar[armID].armParameter.PWM_Max){
		value=checkPar[armID].armParameter.PWM_Max;
	}
	//设置对应舵机的定时器通道的PWM值
	switch(armID){
		case 0:
			TIM3->CCR1=value;
			break;
		case 1:
			TIM3->CCR2=value;
			break;
		case 2:
			TIM3->CCR3=value;
			break;
		case 3:
			TIM3->CCR4=value;
			break;
		case 4:
			TIM4->CCR1=value;
			break;
		case 5:
			TIM4->CCR2=value;
			break;
	}
}
	//定时器2中断函数，更新舵机的PWM值
void TIM2_IRQHandler(void) {
	
	u8 pbuf[23];
	static u8 flag = 0;
	static u8 duoji_index1 = 0;
	int temp;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查 TIM2 更新中断发生与否
	{
		LED0=!LED0;
		handle_PWM();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update ); //清除 TIM2 更新中断标志
		
	
	}
} 

