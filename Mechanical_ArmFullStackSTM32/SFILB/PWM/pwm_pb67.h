#ifndef _PWM_PB67_H
#define _PWM_PB67_H

#include "stm32f4xx.h" 
void setTIM_ENABLE();
void setTIM_DISABLE();
void TIM_Configuration(void);   //TIM3??
void TIM2_PWM_Init(u32 arr,u16 psc);
void TIM14_PWM_Init(u32 arr,u32 psc);
void TIM9_PWM_Init(u32 arr,u32 psc);
void TIM13_PWM_Init(u32 arr,u32 psc);
void TIM4_PWM_Init(u32 arr,u32 psc);
void TIM3_PWM_Init(u32 arr,u32 psc);
void TIM1_PWM_Init(u32 arr,u32 psc);

void TIM2_Int_Init(u32 arr,u16 psc);
#endif
