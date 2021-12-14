package com.hekangyuan.nivi1000.activity;


import android.content.Intent;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.PopupWindow;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.hekangyuan.nivi1000.GpioNative;
import com.hekangyuan.nivi1000.MyApplication;
import com.hekangyuan.nivi1000.R;
import com.hekangyuan.nivi1000.activity.api.PatientInformationView;
import com.hekangyuan.nivi1000.activity.presenter.PatientInformationPresenter;
import com.hekangyuan.nivi1000.base.BaseActivity;
import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.model.PatientInformationBean;
import com.hekangyuan.nivi1000.model.dbbean.PatientdbBean;
import com.hekangyuan.nivi1000.utils.CheckIdCard;
import com.hekangyuan.nivi1000.utils.NetWorkUtils;
import com.hekangyuan.nivi1000.utils.ThreadPoolWrapper;
import com.hekangyuan.nivi1000.utils.ToastUtils;
import com.hekangyuan.nivi1000.utils.Utils;
import com.reader.usbdevice.DeviceLib;
import com.ycuwq.datepicker.date.DatePicker;
import com.ycuwq.datepicker.date.DatePickerDialogFragment;

import java.io.UnsupportedEncodingException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;


//创建患者信息页面
public class PatientInfomationActivity extends BaseActivity<PatientInformationPresenter> implements PatientInformationView, View.OnClickListener {
    private Integer sex = null;//TODO 從界面獲取
    private Button btn_save, bt_update, btn_next, bt_queryinfo;
    private TextView tx_man, tx_woman, et_gender, bt_history, et_birthday;
    private EditText et_caseNo, et_name, et_nationality, et_address, et_phone, et_uniqueId;
    private PatientInformationBean patientInformationBean;
    private View popView;
    private PopupWindow popupWindow;
    private RelativeLayout rl_sex;

    private String id;

    private boolean istrue = true;
    int level1 = 0;
    int lastLevel1 = -1;
    int level2 = 0;
    int lastLevel2 = -1;
    int level3 = 0;
    int lastLevel3 = -1;
    int level4 = 0;
    int lastLevel4 = -1;
    private Runnable parseRunnableGpio1;
    private Runnable parseRunnableGpio2;
    private Runnable parseRunnableGpio3;
    private Runnable parseRunnableGpio4;
    private DeviceLib mdev;
    private Runnable parseRunnableDeviceLib;

    @Override
    protected PatientInformationPresenter createPresenter() {
        return new PatientInformationPresenter(this);
    }


    @Override
    protected int getLayoutId() {
        return R.layout.activity_savepatinf;
    }


