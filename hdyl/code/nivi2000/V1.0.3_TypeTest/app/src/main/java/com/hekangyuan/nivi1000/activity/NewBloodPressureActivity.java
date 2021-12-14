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
import com.hekangyuan.nivi1000.activity.api.NewBloodPressureView;
import com.hekangyuan.nivi1000.activity.presenter.NewBloodPressurePresenter;
import com.hekangyuan.nivi1000.base.BaseActivity;
import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.file.StringUtils;
import com.hekangyuan.nivi1000.model.BloodPressureBean;
import com.hekangyuan.nivi1000.model.PatientInformationBean;
import com.hekangyuan.nivi1000.model.SBPBean;
import com.hekangyuan.nivi1000.model.SRealPressureBean;
import com.hekangyuan.nivi1000.serialport_api.ParseBPCallBack;
import com.hekangyuan.nivi1000.serialport_api.ParseRealBPCallBack;
import com.hekangyuan.nivi1000.serialport_api.SerialParseUtil;
import com.hekangyuan.nivi1000.utils.NetWorkUtils;
import com.hekangyuan.nivi1000.utils.ThreadPoolWrapper;
import com.hekangyuan.nivi1000.utils.ToastUtils;
import com.hekangyuan.nivi1000.utils.Utils;
import com.hekangyuan.nivi1000.view.bargraph.BarGraphView;

import java.util.Timer;
import java.util.TimerTask;

import android_serialport_api.SerialBuffer;
import android_serialport_api.SerialPortUtil;

//新建血压测量页面
public class NewBloodPressureActivity extends BaseActivity<NewBloodPressurePresenter> implements NewBloodPressureView, View.OnClickListener {

    private BloodPressureBean bloodPressureBean = new BloodPressureBean();
    private EditText et_height, et_weight, sbpEdit, dbpEdit, et_brPs, et_brPd, heartRateEdit;
    private View popView;
    private PopupWindow popupWindow;
    private RelativeLayout rl_cmo;
    TextView tx_zero, tx_one, et_arPs;
    BarGraphView bargraphview;
    private PatientInformationBean patientInformationBean;

    private Button creatBtn, measureBtn, stopBtn;
    private Runnable parseRunnable;

    private int avgPressure;

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

    @Override
    protected NewBloodPressurePresenter createPresenter() {
        return new NewBloodPressurePresenter(this);
    }

    @Override
    protected int getLayoutId() {
        return R.layout.activity_measureblood;
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
////        patientInformationBean = (PatientInformationBean) getIntent().getSerializableExtra("user");
//        patientInformationBean = MyApplication.patientInformationBean;
//        initView();
//        //处理串口通信方法：1.初始化数据解析线程 2.打开串口通信
//        initSerialThread();
//        openSerialPort();
    }

