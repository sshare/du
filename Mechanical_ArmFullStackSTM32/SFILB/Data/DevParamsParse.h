

#ifndef DEVPARAMSPARSE_H_
#define DEVPARAMSPARSE_H_

#include "stm32f4xx.h" 



typedef struct _BDData {
	unsigned char jd; //1=>E, 2=>W
	unsigned char jdDu;
	unsigned char jdFen;
	unsigned int jdMiao;

	unsigned char wd; //1=>s, 2=>n
	unsigned char wdDu;
	unsigned char wdFen;
	unsigned int wdMiao;

	unsigned int Altitude;
}BDData;
/*
typedef struct _DataFrame{

	
	unsigned char cmdDataLen;
	unsigned char *cmdData;	  
}DevDataFrame;
*/
//Êý¾Ý½âÎö
void ParseDeviceData(char * src, char len);
void MC20ParseGPSData(unsigned char * src, int len);
//unsigned char PackageBDInfoToBuf(unsigned char *buf);
	int ParseServerData(unsigned char * src, int len);
#endif


