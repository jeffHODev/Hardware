package com.hekangyuan.nivi1000.activity;

import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.provider.DocumentsContract;
import android.provider.Settings;
import android.support.annotation.Nullable;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.hekangyuan.nivi1000.GpioNative;
import com.hekangyuan.nivi1000.MyApplication;
import com.hekangyuan.nivi1000.R;
import com.hekangyuan.nivi1000.activity.api.LoginView;
import com.hekangyuan.nivi1000.activity.presenter.LoginPresenter;
import com.hekangyuan.nivi1000.base.BaseActivity;
import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.float_view.FloatingWindow;
import com.hekangyuan.nivi1000.model.LoginBean;
import com.hekangyuan.nivi1000.utils.Constants;
import com.hekangyuan.nivi1000.utils.SharePreferenceTools;
import com.hekangyuan.nivi1000.utils.ThreadPoolWrapper;
import com.hekangyuan.nivi1000.utils.ToastUtils;
import com.hekangyuan.nivi1000.utils.Utils;
import java.util.HashMap;
import java.util.Map;


public class SettingActivity extends BaseActivity<LoginPresenter> implements LoginView, View.OnClickListener {


    private RelativeLayout modifyPassLayout,wifi,print,choosePass;

    private RadioButton modeOne;
    private RadioButton modeTwo;

    private EditText hospitalNameEdit;
    private Button submit;

    private SharePreferenceTools spTools;

    private boolean istrue = true;
    int level1 = 0;
    int lastLevel1 = -1;
    int level2 = 0;
    int lastLevel2 = -1;
    private Runnable parseRunnableGpio1;
    private Runnable parseRunnableGpio2;


    @Override
    protected LoginPresenter createPresenter() {
        return new LoginPresenter(this);
    }

    @Override
    protected int getLayoutId() {
        return R.layout.activity_settings;
    }


    @Override
    public void onErrorCode(BaseJsonMsg model) {
        //super.onErrorCode(model);
        //我要自定义
        //实现自己的逻辑
    }

    @Override
    public void showError(String msg) {
        String aa = msg;
        if (msg.contains("重新登录")) {
            //处理自己定制逻辑
        } else {
            super.showError(msg);
        }

    }


    @Override
    protected void initData() {
//        TextView tv_title = findViewById(R.id.tv_title);
//        tv_title.setText("设置");
//
//        findViews();
//        setClickListener();
//
//        initModeAndName();
    }