    @Override
    protected void onResume() {
        super.onResume();

        istrue = false;
        GpioNative.closeGpioDev();
        isRunning = false;
        if (SerialPortUtil.isSerialOpen()) {
            SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x03}));
        }

        istrue = true;
        level1 = 0;
        lastLevel1 = -1;
        level2 = 0;
        lastLevel2 = -1;
        level3 = 0;
        lastLevel3 = -1;
        level4 = 0;
        lastLevel4 = -1;

        //        patientInformationBean = (PatientInformationBean) getIntent().getSerializableExtra("user");
        patientInformationBean = MyApplication.patientInformationBean;
        initView();
        //处理串口通信方法：1.初始化数据解析线程 2.打开串口通信
        initSerialThread();
        openSerialPort();
    }

    private void initView() {
        findViewById();
        //初始化右边柱状图
        initBar(0);
        //设置点击事件
        initBtn();
        initTopPop();
    }

    /**
     * 设置右边柱状图为
     * <p>
     * <p>
     * 实时压力，血压计（mmHg）
     */
    int lastBloodPressure = 0;

    private void initBar(int bloodPressure) {
        if (bloodPressure < 0) { //如果血压值小于0，则用上一次的血压值来画
            bloodPressure = lastBloodPressure;
        }
        lastBloodPressure = bloodPressure;

        final int[][] data = {{bloodPressure}};
        final int[] colorData = {getResources().getColor(R.color.titleback), Color.BLACK, Color.GRAY, Color.GREEN, Color.LTGRAY};
        final String[] textData = {"血压"};
        bargraphview.setMaxHeight(240);
        bargraphview.setBarGraphData(data, colorData, textData);
    }

    private void findViewById() {
        TextView tv_title = findViewById(R.id.tv_title);
        tv_title.setText("新建患者");
        bargraphview = (BarGraphView) findViewById(R.id.bargraphview);
        et_arPs = findViewById(R.id.et_arPs);
        et_height = findViewById(R.id.et_height);
        et_height.setOnFocusChangeListener(new View.OnFocusChangeListener() {
            @Override
            public void onFocusChange(View v, boolean hasFocus) {
                if (hasFocus) {
                    if(et_height.getText().toString().equals("0")){
                        et_height.setText("");
                    }
                }
            }
        });
        et_weight = findViewById(R.id.et_weight);
        et_weight.setOnFocusChangeListener(new View.OnFocusChangeListener() {
            @Override
            public void onFocusChange(View v, boolean hasFocus) {
                if (hasFocus) {
                    if(et_weight.getText().toString().equals("0")){
                        et_weight.setText("");
                    }
                }
            }
        });
        et_brPs = findViewById(R.id.et_brPs);
        et_brPs.setOnFocusChangeListener(new View.OnFocusChangeListener() {
            @Override
            public void onFocusChange(View v, boolean hasFocus) {
                if (hasFocus) {
                    if(et_brPs.getText().toString().equals("0")){
                        et_brPs.setText("");
                    }
                }
            }
        });
        et_brPd = findViewById(R.id.et_brPd);
        et_brPd.setOnFocusChangeListener(new View.OnFocusChangeListener() {
            @Override
            public void onFocusChange(View v, boolean hasFocus) {
                if (hasFocus) {
                    if(et_brPd.getText().toString().equals("0")){
                        et_brPd.setText("");
                    }
                }
            }
        });
        rl_cmo = findViewById(R.id.rl_cmo);
        dbpEdit = findViewById(R.id.dbp_edit);
        dbpEdit.setOnFocusChangeListener(new View.OnFocusChangeListener() {
            @Override
            public void onFocusChange(View v, boolean hasFocus) {
                if (hasFocus) {
                    if(dbpEdit.getText().toString().equals("0")){
                        dbpEdit.setText("");
                    }
                }
            }
        });
        sbpEdit = findViewById(R.id.spb_edit);
        sbpEdit.setOnFocusChangeListener(new View.OnFocusChangeListener() {
            @Override
            public void onFocusChange(View v, boolean hasFocus) {
                if (hasFocus) {
                    if(sbpEdit.getText().toString().equals("0")){
                        sbpEdit.setText("");
                    }
                }
            }
        });
//        heartRateEdit = findViewById(R.id.heart_rate_edit);
    }

    private void initBtn() {
        creatBtn = findViewById(R.id.creat_blood);
        creatBtn.setOnClickListener(this);
//        measureBtn = findViewById(R.id.measure_btn);
        stopBtn = findViewById(R.id.stop_btn);
//        measureBtn.setOnClickListener(this);
        stopBtn.setOnClickListener(this);
        rl_cmo.setOnClickListener(this);
    }


    private boolean isRunning = false;

    /**
     * 初始化串口通信线程
     */
    private void initSerialThread() {
        GpioNative.openGpioDev();

        parseRunnable = new Runnable() {
            @Override
            public void run() {
                while (istrue) {
                    if(MyApplication.refCount == 0){
                        istrue = false;
                        GpioNative.closeGpioDev();
                        isRunning = false;
                        if (SerialPortUtil.isSerialOpen()) {
                            SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x03}));
                        }
//                        SerialPortUtil.close();
                        ThreadPoolWrapper.removeTask(parseRunnable);
                        ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                        ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                        ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                        ThreadPoolWrapper.removeTask(parseRunnableGpio4);
                        ThreadPoolWrapper.shutdown();
                    }

                    if (isRunning && SerialPortUtil.serialBuffer != null && SerialBuffer.stringBuffer.length() != 0) {
                        SerialParseUtil.parseData(null, new ParseBPCallBack() {
                            @Override
                            public void onParseSuccess(SBPBean sbpBean) {
//                                Log.e("huang", "abp----" + sbpBean.getSbp());
//                                Log.e("huang", "abp----" + sbpBean.getDbp());
//                                Log.e("huang", "abp----" + sbpBean.getAbp());
//                                Log.e("huang", "abp----" + sbpBean.getHeartRate());
                                processData(sbpBean);
                            }

                        }, new ParseRealBPCallBack() {
                            @Override
                            public void onParseSuccess(SRealPressureBean sRealPressureBean) {
                                //将实时血压数据显示在界面上
                                processRealBPData(sRealPressureBean);
                            }
                        });
                    }
                }
            }
        };
        ThreadPoolWrapper.getThreadPool().executeTask(parseRunnable);

        parseRunnableGpio1 = new Runnable() {
            @Override
            public void run() {
                while (istrue) {
                    try {
                        Thread.currentThread().sleep(100);
                    }catch (Exception e){
                        e.printStackTrace();
                    }
                    GpioNative.openGpioDev();
                    GpioNative.setGpioMode(0,0);
                    level1 = GpioNative.getGpioLevel(0);

                    if (lastLevel1 == level1) continue;
                    lastLevel1 = level1;

                    if(level1 == 1){
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                //创建血压信息
                                isRunning = false;

                                if(null == patientInformationBean || StringUtils.isTrimEmpty(patientInformationBean.getId())){
                                    showToast("患者基本信息不存在，请返回重新查询患者");
                                    return;
                                }
                                if(et_height.getText().toString().trim().length() == 0 || et_height.getText().toString().equals("0")){
                                    showToast("请手动填写身高");
                                    return;
                                }
                                if(Integer.parseInt(et_height.getText().toString()) < 20 || Integer.parseInt(et_height.getText().toString()) > 300 || Integer.parseInt(et_height.getText().toString()) == 0){
                                    showToast("请填写真实身高");
                                    return;
                                }
                                if(et_weight.getText().toString().trim().length() == 0 || et_weight.getText().toString().equals("0")){
                                    showToast("请手动填写体重");
                                    return;
                                }
                                if(Integer.parseInt(et_weight.getText().toString()) < 5 || Integer.parseInt(et_weight.getText().toString()) > 250 || Integer.parseInt(et_weight.getText().toString()) == 0){
                                    showToast("请填写真实体重");
                                    return;
                                }
                                if(sbpEdit.getText().toString().trim().length() == 0 || sbpEdit.getText().toString().equals("0")){
                                    showToast("请先进行测量");
                                    return;
                                }

                                if(dbpEdit.getText().toString().trim().length() == 0 || dbpEdit.getText().toString().equals("0")){
                                    showToast("请先进行测量");
                                    return;
                                }

//                                istrue = false;
//                                GpioNative.closeGpioDev();
//                                ThreadPoolWrapper.removeTask(parseRunnable);
//                                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
//                                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
//                                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
//                                ThreadPoolWrapper.removeTask(parseRunnableGpio4);
//                                ThreadPoolWrapper.shutdown();

                                createBloodPressureInfo();
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
                                startMeasure();
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

//                    GpioNative.setGpioMode(2,0);
//                    level3 = GpioNative.getGpioLevel(2);
//
//                    if (lastLevel3 == level3) continue;
//                    lastLevel3 = level3;
//
//                    if(level3 == 1){
//                        runOnUiThread(new Runnable() {
//                            @Override
//                            public void run() {
//                                stopMeasure();
//                            }
//                        });
//                    }

                    GpioNative.setGpioMode(2,0);
                    level4 = GpioNative.getGpioLevel(2);

                    if (lastLevel4 == level4) continue;
                    lastLevel4 = level4;

                    if(level4 == 1){
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                if(Utils.nextFastClick()){
                                    ToastUtils.showToast(NewBloodPressureActivity.this,"点击过快");
                                    return;
                                }

                                istrue = false;
                                GpioNative.closeGpioDev();
                                isRunning = false;
                                if (SerialPortUtil.isSerialOpen()) {
                                    SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x03}));
                                }
//                SerialPortUtil.close();
                                ThreadPoolWrapper.removeTask(parseRunnable);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio4);
                                ThreadPoolWrapper.shutdown();

                                finish();
                                Utils.exitActivityAnimation(NewBloodPressureActivity.this);
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

//                    GpioNative.setGpioMode(3,0);
//                    level4 = GpioNative.getGpioLevel(3);
//
//                    if (lastLevel4 == level4) continue;
//                    lastLevel4 = level4;
//
//                    if(level4 == 1){
//                        runOnUiThread(new Runnable() {
//                            @Override
//                            public void run() {
//                                if(Utils.nextFastClick()){
//                                    ToastUtils.showToast(NewBloodPressureActivity.this,"点击过快");
//                                    return;
//                                }
//
//                                istrue = false;
//                                GpioNative.closeGpioDev();
//                                isRunning = false;
//                                if (SerialPortUtil.isSerialOpen()) {
//                                    SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x03}));
//                                }
////                SerialPortUtil.close();
//                                ThreadPoolWrapper.removeTask(parseRunnable);
//                                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
//                                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
//                                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
//                                ThreadPoolWrapper.removeTask(parseRunnableGpio4);
//                                ThreadPoolWrapper.shutdown();
//
//                                finish();
//                                Utils.exitActivityAnimation(NewBloodPressureActivity.this);
//                            }
//                        });
//                    }

                    GpioNative.setGpioMode(3,0);
                    level3 = GpioNative.getGpioLevel(3);

                    if (lastLevel3 == level3) continue;
                    lastLevel3 = level3;

                    if(level3 == 1){
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                stopMeasure();
                            }
                        });
                    }
                }
            }
        };
        ThreadPoolWrapper.getThreadPool().executeTask(parseRunnableGpio4);
    }

    /**
     * 串口通信获取数据，并启动定时获取
     */
    private void openSerialPort() {
        if (!SerialPortUtil.isSerialOpen()) {  //串口没有打开，打开串口
            boolean isOpen = SerialPortUtil.open("/dev/ttyXRUSB3", 115200, 0);
//            boolean isOpen = SerialPortUtil.open("/dev/ttyS4", 115200, 0);
            if (!isOpen) {
                Log.e(TAG, "打开失败,请检查设备是否正常连接");
                return;
            }
        }
//        sendPacketByTimer();

    }


    private void processRealBPData(final SRealPressureBean sRealPressureBean) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                initBar(sRealPressureBean.getrPCuff());
            }
        });
    }

    /**
     * 返回了血压数据对象，需要把数据显示在页面上
     *
     * @param sbpBean
     */
    private void processData(final SBPBean sbpBean) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                int sbp = sbpBean.getSbp() < 0 ? -sbpBean.getSbp() : sbpBean.getSbp();//小于0
                sbpEdit.setText(sbp + "");
                dbpEdit.setText(sbpBean.getDbp() + "");
