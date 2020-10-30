
#include <string.h>
#include "DevParamsParse.h"
#include "Data_.h"

//λ����Ϣ�ṹ���ʼ�����
BDData  g_devParams = {0};
//���÷������ݵĽṹ�����ڽ���������ָ��ĶԱȣ��豸ID��
extern DataFrame  dataFrame;
//ָ��ṹ���ʼ������ָ����翪�أ�
OrderData orderData={0};

void initianDeviceParams()
{

}

/*
char *GPS_QGNSSRD_PRE = "\r\n+QGNSSRD: $GNRMC,031959.";
char *GPS_QGNSSRD_PRE_FLAG = "\r\n+QGNSSRD: ";
*/
//char *GPS_QGNSSRD_PRE = "\r\n+QGNSSRD: $GNRMC,";
void MC20ParseGPSData(unsigned char * src, int len)
{
	int i = 0;
	int preStrLen = 0;
	if (len <= 75)
		return;
	
	if (len <= 75)
		return;
	preStrLen = strlen("AT+QGNSSRD=\"NMEA/RMC\"\r\n+QGNSSRD: $GNRMC,");
	while( (i+preStrLen) < len)
	{
		i++;
		if (src[i+preStrLen] == ',')
			break;
	}
	i++;
	while( (i+preStrLen) < len)
	{
		i++;
		if (src[i+preStrLen] == ',')
			break;
	}	
	i++;
	preStrLen += i;
	//��Ч������0
	if (src[i+preStrLen+1] == ',')
	{
		g_devParams.wdDu = 0;
		g_devParams.wdFen = 0;
		g_devParams.wdMiao = 0;

		g_devParams.jdDu = 0;
		g_devParams.jdFen = 0;
		g_devParams.jdMiao = 0;		
	
		g_devParams.Altitude = 0;
		return;
	}
	preStrLen -= 2;
	g_devParams.wdDu = shiftPreToNum(src+preStrLen+2, 2);
	g_devParams.wdFen = shiftPreToNum(src+preStrLen+4, 2);
	g_devParams.wdMiao = shiftPreToNum(src+preStrLen+7, 4);
	g_devParams.wdMiao = g_devParams.wdMiao*60/10000;
	g_devParams.wdMiao *= 10;
	if(*(src+preStrLen+12) == 'S')
		g_devParams.wd = 1;
	else
		g_devParams.wd = 2;
	
	g_devParams.jdDu = shiftPreToNum(src+preStrLen+14, 3);
	g_devParams.jdFen = shiftPreToNum(src+preStrLen+17, 2);
	g_devParams.jdMiao = shiftPreToNum(src+preStrLen+20, 4);
	g_devParams.jdMiao = g_devParams.jdMiao*60/10000;
	g_devParams.jdMiao *= 10;
	if(*(src+preStrLen+25) == 'W')
		g_devParams.jd = 2;
	else
		g_devParams.jd = 1;	
	
}
OrderType getOrderType(u8 id){
	switch(id){
		case 1://
			return TYPE_AS608;
		case 3://
			return TYPE_INFRARED;
	}
}
int getOrderLen(char id){
	switch(id){
		case 1://
			return 4;
		case 2://���� ��
			return 2;
		case 3://�������
			return 1;
		case 4://
			return 1;
		case 5://
			return 1;
		case 6://�¶�
			return 2;
		case 7://ʪ��
			return 2;
		case 8://���� ��
			return 2;
		case 9://���� ��
			return 2;
		case 10://γ�� ��
			return 2;
		case 11://
			return 1;
		case 12://γ�� ��
			return 2;
		case 13://γ�� ��
			return 2;
	}
}

//��������������
int ParseServerData(unsigned char * src, int len){
	DataFrame parseDataFrame={0};
	int j=0;
	int i=0;
	int count=0;
	char buffer3[12];
	//uint8_t Checksum;
	//֡ͷ 28+��������min 3��+֡β 1
//	SendDataToDevice("00",2);
	if(len<32){
		return 0;
	}
	
	//SendDataToDevice("01",2);
	if(src[i++]!=0x7E){
		return 0;
	}
	
	//SendDataToDevice("02",2);
	//��������
	if(src[i++]!=0x01){
		return 0;
	}
	
	//SendDataToDevice("03",2);
	//�ְ���ʶ
	if(src[i++]!=0x00){
		return 0;
	}
	
//	SendDataToDevice("04",2);
	//�豸ID
	
	//dveID
	count=8;
	while(count--){
		parseDataFrame.DeviceID+=(uint64_t)src[i++]<<(8*count);
		
	}
	
	//SendDataToDevice("05",2);
	//�ȶ��豸ID
	if(parseDataFrame.DeviceID!=dataFrame.DeviceID){
		return 0;
	}
	//SendDataToDevice("06",2);
	//Ч���
	parseDataFrame.Checksum=src[i++];
	
	//�ְ����
	count=4;
	while(count--){
		parseDataFrame.SubpackageCount+=src[i++]<<8*(3-count);
	}
	
	//SendDataToDevice("07",2);
	if(parseDataFrame.SubpackageCount!=0x00){
		return 0;
	}
	
	//SendDataToDevice("08",2);
	//����ħ��ֵ
	count=4;
	while(count--){
		parseDataFrame.SubpackageCount+=src[i++]<<8*(3-count);
	}
	if(parseDataFrame.SubpackageCount!=0x00){
		return 0;
	}
	//SendDataToDevice("09",2);
	
	//��֡��
	count=4;
	while(count--){
		parseDataFrame.FrameTotalLength+=src[i++]<<8*(3-count);
	}
	
	//��ǰ֡��
	count=4;
	while(count--){
		parseDataFrame.CurrentFrameLength+=src[i++]<<8*(3-count);
	}
	
	//SendDataToDevice("10",2);
	//�Աȳ���
	if(parseDataFrame.FrameTotalLength!=parseDataFrame.CurrentFrameLength){
		return 0;
	}
	
	//SendDataToDevice("11",2);
	//�Ա����ݳ���
	if(len-parseDataFrame.CurrentFrameLength!=DATA_FRAME_HEAD_LEN){
		return 0;
	}
	//SendDataToDevice("12",2);
	
	//ָ�����
	count=src[i++];
	//ָ�����
	count=src[i++];
	//while(count--){
		orderData.mOrderType=getOrderType(count);
	//����ָ���
		count=getOrderLen(count);
	/*
	if(count==1){
		orderData.mOrderConnect=src[i++];
	}else if(count==2){
		//�͸�λ
		orderData.mOrderConnect+=src[i++];
		
		orderData.mOrderConnect+=src[i++]<<8;
	}
	*/
	orderData.mOrderConnect=0;
	for(;j<count;j++){
		orderData.mOrderConnect+=src[i++]<<(8*j);
	}
	i++;//��β
	//SendDataToDevice("13",2);
	return i;
	//}
}