    @Override
    public void onErrorCode(BaseJsonMsg model) {
//        showToast(model.getHeader().getErrmsg());
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

//        rl_sex = findViewById(R.id.rl_sex);
//        rl_sex.setOnClickListener(this);
//        btn_save = findViewById(R.id.btn_save);
////        et_caseNo=findViewById(R.id.et_caseNo);
//        et_name = findViewById(R.id.et_name);
//        TextView tv_title = findViewById(R.id.tv_title);
//        tv_title.setText("新建患者");
//        et_birthday = findViewById(R.id.et_birthday);
////        et_nationality = findViewById(R.id.et_nationality);
//        et_address = findViewById(R.id.et_address);
//        et_phone = findViewById(R.id.et_phone);
//        et_uniqueId = findViewById(R.id.et_uniqueId);
//        et_gender = findViewById(R.id.et_gender);
//        btn_save.setOnClickListener(this);
//        bt_history = findViewById(R.id.bt_history);
//        bt_history.setOnClickListener(this);
//        bt_queryinfo = findViewById(R.id.bt_queryinfo);
//        bt_queryinfo.setOnClickListener(this);
//        bt_update = findViewById(R.id.bt_update);
//        bt_update.setOnClickListener(this);
//        btn_next = findViewById(R.id.btn_next);
//        btn_next.setOnClickListener(this);
//        final DatePicker datePicker = findViewById(R.id.datePicker);
//        et_birthday.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//
//                DatePickerDialogFragment datePickerDialogFragment = new DatePickerDialogFragment();
//                datePickerDialogFragment.setOnDateChooseListener(new DatePickerDialogFragment.OnDateChooseListener() {
//                    @Override
//                    public void onDateChoose(int year, int month, int day) {
//                        et_birthday.setText(year + "-" + month + "-" + day);
//                    }
//                });
//                datePickerDialogFragment.show(getFragmentManager(), "DatePickerDialogFragment");
//            }
//        });
//        datePicker.setOnDateSelectedListener(new DatePicker.OnDateSelectedListener() {
//            @Override
//            public void onDateSelected(int year, int month, int day) {
//                et_birthday.setText(year + "-" + month + "-" + day);
//
//
//            }
//        });
//        popView = LayoutInflater.from(this).inflate(R.layout.patient_sex, null);
//        tx_man = popView.findViewById(R.id.tx_man);
//        tx_man.setOnClickListener(this);
//        tx_woman = popView.findViewById(R.id.tx_woman);
//        tx_woman.setOnClickListener(this);
//
//        popupWindow = new PopupWindow(popView, ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
//        popupWindow.setBackgroundDrawable(new ColorDrawable(Color.GRAY));
//        popupWindow.setOutsideTouchable(true);
//        popupWindow.setOnDismissListener(new PopupWindow.OnDismissListener() {
//            @Override
//            public void onDismiss() {
//                //popupwindow消失时使背景不透明
//                bgAlpha(1f);
//            }
//        });
//
//        initGpioThread();
//        istrue = true;
//        level1 = 0;
//        lastLevel1 = -1;
//        level2 = 0;
//        lastLevel2 = -1;
//        level3 = 0;
//        lastLevel3 = -1;
    }

