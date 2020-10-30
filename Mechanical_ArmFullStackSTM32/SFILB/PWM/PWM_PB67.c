
#include "pwm_pb67.h"
#include "stm32f4xx.h" 
#include "stm32f4xx_tim.h" 
//TIM14 PWM?????
//PWM?????
//arr?????
//psc?????
void TIM14_PWM_Init(u32 arr,u32 psc)
{		 					 
	//????????IO???
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM14????    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//??PORTF??
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); //GPIOF9∏??????14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //????
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //??????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//??100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //??
	GPIO_Init(GPIOF,&GPIO_InitStructure);              //???PF9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //?????
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????
	TIM_TimeBaseStructure.TIM_Period=arr;   //??????
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);//??????14
	
	//???TIM14 Channel1 PWM??	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //???????:TIM????????2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //??????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //????:TIM???????
	
    //TIM_OCInitStructure.TIM_Pulse = 0;//????????0
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);  //?????????TIM14 OC1??

TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);  //?IM14?CCR1????????
 
  TIM_ARRPreloadConfig(TIM14,ENABLE);//ARPE??
	
	TIM_Cmd(TIM14, ENABLE);  //??TIM14
 
										  
}  


//TIM14 PWM?????
//PWM?????
//arr?????
//psc?????
void TIM13_PWM_Init(u32 arr,u32 psc)
{		 					 
	//????????IO???
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);  	//TIM14????    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//??PORTF??
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource8,GPIO_AF_TIM13); //GPIOF9∏??????14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //????
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //??????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//??100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //??
	GPIO_Init(GPIOF,&GPIO_InitStructure);              //???PF9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //?????
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????
	TIM_TimeBaseStructure.TIM_Period=arr;   //??????
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM13,&TIM_TimeBaseStructure);//??????14
	
	//???TIM14 Channel1 PWM??	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //???????:TIM????????2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //??????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //????:TIM???????
	
    //TIM_OCInitStructure.TIM_Pulse = 0;//????????0
	TIM_OC1Init(TIM13, &TIM_OCInitStructure);  //?????????TIM14 OC1??

TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Enable);  //?IM14?CCR1????????
 
  TIM_ARRPreloadConfig(TIM13,ENABLE);//ARPE??
	
	TIM_Cmd(TIM13, ENABLE);  //??TIM14
 
										  
}  

//TIM14 PWM?????
//PWM?????
//arr?????
//psc?????
void TIM9_PWM_Init(u32 arr,u32 psc)
{		 					 
	//????????IO???
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);  	//TIM14????    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//??PORTF??
	
	//GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_TIM9); //GPIOF9∏??????14
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource6,GPIO_AF_TIM9);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //????
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //??????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//??100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //??
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //???PF9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //?????
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????
	TIM_TimeBaseStructure.TIM_Period=arr;   //??????
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseStructure);//??????14
	
	
	
	//???TIM14 Channel1 PWM??	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //???????:TIM????????2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //??????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //????:TIM???????
	
    //TIM_OCInitStructure.TIM_Pulse = 0;//????????0
	//TIM_OC1Init(TIM9, &TIM_OCInitStructure);  //?????????TIM14 OC1??

//TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);  //?IM14?CCR1????????
 TIM_OC2Init(TIM9, &TIM_OCInitStructure);  //?????????TIM14 OC1??

	TIM_OC2PreloadConfig(TIM9, TIM_OCPreload_Enable);  //?IM14?CCR1????????
  TIM_ARRPreloadConfig(TIM9,ENABLE);//ARPE??

	TIM_Cmd(TIM9, ENABLE);  //??TIM14									  
}  
void setTIM_ENABLE(){
	TIM_Cmd(TIM9, ENABLE);  //??TIM14		
}
void setTIM_DISABLE(){
	TIM_Cmd(TIM9, DISABLE);  //??TIM14		
}
//TIM4 PWM?????
//PWM?????
//arr?????
//psc?????
void TIM4_PWM_Init(u32 arr,u32 psc)
{		 					 
	//????????IO???
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM4????    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 	//??PORTC??
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); //GPIOD13∏??????14
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4); //GPIOD13∏??????14
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_TIM4); //GPIOD13??????14
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;           //GPIOD13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //????
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //??????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//??100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //??
	GPIO_Init(GPIOD,&GPIO_InitStructure);              //???PD13
	
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //?????
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????
	TIM_TimeBaseStructure.TIM_Period=arr;   //??????
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//??????4
	
	//???TIM8 Channel1 PWM??	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //???????:TIM????????2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //??????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //????:TIM???????
	
    //TIM_OCInitStructure.TIM_Pulse = 0;//????????0
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //?????????TIM4 OC2??
	TIM_OC2Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC4Init(TIM4, &TIM_OCInitStructure); 
	
	
TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //?IM4?CCR2????????
 TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
 TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
 
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPE??
	
	TIM_Cmd(TIM4, ENABLE);  //??TIM4
 
										  
}  
//TIM3 PWM?????
//PWM?????
//arr?????
//psc?????
void TIM3_PWM_Init(u32 arr,u32 psc)
{		 					 
	//????????IO???
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3????    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOB, ENABLE); 	//??PORTC??
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3); //GPIOC6∏??????3		
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3); //GPIOC6??????3	
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM3); //GPIOC6??????3	
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM3); //GPIOC6??????3	

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;           //GPIOC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //????
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //??????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//??100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //??
	GPIO_Init(GPIOC,&GPIO_InitStructure);              //???PC6
	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //?????
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????
	TIM_TimeBaseStructure.TIM_Period=arr;   //??????
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//??????3
	 
	//???TIM3 Channel1 PWM??	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //???????:TIM????????2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //??????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //????:TIM???????
	
   TIM_OCInitStructure.TIM_Pulse = 0;//????????0
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //?????????TIM3 OC1??

TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //?IM3?CCR1????????

	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //?????????TIM3 OC1??
TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //?IM3?CCR1????????

	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //?????????TIM3 OC1??
TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //?IM3?CCR1????????

	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //?????????TIM3 OC1??
TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //?IM3?CCR1????????
/**/
 
  TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPC??
	
	TIM_Cmd(TIM3, ENABLE);  //??TIM3
 
										  
}  
//TIM1 PWM?????
//PWM?????
//arr?????
//psc?????
void TIM1_PWM_Init(u32 arr,u32 psc)
{		 					 
	//????????IO???
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1????    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//??PORTC??
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1); //GPIOA9∏??????1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           //GPIOA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //????
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //??????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//??100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //??
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //???PA9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //?????
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????
	TIM_TimeBaseStructure.TIM_Period=arr;   //??????
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//??????1
	
	//???TIM1 Channel1 PWM??	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //???????:TIM????????2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //??????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //????:TIM???????
	
    //TIM_OCInitStructure.TIM_Pulse = 0;//????????0
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //?????????TIM1 OC2??

TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //?IM1?CCR2????????
 
  TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPC??
	
	TIM_Cmd(TIM1, ENABLE);  //??TIM3
 
										  
}  

void TIM2_Int_Init(u32 arr,u16 psc) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //¢Ÿ ±÷” TIM2  πƒ‹	
	//∂® ±∆˜ TIM2 ≥ı ºªØ
	TIM_TimeBaseStructure.TIM_Period = arr; //…Ë÷√◊‘∂Ø÷ÿ◊∞‘ÿºƒ¥Ê∆˜÷‹∆⁄µƒ÷µ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //…Ë÷√ ±÷”∆µ¬ ≥˝ ˝µƒ‘§∑÷∆µ÷µ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //…Ë÷√ ±÷”∑÷∏Ó
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM œÚ…œº∆ ˝
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);  //¢⁄≥ı ºªØ TIM2
	TIM_ARRPreloadConfig(TIM2, DISABLE);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE );  //¢€‘ –Ì∏¸–¬÷–∂œ
	
	//÷–∂œ”≈œ»º∂ NVIC …Ë÷√
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2 ÷–∂œ
	//NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x0000);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //œ»’º”≈œ»º∂ 0 º∂
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //¥””≈œ»º∂ 2 º∂
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //IRQ Õ®µ¿±ª πƒ‹
	NVIC_Init(&NVIC_InitStructure);  //¢‹≥ı ºªØ NVIC ºƒ¥Ê∆˜
	TIM_Cmd(TIM2, ENABLE);  //¢› πƒ‹ TIM2
}



