package com.hekangyuan.nivi1000.base;

import android.app.ActionBar;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.EditText;
import android.widget.TextView;

import com.hekangyuan.nivi1000.MyApplication;
import com.hekangyuan.nivi1000.R;
import com.hekangyuan.nivi1000.base.mvp.BaseHeadMsg;
import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.base.mvp.BasePresenter;
import com.hekangyuan.nivi1000.base.mvp.BaseView;
import com.hekangyuan.nivi1000.utils.KeyBoardUtils;
import com.hekangyuan.nivi1000.utils.StatusBarUtil;
import com.hekangyuan.nivi1000.utils.Utils;
import com.hekangyuan.nivi1000.view.LoadingDialog;
import com.hjq.toast.ToastUtils;


/**
 * File descripition: activity基类
 */
public abstract class BaseActivity<P extends BasePresenter> extends AppCompatActivity implements BaseView {
    protected final String TAG = this.getClass().getSimpleName();
    public Context mContext;
    protected P mPresenter;

    protected abstract P createPresenter();

    private BaseHeadMsg baseHeadMsg;
    private LoadingDialog loadingDialog;

    protected TextView back;

    public static final int FLAG_HOMEKEY_DISPATCHED = 0x80000000; //需要自己定义标志


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        this.getWindow().setFlags(FLAG_HOMEKEY_DISPATCHED, FLAG_HOMEKEY_DISPATCHED);//关键代码

        mContext = this;
        setContentView(getLayoutId());
        mPresenter = createPresenter();
        setStatusBar();
//        statusBarHide(BaseActivity.this);
        setBack();

