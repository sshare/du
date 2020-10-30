package com.example.nettest.tools;

import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import java.io.IOException;
import java.util.Map;
import java.util.concurrent.TimeUnit;

import okhttp3.Call;
import okhttp3.FormBody;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class HttpUtils {
    private static volatile HttpUtils instance;
    private static final String TAG = "HttpUtils";
    private Handler okHttpHandler;
    private OkHttpClient mOkHttpClient;

    public OkHttpClient getmOkHttpClient() {
        return mOkHttpClient;
    }

    private HttpUtils() {
        //初始化OkHttpClient
        mOkHttpClient = new OkHttpClient().newBuilder()
                .connectTimeout(10, TimeUnit.SECONDS)//设置超时时间
                .readTimeout(10, TimeUnit.SECONDS)//设置读取超时时间
                .writeTimeout(10, TimeUnit.SECONDS)//设置写入超时时间
//                .addInterceptor(new LoggingInterceptor())
                .build();
        //初始化Handler,为了避免导致非主线程操作，而致使handeler异常，
        // 创建的时候添加参数Looper.getMainLooper()获取主线程的looper
//        okHttpHandler = new Handler(Looper.getMainLooper());
    }

    /**
     * 双重检测锁-单例模式
     *
     * @return
     */
    public static HttpUtils getInstance() {
        if (instance == null) {
            synchronized (HttpUtils.class) {
                if (instance == null) {
                    instance = new HttpUtils();
                }
            }
        }
        return instance;
    }
//
//    /**
//     * @网络请求--post请求
//     */
//    public void post(String url, Map<String, String> map, final CallBack callback) {
////        OkHttpClient client = new OkHttpClient();
//        FormBody.Builder builder = new FormBody.Builder();
//        for (Map.Entry<String, String> entry : map.entrySet()) {
//            builder.add(entry.getKey(), entry.getValue());
//        }
//        FormBody formBody = builder.build();
//        Request.Builder builder1 = new Request.Builder()
//                .url(url)
//                .post(formBody);
//        Request request = builder1.build();
//        Call call = mOkHttpClient.newCall(request);
//        call.enqueue(new okhttp3.Callback() {
//            @Override
//            public void onFailure(Call call, final IOException e) {
//                if (e.getCause() != null && e.getCause().getStackTrace() != null) {
//                    Log.e(TAG, "onFailure:" + e.getCause().getStackTrace() + e.getMessage());
//                } else {
//                    Log.e(TAG, "onFailure:" + e.toString());
//                }
//                okHttpHandler.post(new Runnable() {
//                    @Override
//                    public void run() {
//                        if (callback != null) {
//                            callback.onFailed("", e);
//                        }
//                    }
//                });
//            }
//
//            @Override
//            public void onResponse(Call call, Response response) throws IOException {
//                final String result = response.body().string();
//                okHttpHandler.post(new Runnable() {
//                    @Override
//                    public void run() {
//                        if (callback != null) {
//                            callback.onSuccess("", result);
//                        }
//                    }
//                });
//            }
//        });
//    }
}
