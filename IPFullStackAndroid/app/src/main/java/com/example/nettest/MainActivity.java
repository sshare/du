package com.example.nettest;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Handler;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONException;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.TimeUnit;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.FormBody;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;

public class MainActivity extends AppCompatActivity {
    private String TAG;//用于打印log用
    private TextView show_data_view;//用于显示获取到的数据的空间TextView
    private TextView textView;//用于显示温湿度信息
    private Button get_data_btn;//按钮，点击获取数据用的
    private Button send_cmd_btn;//按钮，点击发送指令用的
    private OkHttpClient mOkHttpClient;
    //初始化Handler,为了避免导致非主线程操作，而致使handeler异常，
    private Handler handler = new Handler();
    //服务器接口。用于请求数据 http://itemairdino.sinoioetech.com/DeviceCtrl/DevCtrl/OptDeviceCommandMore
//    public static final String SERVICE_ADDRESS = "http://trashwebser.sinoioetech.com/TopicManage/Topic/GetDeviceCurState";
    public static final String SERVICE_ADDRESS = "http://itemairdino.sinoioetech.com/DeviceCtrl/DevCtrl/OptDeviceCommandMore";
    public DataConnectClass dataConnectClass;
    private String deviceID="DC201001661446F4";
    private String devicePwd="123456";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //控件初始化，绑定界面的控件
        show_data_view = findViewById(R.id.show_data_view);
        textView = findViewById(R.id.textView);
        get_data_btn = findViewById(R.id.get_data_btn);
        //监听按钮是否被点击
        get_data_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //按钮点击的方法回调，只要按钮被点击就会执行这里的方法
                if (deviceID.length() != 16) {
                    Toast.makeText(getApplicationContext(), "设备ID格式错误", Toast.LENGTH_SHORT).show();
                    return;
                }
                if (devicePwd.length() != 6) {
                    Toast.makeText(getApplicationContext(), "密码格式错误", Toast.LENGTH_SHORT).show();
                    return;
                }
                HashMap<String, String> map = new HashMap();
                map.put("devid", deviceID);
                map.put("devsecret", devicePwd);
                getParam(SERVICE_ADDRESS, map);
            }
        });

        TAG = getClass().getSimpleName();
        //初始化OkHttpClient，我们用到的开源框架，用于网络请求
        mOkHttpClient = new OkHttpClient().newBuilder()
                .connectTimeout(10, TimeUnit.SECONDS)//设置超时时间
                .readTimeout(10, TimeUnit.SECONDS)//设置读取超时时间
                .writeTimeout(10, TimeUnit.SECONDS)//设置写入超时时间
                .build();
    }

    public int getData(int id, int angle, int seep, int isWait) {
        int count = ((0x10 + id) << (8 * 3)) + (angle << (8 * 2)) + (seep << 8) + isWait;
        return count;
    }
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
        Call call = mOkHttpClient.newCall(request);
        //执行请求，Callback()为请求状态回调（成功，失败）
        call.enqueue(new Callback() {
            @Override //网络请求失败回调
            public void onFailure(Call call, final IOException e) {
                //请求失败，打印异常
                Log.e(TAG, "onFailure:" + e.toString());

                show_data_view.setText("数据请求失败："+ e.toString());
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
            int t = Integer.parseInt(dataConnectClass.val1);
            float temp_z = (float) (t >> 8 & 0xFF);//温度整数部分
            float temp_x = (float) (t & 0xFF) / 100.0f;//温度小数部分
            temp = temp_z + temp_x;
        }
        if (!TextUtils.isEmpty(dataConnectClass.val2)) {
            int h = Integer.parseInt(dataConnectClass.val2);
            float hum_z = (float) (h >> 8 & 0xFF);//湿度整数部分
            float hum_x = (float) (h & 0xFF) / 100.0f;//湿度整数部分
            hum = hum_z + hum_x;
        }
        //显示到界面
        textView.setText("温度：" + temp + "    湿度：" + hum);
    }
}
