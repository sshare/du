#include "arm.h"
#include "sys.h"
#include "USB_To_Usart.h"
#include "oled.h"
#include "led.h"
#include "delay.h"


//�������Ĳ���ֵ
int origin=1500;//���ڻ��л���û�������������������Ϊ���0
//int mCurrentLocation=1500;//��ǰλ�ã�500-2500��
/*
int mSteeringEngineAngle=180;//����Ƕȣ�ʵ�ʿɿؽǶȣ���Ҫ��̲��ԣ�
int angleToData=0;//1�ȶ�Ӧ��ռ�ձȵ�ֵ��500-2500��
int PWM_Min=500;
int PWM_Max=2500;
int mOffsetValue=0;//����1�ȵ����������۵�ֵ
//ʵ��У����Ҫ�Ĳ���ֵ
int mCheckAngle=0;//У�ԽǶȣ�������ĳ���Ƕ������ȷ�Ƕȵ�ƫ������*/

int mArmParSize=0;//�������
CheckParameter checkPar[16];

//��ʼ����е�ۣ�����ռ�ձȵĲ�����Χ��ʵ�ʲٿصĽǶȣ���Ҫ���в��Ի�ȡ������漰��е�۵Ĳ�����׼�ȣ������ϸ����
void intit_SteeringEngine(ArmParameter armPar[],int armParSize){
	 u8 pbuf[100];
	 float count;
	CheckParameter check;
	int i=0;
	mArmParSize=armParSize;
	
	//���ÿ������Ĳ�����У�Բ���������checkPar
	for(i=0;i<armParSize;i++){
		check.mCheckAngle=0;//��װƫ��Ƕ�
		check.armParameter=armPar[i];//�������
		check.actualValue=(float)(armPar[i].PWM_Max-armPar[i].PWM_Min)/armPar[i].mSteeringEngineAngle;//1�ȶ�Ӧ��ռ�ձ�ֵ
		//����1�ȴ����PWMֵ��������ֵ����check.actualValue�Ļ����϶����С������������λ��1�����ȵ�һ������ֵ
		if((int)check.actualValue==check.actualValue){
			check.angleToData=(int)check.actualValue;
		}else{
			check.angleToData=(int)check.actualValue+1;
		}
		
		check.armParameter.mSeep=10;//��¼�ƶ��ٶ�
		checkPar[i]=check;
	}
	 
}
//����ֵ������ȥ����ֵ�ķ���
  float Abs(float value)
{
    if (value >= 0)
    {
        return value;
    }
    return -1*value;
}
 
 //�����в�������ǰ�����ʾ��λ�õ������涨λ�ã��Դ������㰲װƫ������
//�Ӷ��淶�Ƕȣ����˴�С��Ӱ��֮��Ŀɲ�����Χ���п��ܻᵼ�¿ɲ�����Χ����180��
 int sheCheckValue(int armID,int angle){
		int count;
		if(mArmParSize<armID&&armID<0) return 0;
		if(angle<0&&angle>180)return 0;
		checkPar[armID].mCheckAngle=angle;//���ö�Ӧ����İ�װƫ��
		return 1;
 }