    @Override
    protected void onResume() {
        super.onResume();

        istrue = false;
        GpioNative.closeGpioDev();
//                                mdev.closeDevice();
//        ThreadPoolWrapper.removeTask(parseRunnableGpio1);
//        ThreadPoolWrapper.removeTask(parseRunnableGpio2);
//        ThreadPoolWrapper.removeTask(parseRunnableGpio3);
//        ThreadPoolWrapper.removeTask(parseRunnableGpio4);
//        ThreadPoolWrapper.removeTask(parseRunnableDeviceLib);
//        ThreadPoolWrapper.shutdown();

        rl_sex = findViewById(R.id.rl_sex);
        rl_sex.setOnClickListener(this);
        btn_save = findViewById(R.id.btn_save);
//        et_caseNo=findViewById(R.id.et_caseNo);
        et_name = findViewById(R.id.et_name);
        TextView tv_title = findViewById(R.id.tv_title);
        tv_title.setText("新建患者");
        et_birthday = findViewById(R.id.et_birthday);
//        et_nationality = findViewById(R.id.et_nationality);
        et_address = findViewById(R.id.et_address);
        et_phone = findViewById(R.id.et_phone);
        et_uniqueId = findViewById(R.id.et_uniqueId);
        et_gender = findViewById(R.id.et_gender);
        btn_save.setOnClickListener(this);
        bt_history = findViewById(R.id.bt_history);
        bt_history.setOnClickListener(this);
        bt_queryinfo = findViewById(R.id.bt_queryinfo);
        bt_queryinfo.setOnClickListener(this);
        bt_update = findViewById(R.id.bt_update);
        bt_update.setOnClickListener(this);
        btn_next = findViewById(R.id.btn_next);
        btn_next.setOnClickListener(this);
        final DatePicker datePicker = findViewById(R.id.datePicker);
        et_birthday.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(et_uniqueId.getText().toString().trim().length() == 0){
                    showToast("请先填写身份证号");
                    return;
                }
                if(!CheckIdCard.check(et_uniqueId.getText().toString())){
                    showToast("身份证号码格式不正确");
                    return;
                }
                if(et_birthday.getText().toString().equals("")){
                    et_birthday.setText(CheckIdCard.getBirthday(et_uniqueId.getText().toString()));
                    return;
                }

                DatePickerDialogFragment datePickerDialogFragment = new DatePickerDialogFragment();
                datePickerDialogFragment.setOnDateChooseListener(new DatePickerDialogFragment.OnDateChooseListener() {
                    @Override
                    public void onDateChoose(int year, int month, int day) {
                        et_birthday.setText(year + "-" + month + "-" + day);
                    }
                });
                datePickerDialogFragment.show(getFragmentManager(), "DatePickerDialogFragment");
            }
        });
        datePicker.setOnDateSelectedListener(new DatePicker.OnDateSelectedListener() {
            @Override
            public void onDateSelected(int year, int month, int day) {
                et_birthday.setText(year + "-" + month + "-" + day);


            }
        });
        popView = LayoutInflater.from(this).inflate(R.layout.patient_sex, null);
        tx_man = popView.findViewById(R.id.tx_man);
        tx_man.setOnClickListener(this);
        tx_woman = popView.findViewById(R.id.tx_woman);
        tx_woman.setOnClickListener(this);

        popupWindow = new PopupWindow(popView, ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        popupWindow.setBackgroundDrawable(new ColorDrawable(Color.GRAY));
        popupWindow.setOutsideTouchable(true);
        popupWindow.setOnDismissListener(new PopupWindow.OnDismissListener() {
            @Override
            public void onDismiss() {
                //popupwindow消失时使背景不透明
                bgAlpha(1f);
            }
        });

        back = (TextView) findViewById(R.id.title_left);
        if (back == null) {
            return;
        }
        back.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View arg0) {
                istrue = false;
                GpioNative.closeGpioDev();
//                mdev.closeDevice();
                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                ThreadPoolWrapper.removeTask(parseRunnableGpio4);
                ThreadPoolWrapper.removeTask(parseRunnableDeviceLib);
                ThreadPoolWrapper.shutdown();

                finish();
//                Utils.exitActivityAnimation(PatientInfomationActivity.this);
                Intent intent = new Intent(PatientInfomationActivity.this, LoginActivity.class);
                startActivity(intent);
            }
        });

        istrue = true;
        level1 = 0;
        lastLevel1 = -1;
        level2 = 0;
        lastLevel2 = -1;
        level3 = 0;
        lastLevel3 = -1;
        level4 = 0;
        lastLevel4 = -1;

        mdev = MyApplication.mdev;

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
//                                mdev.closeDevice();
                        ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                        ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                        ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                        ThreadPoolWrapper.removeTask(parseRunnableGpio4);
                        ThreadPoolWrapper.removeTask(parseRunnableDeviceLib);
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
                                if(id == null || id.trim().length() == 0 || id == ""){
                                    showToast("请先查询具体患者");
                                    return;
                                }
                                patientInformationBean = getdata();
                                if(patientInformationBean.getUniqueId().trim().length() == 0){
                                    showToast("请填写身份证号");
                                    return;
                                }
                                if(!CheckIdCard.check(patientInformationBean.getUniqueId())){
                                    showToast("身份证号码格式不正确");
                                    return;
                                }
                                if(patientInformationBean.getName().trim().length() == 0){
                                    showToast("请填写姓名");
                                    return;
                                }
                                if(patientInformationBean.getName().length() > 20){
                                    showToast("姓名字数不能大于20");
                                    return;
                                }