    @Override
    protected void onResume() {
        super.onResume();

        istrue = false;
        GpioNative.closeGpioDev();

        TextView tv_title = findViewById(R.id.tv_title);
        tv_title.setText("设置");

        findViews();
        setClickListener();

        initModeAndName();

        back = (TextView) findViewById(R.id.title_left);
        if (back == null) {
            return;
        }
        back.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View arg0) {
                istrue = false;
                GpioNative.closeGpioDev();
                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                ThreadPoolWrapper.shutdown();

                finish();
                Utils.exitActivityAnimation(SettingActivity.this);
            }
        });

        istrue = true;
        level1 = 0;
        lastLevel1 = -1;
        level2 = 0;
        lastLevel2 = -1;
        initGpioThread();
    }

    private void initGpioThread() {
        GpioNative.openGpioDev();
        parseRunnableGpio1 = new Runnable() {
            @Override
            public void run() {
                while (istrue) {
                    if(MyApplication.refCount == 0){
                        istrue = false;
                        GpioNative.closeGpioDev();
                        ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                        ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                        ThreadPoolWrapper.shutdown();
                    }

                    try {
                        Thread.currentThread().sleep(100);
                    }catch (Exception e){
                        e.printStackTrace();
                    }
                    GpioNative.setGpioMode(2,0);
                    level1 = GpioNative.getGpioLevel(2);

                    if (lastLevel1 == level1) continue;
                    lastLevel1 = level1;

                    if(level1 == 1){
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                istrue = false;
                                GpioNative.closeGpioDev();
                                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                                ThreadPoolWrapper.shutdown();

                                finish();
                                Utils.exitActivityAnimation(SettingActivity.this);
                            }
                        });
                    }

                }
            }
        };
        ThreadPoolWrapper.getThreadPool().executeTask(parseRunnableGpio1);

        parseRunnableGpio2 = new Runnable() {
            @Override
            public void run() {
                while (istrue) {
                    try {
                        Thread.currentThread().sleep(100);
                    }catch (Exception e){
                        e.printStackTrace();
                    }
                    GpioNative.setGpioMode(1,0);
                    level2 = GpioNative.getGpioLevel(1);

                    if (lastLevel2 == level2) continue;
                    lastLevel2 = level2;

                    if(level2 == 1){
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                submit();
                            }
                        });
                    }

                }
            }
        };
        ThreadPoolWrapper.getThreadPool().executeTask(parseRunnableGpio2);
    }

    /**
     * 从sp中初始化
     */
    private void initModeAndName() {
        if(spTools == null){
            spTools = new SharePreferenceTools(this);
        }

        Map nameMap = spTools.readSharedPreference(Constants.SP_HOSPITAL_NAME_MAP);
        if(nameMap != null && nameMap.size() > 0){
            String name = (String) nameMap.get(Constants.SP_HOSPITAL_NAME);
            hospitalNameEdit.setText(name);
        }


        Map modeMap = spTools.readSharedPreference(Constants.SP_MODE_MAP);
        if(modeMap != null && modeMap.size() > 0){
            int mode = (int) modeMap.get(Constants.SP_MODE);
            if(mode == Constants.MEASURE_MODE_SYNC){
                modeOne.setChecked(true);
            }else{
                modeTwo.setChecked(true);
            }
        }

    }

    private void findViews() {
        modifyPassLayout = findViewById(R.id.layout_modify_pass);
        modeOne = findViewById(R.id.mode_one);
        modeTwo = findViewById(R.id.mode_two);

        hospitalNameEdit = findViewById(R.id.hospital_name_edit);
        submit = findViewById(R.id.submit);
        wifi = findViewById(R.id.wifi);
        print = findViewById(R.id.print);
        choosePass = findViewById(R.id.choosePass);
    }

    private void setClickListener() {
        modifyPassLayout.setOnClickListener(this);

        modeOne.setOnClickListener(this);
        modeTwo.setOnClickListener(this);
        submit.setOnClickListener(this);
        wifi.setOnClickListener(this);
        print.setOnClickListener(this);
        choosePass.setOnClickListener(this);
    }



    @Override
    public void onMainSuccess(BaseJsonMsg<LoginBean> o) {


    }


    @Override
    public void onClick(View v) {
        switch (v.getId()) {

            case R.id.layout_modify_pass:
                modifyPass();
                break;

            case R.id.mode_one:
                saveModeToSp(Constants.MEASURE_MODE_SYNC);
                break;

            case R.id.mode_two:
                saveModeToSp(Constants.MEASURE_MODE_ASYNC);
                break;
            case R.id.submit:
                submit();
                break;
            case R.id.wifi:
//                showFloatWindow("" + "", "outside_order");
                wifi();
                break;
            case R.id.print:
                isAppInstalled(SettingActivity.this.mContext, "org.mopria.printplugin");
                break;
            case R.id.choosePass:
                // Choose a directory using the system's file picker.
//                Intent intent = new Intent();
//                intent.setAction(Intent.ACTION_CREATE_DOCUMENT );
//                // Provide read access to files and sub-directories in the user-selected
//                // directory.
////                intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
////                intent.putExtra(DocumentsContract.EXTRA_INITIAL_URI, uriToLoad);
//
//                startActivity(intent);

//                Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT_TREE);
//                //intent.setType(“image/*”);//选择图片
//                //intent.setType(“audio/*”); //选择音频
//                //intent.setType(“video/*”); //选择视频 （mp4 3gp 是android支持的视频格式）
//                //intent.setType(“video/*;image/*”);//同时选择视频和图片
////                Uri uri = Uri.parse("content://com.android.externalstorage.documents/document/primary:Download");
////                Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
////                intent.addCategory(Intent.CATEGORY_OPENABLE);
//                intent.setType("*/*");
////                intent.putExtra(DocumentsContract.EXTRA_INITIAL_URI, uri);
//                startActivity(intent);

//                Intent intent = new Intent(SettingActivity.this, UsbActivity.class);
//                startActivity(intent);
                break;
        }
    }
    /*
     * check the app is installed
     */
    private boolean isAppInstalled(Context context, String packagename)
    {
        PackageInfo packageInfo;
        try {
            packageInfo = context.getPackageManager().getPackageInfo(packagename, 0);
        }catch (PackageManager.NameNotFoundException e) {
            packageInfo = null;
            e.printStackTrace();
        }
        if(packageInfo ==null){
            //System.out.println("没有安装");
            showToast("请安装打印机应用");
            return false;
        }else{
            //System.out.println("已经安装");
            print();
            return true;
        }
    }
    // 调用打印
    private void print() {
        if (!requestOverlayPermission()) {
            showFloatingWindow();
            PackageManager packageManager = mContext.getPackageManager();
            Intent intent = packageManager.getLaunchIntentForPackage("org.mopria.printplugin");
            this.startActivityForResult(intent, 1001);
        } else {
            Toast.makeText(this, "请开启悬浮窗权限", Toast.LENGTH_SHORT).show();
        }


//        startActivity(getPackageManager().getLaunchIntentForPackage("org.mopria.printplugin"));
    }

    @Override
    protected void onRestart() {
        super.onRestart();
        if (mFloatingWindow != null){
            mFloatingWindow.dismiss();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mFloatingWindow != null){
            mFloatingWindow.dismiss();
        }
    }

    // 调用系统wifi
    private void wifi() {
        Intent intentWifi = new Intent(Settings.ACTION_WIFI_SETTINGS);//WIFI网络
        intentWifi.putExtra("extra_prefs_show_button_bar", true);
        intentWifi.putExtra("extra_prefs_set_back_text","返回");
        intentWifi.putExtra("extra_prefs_set_next_text","");
        startActivity(intentWifi);
    }

    /**
     * 保存机构名称
     */
    private void submit() {
        String name = hospitalNameEdit.getText().toString();
        isSaveNameSuccess = saveName(name);
        if(isSaveNameSuccess) {
            ToastUtils.showToast(SettingActivity.this, "设置成功");
        }

    }

    boolean isSaveNameSuccess = false;
    /**
     * 设置测量模式
     * @param mode
     */
    private void saveModeToSp(int mode) {
        if(spTools == null){
            spTools = new SharePreferenceTools(this);
        }

        Map<String,Object> modeMap = new HashMap();
        modeMap.put(Constants.SP_MODE,mode);
        spTools.writeSharedPreference(Constants.SP_MODE_MAP,modeMap);
    }

    /**
     * 设置机构名称
     */
    private boolean saveName(String name) {
        Map<String,Object> nameMap = new HashMap();
        nameMap.put(Constants.SP_HOSPITAL_NAME,name);
        return spTools.writeSharedPreference(Constants.SP_HOSPITAL_NAME_MAP,nameMap);
    }


    /**
     * 修改密码
     */
    private void modifyPass() {
        Intent intent = new Intent(SettingActivity.this, ModifyPasswordActivity.class);
        startActivity(intent);
    }


    private void showFloatingWindow() {
        mFloatingWindow = new FloatingWindow();
        View view = initFloatView();
        mFloatingWindow.showFloatingWindowView(this, view);
    }

    private View initFloatView() {
        View view = View.inflate(this, R.layout.view_floating_window, null);
        // 设置视频封面
//        final ImageView mThumb = (ImageView) view.findViewById(R.id.thumb_floating_view);
        view.findViewById(R.id.returnMainApp).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mFloatingWindow.setTopApp(SettingActivity.this, "aaa");
                mFloatingWindow.dismiss();
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

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == REQUEST_OVERLAY_CODE) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                if (Settings.canDrawOverlays(SettingActivity.this)) {
                    showFloatingWindow();
                } else {
                    Toast.makeText(this, "获取悬浮窗权限失败", Toast.LENGTH_SHORT).show();
                }
            }
        }
        if (requestCode == 1001){
//            showToast("1001");
//            if (mFloatingWindow != null){
//                mFloatingWindow.dismiss();
//            }
        }
    }

    private static final int REQUEST_OVERLAY_CODE = 101;
    private FloatingWindow mFloatingWindow;

    /**
     * 判断是否开启悬浮窗口权限，否则，跳转开启页
     */
    public boolean requestOverlayPermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (!Settings.canDrawOverlays(this)) {
                Intent intent = new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION, Uri.parse("package:" + getPackageName()));
                intent.putExtra("extra_prefs_show_button_bar", true);
                intent.putExtra("extra_prefs_set_back_text","返回");
                intent.putExtra("extra_prefs_set_next_text","");
                startActivityForResult(intent, REQUEST_OVERLAY_CODE);
                return true;
            } else {
                return false;
            }
        }
        return false;
    }
}
