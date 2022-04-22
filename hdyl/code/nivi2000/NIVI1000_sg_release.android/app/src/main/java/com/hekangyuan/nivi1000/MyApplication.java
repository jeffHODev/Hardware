package com.hekangyuan.nivi1000;

import android.app.Application;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;

import com.blankj.utilcode.util.Utils;
import com.hekangyuan.nivi1000.activity.UsbActivity;
import com.hekangyuan.nivi1000.float_view.FloatingWindow;
import com.hekangyuan.nivi1000.model.BloodPressureBean;
import com.hekangyuan.nivi1000.model.EcgMonitorBean;
import com.hekangyuan.nivi1000.model.PatientInformationBean;
import com.hekangyuan.nivi1000.serialport_api.SerialParseUtil;
import com.hekangyuan.nivi1000.serialport_api.SerialPort;
import com.hekangyuan.nivi1000.serialport_api.SerialPortFinder;
import com.hekangyuan.nivi1000.utils.ActivityLifecycleListener;
import com.hjq.toast.ToastUtils;
import com.nivi.nivifilter.Hello;
import com.nivi.nivifilter.Max;
import com.orhanobut.logger.AndroidLogAdapter;
import com.orhanobut.logger.FormatStrategy;
import com.orhanobut.logger.Logger;
import com.orhanobut.logger.PrettyFormatStrategy;
import com.reader.usbdevice.DeviceLib;
import com.reader.usbdevice.DeviceStatusCallback;

import java.io.File;
import java.io.IOException;
import java.security.InvalidParameterException;

import android_serialport_api.SerialPortUtil;
import me.jessyan.autosize.AutoSize;
import me.jessyan.autosize.AutoSizeConfig;

public class MyApplication extends Application {
    private static Context mContext;
    public SerialPortFinder mSerialPortFinder = new SerialPortFinder();
    private SerialPort mSerialPort = null;
    public static DeviceLib mdev;
    public static String pkName = null;

    public static EcgMonitorBean ecgMonitorBean;
    public static BloodPressureBean bloodPressureBean;
    public static BloodPressureBean bloodPressureBeanNew;
    public static PatientInformationBean patientInformationBean;
    public static int refCount = 0;
    @Override
    public void onCreate() {
        super.onCreate();
        // 在 Application 中初始化
        ToastUtils.init(this);
        mContext = this;
        myApplication = this;
        initLogger();
        Utils.init(MyApplication.this);

        AutoSizeConfig.getInstance().setCustomFragment(true);
        AutoSize.initCompatMultiProcess(this);
        pkName = this.getPackageName();

        mdev = new DeviceLib(getApplicationContext(), new DeviceStatusCallback() {
            @Override
            public void UsbAttach() {}
            @Override
            public void UsbDeAttach() {}
        });
        mdev.openDevice(101);

        registerActivityLifecycleCallbacks(new ActivityLifecycleListener());
    }

    public static Context getContext() {
        return mContext;
    }



    public SerialPort getSerialPort() throws SecurityException, IOException, InvalidParameterException {
        if (mSerialPort == null) {
           //本地储存设备
            SharedPreferences sp = getSharedPreferences("android_serialport_api.sample_preferences", MODE_PRIVATE);
            String path = sp.getString("DEVICE", "");
            int baudrate = Integer.decode(sp.getString("BAUDRATE", "-1"));

            //检查权限
            if ( (path.length() == 0) || (baudrate == -1)) {
                throw new InvalidParameterException();
            }

            //打开串口
            mSerialPort = new SerialPort(new File(path), baudrate, 0);
        }
        return mSerialPort;
    }
    public void closeSerialPort() {
        if (mSerialPort != null) {
            mSerialPort.close();
            mSerialPort = null;
        }
    }
    private void initLogger() {
        FormatStrategy formatStrategy = PrettyFormatStrategy.newBuilder()
                .showThreadInfo(false)  // 是否显示线程信息 默认显示 上图Thread Infrom的位置
                .methodCount(0)         // 展示方法的行数 默认是2  上图Method的行数
                .methodOffset(7)        // 内部方法调用向上偏移的行数 默认是0
//                .logStrategy(customLog) // 改变log打印的策略一种是写本地，一种是logcat显示 默认是后者（当然也可以自己定义）
                .tag("My custom tag")   // 自定义全局tag 默认：PRETTY_LOGGER
                .build();
        Logger.addLogAdapter(new AndroidLogAdapter(formatStrategy) {
            @Override
            public boolean isLoggable(int priority, String tag) {
                return true;
            }
        });
    }
    public static MyApplication myApplication;

    public static MyApplication getMyApplication() {
        return myApplication;
    }

    public FloatingWindow mFloatingWindow;

    public void showFloatingWindow() {
        mFloatingWindow = new FloatingWindow();
        View view = initFloatView();
        mFloatingWindow.showFloatingWindowView(this, view);
    }

    public View initFloatView() {
        View view = View.inflate(this, R.layout.view_floating_window, null);
        // 设置视频封面
//        final ImageView mThumb = (ImageView) view.findViewById(R.id.thumb_floating_view);
        view.findViewById(R.id.returnMainApp).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mFloatingWindow.dismiss();
                mFloatingWindow.setTopApp(MyApplication.getContext(),"ddd");
            }
        });
//        // 悬浮窗关闭
//        view.findViewById(R.id.close_floating_view).setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//                mFloatingWindow.dismiss();
//            }
//        });
//   //       返回前台页面
//        view.findViewById(R.id.back_floating_view).setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//                mFloatingWindow.setTopApp(SettingActivity.this);
//                mFloatingWindow.dismiss();
//            }
//        });
//        final VideoView videoView = view.findViewById(R.id.video_view);
//        //视频内容设置
//        videoView.setVideoPath("https://stream7.iqilu.com/10339/article/202002/18/2fca1c77730e54c7b500573c2437003f.mp4");
//        // 视频准备完毕，隐藏正在加载封面，显示视频
//        videoView.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
//            @Override
//            public void onPrepared(MediaPlayer mp) {
//                mThumb.setVisibility(View.GONE);
//            }
//        });
//        // 循环播放
//        videoView.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
//            @Override
//            public void onCompletion(MediaPlayer mp) {
//                videoView.start();
//            }
//        });
//        // 开始播放视频
//        videoView.start();
        return view;
    }
}
