
#include <string.h>

#include "Data_.h"
#include "USB_To_Usart.h"	

#include "oled.h"
#include "delay.h"
DataFrame dataFrame={0};
uint64_t DeviceID;
uint32_t  UserPwd;
//У��ͼ��㷽����
//offset ������
//frameLen �ܳ���
unsigned char GetSumCheckNum(char * buf, int offset, int frameLen)
{
	int i = offset;
 unsigned char xorResult = 0;
	
 for (; i < frameLen; i++)
 {
  xorResult ^= buf[i];
 }
 return xorResult;
}
//�˷����ǽ��������ߴ�С�˴�������0x1234�����Ϊ0x3412��
//*buf ƴ�����ݵ�����ָ�� 
//dataҪת��������ֵ ,���Ϊ��0xFFFFFFFF
//len Ҫת�����ֽ���,���Ϊ��4
//*count �����±�
void bigToSmall(unsigned char * buf,uint32_t data,int len,int *count){
	int i=0;
	int l=*count;
	
	for(;i<len;i++){
		buf[l++]=data>>8*(i)&0xFF;
	}
	*count=l;
}

//ƴ�� �ϴ������ݰ�
//TxBuffer �������ݵ����� len���ԣ�sendData[]�ϴ������ݣ�dataLen�ϴ����ݸ��������ô���32��
int getData(unsigned char *TxBuffer,int *len,uint32_t sendData[],int dataLen){
	int i=0,j=0,count=0;
	int ChecksumCursor=0;//��¼Ч��������ݰ���λ�ã����ں�������Ч�������
	//ƴ��֡ͷ
	TxBuffer[i++]=0x7E;
	//ƴ��֡����
	TxBuffer[i++]=0x01;
	//ƴ���豸ID
	 count=DEVICE_ID_LEN;
	while(count--){
		TxBuffer[i++]=DeviceID>>(8*count)&0xFF;
	}
	//��¼Ч���λ��
	ChecksumCursor=i++;//������Ŀǰû����ֵ���Ⱥ���������������ڸ�Ч��͸�ֵ,
	//����ƴ��
	 count=PWD_COUNT_LEN;
	while(count--){
		TxBuffer[i++]=UserPwd>>(8*count)&0xFF;
	}
	//ƴ�����ݳ��ȣ�������+֡β��
	TxBuffer[i++]=1+dataLen*(DATA_LEN+1)+1;
	//ƴ�����������
	TxBuffer[i++]=dataLen;
	for(;j<dataLen;j++){
		TxBuffer[i++]=j+1;//�������
		//�������ݣ�ÿ������4���ֽڣ�����Ҫ�����ݲ�
		bigToSmall(TxBuffer,sendData[j],DATA_LEN,&i);
	}
	//�������Ѿ������������������Ч��͵ĸ�ֵ
	TxBuffer[ChecksumCursor]=GetSumCheckNum((char *)TxBuffer,DATA_FRAME_HEAD_LEN,i);
	/**/
	//ƴ��֡β
	TxBuffer[i++]=0x7E;
	
	return i;
}

void initDataFrame(uint64_t  DeviceID_,uint32_t  UserPwd_){
	DeviceID=DeviceID_;
	UserPwd=UserPwd_;
}


//�������������ݣ������
//src���յ������ݣ�len���յ������ݳ���
int ParseServerData( u8 * src, u8 len){
	int ss=0;
	u8 buffer[100];
	char mySumCheck;
	int j=0,k=0,i=0,count=0,num=0;
	//�ж��Ƿ������С���ݳ���(һ��ָ�����ݵ����ݰ�����)
	if(len<22){
		SendDataToDevice_a("���������ݳ��Ȳ����\r\n",22);
		return 0;
	}
	//��������֡ͷ
	if((dataFrame.FrameHead=src[i++])!=0x7E){
		SendDataToDevice_a("û���ҵ�����ͷ\r\n",16);
		return 0;
	}
	//������������
	if((dataFrame.FrameType=src[i++])!=0x01){
		SendDataToDevice_a("����ͷ�����쳣\r\n",16);
		return 0;
	}
	count=DEVICE_ID_LEN;//�豸ID����
	dataFrame.DeviceID=0;//��ֵ����
	//ƴ���豸ID
	while(count--){
		dataFrame.DeviceID+=(uint64_t)src[i++]<<(8*count);
		
	}
	//�ȶ��豸ID
	if(dataFrame.DeviceID!=DeviceID){
		SendDataToDevice_a("�豸ID�쳣\r\n",12);
		return 0;
	}
	//Ч���
	dataFrame.Checksum=src[i++];
	//����Ч��ͣ�������֡β���ԣ�len-1��
	mySumCheck=GetSumCheckNum((char*)src,DATA_FRAME_HEAD_LEN,len-1);
	//�ȶ�Ч���
	if(dataFrame.Checksum!=mySumCheck){
			//sprintf((char *)buffer,"Ч��� %d\r\n", mySumCheck);
					//	SendDataToDevice_a(buffer,16);
		//SendDataToDevice_a("Ч����쳣\r\n",12);
		//return 0;
	}
	//����
	count=PWD_COUNT_LEN;
	dataFrame.UserPwd=0;//��ֵ����
	while(count--){
		dataFrame.UserPwd+=(uint32_t)src[i++]<<(8*count);
		
	}
	//����ָ���
	dataFrame.DataLen=src[i++];
	if(dataFrame.DataLen!=(len-i)){
		sprintf((char *)buffer,"���ݳ��� %d  %d\r\n", len,(len-i));
						SendDataToDevice_a(buffer,16);
		//SendDataToDevice_a("���ݳ����쳣\r\n",14);
		return 0;
	}
	dataFrame.mOrderDataCount=src[i++];
	//�洢ָ����ֵ��dataFrame.mOrderConnect[]���飬
	//��Ӧ��ָ����ֵ��������±�=���ݱ��-1�����磬val5=dataFrame.mOrderConnect[5-1]��
	for(;k<dataFrame.mOrderDataCount;k++){
		num=src[i++];//���ݱ��
		dataFrame.mOrderConnect[num-1]=0;//��ֵ����
		//ƴ����������
		for(j=0;j<4;j++){
			dataFrame.mOrderConnect[num-1]+=src[i++]<<8*j;
		}
	
	}
	//��β
	if((dataFrame.FrameTail=src[i++])!=0x7E){
		
		SendDataToDevice_a("û���ҵ�����β\r\n",16);
		return 0;
	}
	/*for(ss=0;ss<32;ss++){
						sprintf((char *)buffer,"val%d��ֵ:%d %d %d %d  %d\r\n", ss+1,dataFrame.mOrderConnect[ss]>>24&0xFF,dataFrame.mOrderConnect[ss]>>16&0xFF,dataFrame.mOrderConnect[ss]>>8&0xFF,dataFrame.mOrderConnect[ss]&0xFF,dataFrame.mOrderConnect[ss]);
						SendDataToDevice_a(buffer, 40);
						delay_ms(10);
						
					}*/
	return i;
}


