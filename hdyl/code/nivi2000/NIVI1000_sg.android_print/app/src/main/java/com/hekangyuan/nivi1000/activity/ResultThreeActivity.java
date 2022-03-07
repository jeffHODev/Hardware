package com.hekangyuan.nivi1000.activity;


import android.Manifest;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.print.PrintManager;
import android.provider.Settings;
import android.support.annotation.NonNull;
import android.support.annotation.RequiresApi;
import android.support.v4.content.ContextCompat;
import android.util.Base64;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.PopupWindow;
import android.widget.TextView;

import com.hekangyuan.nivi1000.GpioNative;
import com.hekangyuan.nivi1000.MyApplication;
import com.hekangyuan.nivi1000.R;
import com.hekangyuan.nivi1000.activity.api.PrintPDFView;
import com.hekangyuan.nivi1000.activity.presenter.PringtPDFPresenter;
import com.hekangyuan.nivi1000.base.BaseActivity;
import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.file.FileUtils;
import com.hekangyuan.nivi1000.model.PDFBean;
import com.hekangyuan.nivi1000.print.MyPrintAdapter2;
import com.hekangyuan.nivi1000.utils.NetWorkUtils;
import com.hekangyuan.nivi1000.utils.ThreadPoolWrapper;
import com.hekangyuan.nivi1000.utils.ToastUtils;
import com.hekangyuan.nivi1000.utils.Utils;
import com.hekangyuan.nivi1000.view.graphViewForLive.PRView;
import com.hekangyuan.nivi1000.view.graphViewForLive.QView;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.List;


public class ResultThreeActivity extends BaseActivity<PringtPDFPresenter> implements PrintPDFView, View.OnClickListener {

    public Button bt_newtext, bt_next, bt_previous, btn_sure;
    private int xone[];
    private int yone[];
    private int xtwo[];
    private int ytwo[];
    private View popView;
    private PopupWindow popupWindow;
    private Drawable drawable;
    private ImageView imgHealthyResult, im_btn_result;

    private PRView prView;
    private QView qView;
    private TextView tx_oindex, tx_range, tx_horn;
    private TextView textViewPR, textViewC, textViewQ, textViewASC;
    private TextView textViewPRR, textViewCR, textViewQR, textViewASCR, textViewCBPs;

    private int[] healthyResultIds = {
            R.mipmap.healthyres10, R.mipmap.healthyres9, R.mipmap.healthyres8,
            R.mipmap.healthyres7, R.mipmap.healthyres6, R.mipmap.healthyres5,
            R.mipmap.healthyres4, R.mipmap.healthyres3, R.mipmap.healthyres2,
            R.mipmap.healthyres1,
    };
    private int width, height;

    private float prX, prY;
    private float qX, qY;

    private float prRX, prRY;
    private float qRX, qRY;

    private boolean istrue = true;
    private boolean istrue5;
    int level1 = 0;
    int lastLevel1 = -1;
    int level2 = 0;
    int lastLevel2 = -1;
    int level3 = 0;
    int lastLevel3 = -1;
    int level4 = 0;
    int lastLevel4 = -1;
    int level5 = 0;
    int lastLevel5 = -1;
    private Runnable parseRunnableGpio1;
    private Runnable parseRunnableGpio2;
    private Runnable parseRunnableGpio3;
    private Runnable parseRunnableGpio4;
    private Runnable parseRunnableGpio5;

    @Override
    protected PringtPDFPresenter createPresenter() {
        return new PringtPDFPresenter(this);
    }

    @Override
    protected int getLayoutId() {
        return R.layout.activity_result_three;
    }