//                heartRateEdit.setText(sbpBean.getHeartRate() + "");
                avgPressure = sbpBean.getAbp();
            }
        });
    }

    /**
     * 开始测量
     */
    boolean bpSwitch = false;

    private void startMeasure() {
        isRunning = true;
        if (SerialPortUtil.isSerialOpen()) {
            if (!bpSwitch) {
                SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x0B, 0x02})); //血压气阀打开
                bpSwitch = !bpSwitch;
            }

            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x02})); //启动血压测量

        }
    }

    /**
     * 停止测量
     */
    private void stopMeasure() {
        isRunning = false;
        if (SerialPortUtil.isSerialOpen()) {
            SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x03}));
        }
    }


    /**
     * 200秒交替发送
     */
    private boolean timerFlag = true;

    private void sendPacketByTimer() {
        Timer timer = new Timer();
        TimerTask task = new TimerTask() {
            @Override
            public void run() {
//                if (timerFlag) {
//                    SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x05}));
//                } else {
                SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x06}));
//                }
//                timerFlag = !timerFlag;
            }
        };
        timer.schedule(task, 0, 400);
    }


    @Override
    public void onMainSuccess(BaseJsonMsg<BloodPressureBean> o) {
        if(o.getHeader().getErrcode() == 2){
            showToast(o.getHeader().getErrmsg());
            return;
        }

        istrue = false;
        GpioNative.closeGpioDev();
        isRunning = false;
        if (SerialPortUtil.isSerialOpen()) {
            SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x03}));
        }
