package com.example.nettest;

import java.io.Serializable;

public class NetReqResult implements Serializable {
    public String result;//网络请求结果标识码
    public String message;//对请求结果的描述
    public String data;//网络请求返回的具体内容

}