    @Override
    protected void initData() {
//        TextView tv_title = findViewById(R.id.tv_title);
//        tv_title.setText("冠脉风险评价");
//        tv_title.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//                Intent intent = new Intent(ResultThreeActivity.this, ResultOneActivity.class);
//
//                startActivity(intent);
//            }
//        });
//        bt_next = findViewById(R.id.bt_next);
//        bt_next.setOnClickListener(this);
//        bt_previous = findViewById(R.id.bt_previous);
//        bt_previous.setOnClickListener(this);
//        bt_newtext = findViewById(R.id.bt_newtext);
//        bt_newtext.setOnClickListener(this);
//
//        prView = findViewById(R.id.im_pr_result);
//        qView = findViewById(R.id.im_q_result);
//
//        im_btn_result = findViewById(R.id.im_btn_result);
//
//        tx_oindex = findViewById(R.id.tx_goindex);
//        tx_range = findViewById(R.id.tx_grange);
//        tx_horn = findViewById(R.id.tx_ghorn);
////        drawable = getResources().getDrawable(R.mipmap.oindex);
////        drawable.setBounds(0, 0, 40, 32);
////        tx_oindex.setCompoundDrawables(null, null, drawable, null);
////        drawable = getResources().getDrawable(R.mipmap.range);
////        drawable.setBounds(0, 0, 40, 40);
////        tx_range.setCompoundDrawables(null, null, drawable, null);
////        drawable = getResources().getDrawable(R.mipmap.horn);
////        drawable.setBounds(0, 0, 40, 32);
////        tx_horn.setCompoundDrawables(null, null, drawable, null);
//
//        textViewPR = findViewById(R.id.tv_pr);
//        textViewC = findViewById(R.id.tv_c);
//        textViewQ = findViewById(R.id.tv_q);
//        textViewASC = findViewById(R.id.tv_asc);
//
//        textViewPRR = findViewById(R.id.tv_prr);
//        textViewCR = findViewById(R.id.tv_cr);
//        textViewQR = findViewById(R.id.tv_qr);
//        textViewASCR = findViewById(R.id.tv_ascr);
//
//
//        initTopPop();
//        im_btn_result.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//                bgAlpha(0.5f);
//                popupWindow.showAtLocation(popView, Gravity.NO_GRAVITY, 330, 100);
//            }
//        });
//
//
//        setData();
    }

