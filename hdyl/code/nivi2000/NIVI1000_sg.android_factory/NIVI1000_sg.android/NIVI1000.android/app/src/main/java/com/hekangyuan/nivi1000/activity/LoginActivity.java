package com.hekangyuan.nivi1000.activity;

import android.content.Intent;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.text.Selection;
import android.text.Spannable;
import android.text.method.HideReturnsTransformationMethod;
import android.text.method.PasswordTransformationMethod;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.PopupWindow;
import android.widget.TextView;

import com.hekangyuan.nivi1000.GpioNative;
import com.hekangyuan.nivi1000.MyApplication;
import com.hekangyuan.nivi1000.R;
import com.hekangyuan.nivi1000.activity.api.LoginView;
import com.hekangyuan.nivi1000.activity.presenter.LoginPresenter;
import com.hekangyuan.nivi1000.base.BaseActivity;
import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.model.LoginBean;
import com.hekangyuan.nivi1000.model.dbbean.PatientdbBean;
import com.hekangyuan.nivi1000.serialport_api.SerialParseUtil;
import com.hekangyuan.nivi1000.utils.CheckIdCard;
import com.hekangyuan.nivi1000.utils.Constants;
import com.hekangyuan.nivi1000.utils.DeviceUtil;
import com.hekangyuan.nivi1000.utils.NetWorkUtils;
import com.hekangyuan.nivi1000.utils.SharePreferenceTools;
import com.hekangyuan.nivi1000.utils.ThreadPoolWrapper;
import com.hekangyuan.nivi1000.utils.ToastUtils;
import com.hekangyuan.nivi1000.utils.Utils;
import com.ycuwq.datepicker.date.DatePicker;
import com.ycuwq.datepicker.date.DatePickerDialogFragment;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import android_serialport_api.SerialPortUtil;


public class LoginActivity extends BaseActivity<LoginPresenter> implements LoginView, View.OnClickListener {
    private Button logins;
    private TextView userNameTextView;
    private TextView modifyPassTextView;
    private EditText et_password;
    private LoginBean mainBeans;
    protected boolean isHidden = true;
    private ImageView ispas;
    private CheckBox iv_mima_status;
    private boolean isiorgetpas;
    private SharePreferenceTools spTools;
    private String username, password;

    private boolean istrue = true;
    int level1 = 0;
    int lastLevel1 = -1;
    int level2 = 0;
    int lastLevel2 = -1;
    int level3 = 0;
    int lastLevel3 = -1;
    private Runnable parseRunnableGpio1;

    @Override
    protected LoginPresenter createPresenter() {
        return new LoginPresenter(this);
    }

    @Override
    protected int getLayoutId() {
        return R.layout.activity_login;
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
//        intiViews();
//        initListener();
//        saveLogin();
    }

    @Override
    protected void onResume() {
        super.onResume();

        istrue = false;
        GpioNative.closeGpioDev();
//        ThreadPoolWrapper.removeTask(parseRunnableGpio1);
//        ThreadPoolWrapper.shutdown();

        intiViews();
        initListener();
        saveLogin();

        istrue = true;
        level1 = 0;
        lastLevel1 = -1;
        level2 = 0;
        lastLevel2 = -1;
        level3 = 0;
        lastLevel3 = -1;
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
                        ThreadPoolWrapper.shutdown();
                    }

                    try {
                        Thread.currentThread().sleep(100);
                    }catch (Exception e){
                        e.printStackTrace();
                    }
                    GpioNative.setGpioMode(1,0);
                    level1 = GpioNative.getGpioLevel(1);

                    if (lastLevel1 == level1) continue;
                    lastLevel1 = level1;

