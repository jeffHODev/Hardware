package com.hekangyuan.nivi1000.activity;

import android.content.Intent;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.hekangyuan.nivi1000.GpioNative;
import com.hekangyuan.nivi1000.MyApplication;
import com.hekangyuan.nivi1000.R;
import com.hekangyuan.nivi1000.activity.api.UploadEcgView;
import com.hekangyuan.nivi1000.activity.presenter.UploadEcgPresenter;
import com.hekangyuan.nivi1000.base.BaseActivity;
import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.db.PatientDaoHelper;
import com.hekangyuan.nivi1000.file.StringUtils;
import com.hekangyuan.nivi1000.jnastructure.AnalHPA_Data;
import com.hekangyuan.nivi1000.jnastructure.CLibrary;
import com.hekangyuan.nivi1000.model.BloodPressureBean;
import com.hekangyuan.nivi1000.model.EcgMonitorBean;
import com.hekangyuan.nivi1000.model.PatientInformationBean;
import com.hekangyuan.nivi1000.model.SerialBean;
import com.hekangyuan.nivi1000.model.dbbean.PatientCheckDataBean;
import com.hekangyuan.nivi1000.serialport_api.ParseSerialCallBack;
import com.hekangyuan.nivi1000.serialport_api.SerialParseUtil;
import com.hekangyuan.nivi1000.utils.Constants;
import com.hekangyuan.nivi1000.utils.FFT;
import com.hekangyuan.nivi1000.utils.SharePreferenceTools;
import com.hekangyuan.nivi1000.utils.ThreadPoolWrapper;
import com.hekangyuan.nivi1000.utils.ToastUtils;
import com.hekangyuan.nivi1000.utils.Utils;
import com.hekangyuan.nivi1000.utils.jnatojavasdk.Hpassys;
import com.hekangyuan.nivi1000.view.graphViewForLive.DataForView;
import com.hekangyuan.nivi1000.view.graphViewForLive.PathGraphView;
import com.nivi.nivifilter.SerialBeanFilterHead;
import com.nivi.nivifilter.SerialBeanFilterPcg;
import com.nivi.nivifilter.SerialBeanFilterEcg;
import com.nivi.nivifilter.SerialBeanFilterTst;
import com.sun.jna.Memory;
import com.sun.jna.Native;
import com.sun.jna.Pointer;

import java.lang.reflect.Array;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.LinkedBlockingDeque;
import android_serialport_api.SerialBuffer;
import android_serialport_api.SerialPortUtil;

public class DrawsActivity extends BaseActivity<UploadEcgPresenter> implements UploadEcgView, View.OnClickListener {
    private Button btSuspend, bt_get, bt_next, bt_previous_step, bt_start, bt_yadj;
    private TextView tv_ecg, tv_heart_sounds, tv_pulse, tv_title;
//    private EditText et_first, et_second, et_third;
    private RelativeLayout thirdViewLayout;
    private Float[] hypecgs;
    private Float[] hyppcgs;
    private Float[] hypapgs;
    private Float[] hypecgd;
    private Float[] hyppcgd;
    private Float[] hypapgd;
    private Float[] ecgPl;
    private Float[] pcgPl;
    private Float[] apgL;
    private Float[] ecgPr;
    private Float[] pcgPr;
    private Float[] apgR;
    private Float[] ecgTh;
    private Float[] apgTh;
    private boolean isStart = false;
    int yadj_set = 3000;
    public static float widthPerPointone;
    public static float widthPerPointonePcg;
    public int titleIndex = 0;
    float sumtemp = 0;
    float sumtempdiff = 0;
    float sumtempdiff2a = 0;
    float sumtempdiff2b = 0;
    float tempmax = 0;
    float tempmin = 0;
    int pointerx = 0;
    float[] temp = new float[1024];
    double[] fftmod = new double[1024];
    float[] tempdiff2 = new float[833];
    double[] imaginary = new double[1024];
    double[] real = new double[1024];
    int filtercountsanjiao=0;
    int filtercountfang=0;
    int filtercountsin1=0;
    FFT fft = new FFT(1024);
    private String[][] ecgTitle = {{"心电波", "心音波", "超收缩压脉搏波"},
            {"心电波", "心音波", "低舒张压脉搏波"},
            {"心电波", "心音波", "左颈动脉脉搏波"},
            {"心电波", "心音波", "右颈动脉脉搏波"},
            {"心电波", "股动脉脉搏波",""}};

    private String[] title = {"采集超收缩压", "采集低舒张压", "采集左颈动脉", "采集右颈动脉", "采集股动脉"};
    private PatientInformationBean patientInformationBean;
    private BloodPressureBean bloodPressureBean;

    private EcgMonitorBean ecgMonitorBean;

    PathGraphView firstPathView;
    PathGraphView secondPathView;
    PathGraphView thirdPathView;

    private int viewHeight = 0;
    private int viewWidth = 0;
    private Runnable parseRunnable;
    private boolean isRunning;
    private boolean startOrStop;

    int curPos = 0;

    LinkedList<Float> ecgPointList = new LinkedList<>();
    LinkedList<Float> pcgPointList = new LinkedList<>();
    LinkedList<Float> heatPointList = new LinkedList<>();
    LinkedList<Float> tstPointList = new LinkedList<>();

    //初始化画图专用集合
    LinkedList<Float> ecgPointListView = new LinkedList<>();
    LinkedList<Float> pcgPointListView = new LinkedList<>();
    LinkedList<Float> heatPointListView = new LinkedList<>();
    LinkedList<Float> tstPointListView = new LinkedList<>();

    private int DRAW_COUNT = 40;
    private int DRAW_COUNT_View = 40; //7200

    DataForView ecgData, pcgData, heatData,tstData;
    //public volatile static float[][] buffer;
    public float[][] buffer;
    public float[][] bufferFilter; //存放滤波之后的buffer
    public float[][] bufferf;
    public float[][] bufferFilterf;
    LinkedBlockingDeque<float[][]> deque = new LinkedBlockingDeque(3);
    LinkedBlockingDeque<float[][]> dequeFilter = new LinkedBlockingDeque(3);

    private int mode = Constants.MEASURE_MODE_SYNC;
    private SharePreferenceTools spTools;
    private TimerTask task;
    private boolean istrue = true;
    int level1 = 0;
    int lastLevel1 = -1;
    int level2 = 0;
    int lastLevel2 = -1;
    int level3 = 0;
    int lastLevel3 = -1;
    private Runnable parseRunnableGpio1;
    private Runnable parseRunnableGpio2;
    private Runnable parseRunnableGpio3;

    private int firstYadj = 0;
    private int secondYadj = 0;
    private int thirdYadj = 0;

    private AnalHPA_Data.ByReference analHPADataStruct; //todo 这里暂时先声明成类全局

    @Override
    protected UploadEcgPresenter createPresenter() {
        return new UploadEcgPresenter(this);
    }

    @Override
    protected int getLayoutId() {
        return R.layout.activity_transfer_files_via_bt;
    }

    @Override
    protected void initData() {

//        initBean();
//
//        initView();
//
//        initSerialThread();//preprocessdata buffer per 240ms new thread
//
//        drawViewByTimer();
//
//        initMode();
    }

    @Override
    protected void onResume() {
        Log.e(TAG, "测试11111111111");
        super.onResume();
        Log.e(TAG, "测试22222222222");
        istrue = false;
        GpioNative.closeGpioDev();
        isRunning = false;
        if (SerialPortUtil.isSerialOpen()) { Log.e(TAG, "测试33333");
            SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x08}));//停止波形传输

            SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x0A}));

            SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x0B, 0x02})); //血压气阀打开
            ecgPointList.clear();
            pcgPointList.clear();
            heatPointList.clear();
            tstPointList.clear();

            ecgPointListView.clear();
            pcgPointListView.clear();
            heatPointListView.clear();
            tstPointListView.clear();

            curPos = 0;

            dequeFilter.clear();
            SerialBuffer.clearBuffer();
            deque.clear();
        }
//                SerialPortUtil.close();
        if(task != null){ Log.e(TAG, "测试55555555");
            task.cancel();
            task = null;
        }
        if(timer != null){ Log.e(TAG, "测试66666666");
            timer.cancel();
            timer.purge();
            timer = null;
        }
//        task.cancel();
//        task = null;
//        timer.cancel();
//        timer.purge();
//        timer = null;

        viewHeight = 0;
        viewWidth = 0;
        istrue = true;
        curPos = 0;
        level1 = 0;
        lastLevel1 = -1;
        level2 = 0;
        lastLevel2 = -1;
        level3 = 0;
        lastLevel3 = -1;
        firstYadj = 0;
        secondYadj = 0;
        thirdYadj = 0;
        analHPADataStruct = new AnalHPA_Data.ByReference();
        // NewBloodPressureActivity.this.finish();
       // Thread.interrupted();
        initBean();

        initView();

        initSerialThread();//preprocessdata buffer per 240ms new thread

        openSerialPort();

        drawViewByTimer();

        initMode();
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

    /**
     * 判断是哪种测试模式
     */
    private void initMode() {
        if(spTools == null){
            spTools = new SharePreferenceTools(this);
        }


        Map modeMap = spTools.readSharedPreference(Constants.SP_MODE_MAP);
        if(modeMap != null && modeMap.size() > 0){
            int mode = (int) modeMap.get(Constants.SP_MODE);
            this.mode = mode;
        }
    }

    /**
     * 初始化bean对象
     */
    private void initBean() {
//        patientInformationBean = (PatientInformationBean) getIntent().getSerializableExtra("user");
//        bloodPressureBean = (BloodPressureBean) getIntent().getSerializableExtra("bloodPressure");
        bloodPressureBean = MyApplication.bloodPressureBeanNew;
    }

    /**
     * 初始化view
     */
    private void initView() {
        tv_title = findViewById(R.id.tv_title);
        tv_title.setText(title[titleIndex]);
        btSuspend = findViewById(R.id.bt_suspend);
        btSuspend.setOnClickListener(this);
        bt_next = findViewById(R.id.bt_next);
        bt_next.setOnClickListener(this);
        bt_previous_step = findViewById(R.id.bt_previous_step);
        bt_previous_step.setOnClickListener(this);

        firstPathView = (PathGraphView) findViewById(R.id.path_view0);
        secondPathView = (PathGraphView) findViewById(R.id.path_view1);
        thirdPathView = (PathGraphView) findViewById(R.id.path_view2);

        tv_ecg = findViewById(R.id.tv_ecg);
        tv_heart_sounds = findViewById(R.id.tv_heart_sounds);
        tv_pulse = findViewById(R.id.tv_pulse);
        thirdViewLayout = findViewById(R.id.third_view_layout);

        back = (TextView) findViewById(R.id.title_left);
        if (back == null) {
            return;
        }
        back.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View arg0) {
//                if(Utils.nextFastClick()){
//                    ToastUtils.showToast(DrawsActivity.this,"点击过快");
//                    return;
//                }
//
//                istrue = false;
//                GpioNative.closeGpioDev();
//                isRunning = false;
//                SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x08}));//停止波形传输
//
//                SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x0A}));
//
//                SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x0B, 0x02})); //血压气阀打开
////                SerialPortUtil.close();
//                task.cancel();
//                task = null;
//                timer.cancel();
//                timer.purge();
//                timer = null;
//                ThreadPoolWrapper.removeTask(parseRunnable);
//                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
//                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
//                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
//                ThreadPoolWrapper.shutdown();
//
//                finish();
//                Utils.exitActivityAnimation(DrawsActivity.this);
            }
        });

        if(titleIndex == 0){
            bt_previous_step.setText("返回");
        }

//        bt_yadj = findViewById(R.id.bt_yadj);
//        bt_yadj.setOnClickListener(this);
    }

    /**
     * 串口通信
     */
    private void initSerialThread() {
        GpioNative.openGpioDev();
//        isRunning = true;
        parseRunnable = new Runnable() {
            @Override
            public void run() {Log.e(TAG, "测试6666666");
                while (istrue) {
                    if(MyApplication.refCount == 0){
                        istrue = false;
                        GpioNative.closeGpioDev();
                        isStart = false;
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                btSuspend.setText("开始");
                            }
                        });
                        isRunning = false;
                        startOrStop = false;
                        SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x08}));//停止波形传输
                        SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x0A}));
                        SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x0B, 0x02})); //血压气阀打开
//                        SerialPortUtil.close();
                        if(task != null){
                            task.cancel();
                            task = null;
                        }
                        if(timer != null){
                            timer.cancel();
                            timer.purge();
                            timer = null;
                        }
                        ThreadPoolWrapper.removeTask(parseRunnable);
                        ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                        ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                        ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                        ThreadPoolWrapper.shutdown();
                    }

