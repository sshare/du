#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"


//uint8_t 1字节，uint16_t/uint32_t/uint64_t  2/4/8字节
typedef struct _ArmParameter_{
	int mSteeringEngineAngle;//舵机角度（实际可控角度，需要编程测试）
	int PWM_Min;//最小操作值
	int PWM_Max;//最大操作值
	int mCurrentLocation;//当前位置（占空比值），暂时没有用到
	int mTraget;//目标位置（占空比值），暂时没有用到
	
	int mSeep;//速度
}ArmParameter;

//uint8_t 1字节，uint16_t/uint32_t/uint64_t  2/4/8字节
typedef struct _CheckParameter_{
	ArmParameter armParameter;
	int angleToData;//1度对应的占空比的值（500-2500），是个近似值，多出的小数部分做+1处理，例如1.237=2
	float actualValue;//PWM/单位角度（一个无线接近的实际值）
	//int mOffsetValue;//产生1度的误差所需积累的值
	//实际校对需要的参数值
	int mCheckAngle;//校对角度，（就是某个角度相对正确角度的偏移量）
	
}CheckParameter;
extern int mArmParSize;
extern CheckParameter checkPar[16];
//初始化机械臂，其中占空比的操作范围、实际操控的角度，需要自行测试获取，这个涉及机械臂的操作精准度，务必仔细测量
	void intit_SteeringEngine(ArmParameter armPar[],int armParSize);
 //需自行操作将当前舵机显示的位置调整到规定位置，以此来计算安装偏移量，
//从而规范角度（便宜大小会影响之后的可操作范围，有可能会导致可操作范围不足180）
	int sheCheckValue(int armID,int angle);
	int sheAngle(int armID,int angle,int seep);
	void checkInit();//说有舵机归中，用于校对
	void shePWM_Value(int armID,int value);//通过PWM值操控舵机
 float Abs(float value);
#endif