//        SerialPortUtil.close();
        ThreadPoolWrapper.removeTask(parseRunnable);
        ThreadPoolWrapper.removeTask(parseRunnableGpio1);
        ThreadPoolWrapper.removeTask(parseRunnableGpio2);
        ThreadPoolWrapper.removeTask(parseRunnableGpio3);
        ThreadPoolWrapper.removeTask(parseRunnableGpio4);
        ThreadPoolWrapper.shutdown();

        bloodPressureBean.setBasisMeasurementId(o.getData().getBasisMeasurementId());
        MyApplication.bloodPressureBean = bloodPressureBean;
        Intent intent = new Intent(NewBloodPressureActivity.this, DrawsActivity.class);
//        intent.putExtra("bloodPressure", bloodPressureBean);
//        intent.putExtra("user", patientInformationBean);
        MyApplication.bloodPressureBeanNew = bloodPressureBean;
        startActivity(intent);
    }


    @Override
    public void onClick(View v) {
        switch (v.getId()) {

            case R.id.creat_blood:// TODO 异步
                //创建血压信息
                isRunning = false;

                if(null == patientInformationBean || StringUtils.isTrimEmpty(patientInformationBean.getId())){
                    showToast("患者基本信息不存在，请返回重新查询患者");
                    break;
                }
                if(et_height.getText().toString().trim().length() == 0 || et_height.getText().toString().equals("0")){
                    showToast("请手动填写身高");
                    break;
                }
                if(Integer.parseInt(et_height.getText().toString()) < 20 || Integer.parseInt(et_height.getText().toString()) > 300 || Integer.parseInt(et_height.getText().toString()) == 0){
                    showToast("请填写真实身高");
                    break;
                }
                if(et_weight.getText().toString().trim().length() == 0 || et_weight.getText().toString().equals("0")){
                    showToast("请手动填写体重");
                    break;
                }
                if(Integer.parseInt(et_weight.getText().toString()) < 5 || Integer.parseInt(et_weight.getText().toString()) > 250 || Integer.parseInt(et_weight.getText().toString()) == 0){
                    showToast("请填写真实体重");
                    break;
                }
                if(sbpEdit.getText().toString().trim().length() == 0 || sbpEdit.getText().toString().equals("0")){
                    showToast("请先进行测量");
                    break;
                }

                if(dbpEdit.getText().toString().trim().length() == 0 || dbpEdit.getText().toString().equals("0")){
                    showToast("请先进行测量");
                    break;
                }

//                istrue = false;
//                GpioNative.closeGpioDev();
//                ThreadPoolWrapper.removeTask(parseRunnable);
//                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
//                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
//                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
//                ThreadPoolWrapper.removeTask(parseRunnableGpio4);
//                ThreadPoolWrapper.shutdown();

                createBloodPressureInfo();
                break;
            case R.id.tx_zero:
                et_arPs.setText("0");
                popupWindow.dismiss();
                break;
            case R.id.tx_one:
                et_arPs.setText("1");
                popupWindow.dismiss();
                break;
//            case R.id.measure_btn:
//                startMeasure();
//                break;
//            case R.id.stop_btn:
//                stopMeasure();
//                break;
            case R.id.stop_btn:
                if(Utils.nextFastClick()){
                    ToastUtils.showToast(NewBloodPressureActivity.this,"点击过快");
                    return;
                }

                istrue = false;
                GpioNative.closeGpioDev();
                isRunning = false;
                if (SerialPortUtil.isSerialOpen()) {
                    SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x03}));
                }
