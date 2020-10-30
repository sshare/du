package com.example.nettest.fragment;

import android.app.ProgressDialog;
import android.content.Context;
import android.net.Uri;
import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.os.Handler;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

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

public class Fragment2 extends Fragment implements View.OnClickListener {

    public final String SERVICE_CMD = "http://jdfcontest.sinoioetech.com/DeviceCtrl/DevCtrl/OptDeviceCommandMore";
    private Button start_btn;//初始
    private Button vertical_btn;//垂直
    private Button gesture_btn;//水平
    private Button level_btn;//自由姿态
    public EditTextChangedListener sg_1_listener, sg_2_listener, sg_3_listener, sg_4_listener, sg_5_listener, sg_6_listener;
    //1-6个关节参数
    private EditText sg_1_et, sg_2_et, sg_3_et, sg_4_et, sg_5_et, sg_6_et;
    DeviceClass deviceClass;
    ArrayList<OrderClass> angleOrderList = new ArrayList<>();//多参数的角度指令，
    ArrayList<OrderClass> orderList = new ArrayList<>();//单一的数字指令
    ProgressDialog waitingDialog;
    //初始化Handler,为了避免导致非主线程操作，而致使handeler异常，
    private Handler handler = new Handler();
   String TAG;
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment2, container, false);
        TAG=getClass().getSimpleName();
        start_btn = view.findViewById(R.id.start_btn);
        vertical_btn = view.findViewById(R.id.vertical_btn);
        level_btn = view.findViewById(R.id.level_btn);
        gesture_btn = view.findViewById(R.id.gesture_btn);

        start_btn.setOnClickListener(this);
        vertical_btn.setOnClickListener(this);
        level_btn.setOnClickListener(this);
        gesture_btn.setOnClickListener(this);

        sg_1_et = view.findViewById(R.id.sg_1_et);
        sg_2_et = view.findViewById(R.id.sg_2_et);
        sg_3_et = view.findViewById(R.id.sg_3_et);
        sg_4_et = view.findViewById(R.id.sg_4_et);
        sg_5_et = view.findViewById(R.id.sg_5_et);
        sg_6_et = view.findViewById(R.id.sg_6_et);

        sg_1_listener = new EditTextChangedListener(sg_1_et.getId());
        sg_2_listener = new EditTextChangedListener(sg_2_et.getId());
        sg_3_listener = new EditTextChangedListener(sg_3_et.getId());
        sg_4_listener = new EditTextChangedListener(sg_4_et.getId());
        sg_5_listener = new EditTextChangedListener(sg_5_et.getId());
        sg_6_listener = new EditTextChangedListener(sg_6_et.getId());

        sg_1_et.addTextChangedListener(sg_1_listener);
        sg_2_et.addTextChangedListener(sg_2_listener);
        sg_3_et.addTextChangedListener(sg_3_listener);
        sg_4_et.addTextChangedListener(sg_4_listener);
        sg_5_et.addTextChangedListener(sg_5_listener);
        sg_6_et.addTextChangedListener(sg_6_listener);
        deviceClass = new DeviceClass();
        deviceClass.cmdlist = new ArrayList<>();
        //添加参数
        for (int i = 0; i < 6; i++) {
            angleOrderList.add(i,null);
        }
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
            case R.id.start_btn://初始
                deviceClass.cmdlist.clear();
                OrderClass orderClass = new OrderClass();
                orderClass.cmdname = "val1";
                orderClass.cmdvalue = "0";
                deviceClass.cmdlist.add(0, orderClass);
                HashMap<String, String> map=new HashMap<>();
                map.put("str",JSON.toJSONString(deviceClass));
                sendCmd(SERVICE_CMD,map);
                break;
            case R.id.vertical_btn://垂直
                deviceClass.cmdlist.clear();
                orderClass = new OrderClass();
                orderClass.cmdname = "val1";
                orderClass.cmdvalue = "1";
                deviceClass.cmdlist.add(0, orderClass);

                map=new HashMap<>();
                map.put("str",JSON.toJSONString(deviceClass));
                sendCmd(SERVICE_CMD,map);
                break;
            case R.id.level_btn://水平
                deviceClass.cmdlist.clear();
                orderClass = new OrderClass();
                orderClass.cmdname = "val1";
                orderClass.cmdvalue = "2";
                deviceClass.cmdlist.add(0, orderClass);

                map=new HashMap<>();
                map.put("str",JSON.toJSONString(deviceClass));
                sendCmd(SERVICE_CMD,map);
                break;
            case R.id.gesture_btn://自由
                deviceClass.cmdlist.clear();
                int i=0;
                Log.e(TAG, i+"============="+angleOrderList.size());
                for (OrderClass order : angleOrderList) {
                    if (order == null) {
                        Log.e(TAG, i+"=============");
                        Toast.makeText(getActivity(), "6个舵机角度不能为空", Toast.LENGTH_SHORT).show();
                        return;
                    }
                    i++;
                }
                deviceClass.cmdlist.addAll(angleOrderList);

                map=new HashMap<>();
                map.put("str",JSON.toJSONString(deviceClass));
                sendCmd(SERVICE_CMD,map);
                break;
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }

    class EditTextChangedListener implements TextWatcher {
        int editID;
        public EditTextChangedListener(int editID) {
            this.editID = editID;
        }
        @Override
        public void beforeTextChanged(CharSequence s, int start, int count, int after) { }
        @Override
        public void onTextChanged(CharSequence s, int start, int before, int count) { }
        @Override
        public void afterTextChanged(Editable s) {
            //根据编辑框内容变化动态修改指令内容，方便拼接请求指令需求的参数内容
            Log.e(TAG,s.toString());
            switch (editID) {
                case R.id.sg_1_et://关节1
                    angleOrderList.remove(0);
                    if (TextUtils.isEmpty(s)) {
                        angleOrderList.add(0, null);
                        return;
                    }
                    OrderClass orderClass = new OrderClass();
                    orderClass.cmdname = "val1";
                    orderClass.cmdvalue = getData(0,Integer.parseInt(s.toString()),10,0)+"";
                    angleOrderList.add(0, orderClass);
                    break;
                case R.id.sg_2_et://关节2
                    angleOrderList.remove(1);
                    if (TextUtils.isEmpty(s)) {
                        angleOrderList.add(1, null);
                        return;
                    }
                    orderClass = new OrderClass();
                    orderClass.cmdname = "val2";
                    orderClass.cmdvalue = getData(1,Integer.parseInt(s.toString()),10,0)+"";
                    angleOrderList.add(1, orderClass);
                    break;
                case R.id.sg_3_et://关节3
                    angleOrderList.remove(2);
                    if (TextUtils.isEmpty(s)) {
                        angleOrderList.add(2, null);
                        return;
                    }
                    orderClass = new OrderClass();
                    orderClass.cmdname = "val3";
                    orderClass.cmdvalue = getData(2,Integer.parseInt(s.toString()),10,0)+"";
                    angleOrderList.add(2, orderClass);
                    break;
                case R.id.sg_4_et://关节4
                    angleOrderList.remove(3);
                    if (TextUtils.isEmpty(s)) {
                        angleOrderList.add(3, null);
                        return;
                    }
                    orderClass = new OrderClass();
                    orderClass.cmdname = "val4";
                    orderClass.cmdvalue = getData(3,Integer.parseInt(s.toString()),10,0)+"";
                    angleOrderList.add(3, orderClass);
                    break;
                case R.id.sg_5_et://关节5
                    angleOrderList.remove(4);
                    if (TextUtils.isEmpty(s)) {
                        deviceClass.cmdlist.add(4, null);
                        return;
                    }
                    orderClass = new OrderClass();
                    orderClass.cmdname = "val5";
                    orderClass.cmdvalue = getData(4,Integer.parseInt(s.toString()),10,0)+"";
                    angleOrderList.add(4, orderClass);
                    break;
                case R.id.sg_6_et://关节6
                    angleOrderList.remove(5);
                    if (TextUtils.isEmpty(s)) {
                        angleOrderList.add(5, null);
                        return;
                    }
                    orderClass = new OrderClass();
                    orderClass.cmdname = "val6";
                    orderClass.cmdvalue = getData(5,Integer.parseInt(s.toString()),10,0)+"";
                    angleOrderList.add(5, orderClass);
                    break;
            }
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
        waitingDialog =
                new ProgressDialog(getActivity());
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
    public int getData(int id, int angle, int seep, int isWait) {
        int count = ((0x10 + id) << (8 * 3)) + (angle << (8 * 2)) + (seep << 8) + isWait;
        return count;
    }
}
