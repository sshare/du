

#ifndef DATA__H_
#define DATA__H_

#include "stm32f4xx.h" 

#define	DEVICE_ID_LEN			8
#define	PWD_COUNT_LEN			3       //密码长度
#define	DATA_LEN			4     //每组数据长度
#define	DATA_FRAME_HEAD_LEN			15   //帧头长

//uint8_t 1字节，uint16_t/uint32_t/uint64_t  2/4/8字节
/*typedef struct _DataFrame_{
	__IO uint8_t  FrameHead;//帧头 1
	__IO uint8_t  FrameType;//帧类型 1
		
	__IO uint64_t DeviceID;//设备ID 8
	
	
	__IO uint32_t  UserPwd;//登录密码 3字节
	
	__IO uint8_t FrameTail;//帧尾 1
	
}DataFrame;*/

typedef struct _DataFrame_{
	
	__IO uint8_t  FrameHead;//帧头 1
	__IO uint8_t  FrameType;//帧类型 1
		
	__IO uint64_t DeviceID;//设备ID 8
	
	__IO uint8_t Checksum;//效验和 1
	
	__IO uint32_t  UserPwd;//登录密码 3字节
	
	__IO uint8_t DataLen;//数据长度（数据区+帧尾） 1
	
	int mOrderDataCount;//数据个数（val1...val32） 1
	__IO uint8_t FrameTail;//帧尾 1
	uint32_t mOrderConnect[32]; 
}DataFrame;


extern DataFrame dataFrame;
	void initDataFrame(uint64_t  DeviceID_,uint32_t  UserPwd_);
int getData(unsigned char *TxBuffer,int *len,uint32_t sendData[],int dataLen);
int ParseServerData( u8 * src, u8 len);
	//int getSendData(OrderType dataType,unsigned char * buffer);
#endif


