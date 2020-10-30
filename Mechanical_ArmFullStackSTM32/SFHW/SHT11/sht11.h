
#ifndef __SHT11_H__
#define __SHT11_H__
#include "stm32f4xx.h"

enum {TEMP, HUMI};

/* GPIO????? */
#define SHT11_AHB1_CLK        RCC_AHB1Periph_GPIOB
#define SHT11_DATA_PIN        GPIO_Pin_9
#define SHT11_SCK_PIN        	GPIO_Pin_8
#define SHT11_DATA_PORT       GPIOB
#define SHT11_SCK_PORT        GPIOB


#define SHT11_DATA_H()       GPIO_SetBits(SHT11_DATA_PORT, SHT11_DATA_PIN)                       
#define SHT11_DATA_L()       GPIO_ResetBits(SHT11_DATA_PORT, SHT11_DATA_PIN)                       
#define SHT11_DATA_R()       GPIO_ReadInputDataBit(SHT11_DATA_PORT, SHT11_DATA_PIN)         

#define SHT11_SCK_H()        GPIO_SetBits(SHT11_SCK_PORT, SHT11_SCK_PIN)                           
#define SHT11_SCK_L()        GPIO_ResetBits(SHT11_SCK_PORT, SHT11_SCK_PIN)                        


#define       noACK       			 0
#define 			ACK                1
                                                            
#define STATUS_REG_W        0x06       
#define STATUS_REG_R        0x07       
#define MEASURE_TEMP        0x03       
#define MEASURE_HUMI        0x05       
#define SOFTRESET    			  0x1e        


void SHT11_Config(void);
void SHT11_ConReset(void);
u8 SHT11_Measure(u16 *p_value, u8 *p_checksum, u8 mode);
void SHT11_Calculate(u16 t, u16 rh,float *p_temperature, float *p_humidity);
//u8 SHT11_SoftReset(void);
//float SHT11_CalcuDewPoint(float t, float h);
//void SHT11_Dly(void);
//u8 SHT11_ReadStatusReg(u8 *p_value, u8 *p_checksum);
#endif