//                                if(patientInformationBean.getPhone().trim().length() == 0){
//                                    showToast("请填写手机号");
//                                    return;
//                                }
                                if(patientInformationBean.getPhone().trim().length() > 0){
                                    if(!Utils.isPhone(patientInformationBean.getPhone())){
                                        showToast("手机号码格式不正确");
                                        return;
                                    }
                                }

                                if(patientInformationBean.getBirthday().equals("")){
                                    showToast("请选择出生日期");
                                    return;
                                }
                                if(sex == null){
                                    showToast("请选择性别");
                                    return;
                                }
                                patientInformationBean.setSex(sex);

                                SimpleDateFormat sf1 = new SimpleDateFormat("yyyy-MM-dd");
                                Date date1;
                                Calendar calendar1 = Calendar.getInstance();;
                                try {
                                    date1 = sf1.parse(patientInformationBean.getBirthday());
                                    calendar1.setTime(date1);
                                } catch (ParseException e) {
                                    e.printStackTrace();
                                }
                                if(Calendar.getInstance().get(Calendar.YEAR) <= calendar1.get(Calendar.YEAR)){
                                    showToast("出生日期不合法");
                                    return;
                                }

                                mPresenter.UpPatientInformationhttp(patientInformationBean);
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
                                patientInformationBean = getdata();
                                if(patientInformationBean.getUniqueId().trim().length() == 0){
                                    showToast("请填写身份证号");
                                    return;
                                }
                                if(!CheckIdCard.check(patientInformationBean.getUniqueId())){
                                    showToast("身份证号码格式不正确");
                                    return;
                                }
                                if(patientInformationBean.getName().trim().length() == 0){
                                    showToast("请填写姓名");
                                    return;
                                }
                                if(patientInformationBean.getName().length() > 20){
                                    showToast("姓名字数不能大于20");
                                    return;
                                }
//                                if(patientInformationBean.getPhone().trim().length() == 0){
//                                    showToast("请填写手机号");
//                                    return;
//                                }
                                if(patientInformationBean.getPhone().trim().length() > 0){
                                    if(!Utils.isPhone(patientInformationBean.getPhone())){
                                        showToast("手机号码格式不正确");
                                        return;
                                    }
                                }

                                if(patientInformationBean.getBirthday().equals("")){
                                    showToast("请选择出生日期");
                                    return;
                                }
                                if(sex == null){
                                    showToast("请选择性别");
                                    return;
                                }
                                patientInformationBean.setSex(sex);

                                SimpleDateFormat sf = new SimpleDateFormat("yyyy-MM-dd");
                                Date date;
                                Calendar calendar = Calendar.getInstance();;
                                try {
                                    date = sf.parse(patientInformationBean.getBirthday());
                                    calendar.setTime(date);
                                } catch (ParseException e) {
                                    e.printStackTrace();
                                }
                                if(Calendar.getInstance().get(Calendar.YEAR) <= calendar.get(Calendar.YEAR)){
                                    showToast("出生日期不合法");
                                    return;
                                }

                                PatientdbBean patientdbBean = new PatientdbBean();
                                patientdbBean.setCaseNo(patientInformationBean.getCaseNo());
                                patientdbBean.setName(patientInformationBean.getName());
                                patientdbBean.setSex(patientInformationBean.getSex());
                                patientdbBean.setBirthday(patientInformationBean.getBirthday());
                                patientdbBean.setNationality(patientInformationBean.getNationality());
                                patientdbBean.setAddress(patientInformationBean.getAddress());
                                patientdbBean.setPhone(patientInformationBean.getPhone());
                                patientdbBean.setUniqueId(patientInformationBean.getUniqueId());
                                patientdbBean.setCreateDate(patientInformationBean.getCreateDate());