                    if(level1 == 1){
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                username = userNameTextView.getText().toString();
                                password = et_password.getText().toString();
                                if (!check()) {
                                    ToastUtils.showToast(LoginActivity.this, R.string.pass_null);
                                    return;
                                }

                                if (!NetWorkUtils.isNetworkAvailable(LoginActivity.this)) {
                                    ToastUtils.showToast(LoginActivity.this, R.string.no_wifi);
                                    return;
                                }

                                Map modeMap = spTools.readSharedPreference(Constants.SP_MODE_MAP);
                                if(modeMap == null && modeMap.size() == 0){
                                    ToastUtils.showToast(LoginActivity.this, "请先设置测量模式");
                                    return;
                                }

                                doLogin();
                            }
                        });
                    }
                }
            }
        };
        ThreadPoolWrapper.getThreadPool().executeTask(parseRunnableGpio1);
    }

    private void initListener() {
        modifyPassTextView.setOnClickListener(this);
        ispas.setOnClickListener(this);
        iv_mima_status.setOnClickListener(this);
    }

    /**
     * 保存登录状态
     */
    private void saveLogin() {
        spTools = new SharePreferenceTools(this);
        Map userInfo = spTools.readSharedPreference("userinfo");
        if (userInfo != null && userInfo.get("islogin") != null) {
            boolean islogin = (boolean) userInfo.get("islogin");
            if (userInfo.containsKey("loginName") && userInfo.containsKey("password") && islogin == true) {
                iv_mima_status.setChecked(true);
                userNameTextView.setText(userInfo.get("loginName").toString());
                et_password.setText(userInfo.get("password").toString());
            }
        }
    }

    /**
     * 初始化控件
     */
    private void intiViews() {
        TextView tv_title = findViewById(R.id.tv_title);
        tv_title.setText("登录");
        logins = findViewById(R.id.login);
        logins.setOnClickListener(this);
        et_password = findViewById(R.id.password);
        userNameTextView = findViewById(R.id.username);
        modifyPassTextView = findViewById(R.id.settings);

        ispas = findViewById(R.id.ispas);
        iv_mima_status = findViewById(R.id.iv_lmima_status);

        String serialString = DeviceUtil.getSERIAL();
        userNameTextView.setText("" + serialString);
//        userNameTextView.setText("LNGNWVTOCU" );
    }

    @Override
    public void onMainSuccess(BaseJsonMsg<LoginBean> o) {
        if(o.getHeader().getErrcode() == 2){
            showToast(o.getHeader().getErrmsg());
            return;
        }

        mainBeans = o.getData();
        Constants.token = o.getData().getToken();
        Log.e("liuchaotoken", o.getData().getToken());
        isiorgetpas = iv_mima_status.isChecked();
        if (isiorgetpas) {
            Map userInfo = new HashMap();
            userInfo.put("islogin", true);
            userInfo.put("loginName", username);
            userInfo.put("password", password);
            spTools.writeSharedPreference("userinfo", userInfo);
        } else {
            Map userInfo = new HashMap();
            userInfo.put("islogin", false);
            userInfo.put("loginName", username);
            userInfo.put("password", password);
            spTools.writeSharedPreference("userinfo", userInfo);
        }

        istrue = false;
        GpioNative.closeGpioDev();
        ThreadPoolWrapper.removeTask(parseRunnableGpio1);
        ThreadPoolWrapper.shutdown();

        Intent intent = new Intent(LoginActivity.this, PatientInfomationActivity.class);
        startActivity(intent);
    }


    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.login:
                username = userNameTextView.getText().toString();
                password = et_password.getText().toString();
                if (!check()) {
                    ToastUtils.showToast(LoginActivity.this, R.string.pass_null);
                    break;
                }

                if (!NetWorkUtils.isNetworkAvailable(LoginActivity.this)) {
                    ToastUtils.showToast(LoginActivity.this, R.string.no_wifi);
                    break;
                }

                Map modeMap = spTools.readSharedPreference(Constants.SP_MODE_MAP);
                if(modeMap == null && modeMap.size() == 0){
                    ToastUtils.showToast(LoginActivity.this, "请先设置测量模式");
                    break;
                }

                doLogin();

