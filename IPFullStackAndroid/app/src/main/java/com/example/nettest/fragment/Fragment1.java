package com.example.nettest.fragment;

import android.os.Bundle;
import android.os.Handler;
import android.text.TextUtils;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import androidx.fragment.app.Fragment;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONException;
import com.example.nettest.DataConnectClass;
import com.example.nettest.HomeActivity;
import com.example.nettest.NetReqResult;
import com.example.nettest.R;
import com.example.nettest.tools.HttpUtils;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.FormBody;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;

public class Fragment1 extends Fragment {
    private String TAG;//用于打印log用
    private TextView show_data_view;//用于显示获取到的数据的空间TextView
    private TextView textView;//用于显示温湿度信息
    private Button get_data_btn;//按钮，点击获取数据用的
    EditText cmd_et;//指令编辑框
    //初始化Handler,为了避免导致非主线程操作，而致使handeler异常，
    private Handler handler = new Handler();
    //服务器接口。用于请求数据
    public  final String SERVICE_ADDRESS = "http://jdfcontest.sinoioetech.com/DeviceCtrl/DevCtrl/GetDeviceCurState";
    public DataConnectClass dataConnectClass;
    /**
     * @网络请求-
     */
    public void getParam(String url, Map<String, String> map) {
        show_data_view.setText("数据请求中。。。");
        //普通表单提交,添加参数
        FormBody.Builder builder = new FormBody.Builder();
        for (Map.Entry<String, String> entry : map.entrySet()) {
            builder.add(entry.getKey(), entry.getValue());
        }
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

                        show_data_view.setText("数据请求失败："+ e.toString());
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
                    // {"result":"0","message":"","data":"{\"id\":5,\"deviceid\":\"0102030405060708\",\"val1\":null,...\"val32\":null,\"updatetime\":\"2020-03-07T15:58:57\"}"}
                    //根据结构解析JSON数据，可获得请求返回码result和返回状态描述message，以及具体内容data（JSON结构的字符串）
                    NetReqResult netReqResult = JSON.parseObject(result, NetReqResult.class);
                    //result 0成功；1失败；
                    if ("0".equals(netReqResult.result) && !TextUtils.isEmpty(netReqResult.data)) {
                        //根据data内容结构解析JSON数据到类DataConnectClass
                        dataConnectClass = JSON.parseObject(netReqResult.data, DataConnectClass.class);
                    }
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            //显示请求原始数据到界面
                            show_data_view.setText(result.toString());
                            //如果具体内容不为空，就将获取到的温湿度数据显示到界面
                            if (dataConnectClass != null) {
                                setData();//设置需要显示的温湿度数据
                            }
                        }
                    });
                } catch (JSONException e) {
                    e.printStackTrace();
                    Log.e("解析异常", "解析异常：" + e.toString());
                }
            }
        });
    }

    private void setData() {
        float temp = 0f;
        float hum = 0f;
        if (!TextUtils.isEmpty(dataConnectClass.val1)) {
            temp = Integer.parseInt(dataConnectClass.val1)/100.0f;
        }
        if (!TextUtils.isEmpty(dataConnectClass.val2)) {
            hum= Integer.parseInt(dataConnectClass.val2)/100.0f;
        }
        //显示到界面
        textView.setText("温度：" + temp + "    湿度：" + hum+"\n  时间："+dataConnectClass.updatetime);
    }
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view=inflater.inflate(R.layout.fragment1, container, false);
        //控件初始化，绑定界面的控件
        show_data_view = view.findViewById(R.id.show_data_view);
        textView = view.findViewById(R.id.textView);
        get_data_btn = view.findViewById(R.id.get_data_btn);
        cmd_et = view.findViewById(R.id.cmd_et);
        //监听按钮是否被点击
        get_data_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //按钮点击的方法回调，只要按钮被点击就会执行这里的方法
                if (TextUtils.isEmpty(HomeActivity.deviceID) || TextUtils.isEmpty(HomeActivity.pwd)) {
                    Toast.makeText(getActivity(), "设备ID或密码有误", Toast.LENGTH_SHORT).show();
                    return;
                }
                HashMap<String, String> map = new HashMap();
                map.put("devid", HomeActivity.deviceID);
                map.put("devsecret", HomeActivity.pwd);
                getParam(SERVICE_ADDRESS, map);
            }
        });
        TAG = getClass().getSimpleName();
        return view;
    }
}
