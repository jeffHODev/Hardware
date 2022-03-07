package com.hekangyuan.nivi1000.activity;

import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.GradientDrawable;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.hekangyuan.nivi1000.GpioNative;
import com.hekangyuan.nivi1000.MyApplication;
import com.hekangyuan.nivi1000.R;
import com.hekangyuan.nivi1000.base.BaseActivity;
import com.hekangyuan.nivi1000.base.mvp.BasePresenter;
import com.hekangyuan.nivi1000.utils.ThreadPoolWrapper;
import com.hekangyuan.nivi1000.utils.Utils;


public class ResultTwoActivity extends BaseActivity implements View.OnClickListener {


    private TextView tx_oindex, tx_range, tx_horn, tx_light, blood_flow, tx_pulse_output, tx_hl_pressure;

    private TextView bpTextView1;
    private Drawable drawable;
    public Button bt_newtext, bt_next, bt_previous;


    private int[] textViewIDs = new int[]{R.id.bp_pressure_1, R.id.bp_pressure_7, R.id.bp_pressure_13, R.id.bp_pressure_2,
            R.id.bp_pressure_8, R.id.bp_pressure_14, R.id.bp_pressure_3, R.id.bp_pressure_9,
            R.id.bp_pressure_15, R.id.bp_pressure_4, R.id.bp_pressure_10, R.id.bp_pressure_16,
            R.id.bp_pressure_5, R.id.bp_pressure_11, R.id.bp_pressure_17, R.id.bp_pressure_6,
            R.id.bp_pressure_12, R.id.bp_pressure_18};

    private int[] colors = new int[]{R.color.blood_pressure_1, R.color.blood_pressure_7, R.color.blood_pressure_13, R.color.blood_pressure_2,
            R.color.blood_pressure_8, R.color.blood_pressure_14, R.color.blood_pressure_3, R.color.blood_pressure_9,
            R.color.blood_pressure_15, R.color.blood_pressure_4, R.color.blood_pressure_10, R.color.blood_pressure_16,
            R.color.blood_pressure_5, R.color.blood_pressure_11, R.color.blood_pressure_17, R.color.blood_pressure_6,
            R.color.blood_pressure_12, R.color.blood_pressure_18};

    private TextView[] textViews = new TextView[18];

    private int analysisResult = 0;

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

    @Override
    protected BasePresenter createPresenter() {
        return null;
    }

    @Override
    protected int getLayoutId() {
        return R.layout.activity_result_two;
    }


    @Override
    protected void initData() {
//        if (MyApplication.ecgMonitorBean != null) {
//            analysisResult = (int) MyApplication.ecgMonitorBean.getRect2() - 1; //下标减1
//        }
//        Log.e("huang", "analysisResult " + analysisResult);
//        TextView tv_title = findViewById(R.id.tv_title);
//        tv_title.setText("冠脉风险评价");
//        tx_oindex = findViewById(R.id.tx_oindex);
//        tx_range = findViewById(R.id.tx_range);
//        tx_horn = findViewById(R.id.tx_horn);
//        tx_light = findViewById(R.id.tx_light);
//
//        bpTextView1 = findViewById(R.id.bp_pressure_1);
//
//        for (int i = 0; i < textViewIDs.length; i++) {
//            textViews[i] = findViewById(textViewIDs[i]);
//        }

//        drawable = getResources().getDrawable(R.mipmap.oindex);
//        drawable.setBounds(0, 0, 40, 32);
//        tx_oindex.setCompoundDrawables(null, null, drawable, null);

//        drawable = getResources().getDrawable(R.mipmap.range);
//        drawable.setBounds(0, 0, 40, 40);
//        tx_range.setCompoundDrawables(null, null, drawable, null);

//        drawable = getResources().getDrawable(R.mipmap.horn);
//        drawable.setBounds(0, 0, 40, 32);
//        tx_horn.setCompoundDrawables(null, null, drawable, null);

//        drawable = getResources().getDrawable(R.mipmap.threelight);
//        drawable.setBounds(0, 0, 30, 40);
//        tx_light.setCompoundDrawables(null, null, drawable, null);

//        drawable = getResources().getDrawable(R.mipmap.greeone);
//        drawable.setBounds(0,0,240,70);
//        drawable.set
//        res_thr_normal.setCompoundDrawables(null,null, drawable,null);


//        GradientDrawable gd = (GradientDrawable) textViews[analysisResult].getBackground();
//        gd.setColor(getResources().getColor(colors[analysisResult]));

//        if(analysisResult <= 0){
//            analysisResult = 0;
//        }
//        textViews[analysisResult].setBackgroundColor(getResources().getColor(colors[analysisResult]));
//
//        bt_next = findViewById(R.id.bt_next);
//        bt_next.setOnClickListener(this);
//        bt_previous = findViewById(R.id.bt_previous);
//        bt_previous.setOnClickListener(this);
//        bt_newtext = findViewById(R.id.bt_newtext);
//        bt_newtext.setOnClickListener(this);
//
//        blood_flow = findViewById(R.id.blood_flow); //血流总阻力
//        tx_pulse_output = findViewById(R.id.tx_pulse_output); //输出量
//        tx_hl_pressure = findViewById(R.id.tx_hl_pressure); //高低压
//
//        if (MyApplication.ecgMonitorBean != null) {
//
//
//            //TODO 高低压拼接
//            tx_hl_pressure.setText(MyApplication.ecgMonitorBean.getJpsResult()+"");
//
//            tx_pulse_output.setText(MyApplication.ecgMonitorBean.getJpdResult() + "");
//
//            blood_flow.setText(MyApplication.ecgMonitorBean.getJpaResult() + ""); //源代码没有这句，下面的if里面有判断这个的值；
//
////        if(MyApplication.bloodPressureBean != null) {
////                blood_flow.setText(MyApplication.bloodPressureBean.getPavrg() + "");
////            }
//        }

    }