        this.initData();
        Utils.enterActivityAnimation(BaseActivity.this, false);
    }

    private void setBack() {
        back = (TextView) findViewById(R.id.title_left);
        if (back == null) {
            return;
        }
        back.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View arg0) {
                finish();
                Utils.exitActivityAnimation(BaseActivity.this);
            }
        });

    }

    /**
     * 获取布局ID
     *
     * @return
     */
    protected abstract int getLayoutId();

    /**
     * 数据初始化操作
     */
    protected abstract void initData();

    /**
     * 此处设置沉浸式地方
     */
    protected void setStatusBar() {
        StatusBarUtil.setTranslucentForImageViewInFragment(this, 0, null);
    }

    /**
     * 设置Activity的statusBar隐藏
     *
     * @param activity
     */
    public static void statusBarHide(Activity activity) {
        // 代表 5.0 及以上
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            View decorView = activity.getWindow().getDecorView();
            int option = View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN;
            decorView.setSystemUiVisibility(option);
            activity.getWindow().setStatusBarColor(Color.TRANSPARENT);
            ActionBar actionBar = activity.getActionBar();
            actionBar.hide();
            return;
        }

        // versionCode > 4.4  and versionCode < 5.0
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT && Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP) {
            activity.getWindow().addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
        }

    }

    /**
     * 封装toast方法（自行定制实现）
     *
     * @param str
     */
    public void showToast(String str) {
        ToastUtils.show(str);
    }

    public void showLongToast(String str) {
        ToastUtils.show(str);
    }

    @Override
    public void showError(String msg) {
        showToast(msg);
    }

    /**
     * 返回所有状态  除去指定的值  可设置所有（根据需求）
     *
     * @param model
     */
    @Override
    public void onErrorCode(BaseJsonMsg model) {
        baseHeadMsg = model.getHeader();
        if (baseHeadMsg.getErrcode() == 10000000) {
            //处理些后续逻辑   如果某个页面不想实现  子类重写这个方法  将super去掉  自定义方法
//            App.put();
//            startActivity(LoginActivity.class);
        }
    }

    @Override
    public void showLoading() {
        showLoadingDialog();
    }

    @Override
    public void hideLoading() {
        dissMissDialog();
    }

    private void showLoadingDialog() {
        showLoadingDialog("加载中...");
    }

    /**
     * 加载  黑框...
     */
    public void showLoadingDialog(String msg) {
        if (loadingDialog == null) {
            loadingDialog = new LoadingDialog(this);
        }
        loadingDialog.setMessage(msg);
        if (!loadingDialog.isShowing()) {
            loadingDialog.show();
        }
    }

    /**
     * 消失  黑框...
     */
    public void dissMissDialog() {
        if (loadingDialog != null) {
            loadingDialog.dismiss();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mPresenter != null) {
            mPresenter.detachView();
        }
        if (loadingDialog != null) {
            loadingDialog.dismiss();
        }
        if (mPresenter != null) {
            mPresenter.detachView();
        }

//        if(SerialPortUtil.isSerialOpen()){
//            SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x03})); //停止血压测量
//            SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55,new byte[]{0x08}));//停止波形传输
//            SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x0B,0x02})); //关闭气阀
//            SerialPortUtil.close();
//        }
    }


    /**
     * [页面跳转]
     *
     * @param clz
     */
    public void startActivity(Class<?> clz) {
        startActivity(clz, null);
    }


    /**
     * [携带数据的页面跳转]
     *
     * @param clz
     * @param bundle
     */
    public void startActivity(Class<?> clz, Bundle bundle) {
        Intent intent = new Intent();
        intent.setClass(this, clz);
        if (bundle != null) {
            intent.putExtras(bundle);
        }
        startActivity(intent);
    }

    /**
     * [含有Bundle通过Class打开编辑界面]
     *
     * @param cls
     * @param bundle
     * @param requestCode
     */
    public void startActivityForResult(Class<?> cls, Bundle bundle, int requestCode) {
        Intent intent = new Intent();
        intent.setClass(this, cls);
        if (bundle != null) {
            intent.putExtras(bundle);
        }
        startActivityForResult(intent, requestCode);
    }

    /**
     * 以下是关于软键盘的处理
     */

    /**
     * 清除editText的焦点
     *
     * @param v   焦点所在View
     * @param ids 输入框
     */
    public void clearViewFocus(View v, int... ids) {
        if (null != v && null != ids && ids.length > 0) {
            for (int id : ids) {
                if (v.getId() == id) {
                    v.clearFocus();
                    break;
                }
            }
        }
    }

    /**
     * 隐藏键盘
     *
     * @param v   焦点所在View
     * @param ids 输入框
     * @return true代表焦点在edit上
     */
    public boolean isFocusEditText(View v, int... ids) {
        if (v instanceof EditText) {
            EditText et = (EditText) v;
            for (int id : ids) {
                if (et.getId() == id) {
                    return true;
                }
            }
        }
        return false;
    }

    //是否触摸在指定view上面,对某个控件过滤
    public boolean isTouchView(View[] views, MotionEvent ev) {
        if (views == null || views.length == 0) {
            return false;
        }
        int[] location = new int[2];
        for (View view : views) {
            view.getLocationOnScreen(location);
            int x = location[0];
            int y = location[1];
            if (ev.getX() > x && ev.getX() < (x + view.getWidth())
                    && ev.getY() > y && ev.getY() < (y + view.getHeight())) {
                return true;
            }
        }
        return false;
    }

    @Override
    public boolean dispatchTouchEvent(MotionEvent ev) {
        if (ev.getAction() == MotionEvent.ACTION_DOWN) {
            if (isTouchView(filterViewByIds(), ev)) {
                return super.dispatchTouchEvent(ev);
            }
            if (hideSoftByEditViewIds() == null || hideSoftByEditViewIds().length == 0) {
                return super.dispatchTouchEvent(ev);
            }
            View v = getCurrentFocus();
            if (isFocusEditText(v, hideSoftByEditViewIds())) {
                KeyBoardUtils.hideInputForce(this);
                clearViewFocus(v, hideSoftByEditViewIds());
            }
        }
        return super.dispatchTouchEvent(ev);
    }


    /**
     * 传入EditText的Id
     * 没有传入的EditText不做处理
     *
     * @return id 数组
     */
    public int[] hideSoftByEditViewIds() {
        return null;
    }

    /**
     * 传入要过滤的View
     * 过滤之后点击将不会有隐藏软键盘的操作
     *
     * @return id 数组
     */
    public View[] filterViewByIds() {
        return null;
    }


    /*实现案例===============================================================================================*/
    /*

    @Override
    public int[] hideSoftByEditViewIds() {
        int[] ids = {R.id.et_company_name, R.id.et_address};
        return ids;
    }

    @Override
    public View[] filterViewByIds() {
        View[] views = {mEtCompanyName, mEtAddress};
        return views;
    }

    */

    public boolean onKeyDown(int keyCode, KeyEvent event) { //监听物理键
        if (keyCode == KeyEvent.KEYCODE_BACK) {// 返回键
            if(keyMonitorListener != null){
                keyMonitorListener.onPreviousClick();
            }
            return true;
        }
        if (keyCode == KeyEvent.KEYCODE_VOLUME_DOWN) {// 音量键下
            if(keyMonitorListener != null){
                keyMonitorListener.onNextClick();
            }
            return true;
        }
        if (keyCode == KeyEvent.KEYCODE_VOLUME_UP) {// 音量键上

            if(keyMonitorListener != null){
                keyMonitorListener.onMiddleClick();
            }
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }

    public KeyMonitorListener keyMonitorListener;

    public interface KeyMonitorListener{
        void onPreviousClick();
        void onNextClick();
        void onMiddleClick();
    }

    @Override
    protected void onStart() {
        super.onStart();
        if (MyApplication.getMyApplication().mFloatingWindow !=null){
            MyApplication.getMyApplication().mFloatingWindow.dismiss();
        }
    }
}