//                SerialPortUtil.close();
                ThreadPoolWrapper.removeTask(parseRunnable);
                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                ThreadPoolWrapper.removeTask(parseRunnableGpio4);
                ThreadPoolWrapper.shutdown();

                finish();
                Utils.exitActivityAnimation(NewBloodPressureActivity.this);
                break;
            case R.id.rl_cmo:
                //设置冠脉药效等级点击选择
                bgAlpha(1f);
                popupWindow.showAtLocation(popView, Gravity.NO_GRAVITY, 850, 580);
                break;
        }
    }

    /**
     * 创建血压测量信息，并上传
     */
    private void createBloodPressureInfo() {
        bloodPressureBean.setHeight(Integer.parseInt(et_height.getText().toString()));
        bloodPressureBean.setWeight(Integer.parseInt(et_weight.getText().toString()));
        bloodPressureBean.setPs(Integer.parseInt(sbpEdit.getText().toString()));
        bloodPressureBean.setPd(Integer.parseInt(dbpEdit.getText().toString()));
        bloodPressureBean.setBrPs(Integer.parseInt(et_brPs.getText().toString()));
        bloodPressureBean.setBrPd(Integer.parseInt(et_brPd.getText().toString()));
        bloodPressureBean.setArPs(Integer.parseInt(et_arPs.getText().toString()));
//        bloodPressureBean.setPhr(Integer.parseInt(heartRateEdit.getText().toString()));
        bloodPressureBean.setPavrg(avgPressure);
        bloodPressureBean.setPatientId(patientInformationBean.getId());

        if (NetWorkUtils.isNetworkAvailable(NewBloodPressureActivity.this)) {
            mPresenter.NewBloodPressurePresenterhttp(bloodPressureBean);
        } else {
            ToastUtils.showToast(NewBloodPressureActivity.this, R.string.no_wifi);
            Intent intent = new Intent(NewBloodPressureActivity.this, DrawsActivity.class);
//            intent.putExtra("bloodPressure", bloodPressureBean);
            MyApplication.bloodPressureBeanNew = bloodPressureBean;
            startActivity(intent);
        }
    }

    private void initTopPop() {
        popView = LayoutInflater.from(this).inflate(R.layout.newblood_pop, null);
        tx_zero = popView.findViewById(R.id.tx_zero);
        tx_zero.setOnClickListener(this);
        tx_one = popView.findViewById(R.id.tx_one);
        tx_one.setOnClickListener(this);

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
                if(Utils.nextFastClick()){
                    ToastUtils.showToast(NewBloodPressureActivity.this,"点击过快");
                    return;
                }

                istrue = false;
                GpioNative.closeGpioDev();
                isRunning = false;
                if (SerialPortUtil.isSerialOpen()) {
                    SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x03}));
                }
//                SerialPortUtil.close();
                ThreadPoolWrapper.removeTask(parseRunnable);
                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                ThreadPoolWrapper.removeTask(parseRunnableGpio4);
                ThreadPoolWrapper.shutdown();

                finish();
                Utils.exitActivityAnimation(NewBloodPressureActivity.this);
            }
        });
    }

    private void bgAlpha(float bgAlpha) {
        WindowManager.LayoutParams lp = getWindow().getAttributes();


        lp.alpha = bgAlpha; //0.0-1.0
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_DIM_BEHIND);
        getWindow().setAttributes(lp);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
//        Log.e("huang", "onDestroy run---------");
        isRunning = false;
    }
}