//                    //当buffer里面不为空的 时候就开始进行数据解析
                    if (isRunning && SerialPortUtil.serialBuffer != null && SerialBuffer.stringBuffer.length() != 0) {
                        SerialParseUtil.parseData(new ParseSerialCallBack() {
                            @Override
                            public void onParseSuccess(SerialBean serialBean) {
//                                Log.e("huang", "ecgdata----" + serialBean.getEcgdata()[0]);
//                                Log.e("huang", "headata----" + serialBean.getHeadata()[0]);
//                                Log.e("huang", "pcgdata----" + serialBean.getPcgdata()[0]);
//                                Log.e("huang", "tstdata----" + serialBean.getTstdata()[0]);

                                //1.runOnUIThread 将数据画到界面上，每次来了就通过setData刷新界面
                                preProcessdata(serialBean);
                            }
                        }, null, null);
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
                    GpioNative.setGpioMode(0,0);
                    level1 = GpioNative.getGpioLevel(0);

                    if (lastLevel1 == level1) continue;
                    lastLevel1 = level1;

                    if(level1 == 1){
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                if (bt_next.getText().equals("完成")) {
                                    if(Utils.isFastClick()){
                                      //  ToastUtils.showToast(DrawsActivity.this,"点击过快");
                                       // return;
                                    }
                                }else{
                                    if(Utils.nextFastClick()){
                                      //  ToastUtils.showToast(DrawsActivity.this,"点击过快");
                                     //   return;
                                    }
                                }

                                if(startOrStop == true){
                                    ToastUtils.showToast(DrawsActivity.this,"请先暂停数据采集");
                                    return;
                                }

                                if(titleIndex == 0){
                                    if(hypecgs == null || hyppcgs == null || hypapgs == null){
                                    //    ToastUtils.showToast(DrawsActivity.this,"当前页面数据采集有误或还没有进行采集，请先采集");
                                    //    return;
                                    }
                                }
                                if(titleIndex == 1){
                                    if(hypecgd == null || hyppcgd == null || hypapgd == null){
                                     //   ToastUtils.showToast(DrawsActivity.this,"当前页面数据采集有误或还没有进行采集，请先采集");
                                      //  return;
                                    }
                                }
                                if(titleIndex == 2){
                                    if(ecgPl == null || pcgPl == null || apgL == null){
                                     //   ToastUtils.showToast(DrawsActivity.this,"当前页面数据采集有误或还没有进行采集，请先采集");
                                    //    return;
                                    }
                                }
                                if(titleIndex == 3){
                                    if(mode == Constants.MEASURE_MODE_SYNC){
                                        if(ecgPr == null || apgR == null || apgTh == null || pcgPr == null || ecgTh == null){
                                           // ToastUtils.showToast(DrawsActivity.this,"当前页面数据采集有误或还没有进行采集，请先采集");
                                         //   return;
                                        }
                                    }else{
                                        if(ecgPr == null || pcgPr == null || apgR == null){
                                          //  ToastUtils.showToast(DrawsActivity.this,"当前页面数据采集有误或还没有进行采集，请先采集");
                                          //  return;
                                        }
                                    }
                                }
                                if(titleIndex == 4){
                                    if(mode == Constants.MEASURE_MODE_ASYNC){
                                        if(ecgTh == null || apgTh == null){
                                           // ToastUtils.showToast(DrawsActivity.this,"当前页面数据采集有误或还没有进行采集，请先采集");
                                           // return;
                                        }
                                    }
                                }

                                clearList();
                                bt_previous_step.setText("上一步");
                                if (bt_next.getText().equals("完成")) {
                                    Log.e(TAG, "测试55555555ggg");
                                   // uploadInfo();

                                } else {Log.e(TAG, "测试55555555tggg");
                                    titleIndex++;
                                    setEcgTitle();
                                    //分两种情况，同步模式只能加到3，异步模式可以加到4；
                                    if(mode == Constants.MEASURE_MODE_SYNC){
                                        if(titleIndex == 3){
                                            tv_heart_sounds.setText("右颈动脉脉搏波");
                                            tv_pulse.setText("股动脉脉搏波");
                                            bt_next.setText("完成");
                                        }
                                    }else{
                                        if (titleIndex == 4) {
                                            bt_next.setText("下一步");
                                            thirdViewLayout.setVisibility(View.GONE);
                                        }
                                    }
                                }
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
                                if(Utils.nextFastClick()){
                                   // ToastUtils.showToast(DrawsActivity.this,"点击过快");
                                 //  return;
                                }
                                if(!isStart){
                                    isStart = true;
                                    btSuspend.setText("暂停");
                                    startMeasure();
                                }else{
//                                    if(ecgPointList.size() < 5000 || pcgPointList.size() < 5000 || heatPointList.size() < 5000 || tstPointList.size() < 5000){
//                                        ToastUtils.showToast(DrawsActivity.this,"请等待数据采集完整");
//                                        return;
//                                    } //xingjian

                                    isStart = false;
                                    btSuspend.setText("开始");
                                    stopMeasure();
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

                    GpioNative.setGpioMode(2,0);
                    level3 = GpioNative.getGpioLevel(2);

                    if (lastLevel3 == level3) continue;
                    lastLevel3 = level3;

                    if(level3 == 1){
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                if(Utils.nextFastClick()){
                                   // ToastUtils.showToast(DrawsActivity.this,"点击过快");
                                   // return;
                                }

                                if(startOrStop == true){
                                    ToastUtils.showToast(DrawsActivity.this,"请先暂停数据采集");
                                    return;
                                }
                                clearList();

                                thirdViewLayout.setVisibility(View.VISIBLE);
                                bt_next.setText("下一步");
//                                if (bt_previous_step.getText().equals("返回") || titleIndex == 0) {
                                if (titleIndex == 0) {
                                    //当他等于返回的时候，可以不做操作，或者返回到上一个页面
                                    tv_ecg.setText(ecgTitle[titleIndex][0]);
                                    tv_heart_sounds.setText(ecgTitle[titleIndex][1]);
                                    tv_pulse.setText(ecgTitle[titleIndex][2]);

                                    istrue = false;
                                    GpioNative.closeGpioDev();
                                    isRunning = false;
                                    SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x08}));//停止波形传输

                                    SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x0A}));

                                    SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x0B, 0x02})); //血压气阀打开

                                    task.cancel();
                                    task = null;
                                    timer.cancel();
                                    timer.purge();
                                    timer = null;
                                    ThreadPoolWrapper.removeTask(parseRunnable);
                                    ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                                    ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                                    ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                                    ThreadPoolWrapper.shutdown();

                                    finish();
                                    Utils.exitActivityAnimation(DrawsActivity.this);

                                } else {
                                    titleIndex--;
                                    setEcgTitle();
                                    if (titleIndex == 0) {
                                        bt_previous_step.setText("返回");
                                        setEcgTitle();
                                    } else {
                                        setEcgTitle();
                                    }
                                }
                            }
                        });
                    }
                }
            }
        };
        ThreadPoolWrapper.getThreadPool().executeTask(parseRunnableGpio3);
    }

    private void startMeasure() {

   /*     if(StringUtils.isTrimEmpty(bloodPressureBean.getBasisMeasurementId())){
            showToast("测试号为空，请重新创建患者血压数据");
            return;
        }*/

        isRunning = true;
        startOrStop = true;
        //串口已经打开，直接获取数据

        SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x07}));//启动波形传输
        try {
//            Thread.sleep(500);
            Thread.sleep(900);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        int desPressure = 0;

//        if(titleIndex == 0){
//            desPressure = (int) (bloodPressureBean.getPs() * 1.05);
//        }else if(titleIndex == 1){
//            desPressure = (int) (bloodPressureBean.getPd() * 0.95);
//        }else{
//            desPressure = (int) ((bloodPressureBean.getPs() + bloodPressureBean.getPd()) / 2);
//        }

        if(titleIndex == 0){Log.e(TAG, "测试cccc");
            desPressure = (int) (120 * 1.02);
        }else if(titleIndex == 1){
            desPressure = (int) (80 * 0.98);
        }else{Log.e(TAG, "测试eee");
          desPressure = 180;
        }

        if(titleIndex == 0 || titleIndex == 1){Log.e(TAG, "测试ddd");
            //TODO 指定升压，气阀打开
            SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55,new byte[]{0x09, (byte) desPressure}));//指定升压，从上一个取值

            try {
//                Thread.sleep(500);
                Thread.sleep(900);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x0B,0x01})); //升压气阀打开
        }

        if(mode == Constants.MEASURE_MODE_SYNC && titleIndex ==3){
            SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55,new byte[]{0x09, (byte) desPressure}));//指定升压，从上一个取值

            try {
//                Thread.sleep(500);
                Thread.sleep(900);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x0B,0x01})); //升压气阀打开
        }

        //TODO 02
//        if(mode == Constants.MEASURE_MODE_ASYNC && titleIndex == 4){
//            //异步模式，02
//            SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55,new byte[]{0x09, (byte) desPressure}));//指定升压，从上一个取值
//
//            try {
////                Thread.sleep(500);
//                Thread.sleep(1000);
//            } catch (InterruptedException e) {
//                e.printStackTrace();
//            }
//            SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x0B,0x01})); //升压气阀打开
//        }


        //TODO 第一个页面 收缩压*1.05，第二个舒张压*0.95
    }

    private void stopMeasure() {
        isRunning = false;
        startOrStop = false;

        SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x08}));//停止波形传输
        try {
            Thread.sleep(900);
//            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x0A}));
        try {
            Thread.sleep(900);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x0B, 0x02})); //血压气阀打开

//        try {
//            Thread.sleep(500);
//        } catch (InterruptedException e) {
//            e.printStackTrace();
//        }
//
//        SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x0A})); ///// 2021-03-16 cuinanyi edit

