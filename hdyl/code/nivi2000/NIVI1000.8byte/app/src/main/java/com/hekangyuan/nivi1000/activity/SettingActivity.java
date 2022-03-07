package com.hekangyuan.nivi1000.activity;

import android.content.Intent;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RelativeLayout;
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
import com.hekangyuan.nivi1000.utils.CheckIdCard;
import com.hekangyuan.nivi1000.utils.Constants;
import com.hekangyuan.nivi1000.utils.NetWorkUtils;
import com.hekangyuan.nivi1000.utils.SharePreferenceTools;
import com.hekangyuan.nivi1000.utils.ThreadPoolWrapper;
import com.hekangyuan.nivi1000.utils.ToastUtils;
import com.hekangyuan.nivi1000.utils.Utils;

import java.io.UnsupportedEncodingException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;


public class SettingActivity extends BaseActivity<LoginPresenter> implements LoginView, View.OnClickListener {


    private RelativeLayout modifyPassLayout;

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
    }

    private void setClickListener() {
        modifyPassLayout.setOnClickListener(this);

        modeOne.setOnClickListener(this);
        modeTwo.setOnClickListener(this);
        submit.setOnClickListener(this);
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
        }
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


}
