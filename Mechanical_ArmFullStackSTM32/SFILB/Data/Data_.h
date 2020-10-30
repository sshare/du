

#ifndef DATA__H_
#define DATA__H_

#include "stm32f4xx.h" 

#define	DEVICE_ID_LEN			8
#define	PWD_COUNT_LEN			3       //���볤��
#define	DATA_LEN			4     //ÿ�����ݳ���
#define	DATA_FRAME_HEAD_LEN			15   //֡ͷ��

//uint8_t 1�ֽڣ�uint16_t/uint32_t/uint64_t  2/4/8�ֽ�
/*typedef struct _DataFrame_{
	__IO uint8_t  FrameHead;//֡ͷ 1
	__IO uint8_t  FrameType;//֡���� 1
		
	__IO uint64_t DeviceID;//�豸ID 8
	
	
	__IO uint32_t  UserPwd;//��¼���� 3�ֽ�
	
	__IO uint8_t FrameTail;//֡β 1
	
}DataFrame;*/

typedef struct _DataFrame_{
	
	__IO uint8_t  FrameHead;//֡ͷ 1
	__IO uint8_t  FrameType;//֡���� 1
		
	__IO uint64_t DeviceID;//�豸ID 8
	
	__IO uint8_t Checksum;//Ч��� 1
	
	__IO uint32_t  UserPwd;//��¼���� 3�ֽ�
	
	__IO uint8_t DataLen;//���ݳ��ȣ�������+֡β�� 1
	
	int mOrderDataCount;//���ݸ�����val1...val32�� 1
	__IO uint8_t FrameTail;//֡β 1
	uint32_t mOrderConnect[32]; 
}DataFrame;


extern DataFrame dataFrame;
	void initDataFrame(uint64_t  DeviceID_,uint32_t  UserPwd_);
int getData(unsigned char *TxBuffer,int *len,uint32_t sendData[],int dataLen);
int ParseServerData( u8 * src, u8 len);
	//int getSendData(OrderType dataType,unsigned char * buffer);
#endif