//        getDataByIndex();
    }


    /**
     *
     *  3: tst  2: head
     *      * 异步模式：
     *      * 第一个页面：013
     *      * 第二个页面：013
     *      * 第三个页面：012
     *      * 第四个页面：012
     *      * 第五个页面：02
     *
     *      TODO 03
     *      *
     *      *      * 同步模式：
     *      *      * 第一个页面：013
     *      *      * 第二个页面：013
     *      *      * 第三个页面：012
     *      *      * 第四个页面：023
     */
    private void getDataByIndex() {

        switch (titleIndex){ //根据titleIndex初始化14个数组中的哪个，先进行倒置后添加到数组中
            case 0:
                hypecgs = ecgPointList.toArray(new Float[ecgPointList.size()]);
                hyppcgs = pcgPointList.toArray(new Float[pcgPointList.size()]);
                hypapgs = tstPointList.toArray(new Float[tstPointList.size()]);

                //心电超收缩判断
                short sampleNum = (short) hypecgs.length;
                float[] ecgsf = new float[sampleNum];
                for (int i = 0; i < sampleNum; i++){
                    ecgsf[i] = hypecgs[i];
                }

                CLibrary.AnalHPA.ByReference analHPAStructs = new CLibrary.AnalHPA.ByReference();
//                float max_num = CLibrary.INSTANCE2.DetectMaxValue(ecgsf, (short) 0, sampleNum);
//                if (max_num < 0.5f)
//                {
//                    ToastUtils.showToastLong(DrawsActivity.this,"超收缩压心电数据不规范，请重新检测");
//                    hypecgs = null;
//                    break;
//                }

                Pointer pecg = new Memory(sampleNum * 4);
                CLibrary.INSTANCE2.Filter(ecgsf, pecg, sampleNum, (short) 25);
                Pointer ecgfp = new Memory(sampleNum * 4);
                CLibrary.INSTANCE2.FilterMibun(pecg.getFloatArray(0, sampleNum), ecgfp, sampleNum, (short) 25);

//                ecgfp.write(0, ecgsf, 0, sampleNum);
                CLibrary.INSTANCE2.FilterMibun(ecgfp.getFloatArray(0, sampleNum), pecg, sampleNum, (short) 25);

//                short res = CLibrary.INSTANCE3.RQT_Analysis(ecgfp.getFloatArray(0, sampleNum), pecg.getFloatArray(0, sampleNum), sampleNum, analHPAStructs, analHPADataStruct);

                short res;
               /* try{
                    res = Hpassys.RQT_Analysis(pecg.getFloatArray(0, sampleNum), ecgfp.getFloatArray(0, sampleNum), sampleNum, analHPAStructs, analHPADataStruct);
                }catch (Exception e){
                    freePointer(pecg);
                    freePointer(ecgfp);
                    ToastUtils.showToastLong(DrawsActivity.this,"超收缩压心电数据不规范，请重新检测RQT_Analysis");
                    hypecgs = null;
                    break;
                }

                freePointer(pecg);
                freePointer(ecgfp);

                if (res == 0)
                {
                    ToastUtils.showToastLong(DrawsActivity.this,"超收缩压心电数据不规范，请重新检测Rtop_num<2");
                    hypecgs = null;
                    break;
                }*/
//                if(analHPAStructs.getRtops()[0] <= 0 || analHPAStructs.RtopNum <= 0){
//                    ToastUtils.showToastLong(DrawsActivity.this,"超收缩压心电数据不规范，请重新检测");
//                    hypecgs = null;
//                    break;
//                }
                if(analHPAStructs.getRtops()[0] <= 0){
                   // ToastUtils.showToastLong(DrawsActivity.this,"超收缩压心电数据不规范，请重新检测Rtops()[0] <= 0");
                   // hypecgs = null;
                  //  break;
                }

                //心音超收缩判断
                short passBand	= 3;
                float stepHyp = 0.0012f;
                short pcgsSize = (short) hyppcgs.length;
                float[] pcgsf = new float[pcgsSize];
                for (int i = 0; i < pcgsSize; i++){
                    pcgsf[i] = hyppcgs[i];
                }

//                float max_numprg = CLibrary.INSTANCE2.DetectMaxValue(pcgsf, (short) 0, pcgsSize);
//                if (max_numprg < 0.5f)
//                {
//                    ToastUtils.showToastLong(DrawsActivity.this,"超收缩压心音数据不规范，请重新检测");
//                    hyppcgs = null;
//                    break;
//                }
                Pointer ppcg = new Memory(pcgsSize * 4);
                CLibrary.INSTANCE2.Filter(pcgsf, ppcg, pcgsSize, (short) 25);
//                max_numprg = CLibrary.INSTANCE2.DetectMaxValue(ppcg.getFloatArray(0, pcgsSize), (short) 0, pcgsSize);
//                if (max_numprg < 0.2f)
//                {
//                    freePointer(ppcg);
//                    ToastUtils.showToastLong(DrawsActivity.this,"超收缩压心音数据不规范，请重新检测");
//                    hyppcgs = null;
//                    break;
//                }

                Pointer ppcgf = new Memory(pcgsSize * 4);
                ppcgf.write(0, pcgsf,0, pcgsSize);
                CLibrary.INSTANCE2.FFT_Filter(passBand, ppcgf, pcgsSize, 5.0f, 500.0f, stepHyp);
//                analHPAStructs.I_start = CLibrary.INSTANCE3.PcgIanalysis(ppcgf.getFloatArray(0, pcgsSize), pcgsSize, analHPAStructs);
//                analHPAStructs.II_start = CLibrary.INSTANCE3.PcgIIanalysis(ppcgf.getFloatArray(0, pcgsSize), pcgsSize, analHPAStructs);

                try{
                    analHPAStructs.I_start = Hpassys.PcgIanalysis(ppcgf.getFloatArray(0, pcgsSize), pcgsSize, analHPAStructs);
                    analHPAStructs.II_start = Hpassys.PcgIIanalysis(ppcgf.getFloatArray(0, pcgsSize), pcgsSize, analHPAStructs);
                }catch(Exception e){
                    freePointer(ppcg);
                    freePointer(ppcgf);
                    //ToastUtils.showToastLong(DrawsActivity.this,"超收缩压心音数据不规范，请重新检测I_start或II_start 为空");
                   // hyppcgs = null;
                   // break;
                }

                freePointer(ppcg);
                freePointer(ppcgf);

//                if(analHPAStructs.Istarts[0] <= 0 || analHPAStructs.IIstarts[0] <= 0){
//                    ToastUtils.showToastLong(DrawsActivity.this,"超收缩压心音数据不规范，请重新检测");
//                    hyppcgs = null;
//                    break;
//                }

                //脉搏超收缩判断
                short apgsSize = (short) hypapgs.length;

//                for(int i = 0; i < apgsSize; i++){
//                    if(hypapgs[i] < 0 || hypapgs[i] < 10){
//                        ToastUtils.showToastLong(DrawsActivity.this,"超收缩压脉搏数据不规范---测试1，请重新检测");
//                        hypapgs = null;
//                        return;
//                    }
//                }

                float[] apgsf = new float[apgsSize];
                float[] bsnlapgsf = new float[apgsSize];

                for (int i = 0; i < apgsSize; i++){
                    apgsf[i] = hypapgs[i];
                    bsnlapgsf[i] = hypapgs[i];
                }
                Pointer filterLowsPointer = new Memory(apgsSize * 4);
                filterLowsPointer.write(0, bsnlapgsf, 0, apgsSize);

                short ss;
                short dd;
//                float max_numapgs = CLibrary.INSTANCE2.DetectMaxValue(apgsf, (short) 0, apgsSize);
//                if (max_numapgs < 0.5f)
//                {
//                    freePointer(filterLowsPointer);
//                    ToastUtils.showToastLong(DrawsActivity.this,"超收缩压脉搏数据不规范---测试2，请重新检测");
//                    hypapgs = null;
//                    break;
//                }

                float[] japgf = new float[apgsSize];
                for (int i = 0; i < apgsSize; i++){
                    japgf[i] = hypapgs[i];
                }
                Pointer japgfP = new Memory(apgsSize * 4);
                japgfP.write(0, japgf, 0, apgsSize);
                CLibrary.INSTANCE2.FilterLow(apgsf, japgfP, apgsSize, 0.008f);
                Pointer papgf = new Memory(apgsSize * 4);
                CLibrary.INSTANCE2.Filter(japgfP.getFloatArray(0, apgsSize), papgf, apgsSize, (short) 25);
                CLibrary.INSTANCE2.FilterMibun(papgf.getFloatArray(0, apgsSize), japgfP, apgsSize, (short) 25);

                papgf.write(0, apgsf,0, apgsSize);

                CLibrary.INSTANCE2.FilterMibun(japgfP.getFloatArray(0, apgsSize), papgf, apgsSize, (short) 25);

                try{
                    for (int i = 0; i <= analHPAStructs.getRtopNum(); i++)
                    {
                        dd = 0;
                        ss = 0;
                        dd = analHPAStructs.getRtops()[i];
                        ss = (short) (analHPAStructs.getRtops()[i] + (short) 250);
//                    analHPAStructs.USpnts[i] = CLibrary.INSTANCE3.DetectUSpoint(papgf.getFloatArray(0, apgsSize), dd, ss, (short) i, apgsSize, analHPAStructs);
                        analHPAStructs.USpnts[i] = Hpassys.DetectUSpoint(papgf.getFloatArray(0, apgsSize), dd, ss, (short) i, apgsSize, analHPAStructs);
                    }
                }catch (Exception e){
                    freePointer(japgfP);
                    freePointer(papgf);
                    freePointer(filterLowsPointer);
                   // ToastUtils.showToastLong(DrawsActivity.this,"超收缩压脉搏数据不规范---USpnts not found，请重新检测");
                   // hypapgs = null;
                  //  break;
                }

                analHPAStructs.setUS_point(analHPAStructs.getUSpnts()[analHPAStructs.getRpos()]);

                for (int i = 0; i <= analHPAStructs.getRtopNum(); i++)
                {
                    dd = 0;
                    ss = 0;
                    dd = (short) (analHPAStructs.getIIstarts()[i] + (short) 5);
                    ss = (short) (analHPAStructs.getIIstarts()[i] + (short) 160);
                   // analHPAStructs.DNpnts[i] = CLibrary.INSTANCE3.DN_point(papgf.getFloatArray(0, apgsSize), dd, ss, (short) i, apgsSize, analHPAStructs);
                    analHPAStructs.DNpnts[i] = (short) (analHPAStructs.DNpnts[i] - (short) 3);
                }
                analHPAStructs.setDN_point(analHPAStructs.getDNpnts()[analHPAStructs.getRpos()]);

                for (int i = 0; i <= analHPAStructs.getRtopNum(); i++)
                {
                    dd = 0;
                    ss = 0;
                    dd = analHPAStructs.getUSpnts()[i];
                    ss = analHPAStructs.getDNpnts()[i];
                    analHPAStructs.UTpnts[i] = (short)CLibrary.INSTANCE2.DetectMax(filterLowsPointer.getFloatArray(0, apgsSize), dd, ss); //bsnlapgf = 脉搏原始波形apg[];
                }
                analHPAStructs.setUT_point(analHPAStructs.getUTpnts()[analHPAStructs.getRpos()]);

//                for (int i = 0; i < apgsSize; i++)
//                {
//                    japgf[i] = hypapgs[i];
//                }
//                CLibrary.INSTANCE2.FilterLow(japgf, filterLowsPointer, apgsSize, 0.008f);

                freePointer(japgfP);
                freePointer(papgf);
                freePointer(filterLowsPointer);

//                if(analHPAStructs.getUSpnts()[0] <= 0 || analHPAStructs.getDNpnts()[0] <= 0 || analHPAStructs.getUTpnts()[0] <= 0){
//                    ToastUtils.showToastLong(DrawsActivity.this,"超收缩压脉搏数据不规范---测试4，请重新检测");
//                    hypapgs = null;
//                    break;
//                }

//                for (int i = 0; i < analHPAStructs.getUSpnts().length - 1; i++){
//                    if(analHPAStructs.getUSpnts()[i + 1] != 0){
//                        if(analHPAStructs.getUSpnts()[i] > analHPAStructs.getUSpnts()[i + 1]){
//                            ToastUtils.showToastLong(DrawsActivity.this,"超收缩压脉搏数据不规范---测试5，请重新检测");
//                            hypapgs = null;
//                            return;
//                        }
//                    }
//                }
//
//                for (int i = 0; i < analHPAStructs.getUTpnts().length - 1; i++){
//                    if(analHPAStructs.getUTpnts()[i + 1] != 0){
//                        if(analHPAStructs.getUTpnts()[i] > analHPAStructs.getUTpnts()[i + 1]){
//                            ToastUtils.showToastLong(DrawsActivity.this,"超收缩压脉搏数据不规范---测试6，请重新检测");
//                            hypapgs = null;
//                            return;
//                        }
//                    }
//                }

                break;
            case 1:
                hypecgd = ecgPointList.toArray(new Float[ecgPointList.size()]);
                hyppcgd = pcgPointList.toArray(new Float[pcgPointList.size()]);
                hypapgd = tstPointList.toArray(new Float[tstPointList.size()]);

                CLibrary.AnalHPA.ByReference analHPAStructd = new CLibrary.AnalHPA.ByReference();

                //心电低舒张
                short ecgdSize = (short) hypecgd.length;
                float[] ecgdf = new float[ecgdSize];
                for (int i = 0; i < ecgdSize; i++){
                    ecgdf[i] = hypecgd[i];
                }
//                float max_numecgd = CLibrary.INSTANCE2.DetectMaxValue(ecgdf, (short) 0, ecgdSize);
//                if (max_numecgd < 0.5f)
//                {
//                    ToastUtils.showToastLong(DrawsActivity.this,"低舒张压心电数据不规范，请重新检测");
//                    hypecgd = null;
//                    break;
//                }

                Pointer pecgd = new Memory(ecgdSize * 4);
                CLibrary.INSTANCE2.Filter(ecgdf, pecgd, ecgdSize, (short) 25);
                Pointer ecgfpd = new Memory(ecgdSize * 4);
                CLibrary.INSTANCE2.FilterMibun(pecgd.getFloatArray(0, ecgdSize), ecgfpd, ecgdSize, (short) 25);

//                ecgfpd.write(0, ecgdf, 0, ecgdSize);
                CLibrary.INSTANCE2.FilterMibun(ecgfpd.getFloatArray(0, ecgdSize), pecgd, ecgdSize, (short) 25);

//                short resd = CLibrary.INSTANCE3.RQT_Analysis(ecgfpd.getFloatArray(0, ecgdSize), pecgd.getFloatArray(0, ecgdSize), ecgdSize, analHPAStructd, analHPADataStruct);

                short resd;
                /*try{
                    resd = Hpassys.RQT_Analysis(pecgd.getFloatArray(0, ecgdSize), ecgfpd.getFloatArray(0, ecgdSize), ecgdSize, analHPAStructd, analHPADataStruct);
                }catch (Exception e){
                    freePointer(pecgd);
                    freePointer(ecgfpd);
                    ToastUtils.showToastLong(DrawsActivity.this,"低舒张压心电数据不规范，请重新检测RQT_Analysis");
                    hypecgd = null;
                    break;
                }*/
                freePointer(pecgd);
                freePointer(ecgfpd);

               /* if (resd == 0)
                {
                    ToastUtils.showToastLong(DrawsActivity.this,"低舒张压心电数据不规范，请重新检测RTopnum < 2");
                    hypecgd = null;
                    break;
                }*/
//                if(analHPAStructd.getRtops()[0] <= 0 || analHPAStructd.RtopNum <= 0){
//                    ToastUtils.showToastLong(DrawsActivity.this,"低舒张压心电数据不规范，请重新检测");
//                    hypecgd = null;
//                    break;
//                }
               /* if(analHPAStructd.getRtops()[0] <= 0){
                    ToastUtils.showToastLong(DrawsActivity.this,"低舒张压心电数据不规范，请重新检测Rtops()[0] <= 0");
                    hypecgd = null;
                    break;
                }*/

                //心音低舒张
                short passBandd	= 3;
                float stepHypd = 0.0012f;
                short pcgdSize = (short) hyppcgd.length;
                float[] pcgdf = new float[pcgdSize];
                for (int i = 0; i < pcgdSize; i++){
                    pcgdf[i] = hyppcgd[i];
                }
                float max_numpcgd = CLibrary.INSTANCE2.DetectMaxValue(pcgdf, (short) 0, pcgdSize);
//                if (max_numpcgd < 0.5f)
//                {
//                    ToastUtils.showToastLong(DrawsActivity.this,"低舒张压心音数据不规范，请重新检测");
//                    hyppcgd = null;
//                    break;
//                }
                Pointer ppcgd = new Memory(pcgdSize * 4);
                CLibrary.INSTANCE2.Filter(pcgdf, ppcgd, pcgdSize, (short) 25);
//                max_numpcgd = CLibrary.INSTANCE2.DetectMaxValue(ppcgd.getFloatArray(0, pcgdSize), (short) 0, pcgdSize);
//                if (max_numpcgd < 0.2f)
//                {
//                    freePointer(ppcgd);
//                    ToastUtils.showToastLong(DrawsActivity.this,"低舒张压心音数据不规范，请重新检测");
//                    hyppcgd = null;
//                    break;
//                }
                Pointer ppcgfd = new Memory(pcgdSize * 4);
                ppcgfd.write(0, pcgdf,0, pcgdSize);
                CLibrary.INSTANCE2.FFT_Filter(passBandd, ppcgfd, pcgdSize, 5.0f, 500.0f, stepHypd);
//                analHPAStructd.setI_start(CLibrary.INSTANCE3.PcgIanalysis(ppcgfd.getFloatArray(0, pcgdSize), pcgdSize, analHPAStructd));
//                analHPAStructd.setII_start(CLibrary.INSTANCE3.PcgIIanalysis(ppcgfd.getFloatArray(0, pcgdSize), pcgdSize, analHPAStructd));

              /*  try{
                    analHPAStructd.setI_start(Hpassys.PcgIanalysis(ppcgfd.getFloatArray(0, pcgdSize), pcgdSize, analHPAStructd));
                    analHPAStructd.setII_start(Hpassys.PcgIIanalysis(ppcgfd.getFloatArray(0, pcgdSize), pcgdSize, analHPAStructd));
                }catch (Exception e){
                    freePointer(ppcgd);
                    freePointer(ppcgfd);
                    ToastUtils.showToastLong(DrawsActivity.this,"低舒张压心音数据不规范，请重新检测setI_start or setII_start is null");
                    hyppcgd = null;
                    break;
                }*/
                freePointer(ppcgd);
                freePointer(ppcgfd);

//                if(analHPAStructd.Istarts[0] <= 0 || analHPAStructd.IIstarts[0] <= 0){
//                    ToastUtils.showToastLong(DrawsActivity.this,"低舒张压心音数据不规范，请重新检测");
//                    hyppcgd = null;
//                    break;
//                }

                //脉搏低舒张
                short apgdSize = (short) hypapgd.length;

//                for(int i = 0; i < apgdSize; i++){
//                    if(hypapgd[i] < 0 || hypapgd[i] < 10){
//                        ToastUtils.showToastLong(DrawsActivity.this,"低舒张压脉搏数据不规范，请重新检测");
//                        hypapgd = null;
//                        return;
//                    }
//                }

                float[] apgdf = new float[apgdSize];
                float[] bsnlapgdf = new float[apgdSize];
                for (int i = 0; i < apgdSize; i++){
                    apgdf[i] = hypapgd[i];
                    bsnlapgdf[i] = hypapgd[i];
                }
                Pointer filterLowdPointer = new Memory(apgdSize * 4);
                filterLowdPointer.write(0, bsnlapgdf, 0, apgdSize);

                short ssd;
                short ddd;
//                float max_numapgd = CLibrary.INSTANCE2.DetectMaxValue(apgdf, (short) 0, apgdSize);
//                if (max_numapgd < 0.5f)
//                {
//                    freePointer(filterLowdPointer);
//                    ToastUtils.showToastLong(DrawsActivity.this,"低舒张压脉搏数据不规范，请重新检测");
//                    hypapgd = null;
//                    break;
//                }
                float[] japgdf = new float[apgdSize];
                for (int i = 0; i < apgdSize; i++){
                    japgdf[i] = hypapgd[i];
                }
                Pointer japgdfP = new Memory(apgdSize * 4);
                japgdfP.write(0, japgdf, 0, apgdSize);
                CLibrary.INSTANCE2.FilterLow(apgdf, japgdfP, apgdSize, 0.008f);
                Pointer papgdf = new Memory(apgdSize * 4);
                CLibrary.INSTANCE2.Filter(japgdfP.getFloatArray(0, apgdSize), papgdf, apgdSize, (short) 25);
                CLibrary.INSTANCE2.FilterMibun(papgdf.getFloatArray(0, apgdSize), japgdfP, apgdSize, (short) 25);

                papgdf.write(0, apgdf,0, apgdSize);
                CLibrary.INSTANCE2.FilterMibun(japgdfP.getFloatArray(0, apgdSize), papgdf, apgdSize, (short) 25);

              /*  try{
                    for (int i = 0; i <= analHPAStructd.getRtopNum(); i++)
                    {
                        ddd = 0;
                        ssd = 0;
                        ddd = analHPAStructd.getRtops()[i];
                        ssd = (short) (analHPAStructd.getRtops()[i] + (short) 250);
//                    analHPAStructd.USpnts[i] = CLibrary.INSTANCE3.DetectUSpoint(papgdf.getFloatArray(0, apgdSize), ddd, ssd, (short) i, apgdSize, analHPAStructd);
                        analHPAStructd.USpnts[i] = Hpassys.DetectUSpoint(papgdf.getFloatArray(0, apgdSize), ddd, ssd, (short) i, apgdSize, analHPAStructd);
                    }
                }catch(Exception e){
                    freePointer(japgdfP);
                    freePointer(papgdf);
                    freePointer(filterLowdPointer);
                    ToastUtils.showToastLong(DrawsActivity.this,"低舒张压脉搏数据不规范，请重新检测USpnts not found");
                    hypapgd = null;
                    break;
                }*/

                analHPAStructd.setUS_point(analHPAStructd.getUSpnts()[analHPAStructd.getRpos()]);

                for (int i = 0; i <= analHPAStructd.getRtopNum(); i++)
                {
                    ddd = 0;
                    ssd = 0;
                    ddd = (short) (analHPAStructd.getIIstarts()[i] + (short) 5);
                    ssd = (short) (analHPAStructd.getIIstarts()[i] + (short) 160);
                    analHPAStructd.DNpnts[i] = CLibrary.INSTANCE3.DN_point(papgdf.getFloatArray(0, apgdSize), ddd, ssd, (short) i, apgdSize, analHPAStructd);
                    analHPAStructd.DNpnts[i] = (short) (analHPAStructd.DNpnts[i] - (short) 3);
                }
                analHPAStructd.setDN_point(analHPAStructd.getDNpnts()[analHPAStructd.getRpos()]);

                for (int i = 0; i <= analHPAStructd.getRtopNum(); i++)
                {
                    ddd = 0;
                    ssd = 0;
                    ddd = analHPAStructd.getUSpnts()[i];
                    ssd = analHPAStructd.getDNpnts()[i];
                    analHPAStructd.UTpnts[i] = (short)CLibrary.INSTANCE2.DetectMax(filterLowdPointer.getFloatArray(0, apgdSize), ddd, ssd); //bsnlapgf = 脉搏原始波形apg[];
                }
                analHPAStructd.setUT_point(analHPAStructd.getUTpnts()[analHPAStructd.getRpos()]);

//                for (int i = 0; i < apgdSize; i++)
//                {
//                    japgdf[i] = hypapgd[i];
//                }
//                CLibrary.INSTANCE2.FilterLow(japgdf, filterLowdPointer, apgdSize, 0.008f);

                freePointer(japgdfP);
                freePointer(papgdf);
                freePointer(filterLowdPointer);

//                if(analHPAStructd.getUSpnts()[0] <= 0 || analHPAStructd.getDNpnts()[0] <= 0 || analHPAStructd.getUTpnts()[0] <= 0){
//                    ToastUtils.showToastLong(DrawsActivity.this,"低舒张压脉搏数据不规范，请重新检测");
//                    hypapgd = null;
//                    break;
//                }

//                for (int i = 0; i < analHPAStructd.getUSpnts().length - 1; i++){
//                    if(analHPAStructd.getUSpnts()[i + 1] != 0){
//                        if(analHPAStructd.getUSpnts()[i] > analHPAStructd.getUSpnts()[i + 1]){
//                            ToastUtils.showToastLong(DrawsActivity.this,"低舒张压脉搏数据不规范，请重新检测");
//                            hypapgd = null;
//                            return;
//                        }
//                    }
//                }
//
//                for (int i = 0; i < analHPAStructd.getUTpnts().length - 1; i++){
//                    if(analHPAStructd.getUTpnts()[i + 1] != 0){
//                        if(analHPAStructd.getUTpnts()[i] > analHPAStructd.getUTpnts()[i + 1]){
//                            ToastUtils.showToastLong(DrawsActivity.this,"低舒张压脉搏数据不规范，请重新检测");
//                            hypapgd = null;
//                            return;
//                        }
//                    }
//                }

                break;

            case 2:
                ecgPl = ecgPointList.toArray(new Float[ecgPointList.size()]);
                pcgPl = pcgPointList.toArray(new Float[pcgPointList.size()]);
                apgL = heatPointList.toArray(new Float[heatPointList.size()]);

                short apgLSampNum = (short) ecgPl.length;
                float[]	ECGPL = new float[apgLSampNum];
                for (int e = 0; e < apgLSampNum; e++) {
                    ECGPL[e] = ecgPl[e];
                }

                apgLSampNum = (short) pcgPl.length;
                float[]	PCGPL = new float[apgLSampNum];
                for (int a = 0; a < apgLSampNum; a++) {
                    PCGPL[a] = pcgPl[a];
                }

                apgLSampNum = (short) apgL.length;
                float[]	APGL = new float[apgLSampNum];
                for (int b = 0; b < apgLSampNum; b++) {
                    APGL[b] = apgL[b];
                }

                short apgLi;
                short apgLdd =0;
                short apgLss=0;
                short apgLRTopNum;
                short[]	UT = new short[30];

//                Pointer jSnl = new Memory(apgLSampNum * 4);
//                Pointer kSnlP = new Memory(apgLSampNum * 4);
//                CLibrary.AnalHPA.ByReference analHPAStructsRQT = new CLibrary.AnalHPA.ByReference();
//                AnalHPA_Data.ByReference analHPADataStructRQT = new AnalHPA_Data.ByReference();
//                CLibrary.INSTANCE5.filter(ECGPL, jSnl, apgLSampNum, (short) 25);
//                CLibrary.INSTANCE5.filter_mibun(jSnl.getFloatArray(0, apgLSampNum), kSnlP, apgLSampNum, (short) 25);
//                CLibrary.INSTANCE5.filter_mibun(kSnlP.getFloatArray(0, apgLSampNum), jSnl, apgLSampNum, (short) 25);
////                CLibrary.INSTANCE3.RQT_Analysis(jSnl.getFloatArray(0, apgLSampNum), kSnlP.getFloatArray(0, apgLSampNum), apgLSampNum, analHPAStructsRQT, analHPADataStructRQT);
//                try{
//                    Hpassys.RQT_Analysis(jSnl.getFloatArray(0, apgLSampNum), kSnlP.getFloatArray(0, apgLSampNum), apgLSampNum, analHPAStructsRQT, analHPADataStructRQT);
//                }catch(Exception e){
//                    ToastUtils.showToastLong(DrawsActivity.this,"左颈动脉心电数据不规范，请重新检测RQT_Analysis");
//                    ecgPl = null;
//                    freePointer(jSnl);
//                    freePointer(kSnlP);
//                    break;
//                }
//                apgLRTopNum = analHPAStructsRQT.getRtopNum();
//                if(apgLRTopNum <= 2){
//                    ToastUtils.showToastLong(DrawsActivity.this,"左颈动脉心电数据不规范，请重新检测apgLRTopNum <= 2");
//                    ecgPl = null;
//                    freePointer(jSnl);
//                    freePointer(kSnlP);
//                    break;
//                }

                short pulseNumber;
                short chNumber;
                short sigCount;
                short[] apgLRR = new short[20];
                short[] apgLIso = new short[20];
                short[] apgLIIso = new short[20];
                short[] apgLtr = new short[20];
                short[] apgLtpoint = new short[20];

                for(apgLi=0; apgLi<20; apgLi++)
                {
                    apgLRR[apgLi] = 0;
                    apgLIso[apgLi] = 0;
                    apgLIIso[0] = 0;
                    apgLtr[apgLi] = 0;
                    apgLtpoint[apgLi] = 0;
                }

                Pointer jSnlAUL = new Memory(apgLSampNum * 4);
                Pointer kSnlPAUL = new Memory(apgLSampNum * 4);
                CLibrary.AnalHPA.ByReference analHPAStructsRQTAU = new CLibrary.AnalHPA.ByReference();
                AnalHPA_Data.ByReference analHPADataStructRQTAU = new AnalHPA_Data.ByReference();
                CLibrary.INSTANCE5.filter(ECGPL, jSnlAUL, apgLSampNum, (short) 25);
                CLibrary.INSTANCE5.filter_mibun(jSnlAUL.getFloatArray(0, apgLSampNum), kSnlPAUL, apgLSampNum, (short) 25);
                CLibrary.INSTANCE5.filter_mibun(kSnlPAUL.getFloatArray(0, apgLSampNum), jSnlAUL, apgLSampNum, (short) 25);
//                CLibrary.INSTANCE3.RQT_Analysis(jSnlAUL.getFloatArray(0, apgLSampNum), kSnlPAUL.getFloatArray(0, apgLSampNum), apgLSampNum, analHPAStructsRQTAU, analHPADataStructRQTAU);
               /* try{
                    Hpassys.RQT_Analysis(jSnlAUL.getFloatArray(0, apgLSampNum), kSnlPAUL.getFloatArray(0, apgLSampNum), apgLSampNum, analHPAStructsRQTAU, analHPADataStructRQTAU);
                }catch(Exception e){
                    ToastUtils.showToastLong(DrawsActivity.this,"左颈动脉心电数据不规范，请重新检测RQT_Analysis");
                    ecgPl = null;
//                    freePointer(jSnl);
//                    freePointer(kSnlP);
                    freePointer(jSnlAUL);
                    freePointer(kSnlPAUL);
                    break;
                }*/
                pulseNumber = analHPAStructsRQTAU.getRtopNum();

                sigCount = (short) (apgLSampNum - (short) 1);
                for(apgLi=0; apgLi<=pulseNumber; apgLi++)
                    apgLRR[apgLi] = analHPAStructsRQTAU.getRtops()[apgLi];

                /*if ( (pulseNumber < 2) || (pulseNumber > 18) ){
                    ToastUtils.showToastLong(DrawsActivity.this,"左颈动脉心电数据不规范，请重新检测pulseNumber<2 | >18");
                    ecgPl = null;
//                    freePointer(jSnl);
//                    freePointer(kSnlP);
                    freePointer(jSnlAUL);
                    freePointer(kSnlPAUL);
                    break;
                }*/
                Pointer apgLkSnlP = new Memory((apgLSampNum + 1) * 4);
                CLibrary.INSTANCE6.filter(APGL, apgLkSnlP, apgLSampNum, (short) 25);
                Pointer jSnlP = new Memory((apgLSampNum + 1) * 4);
                CLibrary.INSTANCE6.filter_mibun( apgLkSnlP.getFloatArray(0, apgLSampNum + 1), jSnlP, apgLSampNum, (short) 25 );
                CLibrary.INSTANCE6.filter_mibun( jSnlP.getFloatArray(0, apgLSampNum + 1), apgLkSnlP, apgLSampNum, (short) 25 );

                Pointer PCGIL = new Memory(30 * 2);
                Pointer PCGIIL = new Memory(30 * 2);
                Pointer RtopL = new Memory(30 * 2);
                Pointer TsL = new Memory(30 * 2);

                pulseNumber = CLibrary.INSTANCE9.AbI_IIsoPointGet (PCGPL, sigCount, apgLRR, pulseNumber, PCGIL, PCGIIL);
                pulseNumber = CLibrary.INSTANCE9.AbApgStTsGetonPcg (apgLkSnlP.getFloatArray(0, apgLSampNum + 1), sigCount, apgLRR, analHPAStructsRQTAU.getTends(), pulseNumber, RtopL, TsL);
                chNumber = CLibrary.INSTANCE9.AbOneGetbyAuto (RtopL.getShortArray(0, 30), pulseNumber);

                for (apgLi = 0; apgLi < pulseNumber; apgLi++) {
                    apgLdd = 0; apgLss = 0;
                    apgLdd = TsL.getShortArray(0, 30)[apgLi]; apgLss = RtopL.getShortArray(0, 30)[apgLi];
                    UT[apgLi] = CLibrary.INSTANCE2.DetectAbsMax(APGL, apgLss, apgLdd);
                }

//                if(TsL.getShortArray(0, 30)[0] <= 0 || RtopL.getShortArray(0, 30)[0] <= 0 || UT[0] <= 0){
////                    freePointer(jSnl);
////                    freePointer(kSnlP);
//                    freePointer(jSnlAUL);
//                    freePointer(kSnlPAUL);
//                    freePointer(apgLkSnlP);
//                    freePointer(jSnlP);
//                    freePointer(PCGIL);
//                    freePointer(PCGIIL);
//                    freePointer(RtopL);
//                    freePointer(TsL);
//                    ToastUtils.showToastLong(DrawsActivity.this,"左颈动脉数据不规范，请重新检测");
//                    ecgPl = null;
//                    pcgPl = null;
//                    apgL = null;
//                    break;
//                }

//                for(int i = 0; i < apgL.length; i++){
//                    if(apgL[i] < 0 || apgL[i] < 10){
//                        freePointer(jSnl);
//                        freePointer(kSnlP);
//                        freePointer(jSnlAUL);
//                        freePointer(kSnlPAUL);
//                        freePointer(apgLkSnlP);
//                        freePointer(jSnlP);
//                        freePointer(PCGIL);
//                        freePointer(PCGIIL);
//                        freePointer(RtopL);
//                        freePointer(TsL);
//                        ToastUtils.showToastLong(DrawsActivity.this,"左颈动脉脉搏数据不规范，请重新检测");
//                        apgL = null;
//                        return;
//                    }
//                }

//                freePointer(jSnl);
//                freePointer(kSnlP);
                freePointer(jSnlAUL);
                freePointer(kSnlPAUL);
                freePointer(apgLkSnlP);
                freePointer(jSnlP);
                freePointer(PCGIL);
                freePointer(PCGIIL);
                freePointer(RtopL);
                freePointer(TsL);

                break;
            case 3:
                if(mode == Constants.MEASURE_MODE_SYNC){
                    ecgPr = ecgPointList.toArray(new Float[ecgPointList.size()]); //0
                    apgR = heatPointList.toArray(new Float[heatPointList.size()]); //2
                    apgTh = tstPointList.toArray(new Float[tstPointList.size()]);  //3
                    pcgPr = pcgPl;
                    ecgTh = ecgPr;

                    short SYNCi;
                    short SYNCss;
                    short SYNCdd;
                    short SYNCrst1;
                    float SYNCmaxNum;
                    long SYNCrst;
                    float SYNCPTT1;

                    //获取股动脉心电波
                    short SYNCsampleNum = (short) ecgTh.length;
                    float[] kSnl = new float[SYNCsampleNum];

                    for (SYNCi = 0; SYNCi < SYNCsampleNum; SYNCi++) {
                        kSnl[SYNCi] = ecgTh[SYNCi];
                    }
//                    SYNCmaxNum = CLibrary.INSTANCE2.DetectMaxValue(kSnl, (short) 0, SYNCsampleNum);
//                    if (SYNCmaxNum < 0.2f) {
//                        ToastUtils.showToastLong(DrawsActivity.this,"右颈动脉心电数据不规范，请重新检测");
//                        ecgPr = null;
//                        ecgTh = null;
//                        break;
//                    }

                    Pointer SYNCjSnl = new Memory(SYNCsampleNum * 4);
                    CLibrary.INSTANCE2.FilterMibun(kSnl, SYNCjSnl, SYNCsampleNum, (short) 25);
                    Pointer SYNCkSnlP = new Memory(SYNCsampleNum * 4);
                    CLibrary.INSTANCE2.FilterMibun(SYNCjSnl.getFloatArray(0, SYNCsampleNum), SYNCkSnlP, SYNCsampleNum, (short) 25);

                    CLibrary.AnalHPA.ByReference analHPAStruct = new CLibrary.AnalHPA.ByReference();
//                    SYNCrst1 = CLibrary.INSTANCE3.RQT_Analysis(SYNCkSnlP.getFloatArray(0, SYNCsampleNum), SYNCjSnl.getFloatArray(0, SYNCsampleNum), SYNCsampleNum, analHPAStruct, analHPADataStruct);
                   /* try{
                        SYNCrst1 = Hpassys.RQT_Analysis(SYNCkSnlP.getFloatArray(0, SYNCsampleNum), SYNCjSnl.getFloatArray(0, SYNCsampleNum), SYNCsampleNum, analHPAStruct, analHPADataStruct);
                    }catch (Exception e){
                        freePointer(SYNCjSnl);
                        freePointer(SYNCkSnlP);
                        ToastUtils.showToastLong(DrawsActivity.this,"右颈动脉心电数据不规范，请重新检测RQT_Analysis");
                        ecgPr = null;
                        ecgTh = null;
                        break;
                    }*/
                    /*if (SYNCrst1 == 0) {
                        freePointer(SYNCjSnl);
                        freePointer(SYNCkSnlP);
                        ToastUtils.showToastLong(DrawsActivity.this,"右颈动脉心电数据不规范，请重新检测RTOPnum < 2");
                        ecgPr = null;
                        ecgTh = null;
                        break;
                    }*/
                    analHPAStruct.R_top = analHPAStruct.Rtops[analHPAStruct.Rpos];

                    //获取股动脉脉搏波
                    short SYNCApgsampleNuma = (short) apgTh.length;
//                    for(int i = 0; i < SYNCApgsampleNuma; i++){
//                        if(apgTh[i] < 0 || apgTh[i] < 10){
//                            freePointer(SYNCjSnl);
//                            freePointer(SYNCkSnlP);
//                            ToastUtils.showToastLong(DrawsActivity.this,"股动脉脉搏数据不规范，请重新检测");
//                            apgTh = null;
//                            return;
//                        }
//                    }

                    float[] kSnla = new float[SYNCApgsampleNuma];

                    for (SYNCi = 0; SYNCi < SYNCApgsampleNuma; SYNCi++) {
                        kSnla[SYNCi] = apgTh[SYNCi];
                    }
//                    SYNCmaxNum = CLibrary.INSTANCE2.DetectMaxValue(kSnla, (short) 0, SYNCApgsampleNuma);
//                    if (SYNCmaxNum < 0.2f) {
//                        freePointer(SYNCjSnl);
//                        freePointer(SYNCkSnlP);
 //                       ToastUtils.showToastLong(DrawsActivity.this,"股动脉脉搏数据不规范，请重新检测");
 //                       apgTh = null;
 //                       break;
 //                   }

                    Pointer jSnla = new Memory(SYNCApgsampleNuma * 4);
                    CLibrary.INSTANCE2.FilterMibun(kSnla, jSnla, SYNCApgsampleNuma, (short) 25);
                    Pointer kSnlaP = new Memory(SYNCApgsampleNuma * 4);
                    CLibrary.INSTANCE2.FilterMibun(jSnla.getFloatArray(0, SYNCApgsampleNuma), kSnlaP, SYNCApgsampleNuma, (short) 25);

                 /*   try{
                        for (SYNCi = 0; SYNCi <= analHPAStruct.RtopNum; SYNCi++) {
                            SYNCdd = 0; SYNCss = 0; SYNCdd = analHPAStruct.Rtops[SYNCi];
                            SYNCss = (short) (analHPAStruct.Rtops[SYNCi] + (short) 270);
//                        analHPAStruct.USpnts[SYNCi] = CLibrary.INSTANCE3.DetectUSpoint(kSnlaP.getFloatArray(0, SYNCApgsampleNuma), SYNCdd, SYNCss, SYNCi, SYNCApgsampleNuma, analHPAStruct);
                            analHPAStruct.USpnts[SYNCi] = Hpassys.DetectUSpoint(kSnlaP.getFloatArray(0, SYNCApgsampleNuma), SYNCdd, SYNCss, SYNCi, SYNCApgsampleNuma, analHPAStruct);
                        }
                    }catch(Exception e){
                        freePointer(SYNCjSnl);
                        freePointer(SYNCkSnlP);
                        freePointer(jSnla);
                        freePointer(kSnlaP);
                        ToastUtils.showToastLong(DrawsActivity.this,"股动脉脉搏数据不规范，请重新检测USpnts is null");
                        apgTh = null;
                        break;
                    }*/

                    analHPAStruct.US_point = analHPAStruct.USpnts[analHPAStruct.Rpos];
                    SYNCPTT1 = Math.abs((analHPAStruct.US_point - analHPAStruct.R_top) * 0.0012f);

                    freePointer(SYNCjSnl);
                    freePointer(SYNCkSnlP);
                    freePointer(jSnla);
                    freePointer(kSnlaP);

//                    if(analHPAStruct.getUSpnts()[0] <= 0){
//                        ToastUtils.showToastLong(DrawsActivity.this,"股动脉脉搏数据不规范，请重新检测");
//                        apgTh = null;
//                        break;
//                    }
//
//                    for (int i = 0; i < analHPAStruct.getUSpnts().length - 1; i++){
//                        if(analHPAStruct.getUSpnts()[i + 1] != 0){
//                            if(analHPAStruct.getUSpnts()[i] > analHPAStruct.getUSpnts()[i + 1]){
//                                ToastUtils.showToastLong(DrawsActivity.this,"股动脉脉搏数据不规范，请重新检测");
//                                apgTh = null;
//                                return;
//                            }
//                        }
//                    }
//
//                    if ( SYNCPTT1 <= 0.0f){
//                        ToastUtils.showToastLong(DrawsActivity.this,"股动脉脉搏数据不规范，请重新检测");
//                        apgTh = null;
//                        break;
//                    }
                }else{
                    ecgPr = ecgPointList.toArray(new Float[ecgPointList.size()]); //0
                    pcgPr = pcgPointList.toArray(new Float[pcgPointList.size()]); //1
                    apgR = heatPointList.toArray(new Float[heatPointList.size()]); //2
                }

                short apgRSampNum = (short) ecgPr.length;
                float[]	ECGPR = new float[apgRSampNum];
                for (int e = 0; e < apgRSampNum; e++) {
                    ECGPR[e] = ecgPr[e];
                }

                apgRSampNum = (short) pcgPr.length;
                float[]	PCGPR = new float[apgRSampNum];
                for (int a = 0; a < apgRSampNum; a++) {
                    PCGPR[a] = pcgPr[a];
                }

                apgRSampNum = (short) apgR.length;
                float[]	APGR = new float[apgRSampNum];
                for (int b = 0; b < apgRSampNum; b++) {
                    APGR[b] = apgR[b];
                }

                short apgRi;
                short apgRdd =0;
                short apgRss=0;
                short apgRRTopNum;
                short[]	apgRUT = new short[30];

//                Pointer apgRjSnl = new Memory(apgRSampNum * 4);
//                Pointer apgRkSnlP = new Memory(apgRSampNum * 4);
//                CLibrary.AnalHPA.ByReference apgRanalHPAStructsRQT = new CLibrary.AnalHPA.ByReference();
//                AnalHPA_Data.ByReference apgRanalHPADataStructRQT = new AnalHPA_Data.ByReference();
//                CLibrary.INSTANCE5.filter(ECGPR, apgRjSnl, apgRSampNum, (short) 25);
//                CLibrary.INSTANCE5.filter_mibun(apgRjSnl.getFloatArray(0, apgRSampNum), apgRkSnlP, (short) apgRSampNum, (short) 25);
//                CLibrary.INSTANCE5.filter_mibun(apgRkSnlP.getFloatArray(0, apgRSampNum), apgRjSnl, apgRSampNum, (short) 25);
////                CLibrary.INSTANCE3.RQT_Analysis(apgRjSnl.getFloatArray(0, apgRSampNum), apgRkSnlP.getFloatArray(0, apgRSampNum), apgRSampNum, apgRanalHPAStructsRQT, apgRanalHPADataStructRQT);
//                try{
//                    Hpassys.RQT_Analysis(apgRjSnl.getFloatArray(0, apgRSampNum), apgRkSnlP.getFloatArray(0, apgRSampNum), apgRSampNum, apgRanalHPAStructsRQT, apgRanalHPADataStructRQT);
//                }catch(Exception e){
//                    ToastUtils.showToastLong(DrawsActivity.this,"右颈动脉心电数据不规范，请重新检测");
//                    ecgPr = null;
//                    freePointer(apgRjSnl);
//                    freePointer(apgRkSnlP);
//                    break;
//                }
//                apgRRTopNum = apgRanalHPAStructsRQT.getRtopNum();
//
//                if(apgRRTopNum <= 2){
//                    ToastUtils.showToastLong(DrawsActivity.this,"右颈动脉心电数据不规范，请重新检测");
//                    ecgPr = null;
//                    freePointer(apgRjSnl);
//                    freePointer(apgRkSnlP);
//                    break;
//                }

                short apgRpulseNumber;
                short apgRchNumber;
                short apgRsigCount;
                short[] apgRRR = new short[20];
                short[] apgRIso = new short[20];
                short[] apgRIIso = new short[20];
                short[] apgRtr = new short[20];
                short[] apgRtpoint = new short[20];

                for(apgRi=0; apgRi<20; apgRi++)
                {
                    apgRRR[apgRi] = 0;
                    apgRIso[apgRi] = 0;
                    apgRIIso[0] = 0;
                    apgRtr[apgRi] = 0;
                    apgRtpoint[apgRi] = 0;
                }

                Pointer jSnlAUR = new Memory(apgRSampNum * 4);
                Pointer kSnlPAUR = new Memory(apgRSampNum * 4);
                CLibrary.AnalHPA.ByReference apgRanalHPAStructsRQTAU = new CLibrary.AnalHPA.ByReference();
                AnalHPA_Data.ByReference apgRanalHPADataStructRQTAU = new AnalHPA_Data.ByReference();
                CLibrary.INSTANCE5.filter(ECGPR, jSnlAUR, apgRSampNum, (short) 25);
                CLibrary.INSTANCE5.filter_mibun(jSnlAUR.getFloatArray(0, apgRSampNum), kSnlPAUR, apgRSampNum, (short) 25);
                CLibrary.INSTANCE5.filter_mibun(kSnlPAUR.getFloatArray(0, apgRSampNum), jSnlAUR, apgRSampNum, (short) 25);
//                CLibrary.INSTANCE3.RQT_Analysis(jSnlAUR.getFloatArray(0, apgRSampNum), kSnlPAUR.getFloatArray(0, apgRSampNum), apgRSampNum, apgRanalHPAStructsRQTAU, apgRanalHPADataStructRQTAU);
               /* try{
                    Hpassys.RQT_Analysis(jSnlAUR.getFloatArray(0, apgRSampNum), kSnlPAUR.getFloatArray(0, apgRSampNum), apgRSampNum, apgRanalHPAStructsRQTAU, apgRanalHPADataStructRQTAU);
                }catch(Exception e){
                    ToastUtils.showToastLong(DrawsActivity.this,"右颈动脉心电数据不规范，请重新检测RQT_Analysis");
                    ecgPr = null;
//                    freePointer(apgRjSnl);
//                    freePointer(apgRkSnlP);
                    freePointer(jSnlAUR);
                    freePointer(kSnlPAUR);
                    break;
                }*/

                apgRpulseNumber = apgRanalHPAStructsRQTAU.getRtopNum();

                apgRsigCount = (short) (apgRSampNum - (short) 1);
                for(apgRi=0; apgRi<=apgRpulseNumber; apgRi++)
                    apgRRR[apgRi] = apgRanalHPAStructsRQTAU.getRtops()[apgRi];

              /*  if ( (apgRpulseNumber < 2) || (apgRpulseNumber > 18) ){
                    ToastUtils.showToastLong(DrawsActivity.this,"右颈动脉心电数据不规范，请重新检测pulseNumber < 2");
                    ecgPr = null;
//                    freePointer(apgRjSnl);
//                    freePointer(apgRkSnlP);
                    freePointer(jSnlAUR);
                    freePointer(kSnlPAUR);
                    break;
                }*/
                Pointer apgRRkSnlP = new Memory((apgRSampNum + 1) * 4);
                CLibrary.INSTANCE6.filter(APGR, apgRRkSnlP, apgRSampNum, (short) 25);
                Pointer apgRRjSnlP = new Memory((apgRSampNum + 1) * 4);
                CLibrary.INSTANCE6.filter_mibun( apgRRkSnlP.getFloatArray(0, apgRSampNum + 1), apgRRjSnlP, apgRSampNum, (short) 25 );
                CLibrary.INSTANCE6.filter_mibun( apgRRjSnlP.getFloatArray(0, apgRSampNum + 1), apgRRkSnlP, apgRSampNum, (short) 25 );

                Pointer PCGIR = new Memory(30 * 2);
                Pointer PCGIIR = new Memory(30 * 2);
                Pointer RtopR = new Memory(30 * 2);
                Pointer TsR = new Memory(30 * 2);

                if(mode == Constants.MEASURE_MODE_ASYNC){ //异步模式右 调用一次，同步模式右 不调用
                    apgRpulseNumber = CLibrary.INSTANCE9.AbI_IIsoPointGet (PCGPR, apgRsigCount, apgRRR, apgRpulseNumber, PCGIR, PCGIIR);
                }

                apgRpulseNumber = CLibrary.INSTANCE9.AbApgStTsGetonPcg (apgRRkSnlP.getFloatArray(0, apgRSampNum + 1), apgRsigCount, apgRRR, apgRanalHPAStructsRQTAU.getTends(), apgRpulseNumber, RtopR, TsR);
                apgRchNumber = CLibrary.INSTANCE9.AbOneGetbyAuto (RtopR.getShortArray(0, 30), apgRpulseNumber);

                for (apgRi = 0; apgRi < apgRpulseNumber; apgRi++) {
                    apgRdd = 0; apgRss = 0;
                    apgRdd = TsR.getShortArray(0, 30)[apgRi]; apgRss = RtopR.getShortArray(0, 30)[apgRi];
                    apgRUT[apgRi] = CLibrary.INSTANCE2.DetectAbsMax(APGR, apgRss, apgRdd);
                }

//                if(TsR.getShortArray(0, 30)[0] <= 0 || RtopR.getShortArray(0, 30)[0] <= 0 || apgRUT[0] <= 0){
//                    freePointer(apgRjSnl);
//                    freePointer(apgRkSnlP);
//                    freePointer(jSnlAUR);
//                    freePointer(kSnlPAUR);
//                    freePointer(apgRRkSnlP);
//                    freePointer(apgRRjSnlP);
//                    freePointer(PCGIR);
//                    freePointer(PCGIIR);
//                    freePointer(RtopR);
//                    freePointer(TsR);
//                    ToastUtils.showToastLong(DrawsActivity.this,"右颈动脉数据不规范，请重新检测");
//                    ecgPr = null;
 //                   pcgPr = null;
   //                 apgR = null;
    //                break;
    //            }

//                for(int i = 0; i < apgR.length; i++){
//                    if(apgR[i] < 0 || apgR[i] < 10){
//                        freePointer(apgRjSnl);
//                        freePointer(apgRkSnlP);
//                        freePointer(jSnlAUR);
//                        freePointer(kSnlPAUR);
//                        freePointer(apgRRkSnlP);
//                        freePointer(apgRRjSnlP);
//                        freePointer(PCGIR);
//                        freePointer(PCGIIR);
//                        freePointer(RtopR);
//                        freePointer(TsR);
//                        ToastUtils.showToastLong(DrawsActivity.this,"右颈动脉脉搏数据不规范，请重新检测");
//                        apgR = null;
//                        return;
//                    }
//                }

//                freePointer(apgRjSnl);
//                freePointer(apgRkSnlP);
                freePointer(jSnlAUR);
                freePointer(kSnlPAUR);
                freePointer(apgRRkSnlP);
                freePointer(apgRRjSnlP);
                freePointer(PCGIR);
                freePointer(PCGIIR);
                freePointer(RtopR);
                freePointer(TsR);

                break;
            case 4:
                if(mode == Constants.MEASURE_MODE_SYNC){
                    break;
                }
//                ecgTh = ecgPointList.toArray(new Float[ecgPointList.size()]);
//                apgTh = heatPointList.toArray(new Float[heatPointList.size()]);

                ecgTh = ecgPointList.toArray(new Float[ecgPointList.size()]);
//                apgTh = tstPointList.toArray(new Float[tstPointList.size()]); //袖带的
                apgTh = heatPointList.toArray(new Float[heatPointList.size()]); //笔的

                short PTVi;
                short PTVss;
                short PTVdd;
                short PTVrst1;
                float PTVmaxNum;
                long PTVrst;
                float PTVPTT1;

                //获取股动脉心电波
                short PTVsampleNum = (short) ecgTh.length;
                float[] kSnl = new float[PTVsampleNum];

                for (PTVi = 0; PTVi < PTVsampleNum; PTVi++) {
                    kSnl[PTVi] = ecgTh[PTVi];
                }
//                PTVmaxNum = CLibrary.INSTANCE2.DetectMaxValue(kSnl, (short) 0, PTVsampleNum);
//                if (PTVmaxNum < 0.2f) {
//                    ToastUtils.showToastLong(DrawsActivity.this,"股动脉心电数据不规范，请重新检测");
//                    ecgTh = null;
//                    break;
//                }

                Pointer PTVjSnl = new Memory(PTVsampleNum * 4);
                CLibrary.INSTANCE2.FilterMibun(kSnl, PTVjSnl, PTVsampleNum, (short) 25);
                Pointer PTVkSnlP = new Memory(PTVsampleNum * 4);
                CLibrary.INSTANCE2.FilterMibun(PTVjSnl.getFloatArray(0, PTVsampleNum), PTVkSnlP, PTVsampleNum, (short) 25);

                CLibrary.AnalHPA.ByReference analHPAStruct = new CLibrary.AnalHPA.ByReference();
//                PTVrst1 = CLibrary.INSTANCE3.RQT_Analysis(PTVkSnlP.getFloatArray(0, PTVsampleNum), PTVjSnl.getFloatArray(0, PTVsampleNum), PTVsampleNum, analHPAStruct, analHPADataStruct);
             /*   try{
                    PTVrst1 = Hpassys.RQT_Analysis(PTVkSnlP.getFloatArray(0, PTVsampleNum), PTVjSnl.getFloatArray(0, PTVsampleNum), PTVsampleNum, analHPAStruct, analHPADataStruct);
                }catch(Exception e){
                    freePointer(PTVjSnl);
                    freePointer(PTVkSnlP);
                    ToastUtils.showToastLong(DrawsActivity.this,"股动脉心电数据不规范，请重新检测RQT_Analysis");
                    ecgTh = null;
                    break;
                }*/

                /*if (PTVrst1 == 0) {
                    freePointer(PTVjSnl);
                    freePointer(PTVkSnlP);
                    ToastUtils.showToastLong(DrawsActivity.this,"股动脉心电数据不规范，请重新检测RTOPnum < 2");
                    ecgTh = null;
                    break;
                }*/
                analHPAStruct.R_top = analHPAStruct.Rtops[analHPAStruct.Rpos];

                //获取股动脉脉搏波
                short PTVApgsampleNuma = (short) apgTh.length;
//                for(int i = 0; i < PTVApgsampleNuma; i++){
//                    if(apgTh[i] < 0 || apgTh[i] < 10){
//                        freePointer(PTVjSnl);
//                        freePointer(PTVkSnlP);
//                        ToastUtils.showToastLong(DrawsActivity.this,"股动脉脉搏数据不规范，请重新检测");
//                        apgTh = null;
//                        return;
//                    }
//                }

                float[] kSnla = new float[PTVApgsampleNuma];

                for (PTVi = 0; PTVi < PTVApgsampleNuma; PTVi++) {
                    kSnla[PTVi] = apgTh[PTVi];
                }
//                PTVmaxNum = CLibrary.INSTANCE2.DetectMaxValue(kSnla, (short) 0, PTVApgsampleNuma);
//                if (PTVmaxNum < 0.2f) {
//                    freePointer(PTVjSnl);
//                    freePointer(PTVkSnlP);
//                    ToastUtils.showToastLong(DrawsActivity.this,"股动脉脉搏数据不规范，请重新检测");
//                    apgTh = null;
//                    break;
//                }

                Pointer jSnla = new Memory(PTVApgsampleNuma * 4);
                CLibrary.INSTANCE2.FilterMibun(kSnla, jSnla, PTVApgsampleNuma, (short) 25);
                Pointer kSnlaP = new Memory(PTVApgsampleNuma * 4);
                CLibrary.INSTANCE2.FilterMibun(jSnla.getFloatArray(0, PTVApgsampleNuma), kSnlaP, PTVApgsampleNuma, (short) 25);

                /*try{
                    for (PTVi = 0; PTVi <= analHPAStruct.RtopNum; PTVi++) {
                        PTVdd = 0; PTVss = 0; PTVdd = analHPAStruct.Rtops[PTVi];
                        PTVss = (short) (analHPAStruct.Rtops[PTVi] + (short) 270);
//                    analHPAStruct.USpnts[PTVi] = CLibrary.INSTANCE3.DetectUSpoint(kSnlaP.getFloatArray(0, PTVApgsampleNuma), PTVdd, PTVss, PTVi, PTVApgsampleNuma, analHPAStruct);
                        analHPAStruct.USpnts[PTVi] = Hpassys.DetectUSpoint(kSnlaP.getFloatArray(0, PTVApgsampleNuma), PTVdd, PTVss, PTVi, PTVApgsampleNuma, analHPAStruct);
                    }
                }catch(Exception e){
                    freePointer(PTVjSnl);
                    freePointer(PTVkSnlP);
                    freePointer(jSnla);
                    freePointer(kSnlaP);
                    ToastUtils.showToastLong(DrawsActivity.this,"股动脉脉搏数据不规范，请重新检测USpnts is null");
                    apgTh = null;
                    break;
                }*/

                analHPAStruct.US_point = analHPAStruct.USpnts[analHPAStruct.Rpos];
                PTVPTT1 = Math.abs((analHPAStruct.US_point - analHPAStruct.R_top) * 0.0012f);

                freePointer(PTVjSnl);
                freePointer(PTVkSnlP);
                freePointer(jSnla);
                freePointer(kSnlaP);

//                if(analHPAStruct.getUSpnts()[0] <= 0){
//                    ToastUtils.showToastLong(DrawsActivity.this,"股动脉脉搏数据不规范，请重新检测");
//                    apgTh = null;
//                    break;
//                }
//
//                for (int i = 0; i < analHPAStruct.getUSpnts().length - 1; i++){
//                    if(analHPAStruct.getUSpnts()[i + 1] != 0){
//                        if(analHPAStruct.getUSpnts()[i] > analHPAStruct.getUSpnts()[i + 1]){
//                            ToastUtils.showToastLong(DrawsActivity.this,"股动脉脉搏数据不规范，请重新检测");
//                            apgTh = null;
//                            return;
//                        }
//                    }
//                }
//
//                if ( PTVPTT1 <= 0.0f){
//                    ToastUtils.showToastLong(DrawsActivity.this,"股动脉脉搏数据不规范，请重新检测");
//                    apgTh = null;
//                    break;
//                }

                break;
        }

    }

    /**
     * 清空数据
     */
    private void clearList() {
        if(ecgPointList == null || pcgPointList == null || heatPointList == null || tstPointList == null){
            return;
        }
        ecgPointList.clear();
        pcgPointList.clear();
        heatPointList.clear();
        tstPointList.clear();

        ecgPointListView.clear();
        pcgPointListView.clear();
        heatPointListView.clear();
        tstPointListView.clear();

        curPos = 0;

        firstPathView.invalidate();
        secondPathView.invalidate();
        thirdPathView.invalidate();
    }

    /**
     * 数据预处理
     * 不断将数据添加到缓存中
     *
     * @param serialBean
     */
    private void preProcessdata(SerialBean serialBean) {

        bufferf = new float[4][serialBean.getEcgdata().length];
        bufferFilterf = new float[4][];
        bufferFilterf[0] = new float[serialBean.getEcgdata().length];
        bufferFilterf[1] = new float[serialBean.getEcgdata().length];
        bufferFilterf[2] = new float[serialBean.getEcgdata().length];
        bufferFilterf[3] = new float[serialBean.getEcgdata().length];

        bufferf[0] = serialBean.getEcgdata();

        bufferf[1] = serialBean.getPcgdata();

        bufferf[2] = serialBean.getHeadata();

        bufferf[3] = serialBean.getTstdata();



        //幅值判断*******
//        for (int y = 0; y < (bufferf[0].length ); y++){
//            if (pointerx>=800)
//                    pointerx = 0;
//
//            if ((bufferf[0][y] > 1027000 && bufferf[0][y] < 1028600) || (bufferf[0][y] > 1029000 && bufferf[0][y] < 1030230)){
//                temp[pointerx]= 0;
//
//            }
//            else {temp[pointerx] = 1;}
//
//            pointerx = pointerx + 1;
//        }
//        if (pointerx==800) {sumtemp=0;
//            for (int yy = 0; yy < 800; yy++) {
//                sumtemp = sumtemp + temp[yy];
//            }
//            Log.e("huang","yuzhi"+ sumtemp);
//        }
        //幅值判断*******
        //微分判断*******
//        for (int y = 0; y < (bufferf[0].length - 1); y++){
//            if (pointerx>=832)
//                pointerx = 0;
//             float error;
//             error = bufferf[0][y+1] - bufferf[0][y];
//            Log.e("huang","error"+ error);
//            if ((bufferf[0][y+1] - bufferf[0][y])  > (- 3) &&( bufferf[0][y+1] -bufferf[0][y] )< 3) {
//                tempdiff[pointerx]= 0;
//
//            }
//            else {tempdiff[pointerx] = 1;
//            }
//            tempdiff2[pointerx] = bufferf[0][y+1]-bufferf[0][y];
//
//            pointerx = pointerx + 1;
//            if (pointerx>=832) {sumtempdiff=0;
//                for (int yy = 0; yy < 832; yy++) {
//                    sumtempdiff = sumtempdiff + tempdiff[yy];
//
//                }
//                Log.e("huang","yuzhi"+ sumtempdiff);
//
//            }//Log.e("huang","tempdiff2"+  tempdiff2[pointerx-1]);
//        }

       // Arrays.sort(tempdiff2);
        //FFT 判断

        for (int y = 0; y < (bufferf[0].length ); y++){
            temp[pointerx]=bufferf[0][y];
            pointerx = pointerx + 1;
            if (pointerx>=1024){
                for (int yy=0;yy<1024;yy++) {
                     tempmax=Math.max(temp[y], tempmax);
                     tempmin=Math.min(temp[y], tempmin);
                }
                for (int yy=0;yy<1024;yy++) {
                    real[yy] = temp[yy]/(tempmax-tempmin);
                    imaginary[yy] = 0;
                }
                fft.fft(real,imaginary);
                for (int yy=0;yy<1024;yy++) {
                    fftmod[yy]=Math.sqrt(real[yy]*real[yy]+imaginary[yy]*imaginary[yy]);
                }
                Log.e("huang","fft  "+ fftmod[1] +"  "+ fftmod[2]+"  "+ fftmod[3] + "  "+ fftmod[4] + "  "+ fftmod[5] + "  "+ fftmod[6] + "  "+ fftmod[7] + "  "+ fftmod[8] + "  "+ fftmod[9] + "  "+ fftmod[10] + "  "+ fftmod[11] + "  "+ fftmod[12] + "  "+ fftmod[13] + "  "+ fftmod[14] + "  "+ fftmod[15] + "  "+ fftmod[16] + "  "+ fftmod[17] + "  "+ fftmod[18] + "  "+ fftmod[19] + "  "+ fftmod[20] + "  "+ fftmod[21] + "  "+ fftmod[22] + "  "+ fftmod[23] + "  "+ fftmod[24] + "  "+ fftmod[25] + "  "+ fftmod[26] + "  "+ fftmod[27] + "  "+ fftmod[28] + "  "+ fftmod[29] + "  "+ fftmod[30] + "  "+ fftmod[31] + "  "+ fftmod[32] + "  "+ fftmod[33] + "  "+ fftmod[34] + "  "+ fftmod[35] + "  "+ fftmod[36] + "  "+ fftmod[37] + "  "+ fftmod[38] + "  "+ fftmod[39] + "  "+ fftmod[40] + "  "+ fftmod[41] + "  "+ fftmod[42] + "  "+ fftmod[43] + "  "+ fftmod[44] + "  "+ fftmod[45] + "  "+ fftmod[46] + "  "+ fftmod[47] + "  "+ fftmod[48] + "  "+ fftmod[49] + "  "+ fftmod[50] + "  "+ fftmod[51]);
                pointerx = 0;
                filtercountsin1 =0;
                filtercountfang =0;
                filtercountsanjiao =0;
                }

        }
        //fangbo
        if (fftmod[1]>0.03 && fftmod[1]<0.115)
            filtercountfang=filtercountfang+1;
        if (fftmod[2]>0.425 && fftmod[2]<0.495)
            filtercountfang=filtercountfang+1;
        if (fftmod[3]>0.115 && fftmod[3]<0.22)
            filtercountfang=filtercountfang+1;
        if (fftmod[4]>0.14 && fftmod[4]<0.158)
            filtercountfang=filtercountfang+1;
        //sin 1Hz
        if (fftmod[1]>0.025 && fftmod[1]<0.07)
            filtercountsin1=filtercountsin1+1;
        if (fftmod[2]>0.33 && fftmod[2]<0.41)
            filtercountsin1=filtercountsin1+1;
        if (fftmod[3]>0.013 && fftmod[3]<0.048)
            filtercountsin1=filtercountsin1+1;
        if (fftmod[4]>0.08 && fftmod[4]<0.14)
            filtercountsin1=filtercountsin1+1;
        //sanjiao
        if (fftmod[1]>0.035 && fftmod[1]<0.11)
            filtercountsanjiao=filtercountsanjiao+1;
        if (fftmod[2]>0.12 && fftmod[2]<0.22)
            filtercountsanjiao=filtercountsanjiao+1;
        if (fftmod[3]>0.09 && fftmod[3]<0.014)
            filtercountsanjiao=filtercountsanjiao+1;
        if (fftmod[4]>0.063 && fftmod[4]<0.122)
            filtercountsanjiao=filtercountsanjiao+1;
            Log.e("huang","filtercount san  "+ filtercountsanjiao + "   sin   " +filtercountsin1+"   fang   "+filtercountfang);
        //FFT 判断
//        for ( int yy=0;yy<833;yy++){sumtempdiff2b=0;
////            if (yy>416)
////                sumtempdiff2a = sumtempdiff2a + tempdiff2[yy];
////            else
//                sumtempdiff2b = sumtempdiff2b + tempdiff2[yy];
//        }
////        Log.e("huang","halfa    "+ sumtempdiff2a + "    halfb    " + sumtempdiff2b);
//        Log.e("huang", "    halfb    " + sumtempdiff2b);
//       // Log.e("huang","error2a"+  tempdiff2[0]);
//        //Log.e("huang","error2b"+  tempdiff2[799]);
//        //微分判断*******

        for (int y = 0; y < bufferf[0].length; y++){
//            Log.e("huang","心电"+ bufferf[0][y]);

            if (filtercountfang >=4 || filtercountsanjiao >=4 || filtercountsin1 >=4){
                bufferFilterf[0][y] = bufferf[0][y];//xingjian
                yadj_set = 4700;
                }

            else{
                yadj_set = 3000;
                bufferFilterf[0][y] = SerialBeanFilterEcg.serialBeanFilter(bufferf[0][y]);//xingjian
               // Log.e("huang","LLfilterL"+  tempdiff2[799]);
                }

        }

        for (int y = 0; y < bufferf[1].length; y++){
            bufferFilterf[1][y] = SerialBeanFilterPcg.serialBeanFilter(bufferf[1][y]);
        }

        for (int y = 0; y < bufferf[2].length; y++){
            bufferFilterf[2][y] = SerialBeanFilterHead.serialBeanFilter(bufferf[2][y]); //笔滤波器
        }

        for (int y = 0; y < bufferf[3].length; y++){
            bufferFilterf[3][y] = SerialBeanFilterTst.serialBeanFilter(bufferf[3][y]); //xiudai滤波器
        }

//        deque.offer(bufferf);
        dequeFilter.offer(bufferFilterf);
    }


    Timer timer;
    /**
     * 每隔30ms去画25个点数据
     */
    private synchronized void drawViewByTimer() {
        timer = new Timer();

        task = new TimerTask() {
            @Override
            public void run() {
                if (bufferf != null && bufferf[0].length > 30) {
                    if (curPos == 5) {
                        curPos = 0;
                    }
                    if (curPos == 0) {
                        try{
//                            buffer = deque.take();
                            bufferFilter = dequeFilter.take();
                        }catch(Exception e){
                            Log.e("获取队列数据异常", e.getMessage());
                            System.out.println("获取队列数据异常");
                        }
                    }
                    drawViewByData();
                    if(isStart) {
                        curPos++;
                    }
                }
            }
        };
        timer.scheduleAtFixedRate(task, 0, 35);
    }

    private void drawViewByData() {
        //1、准备数据
        prepareData();

        //2、切换到主线程画图
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                setEachViewData(); //调用setData画图
            }
        });

    }


    /**
     * 30ms 更新25个数据
     * buffer是缓存数组，通过回调不断往buffer里面写数据，同时定时器不断从buffer里面读数据来画图
     */
    private synchronized void prepareData() {
        if(!isStart){
            curPos = 0;
            return;
        }

//        for (int i = 0; i < DRAW_COUNT; i++) {
//            if (ecgPointList.size() < 5000) { //没有超过5000个
//                ecgPointList.add(bufferFilter[0][i + curPos * DRAW_COUNT]);
//                pcgPointList.add(bufferFilter[1][i + curPos * DRAW_COUNT]);
//                heatPointList.add(bufferFilter[2][i + curPos * DRAW_COUNT]);
//                tstPointList.add(bufferFilter[3][i + curPos * DRAW_COUNT]);
//
//                ecgPointListView.add(bufferFilter[0][i + curPos * DRAW_COUNT]); //5000
//                pcgPointListView.add(bufferFilter[1][i + curPos * DRAW_COUNT]); //5000
//                heatPointListView.add(bufferFilter[2][i + curPos * DRAW_COUNT]); //5000
//                tstPointListView.add(bufferFilter[3][i + curPos * DRAW_COUNT]); //5000
//            } else { //超过5000个,开始执行替换操作
//
//                updateViewData(ecgPointList, bufferFilter[0][i + curPos * DRAW_COUNT]);
//                updateViewData(pcgPointList, bufferFilter[1][i + curPos * DRAW_COUNT]);
//                updateViewData(heatPointList, bufferFilter[2][i + curPos * DRAW_COUNT]);
//                updateViewData(tstPointList, bufferFilter[3][i + curPos * DRAW_COUNT]);
//
//                updateViewData(ecgPointListView, bufferFilter[0][i + curPos * DRAW_COUNT]); //5000
//                updateViewData(pcgPointListView, bufferFilter[1][i + curPos * DRAW_COUNT]); //5000
//                updateViewData(heatPointListView, bufferFilter[2][i + curPos * DRAW_COUNT]); //5000
//                updateViewData(tstPointListView, bufferFilter[3][i + curPos * DRAW_COUNT]); //5000
//
//            }
//        }

        //添加画图用的list集合

        for (int i = 0; i < DRAW_COUNT_View; i++) { //7200
            if (ecgPointListView.size() < 7200) { //没有超过1250个 //7200
                ecgPointListView.add(bufferFilter[0][i + curPos * DRAW_COUNT_View]); //7200
                pcgPointListView.add(bufferFilter[1][i + curPos * DRAW_COUNT_View]); //7200
                heatPointListView.add(bufferFilter[2][i + curPos * DRAW_COUNT_View]); //7200
                tstPointListView.add(bufferFilter[3][i + curPos * DRAW_COUNT_View]); //7200
            } else { //超过1250个,开始执行替换操作
                updateViewData(ecgPointListView, bufferFilter[0][i + curPos * DRAW_COUNT_View]); //7200
                updateViewData(pcgPointListView, bufferFilter[1][i + curPos * DRAW_COUNT_View]); //7200
                updateViewData(heatPointListView, bufferFilter[2][i + curPos * DRAW_COUNT_View]); //7200
                updateViewData(tstPointListView, bufferFilter[3][i + curPos * DRAW_COUNT_View]); //7200
            }
        } //7200
    }

    private void updateViewData(LinkedList<Float> viewData, float newData) {
        viewData.removeFirst();
        viewData.addLast(newData);
    }


    private synchronized void setEachViewData() { //xingjian
        switch (titleIndex){
            case 0:
            case 1:
                firstPathView.setData1(ecgPointListView,  widthPerPointonePcg, viewHeight,curPos, 4700);
                secondPathView.setData(pcgPointListView, widthPerPointonePcg, viewHeight, curPos, 4700);
                thirdPathView.setData3(tstPointListView,  widthPerPointonePcg, viewHeight,curPos, thirdYadj);
                break;
            case 2:
                firstPathView.setData1(ecgPointListView,  widthPerPointonePcg, viewHeight,curPos, 4700);
                secondPathView.setData(pcgPointListView, widthPerPointonePcg, viewHeight, curPos, 4700);
                thirdPathView.setData2(heatPointListView,  widthPerPointonePcg, viewHeight,curPos, thirdYadj);
                break;
            case 3:
                if(mode == Constants.MEASURE_MODE_SYNC){ //同步模式
                    firstPathView.setData1(ecgPointListView,  widthPerPointonePcg, viewHeight,curPos, 4700);
                    secondPathView.setData4(heatPointListView, widthPerPointonePcg, viewHeight, curPos, secondYadj); //2
                    thirdPathView.setData3(tstPointListView,  widthPerPointonePcg, viewHeight,curPos, thirdYadj); //3 tst,能画出波形
                }else{
                    firstPathView.setData1(ecgPointListView,  widthPerPointonePcg, viewHeight,curPos, 4700); //0
                    secondPathView.setData(pcgPointListView, widthPerPointonePcg, viewHeight, curPos, 4700); //1
                    thirdPathView.setData2(heatPointListView,  widthPerPointonePcg, viewHeight,curPos, thirdYadj); //2
                }
                break;

            case 4:
                if(mode == Constants.MEASURE_MODE_SYNC){
                    return;
                }
                firstPathView.setData1(ecgPointListView,  widthPerPointonePcg, viewHeight,curPos, 4700);

                secondPathView.setData2(heatPointListView, widthPerPointonePcg, viewHeight, curPos, secondYadj);

                //临时 TODO
                break;
        }
    }

    @Override
    public void onErrorCode(BaseJsonMsg model) {

    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        viewHeight = firstPathView.getMeasuredHeight();
        viewWidth = firstPathView.getMeasuredWidth();

//        widthPerPointone = (float) viewWidth / 5000;
//        widthPerPointone = (float) viewWidth / 1250;

//        widthPerPointonePcg = (float) viewWidth / 5000; //renti
        widthPerPointonePcg = (float) viewWidth / 7200;// xingjian 25mm/s
    }

    /**
     * 写入数据库
     * @param patientInformation
     * @param bloodPressure
     * @param ecgMonitor
     */
    public void setdbdata(PatientInformationBean patientInformation, BloodPressureBean bloodPressure, EcgMonitorBean ecgMonitor) {
        PatientCheckDataBean patientCheckDataBean = new PatientCheckDataBean();

        //private String patientId;   没有写
        patientCheckDataBean.setCaseNo(patientInformation.getCaseNo());
        patientCheckDataBean.setName(patientInformation.getName());
        patientCheckDataBean.setSex(patientInformation.getSex());
        patientCheckDataBean.setBirthday(patientInformation.getBirthday());
        patientCheckDataBean.setNationality(patientInformation.getNationality());
        patientCheckDataBean.setAddress(patientInformation.getAddress());
        patientCheckDataBean.setPhone(patientInformation.getPhone());
        patientCheckDataBean.setUniqueId(patientInformation.getUniqueId());
        patientCheckDataBean.setCreateDate(patientInformation.getCreateDate());
        patientCheckDataBean.setHospitalId(patientInformation.getHospitalId());
        patientCheckDataBean.setDelFlag(patientInformation.getDelFlag());

        patientCheckDataBean.setHeight(bloodPressure.getHeight());
        patientCheckDataBean.setWeight(bloodPressure.getWeight());
        patientCheckDataBean.setPs(bloodPressure.getPs());
        patientCheckDataBean.setPd(bloodPressure.getPd());
        patientCheckDataBean.setBrPs(bloodPressure.getBrPs());
        patientCheckDataBean.setBrPd(bloodPressure.getBrPd());
        patientCheckDataBean.setAlPs(bloodPressure.getAlPs());
        patientCheckDataBean.setAlPd(bloodPressure.getAlPd());
        patientCheckDataBean.setArPs(bloodPressure.getArPs());
        patientCheckDataBean.setArPd(bloodPressure.getArPd());
        patientCheckDataBean.setPavrg(bloodPressure.getPavrg());

        //代码块  start
        /*
        patientCheckDataBean.setHypecgs(ecgMonitor.getHypecgs());
        patientCheckDataBean.setHyppcgs(ecgMonitor.getHyppcgs());
        patientCheckDataBean.setHypapgs(ecgMonitor.getHypapgs());
        patientCheckDataBean.setHypecgd(ecgMonitor.getHypecgd());
        patientCheckDataBean.setHyppcgd(ecgMonitor.getHyppcgd());
        patientCheckDataBean.setHypapgd(ecgMonitor.getHypapgd());
        patientCheckDataBean.setEcgPl(ecgMonitor.getEcgPl());
        patientCheckDataBean.setPcgPl(ecgMonitor.getPcgPl());
        patientCheckDataBean.setApgL(ecgMonitor.getApgL());
        patientCheckDataBean.setEcgPr(ecgMonitor.getEcgPr());
        patientCheckDataBean.setPcgPr(ecgMonitor.getPcgPr());
        patientCheckDataBean.setArgR(ecgMonitor.getArgR());
        patientCheckDataBean.setEcgTh(ecgMonitor.getEcgTh());
        patientCheckDataBean.setApgTh(ecgMonitor.getApgTh());
        */

        patientCheckDataBean.setHypecgs("555");
        patientCheckDataBean.setHyppcgs("555");
        patientCheckDataBean.setHypapgs("555");
        patientCheckDataBean.setHypecgd("555");
        patientCheckDataBean.setHyppcgd("555");
        patientCheckDataBean.setHypapgd("555");
        patientCheckDataBean.setEcgPl("555");
        patientCheckDataBean.setPcgPl("555");
        patientCheckDataBean.setApgL("555");
        patientCheckDataBean.setEcgPr("555");
        patientCheckDataBean.setPcgPr("555");
        patientCheckDataBean.setArgR("555");
        patientCheckDataBean.setEcgTh("555");
        patientCheckDataBean.setApgTh("555");
        //代码块结束

        PatientDaoHelper patientDaoHelper = new PatientDaoHelper();
        patientDaoHelper.addCheck(patientCheckDataBean);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {

            case R.id.bt_previous_step: bt_previous_step.setText("上一步");Log.e(TAG, "测试777777");
                if(Utils.nextFastClick()){
                   // ToastUtils.showToast(DrawsActivity.this,"点击过快");
                   // return;
                }

                if(startOrStop == true){Log.e(TAG, "测试888888");
                    ToastUtils.showToast(DrawsActivity.this,"请先暂停数据采集");
                    break;
                }

                clearList();
                Log.e(TAG, "测试999999");
                thirdViewLayout.setVisibility(View.VISIBLE);
                bt_previous_step.setText("上一步");

//                if (bt_previous_step.getText().equals("返回") || titleIndex == 0) {
                if (titleIndex == 0) {
                    //当他等于返回的时候，可以不做操作，或者返回到上一个页面
                    tv_ecg.setText(ecgTitle[titleIndex][0]);
                    tv_heart_sounds.setText(ecgTitle[titleIndex][1]);
                    tv_pulse.setText(ecgTitle[titleIndex][2]);

                    istrue = false;
                    GpioNative.closeGpioDev();
                    isRunning = false;
                    SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x08}));//停止波形传输

                    SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x0A}));

                    SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x0B, 0x02})); //血压气阀打开

                    task.cancel();
                    task = null;
                    timer.cancel();
                    timer.purge();
                    timer = null;
                    ThreadPoolWrapper.removeTask(parseRunnable);
                    ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                    ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                    ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                    ThreadPoolWrapper.shutdown();

                    finish();
                    Utils.exitActivityAnimation(DrawsActivity.this);

                } else {
                    titleIndex--;
                    setEcgTitle();
                    if (titleIndex == 0) {
                        bt_previous_step.setText("退出");
                        setEcgTitle();
                    } else {
                        setEcgTitle();
                    }
                }
                break;
            case R.id.bt_suspend:
                if(Utils.nextFastClick()){ Log.e(TAG, "测试aaaa");
                   // ToastUtils.showToast(DrawsActivity.this,"点击过快");
                   // return;
                }

                if(!isStart){
                    isStart = true;
                    btSuspend.setText("暂停");

                    startMeasure();

                }else{ Log.e(TAG, "测试cccccc");
//                    if(ecgPointList.size() < 5000 || pcgPointList.size() < 5000 || heatPointList.size() < 5000 || tstPointList.size() < 5000){
//                        ToastUtils.showToast(DrawsActivity.this,"请等待数据采集完整");
//                        return;
//                    } //xingjian
                    Log.e(TAG, "测试ddddddd");
                    isStart = false;
                    btSuspend.setText("开始");
                    stopMeasure();
                }

                break;

            case R.id.bt_next:
                if (bt_next.getText().equals("完成")) {
                    if(Utils.isFastClick()){
                      //  ToastUtils.showToast(DrawsActivity.this,"点击过快");
                      //  return;
                    }
                }else{
                    if(Utils.nextFastClick()){
                        //ToastUtils.showToast(DrawsActivity.this,"点击过快");
                       // return;
                    }
                }

                if(startOrStop == true){
                    ToastUtils.showToast(DrawsActivity.this,"请先暂停数据采集");
                    break;
                }

                if(titleIndex == 0){
                    if(hypecgs == null || hyppcgs == null || hypapgs == null){
                       // ToastUtils.showToast(DrawsActivity.this,"当前页面数据采集有误或还没有进行采集，请先采集");
                        //break;
                    }
                }
                if(titleIndex == 1){
                    if(hypecgd == null || hyppcgd == null || hypapgd == null){
                        //ToastUtils.showToast(DrawsActivity.this,"当前页面数据采集有误或还没有进行采集，请先采集");
                       // break;
                    }
                }
                if(titleIndex == 2){
                    if(ecgPl == null || pcgPl == null || apgL == null){
                       // ToastUtils.showToast(DrawsActivity.this,"当前页面数据采集有误或还没有进行采集，请先采集");
                       // break;
                    }
                }
                if(titleIndex == 3){
                    if(mode == Constants.MEASURE_MODE_SYNC){
                        if(ecgPr == null || apgR == null || apgTh == null || pcgPr == null || ecgTh == null){
                          //  ToastUtils.showToast(DrawsActivity.this,"当前页面数据采集有误或还没有进行采集，请先采集");
                           // break;
                        }
                    }else{
                        if(ecgPr == null || pcgPr == null || apgR == null){
                           // ToastUtils.showToast(DrawsActivity.this,"当前页面数据采集有误或还没有进行采集，请先采集");
                           // break;
                        }
                    }
                }
                if(titleIndex == 4){
                    if(mode == Constants.MEASURE_MODE_ASYNC){
                        if(ecgTh == null || apgTh == null){
                           // ToastUtils.showToast(DrawsActivity.this,"当前页面数据采集有误或还没有进行采集，请先采集");
                           // break;
                        }
                    }
                }

                clearList();

                bt_previous_step.setText("上一步");
                if (bt_next.getText().equals("完成")) {
                    Log.e(TAG, "测试999999c");
                   // uploadInfo();

                } else {
                    titleIndex++;
                    setEcgTitle();
                    //分两种情况，同步模式只能加到3，异步模式可以加到4；
                    if(mode == Constants.MEASURE_MODE_SYNC){
                        if(titleIndex == 3){
                            tv_heart_sounds.setText("右颈动脉脉搏波");
                            tv_pulse.setText("股动脉脉搏波");
                            bt_next.setText("完成");
                            Log.e(TAG, "测试999999a");
                        }
                    }else{
                        if (titleIndex == 4) {
                            bt_next.setText("完成");
                            Log.e(TAG, "测试999999b");
                            thirdViewLayout.setVisibility(View.GONE);
                        }
                    }
                }
                break;