//                patientdbBean.setHospitalId(patientInformationBean.getHospitalId());
//                getDaoSession().insert(patientdbBean);

                                if (NetWorkUtils.isNetworkAvailable(PatientInfomationActivity.this)) {
                                    mPresenter.PatientInformationhttp(patientInformationBean);
                                } else {
                                    ToastUtils.showToast(PatientInfomationActivity.this, R.string.no_wifi);
                                    toBloodPressure();

                                }
                            }
                        });
                    }
                }
            }
        };
        ThreadPoolWrapper.getThreadPool().executeTask(parseRunnableGpio2);

        parseRunnableGpio3 = new Runnable() {
            @Override
            public void run() {
                while (istrue) {
                    try {
                        Thread.currentThread().sleep(100);
                    }catch (Exception e){
                        e.printStackTrace();
                    }

                    GpioNative.setGpioMode(0,0);
                    level3 = GpioNative.getGpioLevel(0);

                    if (lastLevel3 == level3) continue;
                    lastLevel3 = level3;

                    if(level3 == 1){
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                if(id == null || id.trim().length() == 0 || id == ""){
                                    showToast("请先创建患者基本信息");
                                    return;
                                }
                                toBloodPressure();
                            }
                        });
                    }
                }
            }
        };
        ThreadPoolWrapper.getThreadPool().executeTask(parseRunnableGpio3);

        parseRunnableGpio4 = new Runnable() {
            @Override
            public void run() {
                while (istrue) {
                    try {
                        Thread.currentThread().sleep(100);
                    }catch (Exception e){
                        e.printStackTrace();
                    }

                    GpioNative.setGpioMode(3,0);
                    level4 = GpioNative.getGpioLevel(3);

                    if (lastLevel4 == level4) continue;
                    lastLevel4 = level4;

                    if(level4 == 1){
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                istrue = false;
                                GpioNative.closeGpioDev();
//                                mdev.closeDevice();
                                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio4);
                                ThreadPoolWrapper.removeTask(parseRunnableDeviceLib);
                                ThreadPoolWrapper.shutdown();

                                Intent intent = new Intent(PatientInfomationActivity.this, HistoryActivity.class);
                                startActivity(intent);
                            }
                        });
                    }
                }
            }
        };
        ThreadPoolWrapper.getThreadPool().executeTask(parseRunnableGpio4);

        parseRunnableDeviceLib = new Runnable() {
            @Override
            public void run() {
                while (istrue) {
                    try {
                        Thread.currentThread().sleep(200);
                    }catch (Exception e){
                        e.printStackTrace();
                    }

                    if (!mdev.isOpen()) {
                        mdev.openDevice(101);
                    }
                    int nRt = mdev.PICC_ReadIDCardMsg(MyApplication.pkName);
                    if( nRt != 0){
                        nRt = -99;
                        byte[] cardInfo = new byte[500];
                        if (!mdev.isOpen()) {
//                            showToast("读卡器连接失败");
                            break;
                        }
                        nRt = mdev.iReadSiCard((byte) 0x11, cardInfo);
                        if (nRt != 0) {
//                            runOnUiThread(new Runnable() {
//                                @Override
//                                public void run() {
//                                    et_name.setText("");
//                                    et_uniqueId.setText("");
//                                    et_birthday.setText("");
//                                    et_address.setText("");
//                                    et_gender.setText("");
//                                    sex = null;
//                                }
//                            });
                            continue;
                        }
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                et_address.setText("");
                            }
                        });
                        try {
//                            showToast("读卡成功："+ new String(cardInfo,"gbk"));
                            String cardInf = new String(cardInfo,"gbk");
                            String[] s = cardInf.split("\\|");
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    et_name.setText(s[3]);
                                    et_uniqueId.setText(s[6]);
                                    et_birthday.setText(s[7].substring(0, 4) + "-" + s[7].substring(4, 6) + "-" + s[7].substring(6, 8));
                                    if (s[4].equals("1")) {
                                        et_gender.setText("男");
                                        sex = 0;
                                    } else {
                                        et_gender.setText("女");
                                        sex = 1;
                                    }
                                }
                            });
                        } catch (UnsupportedEncodingException e) {
                            e.printStackTrace();
                        }
                    }else {
                        if (mdev.GetCardType() == 0) {
                            //showString("居民身份证");
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    et_name.setText(mdev.getName().replace(" ", ""));
                                    et_uniqueId.setText(mdev.getIDNo());
                                    et_birthday.setText(mdev.getBirth().substring(0, 4) + "-" + mdev.getBirth().substring(4, 6) + "-" + mdev.getBirth().substring(6, 8));
                                    et_address.setText(mdev.getAddress());
                                    if (mdev.getSex().equals("男")) {
                                        et_gender.setText("男");
                                        sex = 0;
                                    } else {
                                        et_gender.setText("女");
                                        sex = 1;
                                    }
                                }
                            });

                        }
                        if (mdev.GetCardType() == 1) {
                            //showString("外国人永久居留证");
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    et_name.setText(mdev.getName().replace(" ", ""));
                                    et_uniqueId.setText(mdev.getIDNo());
                                    et_birthday.setText(mdev.getBirth().substring(0, 4) + "-" + mdev.getBirth().substring(4, 6) + "-" + mdev.getBirth().substring(6, 8));
                                    et_address.setText(mdev.getNationalityCode());
                                    if (mdev.getSex().equals("男")) {
                                        et_gender.setText("男");
                                        sex = 0;
                                    } else {
                                        et_gender.setText("女");
                                        sex = 1;
                                    }
                                }
                            });
                        }
                        if (mdev.GetCardType() == 2) {
                            //showString("港澳台居民居住证");
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    et_name.setText(mdev.getName().replace(" ", ""));
                                    et_uniqueId.setText(mdev.getIDNo());
                                    et_birthday.setText(mdev.getBirth().substring(0, 4) + "-" + mdev.getBirth().substring(4, 6) + "-" + mdev.getBirth().substring(6, 8));
                                    et_address.setText(mdev.getAddress());
                                    if (mdev.getSex().equals("男")) {
                                        et_gender.setText("男");
                                        sex = 0;
                                    } else {
                                        et_gender.setText("女");
                                        sex = 1;
                                    }
                                }
                            });
                        }
                    }
                }
            }
        };
        ThreadPoolWrapper.getThreadPool().executeTask(parseRunnableDeviceLib);
    }

    private void bgAlpha(float bgAlpha) {
        WindowManager.LayoutParams lp = getWindow().getAttributes();


        lp.alpha = bgAlpha; //0.0-1.0
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_DIM_BEHIND);
        getWindow().setAttributes(lp);
    }

    @Override
    public void onCreatInfoSuccess(BaseJsonMsg<PatientInformationBean> o) {
        patientInformationBean = o.getData();
        if (o.getHeader().getErrcode() == 2) {
            showToast(o.getHeader().getErrmsg());
        } else {

            showToast("保存成功");
            patientInformationBean = o.getData();
            id = patientInformationBean.getPatientId();
            patientInformationBean.setId(o.getData().getPatientId());
            Log.e("huang"," id: " + id);
            toBloodPressure();
        }

    }

    /**
     * 跳转到血压测量页面
     */
    private void toBloodPressure() {
        istrue = false;
        GpioNative.closeGpioDev();
//        mdev.closeDevice();
        ThreadPoolWrapper.removeTask(parseRunnableGpio1);
        ThreadPoolWrapper.removeTask(parseRunnableGpio2);
        ThreadPoolWrapper.removeTask(parseRunnableGpio3);
        ThreadPoolWrapper.removeTask(parseRunnableGpio4);
        ThreadPoolWrapper.removeTask(parseRunnableDeviceLib);
        ThreadPoolWrapper.shutdown();

        Intent intent = new Intent(PatientInfomationActivity.this, NewBloodPressureActivity.class);
//        intent.putExtra("user", patientInformationBean);
        MyApplication.patientInformationBean = patientInformationBean;
        startActivity(intent);
    }

    @Override
    public void onQueryInfoSuccess(BaseJsonMsg<PatientInformationBean> o) {

        if (o.getHeader().getErrcode() == 1) {
            patientInformationBean = o.getData();
            et_name.setText(patientInformationBean.getName());
            et_phone.setText(patientInformationBean.getPhone());
            et_birthday.setText(patientInformationBean.getBirthday());
//            et_nationality.setText(patientInformationBean.getNationality());
            if ((patientInformationBean.getSex()) == 0) {
                et_gender.setText("男");
                sex = 0;
            } else {
                et_gender.setText("女");
                sex = 1;
            }
            et_address.setText(patientInformationBean.getAddress());
            et_uniqueId.setText(patientInformationBean.getUniqueId());
            id = patientInformationBean.getId();
        } else if (o.getHeader().getErrcode() == 2) {
//            showToast(o.getHeader().getErrmsg());
            showToast("没有查询到对应的患者信息");
        }
    }

    @Override
    public void onUpInfoSuccess(BaseJsonMsg<PatientInformationBean> o) {
        if (o.getHeader().getErrcode() == 1) {
            showToast(o.getHeader().getErrmsg());
        } else if (o.getHeader().getErrcode() == 2) {
            showToast(o.getHeader().getErrmsg());
        }

    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_save:
                patientInformationBean = getdata();
                if(patientInformationBean.getUniqueId().trim().length() == 0){
                    showToast("请填写身份证号");
                    break;
                }
                if(!CheckIdCard.check(patientInformationBean.getUniqueId())){
                    showToast("身份证号码格式不正确");
                    break;
                }
                if(patientInformationBean.getName().trim().length() == 0){
                    showToast("请填写姓名");
                    break;
                }
                if(patientInformationBean.getName().length() > 20){
                    showToast("姓名字数不能大于20");
                    break;
                }
//                if(patientInformationBean.getPhone().trim().length() == 0){
//                    showToast("请填写手机号");
//                    break;
//                }
                if(patientInformationBean.getPhone().trim().length() > 0){
                    if(!Utils.isPhone(patientInformationBean.getPhone())){
                        showToast("手机号码格式不正确");
                        break;
                    }
                }

                if(patientInformationBean.getBirthday().equals("")){
                    showToast("请选择出生日期");
                    break;
                }
                if(sex == null){
                    showToast("请选择性别");
                    break;
                }
                patientInformationBean.setSex(sex);

                SimpleDateFormat sf = new SimpleDateFormat("yyyy-MM-dd");
                Date date;
                Calendar calendar = Calendar.getInstance();;
                try {
                    date = sf.parse(patientInformationBean.getBirthday());
                    calendar.setTime(date);
                } catch (ParseException e) {
                    e.printStackTrace();
                }
                if(Calendar.getInstance().get(Calendar.YEAR) <= calendar.get(Calendar.YEAR)){
                    showToast("出生日期不合法");
                    break;
                }

                PatientdbBean patientdbBean = new PatientdbBean();
                patientdbBean.setCaseNo(patientInformationBean.getCaseNo());
                patientdbBean.setName(patientInformationBean.getName());
                patientdbBean.setSex(patientInformationBean.getSex());
                patientdbBean.setBirthday(patientInformationBean.getBirthday());
                patientdbBean.setNationality(patientInformationBean.getNationality());
                patientdbBean.setAddress(patientInformationBean.getAddress());
                patientdbBean.setPhone(patientInformationBean.getPhone());
                patientdbBean.setUniqueId(patientInformationBean.getUniqueId());
                patientdbBean.setCreateDate(patientInformationBean.getCreateDate());
//                patientdbBean.setHospitalId(patientInformationBean.getHospitalId());
//                getDaoSession().insert(patientdbBean);

                if (NetWorkUtils.isNetworkAvailable(PatientInfomationActivity.this)) {
                    mPresenter.PatientInformationhttp(patientInformationBean);
                } else {
                    ToastUtils.showToast(PatientInfomationActivity.this, R.string.no_wifi);
                    toBloodPressure();

                }

                break;
            case R.id.bt_queryinfo:
                String uniqueId = et_uniqueId.getText().toString();
                if (NetWorkUtils.isNetworkAvailable(PatientInfomationActivity.this)) {

                    mPresenter.QueryPatInfoByUniqueIdhttp(uniqueId);
                } else {
                    ToastUtils.showToast(PatientInfomationActivity
                            .this, R.string.no_wifi);
                }

                break;
            case R.id.bt_update:
                if(id == null || id.trim().length() == 0 || id == ""){
                    showToast("请先查询具体患者");
                    break;
                }
                patientInformationBean = getdata();
                if(patientInformationBean.getUniqueId().trim().length() == 0){
                    showToast("请填写身份证号");
                    break;
                }
                if(!CheckIdCard.check(patientInformationBean.getUniqueId())){
                    showToast("身份证号码格式不正确");
                    break;
                }
                if(patientInformationBean.getName().trim().length() == 0){
                    showToast("请填写姓名");
                    break;
                }
                if(patientInformationBean.getName().length() > 20){
                    showToast("姓名字数不能大于20");
                    break;
                }
//                if(patientInformationBean.getPhone().trim().length() == 0){
//                    showToast("请填写手机号");
//                    break;
//                }
                if(patientInformationBean.getPhone().trim().length() > 0){
                    if(!Utils.isPhone(patientInformationBean.getPhone())){
                        showToast("手机号码格式不正确");
                        break;
                    }
                }

                if(patientInformationBean.getBirthday().equals("")){
                    showToast("请选择出生日期");
                    break;
                }
                if(sex == null){
                    showToast("请选择性别");
                    break;
                }
                patientInformationBean.setSex(sex);

                SimpleDateFormat sf1 = new SimpleDateFormat("yyyy-MM-dd");
                Date date1;
                Calendar calendar1 = Calendar.getInstance();;
                try {
                    date1 = sf1.parse(patientInformationBean.getBirthday());
                    calendar1.setTime(date1);
                } catch (ParseException e) {
                    e.printStackTrace();
                }
                if(Calendar.getInstance().get(Calendar.YEAR) <= calendar1.get(Calendar.YEAR)){
                    showToast("出生日期不合法");
                    break;
                }

                mPresenter.UpPatientInformationhttp(patientInformationBean);
                break;
            case R.id.bt_history:
                Intent intent = new Intent(PatientInfomationActivity.this, HistoryActivity.class);
                startActivity(intent);

                istrue = false;
                GpioNative.closeGpioDev();
//                mdev.closeDevice();
                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                ThreadPoolWrapper.removeTask(parseRunnableGpio4);
                ThreadPoolWrapper.removeTask(parseRunnableDeviceLib);
                ThreadPoolWrapper.shutdown();

                break;
            case R.id.btn_next:
                if(id == null || id.trim().length() == 0 || id == ""){
                    showToast("请先创建患者基本信息");
                    break;
                }
                toBloodPressure();
                break;
//            case R.id.back:
//                finish();
//                break;
            case R.id.tx_man:
                sex = 0;
                et_gender.setText("男");
                popupWindow.dismiss();
                break;
            case R.id.tx_woman:
                sex = 1;
                et_gender.setText("女");
                popupWindow.dismiss();
                break;
            case R.id.rl_sex:
                //设置性别点击选择
                bgAlpha(1f);
                popupWindow.showAtLocation(popView, Gravity.NO_GRAVITY, 920, 530);
                break;
        }
    }

    public PatientInformationBean getdata() {
        PatientInformationBean patientInformationBean = new PatientInformationBean();
        patientInformationBean.setId(id);
        patientInformationBean.setName(et_name.getText().toString());
//        patientInformationBean.setSex(sex);
        patientInformationBean.setBirthday(et_birthday.getText().toString());
//        patientInformationBean.setNationality(et_nationality.getText().toString());
        patientInformationBean.setAddress(et_address.getText().toString());
        patientInformationBean.setPhone(et_phone.getText().toString());
        patientInformationBean.setUniqueId(et_uniqueId.getText().toString());
        return patientInformationBean;
    }


}
