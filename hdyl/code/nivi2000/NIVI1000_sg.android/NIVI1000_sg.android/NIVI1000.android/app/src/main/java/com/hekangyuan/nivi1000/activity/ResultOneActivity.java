package com.hekangyuan.nivi1000.activity;

import android.content.Context;
import android.content.Intent;
import android.graphics.BitmapFactory;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.hekangyuan.nivi1000.GpioNative;
import com.hekangyuan.nivi1000.MyApplication;
import com.hekangyuan.nivi1000.R;
import com.hekangyuan.nivi1000.base.BaseActivity;
import com.hekangyuan.nivi1000.base.mvp.BasePresenter;
import com.hekangyuan.nivi1000.utils.ThreadPoolWrapper;
import com.hekangyuan.nivi1000.utils.Utils;
import com.hekangyuan.nivi1000.view.graphViewForLive.XYView;

public class ResultOneActivity extends BaseActivity implements View.OnClickListener {
    public Button bt_newtext, bt_next, bt_previous;
//    private ImageView img_lr;

    public XYView xyView;

    private float width,height;
    private float x,y;
    private float ptv;

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
        return R.layout.activity_result_one;
    }

    @Override
    protected void initData() {
//        TextView tv_title = findViewById(R.id.tv_title);
//        tv_title.setText("冠脉风险评价");
//        tv_title.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//                Intent intent = new Intent(ResultOneActivity.this, ResultTwoActivity.class);
//
//                startActivity(intent);
//            }
//        });
//
//        initView();
//        //初始化底部三个按钮
//        initBtn();



//        img_lr = findViewById(R.id.img_lr);
//        Bitmap picArr[] = {BitmapFactory.decodeResource(getResources(), R.drawable.b)};
//        img_lr.setImageBitmap(BitmapUtils.mergeBitmap(ResultOneActivity.this, BitmapFactory.decodeResource(getResources(),
//                R.mipmap.carotidpulsebac), picArr, "", R.mipmap.carotidpulsebac, new int[]{1}, new int[1], 2200, 2200));
//        img_lr.setImageBitmap(BitmapFactory.decodeResource(getResources(), R.drawable.carotidpulsebac));
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

        TextView tv_title = findViewById(R.id.tv_title);
        tv_title.setText("颈-股脉搏波传导速度");
        tv_title.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(ResultOneActivity.this, ResultTwoActivity.class);

                startActivity(intent);
            }
        });

        initView();
        //初始化底部三个按钮
        initBtn();

        back = (TextView) findViewById(R.id.title_left);
        if (back == null) {
            return;
        }
        back.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View arg0) {
//                istrue = false;
//                GpioNative.closeGpioDev();
//                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
//                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
//                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
//                ThreadPoolWrapper.shutdown();
//
//                finish();
//                Utils.exitActivityAnimation(ResultOneActivity.this);
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
//                                istrue = false;
//                                GpioNative.closeGpioDev();
//                                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
//                                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
//                                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
//                                ThreadPoolWrapper.shutdown();
//
//                                finish();
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

                                Intent intent = new Intent(ResultOneActivity.this, PatientInfomationActivity.class);
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

                                Intent intent = new Intent(ResultOneActivity.this, ResultTwoActivity.class);
                                startActivity(intent);
                            }
                        });
                    }
                }
            }
        };
        ThreadPoolWrapper.getThreadPool().executeTask(parseRunnableGpio3);
    }

    private void processXY() {
        ptv = MyApplication.ecgMonitorBean.getPTV();
        int age = MyApplication.ecgMonitorBean.getAge();

        x = 0;
        if(age<30){
            x = 0;
        }else if(age >= 30 && age < 40){
            x = width / 5;
        }else if(age >= 40 && age < 50){
            x = width / 5 * 2;
        }else if(age >= 50 && age < 60){
            x = width / 5 * 3;
        }else if(age >= 60 && age < 70){
            x = width / 5 * 4;
        }else if(age >= 70){
            x = width;
        }

        float dpy = (float) (height / (20 - 2.25));
        y = (float) (ptv / 100 - 2.25) * dpy; // 2.25 y坐标在原点的值
    }

    private void initView() {
        xyView = findViewById(R.id.xyView);

        bt_next = findViewById(R.id.bt_next);
        bt_previous = findViewById(R.id.bt_previous);
        bt_newtext = findViewById(R.id.bt_newtext);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        height = xyView.getMeasuredHeight();
        width = xyView.getMeasuredWidth();
        processXY();
        TextView tv_title = findViewById(R.id.pwvview);
        tv_title.setText("cfPWV = " + (float)(Math.round(ptv / 100 * 100))/100 + " m/s");
        xyView.setData(x,y);
    }

    private void initBtn() {
        bt_next.setOnClickListener(this);
        bt_previous.setOnClickListener(this);
        bt_newtext.setOnClickListener(this);
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

                intent = new Intent(ResultOneActivity.this, PatientInfomationActivity.class);
                startActivity(intent);
                break;
            case R.id.bt_next:
                istrue = false;
                GpioNative.closeGpioDev();
                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                ThreadPoolWrapper.shutdown();

                intent = new Intent(ResultOneActivity.this, ResultTwoActivity.class);
                startActivity(intent);

                break;
            case R.id.bt_previous:
//                istrue = false;
//                GpioNative.closeGpioDev();
//                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
//                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
//                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
//                ThreadPoolWrapper.shutdown();
//
//                finish();
                break;

        }
    }
}