//            case R.id.bt_yadj:
//                et_first = findViewById(R.id.et_first);
//                et_second = findViewById(R.id.et_second);
//                et_third = findViewById(R.id.et_third);
//                if(!et_first.getText().toString().equals("")){
//                    firstYadj = Integer.parseInt(et_first.getText().toString());
//                }
//                if(!et_second.getText().toString().equals("")){
//                    secondYadj = Integer.parseInt(et_second.getText().toString());
//                }
//                if(!et_third.getText().toString().equals("")){
//                    thirdYadj = Integer.parseInt(et_third.getText().toString());
//                }
//
//                break;
        }
    }

    /**
     * 设置页面标题和每个view标题
     */
    private void setEcgTitle() {
        if(titleIndex < 0){
            titleIndex = 0;
        }
        if (titleIndex > 4) {
            titleIndex = 4;
        }
        tv_ecg.setText(ecgTitle[titleIndex][0]);
        tv_title.setText(title[titleIndex]);
        tv_heart_sounds.setText(ecgTitle[titleIndex][1]);
        tv_pulse.setText(ecgTitle[titleIndex][2]);
    }


    /**
     * 上传信息
     */
    private void uploadInfo() {
        ecgMonitorBean = new EcgMonitorBean();
//        if(bloodPressureBean == null){
//            ToastUtils.showToast(DrawsActivity.this,"创建血压数据异常，请重试！");
//            return;
//        }
        System.out.println("上传之前打印测试号：" + bloodPressureBean.getBasisMeasurementId());
        ecgMonitorBean.setBasisMeasurementId(bloodPressureBean.getBasisMeasurementId());
//        ecgMonitorBean.setBasisMeasurementId("ceshi2");
//        ecgMonitorBean.setBasisMeasurementId("6ea8f80d3ea34ea5a7bca0362767b811");


        ecgMonitorBean.setIdl(-1);
        ecgMonitorBean.setIdr(-1);
        ecgMonitorBean.setModel(mode);

        setFourteenData();
        if(!check()){
            return;
        }

        mPresenter.UploadEcghttp(ecgMonitorBean);
    }

    private void setFourteenData() {
//        if(mode == Constants.MEASURE_MODE_SYNC){
//            pcgPr = pcgPl;
//            ecgTh = ecgPr;
//        }
        ecgMonitorBean.setHypecgs(hypecgs);
        ecgMonitorBean.setHyppcgs(hyppcgs);
        ecgMonitorBean.setHypapgs(hypapgs);
        ecgMonitorBean.setHypecgd(hypecgd);
        ecgMonitorBean.setHyppcgd(hyppcgd);
        ecgMonitorBean.setHypapgd(hypapgd);
        ecgMonitorBean.setEcgPl(ecgPl);
        ecgMonitorBean.setPcgPl(pcgPl);
        ecgMonitorBean.setApgL(apgL);
        ecgMonitorBean.setEcgPr(ecgPr);
        ecgMonitorBean.setPcgPr(pcgPr);
        ecgMonitorBean.setApgR(apgR);
        ecgMonitorBean.setEcgTh(ecgTh);
        ecgMonitorBean.setApgTh(apgTh);
    }

    private boolean check() {

     /*   if(hypecgs == null || hyppcgs == null || hypapgs == null || hypecgs.length == 0 || hyppcgs.length == 0 || hypapgs.length == 0){
            ToastUtils.showToast(DrawsActivity.this,"超收缩压数据为空，请重新测试");
            return false;
        }
        if(hypecgd == null || hyppcgd == null || hypapgd == null || hypecgd.length == 0 || hyppcgd.length == 0 || hypapgd.length == 0){
            ToastUtils.showToast(DrawsActivity.this,"低舒张压数据为空，请重新测试");
            return false;

        }
        if(ecgPl == null || pcgPl == null || apgL == null || ecgPl.length == 0 || pcgPl.length == 0 || apgL.length == 0){
            ToastUtils.showToast(DrawsActivity.this,"左颈动脉数据为空，请重新测试");
            return false;

        }

        if(ecgPr == null || pcgPr == null || apgR == null || ecgPr.length == 0 || pcgPr.length == 0 || apgR.length == 0){
            ToastUtils.showToast(DrawsActivity.this,"右颈动脉数据为空，请重新测试");
            return false;

        }*/

//        if(mode == Constants.MEASURE_MODE_SYNC){
//            return true;
//        }else{
//            if(ecgTh == null || apgTh == null || ecgTh.length == 0 || apgTh.length == 0 ){
//                ToastUtils.showToast(DrawsActivity.this,"股动脉数据为空，请重新测试");
//                return false;
//
//            }
//        }

        if(ecgTh == null || apgTh == null || ecgTh.length == 0 || apgTh.length == 0 ){
                ToastUtils.showToast(DrawsActivity.this,"股动脉数据为空，请重新测试");
                return false;

        }

        return true;
    }

    @Override
    public void onMainSuccess(BaseJsonMsg<EcgMonitorBean> o) {
        if(o.getHeader().getErrcode() == 2){
            showToast(o.getHeader().getErrmsg());
            return;
        }

        if(o.getData().getPrResult() == -10000.0 || o.getData().getcResult() == -10000.0 ||
                o.getData().getAscResult() == -10000.0 || o.getData().getqResult() == -10000.0 ||
                o.getData().getPrResultR() == -10000.0 || o.getData().getcResultR() == -10000.0 ||
                o.getData().getAscResultR() == -10000.0 || o.getData().getqResultR() == -10000.0 ||
                o.getData().getJsvResult() == -10000.0 || o.getData().getJbrResult() == -10000.0 ||
                o.getData().getJpaResult() == -10000.0 || o.getData().getJpsResult() == -10000.0 ||
                o.getData().getJpdResult() == -10000.0 || o.getData().getPssResult() == -10000.0){
            ToastUtils.showToastLong(DrawsActivity.this,"结果计算不规范，请返回第一个页面重新正规检测");
            return;
        }

        ecgMonitorBean.setHyppcgdIstarts(o.getData().getHyppcgdIstarts()); //低舒张第一心音
        ecgMonitorBean.setHyppcgsIstarts(o.getData().getHyppcgsIstarts()); //超收缩第一心音

        ecgMonitorBean.setHyppcgdIIstarts(o.getData().getHyppcgdIIstarts()); //低舒张第二心音
        ecgMonitorBean.setHyppcgsIIstarts(o.getData().getHyppcgsIIstarts()); //超收缩第二心音

        ecgMonitorBean.setHypapgdDNpnts(o.getData().getHypapgdDNpnts()); //低舒张脉搏波DN
        ecgMonitorBean.setHypapgsDNpnts(o.getData().getHypapgsDNpnts()); //超收缩脉搏波DN

        ecgMonitorBean.setHypapgsUSpnts(o.getData().getHypapgsUSpnts()); //超收缩脉搏波US
        ecgMonitorBean.setHypapgdUSpnts(o.getData().getHypapgdUSpnts()); //低舒张脉搏波US

        ecgMonitorBean.setHypecgdRtops(o.getData().getHypecgdRtops()); //低舒张心电Rtops
        ecgMonitorBean.setHypecgsRtops(o.getData().getHypecgsRtops()); //超收缩心电Rtops

        ecgMonitorBean.setHypapgdUTpnts(o.getData().getHypapgdUTpnts()); //低舒张脉搏波UT
        ecgMonitorBean.setHypapgsUTpnts(o.getData().getHypapgsUTpnts()); //超收缩脉搏波UT


        ecgMonitorBean.setAscResult(o.getData().getAscResult()); //冠状动脉页面ASC
        ecgMonitorBean.setPrResult(o.getData().getPrResult()); //冠状动脉页面PR
        ecgMonitorBean.setqResult(o.getData().getqResult()); //冠状动脉页面Q
        ecgMonitorBean.setcResult(o.getData().getcResult());

        ecgMonitorBean.setAscResultR(o.getData().getAscResultR()); //冠状动脉页面右ASC
        ecgMonitorBean.setPrResultR(o.getData().getPrResultR()); //冠状动脉页面右PR
        ecgMonitorBean.setqResultR(o.getData().getqResultR()); //冠状动脉页面右Q
        ecgMonitorBean.setcResultR(o.getData().getcResultR());

        ecgMonitorBean.setBuf(o.getData().getBuf()); //分析出来的波形结果
        ecgMonitorBean.setgNPSType(o.getData().getgNPSType()); //18个方格闪烁提示
        ecgMonitorBean.setIDL(o.getData().getIDL()); //左颈动脉选择波形
        ecgMonitorBean.setIDR(o.getData().getIDR()); //右颈动脉选择波形
        ecgMonitorBean.setJbrResult(o.getData().getJbrResult()); //血流总阻力结果
        ecgMonitorBean.setJpaResult(o.getData().getJpaResult()); //jpm平均压的结果
        ecgMonitorBean.setJpdResult(o.getData().getJpdResult()); //舒张压结果
        ecgMonitorBean.setJpsResult(o.getData().getJpsResult()); //ps收缩压结果
        ecgMonitorBean.setJsvResult(o.getData().getJsvResult()); //心脏搏出量结果

        ecgMonitorBean.setPssResult(o.getData().getPssResult()); //拐点压结果
        ecgMonitorBean.setRect2(o.getData().getRect2()); //对应十个风险等级中的一个
        ecgMonitorBean.setRtopL(o.getData().getRtopL());  //左颈动脉画图Rtop
        ecgMonitorBean.setRtopR(o.getData().getRtopR()); //右颈动脉画图Rtop
        ecgMonitorBean.setTsL(o.getData().getTsL()); //左颈动脉画图TS
        ecgMonitorBean.setTsR(o.getData().getTsR()); //右颈动脉画图TS
        ecgMonitorBean.setUTL(o.getData().getUTL()); //左颈动脉画图UT
        ecgMonitorBean.setUTR(o.getData().getUTR()); //右颈动脉画图UT

        ecgMonitorBean.setRtopNumL(o.getData().getRtopNumL());
        ecgMonitorBean.setRtopNumR(o.getData().getRtopNumR());
        ecgMonitorBean.setHypecgdRtopsNum(o.getData().getHypecgdRtopsNum());
        ecgMonitorBean.setHypecgsRtopsNum(o.getData().getHypecgsRtopsNum());

        ecgMonitorBean.setPTV(o.getData().getPTV());
        ecgMonitorBean.setPTT(o.getData().getPTT());
        ecgMonitorBean.setAge(o.getData().getAge());
        ecgMonitorBean.setCBPs(o.getData().getCBPs());
        ecgMonitorBean.setjAdL(o.getData().getjAdL());
        ecgMonitorBean.setjAsL(o.getData().getjAsL());
        ecgMonitorBean.setjPssL(o.getData().getjPssL());
        ecgMonitorBean.setjTL(o.getData().getjTL());
        ecgMonitorBean.setjTsL(o.getData().getjTsL());
        ecgMonitorBean.setjAdR(o.getData().getjAdR());
        ecgMonitorBean.setjAsR(o.getData().getjAsR());
        ecgMonitorBean.setjPssR(o.getData().getjPssR());
        ecgMonitorBean.setjTR(o.getData().getjTR());
        ecgMonitorBean.setjTsR(o.getData().getjTsR());

        setFourteenData();

        MyApplication.ecgMonitorBean = ecgMonitorBean;
        toAnalysis();
//        toResult();
//        setdbdata(patientInformationBean, bloodPressureBean, ecgMonitorBean);
//        toAnalysis();

    }

    private void toAnalysis() {
        istrue = false;
        GpioNative.closeGpioDev();
        isRunning = false;
        SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x08}));//停止波形传输

        SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x0A}));

        SerialPortUtil.sendString(SerialParseUtil.packetSend(0x55, new byte[]{0x0B, 0x02})); //血压气阀打开
