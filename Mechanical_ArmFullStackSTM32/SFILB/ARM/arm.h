#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"


//uint8_t 1�ֽڣ�uint16_t/uint32_t/uint64_t  2/4/8�ֽ�
typedef struct _ArmParameter_{
	int mSteeringEngineAngle;//����Ƕȣ�ʵ�ʿɿؽǶȣ���Ҫ��̲��ԣ�
	int PWM_Min;//��С����ֵ
	int PWM_Max;//������ֵ
	int mCurrentLocation;//��ǰλ�ã�ռ�ձ�ֵ������ʱû���õ�
	int mTraget;//Ŀ��λ�ã�ռ�ձ�ֵ������ʱû���õ�
	
	int mSeep;//�ٶ�
}ArmParameter;

//uint8_t 1�ֽڣ�uint16_t/uint32_t/uint64_t  2/4/8�ֽ�
typedef struct _CheckParameter_{
	ArmParameter armParameter;
	int angleToData;//1�ȶ�Ӧ��ռ�ձȵ�ֵ��500-2500�����Ǹ�����ֵ�������С��������+1��������1.237=2
	float actualValue;//PWM/��λ�Ƕȣ�һ�����߽ӽ���ʵ��ֵ��
	//int mOffsetValue;//����1�ȵ����������۵�ֵ
	//ʵ��У����Ҫ�Ĳ���ֵ
	int mCheckAngle;//У�ԽǶȣ�������ĳ���Ƕ������ȷ�Ƕȵ�ƫ������
	
}CheckParameter;
extern int mArmParSize;
extern CheckParameter checkPar[16];
//��ʼ����е�ۣ�����ռ�ձȵĲ�����Χ��ʵ�ʲٿصĽǶȣ���Ҫ���в��Ի�ȡ������漰��е�۵Ĳ�����׼�ȣ������ϸ����
	void intit_SteeringEngine(ArmParameter armPar[],int armParSize);
 //�����в�������ǰ�����ʾ��λ�õ������涨λ�ã��Դ������㰲װƫ������
//�Ӷ��淶�Ƕȣ����˴�С��Ӱ��֮��Ŀɲ�����Χ���п��ܻᵼ�¿ɲ�����Χ����180��
	int sheCheckValue(int armID,int angle);
	int sheAngle(int armID,int angle,int seep);
	void checkInit();//˵�ж�����У�����У��
	void shePWM_Value(int armID,int value);//ͨ��PWMֵ�ٿض��
 float Abs(float value);
#endif