//                hello();
                break;

            case R.id.ispas:
                showPwd(et_password);
                break;
            case R.id.iv_lmima_status:
                if (iv_mima_status.isChecked()) {
                    isiorgetpas = true;
                } else {
                    isiorgetpas = false;
                }

                break;
            case R.id.settings:
                doSettings();
                break;
        }


    }

    private void doLogin() {
        LoginBean loginBean = new LoginBean();
        username = userNameTextView.getText().toString();
        password = et_password.getText().toString();
        loginBean.setLoginName(username);
        loginBean.setPassword(password);
//        loginBean.setLoginName("aaa1");
//        loginBean.setPassword("qweqwe");
        loginBean.setDeviceAddress(""); //TODO

        mPresenter.Loginhttp(loginBean);
    }

    private boolean check() {
        if (username.equals("") || password.equals("")) {
            return false;
        }
        return true;
    }

    private void doSettings() {
        istrue = false;
        GpioNative.closeGpioDev();
        ThreadPoolWrapper.removeTask(parseRunnableGpio1);
        ThreadPoolWrapper.shutdown();

        Intent intent = new Intent(LoginActivity.this, SettingActivity.class);
        startActivity(intent);
    }

//    public float[] a4 = new float[100];
//
//    float f1 [] = {61.0f,4559.2f,922.4f,4665.2f,1753.7f,3599.2f,2266.2f,1477.7f,697.3f,4281.9f,2345.0f,4166.7f,1715.7f,4470.2f,111.4f,3370.8f,766.1f,4659.3f,4287.7f,3254.5f,2860.5f,1314.6f,4132.0f,4987.4f,1861.6f,3448.2f,1094.9f,
//            4125.7f,1176.1f,3942.7f,2373.3f,1920.3f,2278.9f,778.6f,685.5f,3129.2f,1599.4f,255.6f,4974.7f,2965.5f,3020.0f,2226.6f,2547.9f,2551.9f,1326.1f,433.2f,4383.6f,141.6f,4890.4f,3877.5f,957.8f,4031.9f,3801.8f,830.5f,194.1f,1170.8f,3817.2f,
//            2878.1f,1970.6f,4565.6f,154.1f,3113.1f,691.5f,7.7f,1344.9f,3411.7f,635.3f,248.4f,630.1f,1063.7f,1745.2f,3492.6f,1617.5f,1583.7f,1851.3f,1238.0f,259.6f,1485.7f,3413.9f,2886.3f,4400.7f,2324.1f,2193.5f,3924.3f,618.7f,2996.0f,4489.0f,
//            139.7f,226.7f,1989.8f,1241.0f,4611.7f,3766.4f,656.7f,4193.2f,2885.5f,4388.7f,3568.6f,2114.7f,1062.4f};
//
//    public void hello(){
//        Hello.filterMibun(f1,a4,(short)100,(short)25);
//        for(int i=0; i < a4.length; i++){
//            Log.e("huang",a4[i]+"");
//        }
//    }

//   public  void  Upadate()
//   {
//       PgyUpdateManager.register(LoginActivity.this,
//               new UpdateManagerListener() {
//
//                   @Override
//                   public void onUpdateAvailable(final String result) {
//
//                       // 将新版本信息封装到AppBean中
//                       final AppBean appBean = getAppBeanFromString(result);
//                       new AlertDialog.Builder(LoginActivity.this)
//                               .setTitle("更新")
//                               .setMessage("检测到新的版本需要更新")
//                               .setNegativeButton(
//                                       "确定",
//                                       new DialogInterface.OnClickListener() {
//
//                                           @Override
//                                           public void onClick(
//                                                   DialogInterface dialog,
//                                                   int which) {
//                                               startDownloadTask(
//                                                       LoginActivity.this,
//                                                       appBean.getDownloadURL());
//                                           }
//                                       }).show();
//                   }
//
//                   @Override
//                   public void onNoUpdateAvailable() {
//                   }
//               });
//   }

    protected void showPwd(EditText editText) {
        if (isHidden) {
            //editText可见
            editText.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
            /**图标改变*/

        } else {
            //editText不可见
            editText.setTransformationMethod(PasswordTransformationMethod.getInstance());
            /**图标改变*/

        }
        isHidden = !isHidden;
        editText.postInvalidate();
        //切换后将EditText光标置于末尾
        CharSequence charSequence = editText.getText();
        if (charSequence instanceof Spannable) {
            Spannable spanText = (Spannable) charSequence;
            Selection.setSelection(spanText, charSequence.length());
        }
    }



}