//        SerialPortUtil.close();
        task.cancel();
        task = null;
        timer.cancel();
        timer.purge();
        timer = null;
        ThreadPoolWrapper.removeTask(parseRunnable);
        ThreadPoolWrapper.removeTask(parseRunnableGpio1);
        ThreadPoolWrapper.removeTask(parseRunnableGpio2);
        ThreadPoolWrapper.removeTask(parseRunnableGpio3);
        ThreadPoolWrapper.shutdown();

        Intent intent = new Intent();
//        intent.putExtra("ecgMonitorBean", (Serializable) ecgMonitorBean);//直接存入被序列化的对象实例
//        intent.setClass(this, AnalysisActivity.class);
//        startActivity(intent);

        intent = new Intent(this, ResultOneActivity.class);
        startActivity(intent);
    }

    private void toResult() {
        Intent intent = new Intent();
//        intent.putExtra("ecgMonitorBean", (Serializable) ecgMonitorBean);//直接存入被序列化的对象实例
        intent.setClass(this, ResultOneActivity.class);
        startActivity(intent);
    }

    public void freePointer(Pointer p){
        long peer = Pointer.nativeValue(p);
        Native.free(peer);//手动释放内存
        Pointer.nativeValue(p, 0);//避免Memory对象被GC时重复执行Nativ.free()方法
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        isRunning = false;
        startOrStop = false;
        SerialBuffer.clearBuffer();
//        buffer = null;
        deque.clear();
        dequeFilter.clear();
        GpioNative.closeGpioDev();
    }
}
