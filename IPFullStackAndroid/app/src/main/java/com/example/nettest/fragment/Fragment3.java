package com.example.nettest.fragment;

import android.app.ProgressDialog;
import android.os.Bundle;
import android.os.Handler;
import android.text.TextUtils;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

import androidx.fragment.app.Fragment;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONException;
import com.example.nettest.HomeActivity;
import com.example.nettest.NetReqResult;
import com.example.nettest.R;
import com.example.nettest.tools.DeviceClass;
import com.example.nettest.tools.HttpUtils;
import com.example.nettest.tools.OrderClass;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.FormBody;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;

public class Fragment3 extends Fragment implements View.OnClickListener {

    public final String SERVICE_CMD = "http://jdfcontest.sinoioetech.com/DeviceCtrl/DevCtrl/OptDeviceCommandMore";

    //初始化Handler,为了避免导致非主线程操作，而致使handeler异常，
    private Handler handler = new Handler();
    DeviceClass deviceClass;
    ProgressDialog waitingDialog;
    String TAG;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        View view = inflater.inflate(R.layout.fragment3, container, false);
        view.findViewById(R.id.button2).setOnClickListener(this);
        TAG = getClass().getSimpleName();
        deviceClass = new DeviceClass();
        deviceClass.cmdlist = new ArrayList<>();
        return view;
    }

    @Override
    public void onClick(View v) {
        if (TextUtils.isEmpty(HomeActivity.deviceID) || TextUtils.isEmpty(HomeActivity.pwd)) {
            Toast.makeText(getActivity(), "设备ID或密码有误", Toast.LENGTH_SHORT).show();
            return;
        }
        deviceClass.devid = HomeActivity.deviceID;
        deviceClass.devsecret = HomeActivity.pwd;
        switch (v.getId()) {
            case R.id.button2://搬运
                deviceClass.cmdlist.clear();
                OrderClass orderClass = new OrderClass();
                orderClass.cmdname = "val1";
                orderClass.cmdvalue = "3";
                deviceClass.cmdlist.add(0, orderClass);
                HashMap<String, String> map = new HashMap<>();
                map.put("str", JSON.toJSONString(deviceClass));
                sendCmd(SERVICE_CMD, map);
                break;
        }
    }

    /**
     * @网络请求-
     */
    public void sendCmd(String url, Map<String, String> map) {
        showWaitingDialog();
        //普通表单提交,添加参数
        FormBody.Builder builder = new FormBody.Builder();
        for (Map.Entry<String, String> entry : map.entrySet()) {
            builder.add(entry.getKey(), entry.getValue());
        }
        Log.e(TAG, "参数:" + map.toString());
        RequestBody formBody = builder.build();
        //构建请求对象
        Request request = new Request.Builder()
                .url(url)
                .post(formBody)
                .build();
        //创建call
        Call call = HttpUtils.getInstance().getmOkHttpClient().newCall(request);
        //执行请求，Callback()为请求状态回调（成功，失败）
        call.enqueue(new Callback() {
            @Override //网络请求失败回调
            public void onFailure(Call call, final IOException e) {
                //请求失败，打印异常
                Log.e(TAG, "onFailure:" + e.toString());
                handler.post(new Runnable() {
                    @Override
                    public void run() {

                        Toast.makeText(getActivity(), "数据请求失败：" + e.toString(), Toast.LENGTH_SHORT).show();
                        dismissDialog();
                    }
                });
            }

            @Override//网络请求成功回调
            public void onResponse(Call call, Response response) throws IOException {
                try {
                    //请求成功，获取数据
                    final String result = response.body().string();
                    Log.e("返回结果：", result);
                    //数据结构
                    // {"result":"0","message":"""}
                    //根据结构解析JSON数据，可获得请求返回码result和返回状态描述message，以及具体内容data（JSON结构的字符串）
                    NetReqResult netReqResult = JSON.parseObject(result, NetReqResult.class);
                    //result 0成功；1失败；
                    if ("0".equals(netReqResult.result)) {
                        handler.post(new Runnable() {
                            @Override
                            public void run() {
                                //显示请求原始数据到界面

                                Toast.makeText(getActivity(), "指令发送成功", Toast.LENGTH_SHORT).show();
                                dismissDialog();
                            }
                        });

                    }
                } catch (JSONException e) {
                    e.printStackTrace();
                    Log.e("解析异常", "解析异常："
                            + e.toString());
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            //显示请求原始数据到界面

                            Toast.makeText(getActivity(), "数据异常", Toast.LENGTH_SHORT).show();
                            dismissDialog();
                        }
                    });
                }
            }
        });
    }

    private void showWaitingDialog() {
        /* 等待Dialog具有屏蔽其他控件的交互能力
         * @setCancelable 为使屏幕不可点击，设置为不可取消(false)
         * 下载等事件完成后，主动调用函数关闭该Dialog
         */
        waitingDialog =new ProgressDialog(getActivity());
        waitingDialog.setTitle("我是一个等待Dialog");
        waitingDialog.setMessage("数据请求中。。。");
        waitingDialog.setIndeterminate(true);
        waitingDialog.setCancelable(false);
        waitingDialog.show();
        waitingDialog.dismiss();
    }

    private void dismissDialog() {
        if (waitingDialog != null) {
            waitingDialog.dismiss();
            waitingDialog = null;
        }
    }
}
