
#include <string.h>
#include "DevParamsParse.h"
#include "Data_.h"

//位置信息结构体初始化句柄
BDData  g_devParams = {0};
//引用发送数据的结构体用于解析服务器指令的对比（设备ID）
extern DataFrame  dataFrame;
//指令结构体初始化（简单指令比如开关）
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
	//无效，则清0
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
		case 2://经度 度
			return 2;
		case 3://人体红外
			return 1;
		case 4://
			return 1;
		case 5://
			return 1;
		case 6://温度
			return 2;
		case 7://湿度
			return 2;
		case 8://经度 分
			return 2;
		case 9://经度 秒
			return 2;
		case 10://纬度 度
			return 2;
		case 11://
			return 1;
		case 12://纬度 分
			return 2;
		case 13://纬度 秒
			return 2;
	}
}

//解析服务器数据
int ParseServerData(unsigned char * src, int len){
	DataFrame parseDataFrame={0};
	int j=0;
	int i=0;
	int count=0;
	char buffer3[12];
	//uint8_t Checksum;
	//帧头 28+数据区（min 3）+帧尾 1
//	SendDataToDevice("00",2);
	if(len<32){
		return 0;
	}
	
	//SendDataToDevice("01",2);
	if(src[i++]!=0x7E){
		return 0;
	}
	
	//SendDataToDevice("02",2);
	//数据类型
	if(src[i++]!=0x01){
		return 0;
	}
	
	//SendDataToDevice("03",2);
	//分包标识
	if(src[i++]!=0x00){
		return 0;
	}
	
//	SendDataToDevice("04",2);
	//设备ID
	
	//dveID
	count=8;
	while(count--){
		parseDataFrame.DeviceID+=(uint64_t)src[i++]<<(8*count);
		
	}
	
	//SendDataToDevice("05",2);
	//比对设备ID
	if(parseDataFrame.DeviceID!=dataFrame.DeviceID){
		return 0;
	}
	//SendDataToDevice("06",2);
	//效验和
	parseDataFrame.Checksum=src[i++];
	
	//分包序号
	count=4;
	while(count--){
		parseDataFrame.SubpackageCount+=src[i++]<<8*(3-count);
	}
	
	//SendDataToDevice("07",2);
	if(parseDataFrame.SubpackageCount!=0x00){
		return 0;
	}
	
	//SendDataToDevice("08",2);
	//任务魔法值
	count=4;
	while(count--){
		parseDataFrame.SubpackageCount+=src[i++]<<8*(3-count);
	}
	if(parseDataFrame.SubpackageCount!=0x00){
		return 0;
	}
	//SendDataToDevice("09",2);
	
	//总帧长
	count=4;
	while(count--){
		parseDataFrame.FrameTotalLength+=src[i++]<<8*(3-count);
	}
	
	//当前帧长
	count=4;
	while(count--){
		parseDataFrame.CurrentFrameLength+=src[i++]<<8*(3-count);
	}
	
	//SendDataToDevice("10",2);
	//对比长度
	if(parseDataFrame.FrameTotalLength!=parseDataFrame.CurrentFrameLength){
		return 0;
	}
	
	//SendDataToDevice("11",2);
	//对比数据长度
	if(len-parseDataFrame.CurrentFrameLength!=DATA_FRAME_HEAD_LEN){
		return 0;
	}
	//SendDataToDevice("12",2);
	
	//指令个数
	count=src[i++];
	//指令序号
	count=src[i++];
	//while(count--){
		orderData.mOrderType=getOrderType(count);
	//数据指令长度
		count=getOrderLen(count);
	/*
	if(count==1){
		orderData.mOrderConnect=src[i++];
	}else if(count==2){
		//低高位
		orderData.mOrderConnect+=src[i++];
		
		orderData.mOrderConnect+=src[i++]<<8;
	}
	*/
	orderData.mOrderConnect=0;
	for(;j<count;j++){
		orderData.mOrderConnect+=src[i++]<<(8*j);
	}
	i++;//结尾
	//SendDataToDevice("13",2);
	return i;
	//}
}



