#include "SHT11.h"
//#include "usart.h"
#include <math.h>

void SHT11_Dly(void)
{
	u16 i;
	for(i = 500; i > 0; i--);
}



void SHT11_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;        

		RCC_AHB1PeriphClockCmd(SHT11_AHB1_CLK ,ENABLE);
					 
 
		GPIO_InitStructure.GPIO_Pin = SHT11_DATA_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
//		GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_UP;
		GPIO_Init(SHT11_DATA_PORT, &GPIO_InitStructure);



		GPIO_InitStructure.GPIO_Pin = SHT11_SCK_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//复用功能
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输
//		GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_UP;
		GPIO_Init(SHT11_SCK_PORT, &GPIO_InitStructure);

		SHT11_ConReset();   
}



void SHT11_DATAOut(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;       
	GPIO_InitStructure.GPIO_Pin = SHT11_DATA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
//		GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_UP;	
	GPIO_Init(SHT11_DATA_PORT, &GPIO_InitStructure);
}



void SHT11_DATAIn(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//复用功能
	GPIO_InitStructure.GPIO_Pin = SHT11_DATA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出  
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_UP;
	GPIO_Init(SHT11_DATA_PORT, &GPIO_InitStructure);
}



u8 SHT11_WriteByte(u8 value)
{
		u8 i, err = 0;
		
		SHT11_DATAOut(); 



		for(i = 0x80; i > 0; i /= 2)  
		{
				if(i & value)
								SHT11_DATA_H();
				else
								SHT11_DATA_L();
				SHT11_Dly();
				SHT11_SCK_H();
				SHT11_Dly();
				SHT11_SCK_L();
				SHT11_Dly();
		}
		SHT11_DATAIn();                            
		SHT11_SCK_H();
		err = SHT11_DATA_R();             
		SHT11_SCK_L();

		return err;
}


u8 SHT11_ReadByte(u8 Ack)
{
		u8 i, val = 0;

		SHT11_DATAIn();                                
		for(i = 0x80; i > 0; i /= 2)  
		{
			SHT11_Dly();
			SHT11_SCK_H();
			SHT11_Dly();
			if(SHT11_DATA_R())
			val = (val | i);
			SHT11_SCK_L();
		}
		SHT11_DATAOut();                          
		if(Ack)
						SHT11_DATA_L();                          
		else
						SHT11_DATA_H();     				
		SHT11_Dly();
		SHT11_SCK_H();
		SHT11_Dly();
		SHT11_SCK_L();
		SHT11_Dly();

		return val;                                       
}

/*************************************************************
  Function   :SHT11_TransStart  
  Description:??????,????:
                     _____         ________
               DATA:      |_______|
                         ___     ___
               SCK : ___|   |___|   |______        
  Input      : none        
  return     : none    
*************************************************************/

void SHT11_TransStart(void)
{
		SHT11_DATAOut();                          

		//
		SHT11_DATA_H();
		SHT11_SCK_L();
		SHT11_Dly();
		SHT11_SCK_H();
		SHT11_Dly();
		SHT11_DATA_L();
		SHT11_Dly();
		SHT11_SCK_L();
		SHT11_Dly();
		SHT11_SCK_H();
		SHT11_Dly();
		SHT11_DATA_H();
		SHT11_Dly();
		SHT11_SCK_L();

}

/*************************************************************
  Function   :SHT11_ConReset  
  Description:????,????:
                     _____________________________________________________         ________
               DATA:                                                      |_______|
                        _    _    _    _    _    _    _    _    _        ___     ___
               SCK : __| |__| |__| |__| |__| |__| |__| |__| |__| |______|   |___|   |______
  Input      : none        
  return     : none    
*************************************************************/

void SHT11_ConReset(void)
{
		u8 i;

		SHT11_DATAOut();

		SHT11_DATA_H();
		SHT11_SCK_L();

		for(i = 0; i < 9; i++)                
		{
			SHT11_SCK_H();
			SHT11_Dly();
			SHT11_SCK_L();
			SHT11_Dly();
		}
		SHT11_TransStart();                       
}




u8 SHT11_SoftReset(void)
{
	u8 err = 0;

	SHT11_ConReset();                            
	err += SHT11_WriteByte(SOFTRESET);

	return err;
}



u8 SHT11_ReadStatusReg(u8 *p_value, u8 *p_checksum)
{
	u8 err = 0;

	SHT11_TransStart();                                    
	err = SHT11_WriteByte(STATUS_REG_R);
	*p_value = SHT11_ReadByte(ACK);               
	*p_checksum = SHT11_ReadByte(noACK);
	
	return err;
}




u8 SHT11_WriteStatusReg(u8 *p_value)
{
	u8 err = 0;

	SHT11_TransStart();                                   
	err += SHT11_WriteByte(STATUS_REG_W);
	err += SHT11_WriteByte(*p_value);       

	return err;
}




u8 SHT11_Measure(u16 *p_value, u8 *p_checksum, u8 mode)
{
	u8 err = 0;
	u32 i;
	u8 value_H = 0;
	u8 value_L = 0;

	SHT11_TransStart();                                             
	switch(mode)                                                         
	{
	case TEMP:                                                              
		err += SHT11_WriteByte(MEASURE_TEMP);
		break;
	case HUMI:
		err += SHT11_WriteByte(MEASURE_HUMI);
		break;
	default:
		break;
	}
	SHT11_DATAIn();
	for(i = 0; i < 72000000; i++)                            
	{
		if(SHT11_DATA_R() == 0) break;            
	}
	if(SHT11_DATA_R() == 1)                                
					err += 1;
	value_H = SHT11_ReadByte(ACK);
	value_L = SHT11_ReadByte(ACK);
	*p_checksum = SHT11_ReadByte(noACK);          
	*p_value = (value_H << 8) | value_L;
	return err;
}

//根据环境因素做偿运算，计算获取到精确的温湿度数据（可参考文档）
void SHT11_Calculate(u16 t, u16 rh, float *p_temperature, float *p_humidity)
{
	const float d1 = -39.7;
	const float d2 = +0.01;
	const float C1 = -2.0468;
	const float C2 = +0.0367;
	const float C3 = -0.0000015955;        
	const float T1 = +0.01;
	const float T2 = +0.00008;

	float RH_Lin;                                                                           
	float RH_Ture;                                                                               
	float temp_C;

	temp_C = d1 + d2 * t;                                                      
	RH_Lin = C1 + C2 * rh + C3 * rh * rh;                   
	RH_Ture = (temp_C -25) * (T1 + T2 * rh) + RH_Lin;        

	if(RH_Ture > 100)                                                                      
					RH_Ture        = 100;
	if(RH_Ture < (float)0.1)
					RH_Ture = 0.1;                                                                   

	*p_humidity = RH_Ture;
	*p_temperature = temp_C;

}



float SHT11_CalcuDewPoint(float t, float h)
{
	float logEx, dew_point;

	logEx = (float)((float)0.66077 + (float)7.5 * t / ((float)237.3 + t) + (log10(h) - 2));
	dew_point = (float)((((float)0.66077 - logEx) * (float)237.3) / (logEx - (float)8.16077));

	return dew_point; 
}