    @Override
    protected void onResume() {
        super.onResume();

        istrue = false;
        GpioNative.closeGpioDev();
//        ThreadPoolWrapper.removeTask(parseRunnableGpio1);
//        ThreadPoolWrapper.removeTask(parseRunnableGpio2);
//        ThreadPoolWrapper.removeTask(parseRunnableGpio3);
//        ThreadPoolWrapper.removeTask(parseRunnableGpio4);
//        ThreadPoolWrapper.shutdown();

        TextView tv_title = findViewById(R.id.tv_title);
        tv_title.setText("血流动力学指标");
        tv_title.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(ResultThreeActivity.this, ResultOneActivity.class);

                startActivity(intent);
            }
        });
        bt_next = findViewById(R.id.bt_next);
        bt_next.setOnClickListener(this);
        bt_previous = findViewById(R.id.bt_previous);
        bt_previous.setOnClickListener(this);
        bt_newtext = findViewById(R.id.bt_newtext);
        bt_newtext.setOnClickListener(this);

        prView = findViewById(R.id.im_pr_result);
        qView = findViewById(R.id.im_q_result);

        im_btn_result = findViewById(R.id.im_btn_result);

        tx_oindex = findViewById(R.id.tx_goindex);
        tx_range = findViewById(R.id.tx_grange);
        tx_horn = findViewById(R.id.tx_ghorn);

        textViewPR = findViewById(R.id.tv_pr);
        textViewC = findViewById(R.id.tv_c);
        textViewQ = findViewById(R.id.tv_q);
        textViewASC = findViewById(R.id.tv_asc);

        textViewPRR = findViewById(R.id.tv_prr);
        textViewCR = findViewById(R.id.tv_cr);
        textViewQR = findViewById(R.id.tv_qr);
        textViewASCR = findViewById(R.id.tv_ascr);
        textViewCBPs = findViewById(R.id.tv_CBPs);


        initTopPop();
        im_btn_result.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                istrue = false;
                GpioNative.closeGpioDev();
                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                ThreadPoolWrapper.removeTask(parseRunnableGpio4);
                ThreadPoolWrapper.shutdown();

                bgAlpha(0.5f);

                popupWindow.showAtLocation(popView, Gravity.NO_GRAVITY, 330, 100);
                initGpio5Thread();
            }
        });


        setData();

        back = (TextView) findViewById(R.id.title_left);
        if (back == null) {
            return;
        }
        back.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View arg0) {
                istrue = false;
                istrue5 = false;
                GpioNative.closeGpioDev();
                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                ThreadPoolWrapper.removeTask(parseRunnableGpio4);
                ThreadPoolWrapper.removeTask(parseRunnableGpio5);
                ThreadPoolWrapper.shutdown();

                finish();
                Utils.exitActivityAnimation(ResultThreeActivity.this);
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
        level5 = 0;
        lastLevel5 = -1;
        initGpioThread();
    }

    private void initGpioThread() {
        istrue = true;
        GpioNative.openGpioDev();

        parseRunnableGpio1 = new Runnable() {
            @Override
            public void run() {
                while (istrue) {
                    if(MyApplication.refCount == 0){
                        istrue = false;
                        istrue5 = false;
                        GpioNative.closeGpioDev();
                        ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                        ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                        ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                        ThreadPoolWrapper.removeTask(parseRunnableGpio4);
                        ThreadPoolWrapper.removeTask(parseRunnableGpio5);
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
                                istrue5 = false;
                                GpioNative.closeGpioDev();
                                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio4);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio5);
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
                                istrue5 = false;
                                GpioNative.closeGpioDev();
                                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio4);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio5);
                                ThreadPoolWrapper.shutdown();

                                Intent intent = new Intent(ResultThreeActivity.this, PatientInfomationActivity.class);
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
                                if(Utils.nextFastClick()){
                                    ToastUtils.showToast(ResultThreeActivity.this,"点击过快");
                                    return;
                                }

                                if (hasPermission()) {
                                    doGetPDF();
                                } else {
                                    checkPermission();
                                }
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
                                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                                ThreadPoolWrapper.removeTask(parseRunnableGpio4);
                                ThreadPoolWrapper.shutdown();

                                bgAlpha(0.5f);

                                popupWindow.showAtLocation(popView, Gravity.NO_GRAVITY, 330, 100);
                                initGpio5Thread();
                            }
                        });
                    }
                }
            }
        };
        ThreadPoolWrapper.getThreadPool().executeTask(parseRunnableGpio4);
    }

    private void initGpio5Thread() {
        istrue5 = true;
        GpioNative.openGpioDev();

        parseRunnableGpio5 = new Runnable() {
            @Override
            public void run() {
                while (istrue5) {
                    try {
                        Thread.currentThread().sleep(100);
                    }catch (Exception e){
                        e.printStackTrace();
                    }

                    GpioNative.setGpioMode(1,0);
                    level4 = GpioNative.getGpioLevel(1);

                    if (lastLevel4 == level4) continue;
                    lastLevel4 = level4;

                    if(level4 == 1){
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
//                                istrue = true;

                                popupWindow.dismiss();
                                istrue5 = false;
                                GpioNative.closeGpioDev();
                                ThreadPoolWrapper.removeTask(parseRunnableGpio5);
                                ThreadPoolWrapper.shutdown();
                                initGpioThread();
                            }
                        });
                    }
                }
            }
        };
        ThreadPoolWrapper.getThreadPool().executeTask(parseRunnableGpio5);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        height = prView.getMeasuredHeight();
        width = prView.getMeasuredWidth();
        processXY();
        prView.setData(prX, prY, prRX, prRY);
        qView.setData(qX, qY, qRX, qRY);
    }

    private void processXY() {
        if(MyApplication.ecgMonitorBean == null){
            return;
        }
        float pr = MyApplication.ecgMonitorBean.getPrResult();
        float c = MyApplication.ecgMonitorBean.getcResult();

        float q = MyApplication.ecgMonitorBean.getqResult();
        float asc = MyApplication.ecgMonitorBean.getAscResult();

        float prr = MyApplication.ecgMonitorBean.getPrResultR();
        float cr = MyApplication.ecgMonitorBean.getcResultR();

        float qr = MyApplication.ecgMonitorBean.getqResultR();
        float ascr = MyApplication.ecgMonitorBean.getAscResultR();

        prX = (float) (width / 2.2 * c);// 起始坐标 0 终点2.2
        if(c > 2.2){
            prX = (float) width;
        }
        prY = (float) (height / 2.2 * pr); // 起始坐标 0 终点2.2
        if(pr > 2.2){
            prY = (float) height;
        }

        qX = (float) (width / 0.65 * (asc - 0.9)); // 起始坐标 0.9 终点1.55
        if(qX < 0){
            qX = (float) (width / 0.65 * 0);
        }
        if(asc > 1.55){
            qX = (float) (width);
        }
        qY = (float) (height / 0.6 * (q - 0.5)); // 起始坐标 0.5 终点1.1
        if(qY < 0){
            qY = (float) (height / 0.6 * 0);
        }
        if(q > 1.1){
            qY = (float) (height);
        }

        prRX = (float) (width / 2.2 * cr);// 起始坐标 0 终点2.2
        if(cr > 2.2){
            prRX = (float) width;
        }
        prRY = (float) (height / 2.2 * prr); // 起始坐标 0 终点2.2
        if(prr > 2.2){
            prRY = (float) height;
        }

        qRX = (float) (width / 0.65 * (ascr - 0.9)); // 起始坐标 0.9 终点1.55
        if(qRX < 0){
            qRX = (float) (width / 0.65 * 0);
        }
        if(ascr > 1.55){
            qRX = (float) (width);
        }
        qRY = (float) (height / 0.6 * (qr - 0.5)); // 起始坐标 0.5 终点1.1
        if(qRY < 0){
            qRY = (float) (height / 0.6 * 0);
        }
        if(qr > 1.1){
            qRY = (float) (height);
        }
    }

    private void setData() {
        if (MyApplication.ecgMonitorBean != null) {
            DecimalFormat df = new DecimalFormat("#0.00");

            textViewPR.setText(df.format(MyApplication.ecgMonitorBean.getPrResult()) + "");
            textViewC.setText(df.format(MyApplication.ecgMonitorBean.getcResult()) + "");
            textViewQ.setText(df.format(MyApplication.ecgMonitorBean.getqResult()) + "");
            textViewASC.setText(df.format(MyApplication.ecgMonitorBean.getAscResult()) + "");

            textViewPRR.setText(df.format(MyApplication.ecgMonitorBean.getPrResultR()) + "");
            textViewCR.setText(df.format(MyApplication.ecgMonitorBean.getcResultR()) + "");
            textViewQR.setText(df.format(MyApplication.ecgMonitorBean.getqResultR()) + "");
            textViewASCR.setText(df.format(MyApplication.ecgMonitorBean.getAscResultR()) + "");
            textViewCBPs.setText(df.format(MyApplication.ecgMonitorBean.getCBPs()) + "");
        }
    }

    private void initTopPop() {
        popView = LayoutInflater.from(this).inflate(R.layout.activity_pop, null);
        imgHealthyResult = popView.findViewById(R.id.healthyImg);

        int resId = 0;
        if (MyApplication.ecgMonitorBean != null) {
            resId = (int) MyApplication.ecgMonitorBean.getRect2();
        }
        imgHealthyResult.setBackgroundResource(healthyResultIds[resId]);

        btn_sure = popView.findViewById(R.id.btn_sure);
        btn_sure.setOnClickListener(this);
        popupWindow = new PopupWindow(popView, ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        popupWindow.setWidth(590);
        popupWindow.setBackgroundDrawable(new ColorDrawable(Color.GRAY));
        popupWindow.setOutsideTouchable(true);
        popupWindow.setOnDismissListener(new PopupWindow.OnDismissListener() {
            @Override
            public void onDismiss() {
                //popupwindow消失时使背景不透明
                bgAlpha(1f);
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
    public void onClick(View v) {
        Intent intent = new Intent();
        switch (v.getId()) {

            case R.id.bt_newtext:
                istrue = false;
                istrue5 = false;
                GpioNative.closeGpioDev();
                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                ThreadPoolWrapper.removeTask(parseRunnableGpio4);
                ThreadPoolWrapper.removeTask(parseRunnableGpio5);
                ThreadPoolWrapper.shutdown();

                intent = new Intent(ResultThreeActivity.this, PatientInfomationActivity.class);
                startActivity(intent);
                break;
            case R.id.bt_next:
                if(Utils.nextFastClick()){
                    ToastUtils.showToast(ResultThreeActivity.this,"点击过快");
                    return;
                }
                isAppInstalled(ResultThreeActivity.this, "org.mopria.printplugin");
                break;
            case R.id.bt_previous:
                istrue = false;
                istrue5 = false;
                GpioNative.closeGpioDev();
                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                ThreadPoolWrapper.removeTask(parseRunnableGpio2);
                ThreadPoolWrapper.removeTask(parseRunnableGpio3);
                ThreadPoolWrapper.removeTask(parseRunnableGpio4);
                ThreadPoolWrapper.removeTask(parseRunnableGpio5);
                ThreadPoolWrapper.shutdown();

                finish();

                break;

            case R.id.btn_sure:
                popupWindow.dismiss();
                istrue5 = false;
                GpioNative.closeGpioDev();
                ThreadPoolWrapper.removeTask(parseRunnableGpio5);
                ThreadPoolWrapper.shutdown();
                initGpioThread();
                break;

        }
    }
    private boolean isAppInstalled(Context context, String packagename)
    {
       /* PackageInfo packageInfo;
        try {
            packageInfo = context.getPackageManager().getPackageInfo(packagename, 0);
        }catch (PackageManager.NameNotFoundException e) {
            packageInfo = null;
            e.printStackTrace();
        }
        if(packageInfo ==null){
            showToast("请安装打印机应用");
            return false;
        }else*/{
            //检查权限
            if (hasPermission()) { Log.e("huang33332", pdfBean.getPDFByte());
                doGetPDF();

            } else {Log.e("huang4444", pdfBean.getPDFByte());
                checkPermission();

            }
            return true;
        }
    }

    public boolean hasPermission() {
        int hasWriteSdcardPermission =
                ContextCompat.checkSelfPermission(
                        ResultThreeActivity.this,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE);

        int hasReadPermission = ContextCompat.checkSelfPermission(
                ResultThreeActivity.this,
                Manifest.permission.READ_EXTERNAL_STORAGE);

        return (hasWriteSdcardPermission == PackageManager.PERMISSION_GRANTED) &&
                (hasReadPermission == PackageManager.PERMISSION_GRANTED);
    }

    public void checkPermission() {
        if (Build.VERSION.SDK_INT >= 23) {
            List<String> permissionStrs = new ArrayList<>();
            int hasWriteSdcardPermission =
                    ContextCompat.checkSelfPermission(
                            ResultThreeActivity.this,
                            Manifest.permission.WRITE_EXTERNAL_STORAGE);
            if (hasWriteSdcardPermission != PackageManager.PERMISSION_GRANTED) {
                permissionStrs.add(
                        Manifest.permission.WRITE_EXTERNAL_STORAGE
                );
            }

            int hasReadPermission = ContextCompat.checkSelfPermission(
                    ResultThreeActivity.this,
                    Manifest.permission.READ_EXTERNAL_STORAGE);
            if (hasReadPermission != PackageManager.PERMISSION_GRANTED) {
                permissionStrs.add(Manifest.permission.READ_EXTERNAL_STORAGE);
            }
            String[] stringArray = permissionStrs.toArray(new String[0]);
            if (permissionStrs.size() > 0) {
                requestPermissions(stringArray,
                        REQUEST_CODE_ASK_PERMISSIONS);
                return;
            }
        }
    }

    final private int REQUEST_CODE_ASK_PERMISSIONS = 123;

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        switch (requestCode) {
            case REQUEST_CODE_ASK_PERMISSIONS:
                //可以遍历每个权限设置情况
                if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    //这里写你需要相关权限的操作
                    //检查权限
                    doGetPDF();
                } else {
                    ToastUtils.showToast(ResultThreeActivity.this, "权限没有");
                }
        }
    }

    /**
     * 发起获取pdf请求
     */
    private void doGetPDF() {
        if (NetWorkUtils.isNetworkAvailable(ResultThreeActivity.this)) {
            System.out.println("111打印PDF当前测试号为：" + MyApplication.ecgMonitorBean.getBasisMeasurementId());
            PDFBean pdfBean = new PDFBean();
            mPresenter.getPdf(pdfBean,MyApplication.ecgMonitorBean.getBasisMeasurementId());
        } else {
            ToastUtils.showToast(ResultThreeActivity.this, R.string.no_wifi);
        }
    }

    private PDFBean pdfBean;

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    @Override
    public void onMainSuccess(BaseJsonMsg<PDFBean> o) {
        pdfBean = o.getData();

        Log.e("huang", pdfBean.getPDFByte());

        String pdfPath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/nivi/" + "io.pdf";
        //将字符串写成文件保存到本地
        FileUtils.Companion.createOrExistsFile(pdfPath);
        File file = FileUtils.Companion.getFileByPath(pdfPath);

        FileOutputStream outputStream = null;
        try {
            outputStream = new FileOutputStream(file);
            outputStream.write(Base64.decode(pdfBean.getPDFByte().getBytes(), Base64.DEFAULT));
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                outputStream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        doPdfPrint(getPdfPath());
    }

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    private void doPdfPrint(String filePath) {
        PrintManager printManager = (PrintManager) getSystemService(Context.PRINT_SERVICE);
//        if(null == printManager || printManager.getPrintJobs().size() == 0){
//            ToastUtils.showToast(ResultThreeActivity.this, "没有发现打印设备");
//            return;
//        }
        if(null == printManager){
            ToastUtils.showToast(ResultThreeActivity.this, "没有发现打印设备");
            return;
        }
        MyPrintAdapter2 myPrintAdapter = new MyPrintAdapter2(filePath);
        printManager.print("jobName", myPrintAdapter, null);
    }

    private String getPdfPath() {
        String pdfPath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/nivi/" + "io.pdf";
        File file = new File(pdfPath);
        return file.getPath();

    }
}
