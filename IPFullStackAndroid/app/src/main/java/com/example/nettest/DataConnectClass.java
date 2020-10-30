package com.example.nettest;

import java.io.Serializable;
import java.util.ArrayList;

public class DataConnectClass implements Serializable {
    public String id;//
    public String updatetime;//时间
    public String deviceid;//设备id
    public String val1;//温度
    public String val2;//湿度
    public String val3;//3
    public String val4;//3
    public String val5;//3
    public String val6;//3
    public String val7;//3
    public String val8;//3
    public String val9;//3
    public String va10;//3
    public String val11;//3
    public String val12;//3

    public  static void main(String str[]){
//        int a=7173;
//        int h=5120;
//        int s=16777216;
//        int dd=0x1c05;
//int sd=dd&0xFF;
//        int df=a>>8&0xFF;
//        System.out.println(dd);
//        System.out.println(sd);
//        System.out.println(df);
//        System.out.println(sd+df*10);

        ArrayList<String> arrayList=new ArrayList<>(4);

        arrayList.add(0,"12");
        arrayList.add(1,null);
        arrayList.add(2,"3435");

System.out.println(arrayList.get(1));
    }
}