//����У�ԣ����м�ֵΪ90�ȵ��������ڰ�װ�����м�ֵ��λ�ÿ�����Թ涨������ƫ��
void checkInit(){
	int i=0;
	//���ÿ������Ĳ�����У�Բ���������checkPar
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
	 Bias=Abs(Target-Encoder);                                  //����ƫ��
	if(Bias<10) return (int)Bias;
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 Pwm=Position_KP*Bias/100+Position_KI*Integral_bias/100+Position_KD*(Bias-Last_Bias)/100;       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}
//ͨ���Ƕ����ö����λ��
int sheAngle(int armID,int angle,int seep){
	u8 pbuf[100];
	float count;//�����ò���
	if(mArmParSize<armID&&armID<0) return 0;//�ж�id��Ч��
	if(angle<0&&angle>180)return 0;//�жϽǶ���Ч��
	if(seep<0)return 0;//�ж��ٶ���Ч��
	
	//OLED_Clear();
	count=(angle-90)*checkPar[armID].actualValue;//�����ƶ��ĽǶȾ���90�Ⱦ��루�ǶȲ�*ÿ�ȴ����PWMֵ��
	
	//sprintf((char *)pbuf, " %02f %d %02f", count,checkPar[armID].mCheckAngle,checkPar[armID].actualValue);
	//OLED_ShowString(0,0, pbuf,12);
	count=Abs(origin+count+checkPar[armID].mCheckAngle*checkPar[armID].actualValue);//��ȡʵ�ʵ�ռ�ձ�ֵ���м�PWMֵ+�ƶ��ľ���+��װƫ�
	
	//����ռ�ձȵ�ֵ���ܳ�����ΧPWM_Min--PWM_Max����
	if(count<checkPar[armID].armParameter.PWM_Min){
		count=checkPar[armID].armParameter.PWM_Min;
	}else if(count>checkPar[armID].armParameter.PWM_Max){
		count=checkPar[armID].armParameter.PWM_Max;
	}
	checkPar[armID].armParameter.mTraget=count;//��¼Ŀ��λ��
	checkPar[armID].armParameter.mSeep=seep;//��¼�ƶ��ٶ�
	
	//sprintf((char *)pbuf, " %02f  %02f ", count,checkPar[armID].mCheckAngle*checkPar[armID].actualValue);
	//OLED_ShowString(0,24, pbuf,12);
	//OLED_Refresh_Gram();//������ʾ��OLED	 
}
//int d=0;
//���ݵ�ǰλ�ú�Ŀ��λ������ȡ�˶�ƫ�������Ӷ����õ�ǰ��PWM
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
	//OLED_Refresh_Gram();//������ʾ��OLED	 
			continue;
		}
		//count =Position_PID(checkPar[i].armParameter.mCurrentLocation,checkPar[i].armParameter.mTraget);
		
		//count=10;
		count=checkPar[i].armParameter.mSeep;
		//���ڼ����˶��㷨ֻ�����һ�ξ�����˶�����û������֮�֣���������Ҫ���ݵ�ǰλ�ú�Ŀ��λ�����ж��Ǽ��Ǽ�
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
		//��ֹ�����ƶ�λ�ó�ԽĿ��λ�ã����ƶ����벻�������ٶȣ����µ�ǰλ�ûᳬ�� Ŀ��λ��һ�����룬���������һ��С���ٶȣ�
		if(Abs(checkPar[i].armParameter.mCurrentLocation-checkPar[i].armParameter.mTraget)<checkPar[i].armParameter.mSeep){
			checkPar[i].armParameter.mCurrentLocation=checkPar[i].armParameter.mTraget;//ǿ�Ƶ�ǰλ�õ�Ŀ��λ��
		}
		sprintf((char *)pbuf, " %d ", count);
	//OLED_ShowString(80,36, pbuf,12);
	//OLED_Refresh_Gram();//������ʾ��OLED	 
		shePWM_Value(i,checkPar[i].armParameter.mCurrentLocation);//���ö��PWMֵ��
	}
}

//���ݶ��ID���ö�Ӧ��ʱ��ͨ����PWMֵ
	void shePWM_Value(int armID,int value){
		
//	OLED_ShowNum(80,0, armID,1,12);
//����ռ�ձȵ�ֵ���ܳ�����ΧPWM_Min--PWM_Max����
	if(value<checkPar[armID].armParameter.PWM_Min){
		value=checkPar[armID].armParameter.PWM_Min;
	}else if(value>checkPar[armID].armParameter.PWM_Max){
		value=checkPar[armID].armParameter.PWM_Max;
	}
	//���ö�Ӧ����Ķ�ʱ��ͨ����PWMֵ
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
	//��ʱ��2�жϺ��������¶����PWMֵ
void TIM2_IRQHandler(void) {
	
	u8 pbuf[23];
	static u8 flag = 0;
	static u8 duoji_index1 = 0;
	int temp;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //��� TIM2 �����жϷ������
	{
		LED0=!LED0;
		handle_PWM();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update ); //��� TIM2 �����жϱ�־
		
	
	}
} 