    @Override
    protected void onResume() {
        super.onResume();

        istrue = false;
        GpioNative.closeGpioDev();
//        ThreadPoolWrapper.removeTask(parseRunnableGpio1);
//        ThreadPoolWrapper.removeTask(parseRunnableGpio2);
//        ThreadPoolWrapper.removeTask(parseRunnableGpio3);
//        ThreadPoolWrapper.shutdown();

        if (MyApplication.ecgMonitorBean != null) {
            analysisResult = (int) MyApplication.ecgMonitorBean.getgNPSType() - 1; //下标减1
        }
        Log.e("huang", "analysisResult " + analysisResult);
        TextView tv_title = findViewById(R.id.tv_title);
        tv_title.setText("血压类型评价");
        tx_oindex = findViewById(R.id.tx_oindex);
        tx_range = findViewById(R.id.tx_range);
        tx_horn = findViewById(R.id.tx_horn);
        tx_light = findViewById(R.id.tx_light);

        bpTextView1 = findViewById(R.id.bp_pressure_1);

        for (int i = 0; i < textViewIDs.length; i++) {
            textViews[i] = findViewById(textViewIDs[i]);
        }

        if(analysisResult <= 0){
            analysisResult = 0;
        }
        textViews[analysisResult].setBackgroundColor(getResources().getColor(colors[analysisResult]));

        bt_next = findViewById(R.id.bt_next);
        bt_next.setOnClickListener(this);
        bt_previous = findViewById(R.id.bt_previous);
        bt_previous.setOnClickListener(this);
        bt_newtext = findViewById(R.id.bt_newtext);
        bt_newtext.setOnClickListener(this);

        blood_flow = findViewById(R.id.blood_flow); //血流总阻力
        tx_pulse_output = findViewById(R.id.tx_pulse_output); //输出量
        tx_hl_pressure = findViewById(R.id.tx_hl_pressure); //高低压

        if (MyApplication.ecgMonitorBean != null) {


            //TODO 高低压拼接
            tx_hl_pressure.setText(MyApplication.ecgMonitorBean.getJpsResult()+"");

            tx_pulse_output.setText(MyApplication.ecgMonitorBean.getJpdResult() + "");

//            blood_flow.setText(MyApplication.ecgMonitorBean.getJpaResult() + ""); //源代码没有这句，下面的if里面有判断这个的值；
            blood_flow.setText(String.format("%.0f", MyApplication.ecgMonitorBean.getJpaResult()));

//        if(MyApplication.bloodPressureBean != null) {
//                blood_flow.setText(MyApplication.bloodPressureBean.getPavrg() + "");
//            }
        }

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
                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                ThreadPoolWrapper.shutdown();

                finish();
                Utils.exitActivityAnimation(ResultTwoActivity.this);
            }
        });

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
                        ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                        ThreadPoolWrapper.removeTask(parseRunnableGpio3);
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
                                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                                ThreadPoolWrapper.shutdown();

                                finish();
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
                                istrue = false;
                                GpioNative.closeGpioDev();
                                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                                ThreadPoolWrapper.shutdown();

                                Intent intent = new Intent(ResultTwoActivity.this, PatientInfomationActivity.class);
                                startActivity(intent);
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
                                istrue = false;
                                GpioNative.closeGpioDev();
                                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                                ThreadPoolWrapper.shutdown();

                                Intent intent = new Intent(ResultTwoActivity.this, ResultThreeActivity.class);
                                startActivity(intent);
                            }
                        });
                    }
                }
            }
        };
        ThreadPoolWrapper.getThreadPool().executeTask(parseRunnableGpio3);
    }

    @Override
    protected void onPause() {
        super.onPause();

    }

    @Override
    public void onClick(View v) {
        Intent intent = new Intent();
        switch (v.getId()) {

            case R.id.bt_newtext:
                istrue = false;
                GpioNative.closeGpioDev();
                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                ThreadPoolWrapper.shutdown();

                intent = new Intent(ResultTwoActivity.this, PatientInfomationActivity.class);
                startActivity(intent);

                break;
            case R.id.bt_next:
                istrue = false;
                GpioNative.closeGpioDev();
                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                ThreadPoolWrapper.shutdown();

                intent = new Intent(ResultTwoActivity.this, ResultThreeActivity.class);
                startActivity(intent);

                break;
            case R.id.bt_previous:
                istrue = false;
                GpioNative.closeGpioDev();
                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                ThreadPoolWrapper.shutdown();

                finish();
                break;

        }
    }
}
