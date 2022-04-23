package com.hekangyuan.nivi1000.activity;

import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.os.PersistableBundle;
import android.provider.Settings;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Toast;

import com.hekangyuan.nivi1000.MyApplication;
import com.hekangyuan.nivi1000.R;
import com.hekangyuan.nivi1000.activity.api.LoginView;
import com.hekangyuan.nivi1000.activity.presenter.LoginPresenter;
import com.hekangyuan.nivi1000.base.BaseActivity;
import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.float_view.FloatingWindow;
import com.hekangyuan.nivi1000.model.LoginBean;
import com.hekangyuan.nivi1000.utils.Constants;
import com.hekangyuan.nivi1000.utils.DeviceUtil;
import com.hekangyuan.nivi1000.utils.NetWorkUtils;
import com.hekangyuan.nivi1000.utils.ToastUtils;

import java.util.Map;

public class UsbActivity extends BaseActivity<LoginPresenter> implements LoginView, View.OnClickListener {
    private EditText et_password;
    private Button logins;

    @Override
    protected LoginPresenter createPresenter() {
        return new LoginPresenter(this);
    }

    @Override
    protected int getLayoutId() {
        return R.layout.activity_usb;
    }

    @Override
    protected void initData() {
        et_password = findViewById(R.id.password);
        logins = findViewById(R.id.login);
        logins.setOnClickListener(this);
        findViewById(R.id.close_floating_view).setOnClickListener(this);
    }


    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.close_floating_view:
                finish();
                break;
            case R.id.login:
                String password = et_password.getText().toString().trim();
                String serialString = DeviceUtil.getSERIAL();
//                password = et_password.getText().toString().trim();

                if (!NetWorkUtils.isNetworkAvailable(UsbActivity.this)) {
                    ToastUtils.showToast(UsbActivity.this, R.string.no_wifi);
                    break;
                }
                if (!check(serialString, password)) {
                    ToastUtils.showToast(UsbActivity.this, R.string.pass_null);
                    break;
                }
                doLogin(serialString, password);
//                if ("123456".equals(password)){
//
//                } else {
//                    showToast("USB访问权限密码输入有误");
//                }
                break;
        }
    }

    private void doLogin(String username, String psd) {
        LoginBean loginBean = new LoginBean();
//        username = userNameTextView.getText().toString();
//        password = et_password.getText().toString();
        loginBean.setLoginName(username);
        loginBean.setPassword(psd);
//        loginBean.setLoginName("aaa1");
//        loginBean.setPassword("qweqwe");
        loginBean.setDeviceAddress(""); //TODO

        mPresenter.Loginhttp(loginBean);
    }
    @Override
    protected void onStart() {
        super.onStart();
        if (MyApplication.getMyApplication().mFloatingWindow !=null){
            MyApplication.getMyApplication().mFloatingWindow.dismiss();
        }
    }

    @Override
    public void onMainSuccess(BaseJsonMsg<LoginBean> o) {
        if(o.getHeader().getErrcode() == 2){
            showToast(o.getHeader().getErrmsg());
            return;
        }
        if (!requestOverlayPermission()) {
            isAppInstalled(UsbActivity.this.mContext);
//                        MyApplication.getMyApplication().showFloatingWindow();
//                        PackageManager packageManager = mContext.getPackageManager();
//                        Intent intent = packageManager.getLaunchIntentForPackage("com.android.documentsui");
//                        this.startActivityForResult(intent, 1001);
        } else {
            Toast.makeText(this, "请开启悬浮窗权限", Toast.LENGTH_SHORT).show();
        }    }
    private static final int REQUEST_OVERLAY_CODE = 101;
    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == REQUEST_OVERLAY_CODE) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                if (Settings.canDrawOverlays(UsbActivity.this)) {
                    isAppInstalled(UsbActivity.this.mContext);
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

    private boolean isAppInstalled(Context context)
    {
        PackageInfo packageInfo;
        String packagename = "";
        try {
            packageInfo = context.getPackageManager().getPackageInfo("com.android.documentsui", 0);
            packagename = "com.android.documentsui";
        }catch (PackageManager.NameNotFoundException e) {
            packageInfo = null;
            e.printStackTrace();
            try {
                packageInfo = context.getPackageManager().getPackageInfo("com.google.android.documentsui", 0);
                packagename = "com.google.android.documentsui";
            }catch (PackageManager.NameNotFoundException e1) {
                packageInfo = null;
                e1.printStackTrace();
            }
        }
        if(packageInfo ==null){
            //System.out.println("没有安装");
            showToast("请安装文件管理应用");
            return false;
        }else{
            MyApplication.getMyApplication().showFloatingWindow();
            PackageManager packageManager = mContext.getPackageManager();
            Intent intent = packageManager.getLaunchIntentForPackage(packagename);
            this.startActivityForResult(intent, 1001);
            return true;
        }
    }

    private boolean check(String username, String password) {
        if (username.equals("") || password.equals("")) {
            return false;
        }
        return true;
    }
}