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
import com.hekangyuan.nivi1000.mock.MockDataUtil;
import com.hekangyuan.nivi1000.model.AnalysisBean;
import com.hekangyuan.nivi1000.model.EcgMonitorBean;
import com.hekangyuan.nivi1000.model.dbbean.PatientdbBean;
import com.hekangyuan.nivi1000.utils.NetWorkUtils;
import com.hekangyuan.nivi1000.utils.ThreadPoolWrapper;
import com.hekangyuan.nivi1000.utils.ToastUtils;
import com.hekangyuan.nivi1000.utils.Utils;
import com.hekangyuan.nivi1000.view.graphViewForLive.AnalysisView;
import com.hekangyuan.nivi1000.view.graphViewForLive.BufView;
import com.hekangyuan.nivi1000.view.graphViewForLive.SideView;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

import static com.hekangyuan.nivi1000.utils.Constants.ANALYSIS_ECGTITLE;
import static com.hekangyuan.nivi1000.utils.Constants.ANALYSIS_TITLE;


public class AnalysisActivity extends BaseActivity<UploadEcgPresenter> implements UploadEcgView, View.OnClickListener {
    public Button bt_newtext, bt_next, bt_previous;
    public TextView tv_ecg, tv_heart_sounds, tv_pulse, tv_title, tx_zhione, tx_zhitwo, tx_zhithr, path_text5, firstview, secondview, thirdview, fifthview, fourthview,
            firstview2, secondview2, thirdview2, fifthview2, fourthview2;
    AnalysisView analysisView0, analysisView1, analysisView2;
    SideView analysisView3, analysisView4;
    BufView analysisView5;

    public static float widthPerPointone, widthBuff;
    private int step = 0;

    private int viewHeight = 0;
    private int viewWidth = 0;

    private int viewHeight1 = 0;

    private AnalysisBean analysisBean; //绘制数据封装类

    private EcgMonitorBean ecgMonitorBean;
    private EcgMonitorBean mockEcgMonitorBean;

    private RelativeLayout layoutThird, layoutMergeViews;

    private int idL, idR;

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
    protected UploadEcgPresenter createPresenter() {
        return new UploadEcgPresenter(this);
    }

    @Override
    protected int getLayoutId() {
        return R.layout.activity_analysis;
    }

    @Override
    protected void initData() {
//        initView();
//        initListener();
//        initAnalysisBean();
//        initKeyMonitor();
    }

    private void initKeyMonitor() {

        keyMonitorListener = new KeyMonitorListener() {
            @Override
            public void onPreviousClick() {
                doPrevious();
            }

            @Override
            public void onNextClick() {
                doNext();
            }

            @Override
            public void onMiddleClick() {
                doMiddle();
            }
        };
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

        initView();
        initListener();
        initAnalysisBean();
        initKeyMonitor();

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

//                finish();
//                Utils.exitActivityAnimation(AnalysisActivity.this);
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
                                if(Utils.nextFastClick()){
                                    ToastUtils.showToast(AnalysisActivity.this,"点击过快");
                                    return;
                                }

                                doPrevious();
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
                                    ToastUtils.showToast(AnalysisActivity.this,"点击过快");
                                    return;
                                }

                                doMiddle();
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
                                    ToastUtils.showToast(AnalysisActivity.this,"点击过快");
                                    return;
                                }

                                doNext();
                            }
                        });
                    }
                }
            }
        };
        ThreadPoolWrapper.getThreadPool().executeTask(parseRunnableGpio3);
    }

    private void initListener() {
        bt_newtext.setOnClickListener(this);
        bt_next.setOnClickListener(this);
        bt_previous.setOnClickListener(this);

        analysisView0.setListener(new AnalysisView.TouchDataCallBackListener() {
            @Override
            public void callBack(List<Float> mainPointList, int id) {
                analysisView3.setMainData(mainPointList);
                analysisView3.performDraw();
                idL = id - 1;
                if (analysisBean != null) {
                    analysisBean.setIDL(idL);
                }
            }
        });

        analysisView1.setListener(new AnalysisView.TouchDataCallBackListener() {
            @Override
            public void callBack(List<Float> mainPointList, int id) {
                analysisView4.setMainData(mainPointList);
                analysisView4.performDraw();
                idR = id - 1;
                if (analysisBean != null) {
                    analysisBean.setIDR(idR);
                }
            }
        });

        analysisView5.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        Intent intent = new Intent();
        switch (v.getId()) {

            case R.id.bt_newtext:
                if(Utils.nextFastClick()){
                    ToastUtils.showToast(AnalysisActivity.this,"点击过快");
                    return;
                }

                doMiddle();
                break;

            case R.id.bt_next:
                if(Utils.nextFastClick()){
                    ToastUtils.showToast(AnalysisActivity.this,"点击过快");
                    return;
                }

                doNext();
                break;

            case R.id.bt_previous:
                if(Utils.nextFastClick()){
                    ToastUtils.showToast(AnalysisActivity.this,"点击过快");
                    return;
                }

                doPrevious();
                break;

        }
    }

    private void doPrevious() {
        //上一步
        if (step == 0) {
//            finish();
            return;
        }
        step--;
        setViewStatus();
        drawGraphView();
    }

    private void doNext() {
        Intent intent;
        if (step == 2) {
            istrue = false;
            GpioNative.closeGpioDev();
            ThreadPoolWrapper.removeTask(parseRunnableGpio1);
            ThreadPoolWrapper.removeTask(parseRunnableGpio2);
            ThreadPoolWrapper.removeTask(parseRunnableGpio3);
            ThreadPoolWrapper.shutdown();

            intent = new Intent(AnalysisActivity.this, ResultOneActivity.class);
            startActivity(intent);
            return;
        }

        step++;
        setViewStatus();
        drawGraphView();
    }

    private void doMiddle() {
        Intent intent;
        if (step == 2) {
            uploadInfo();
            return;
        }

        istrue = false;
        GpioNative.closeGpioDev();
        ThreadPoolWrapper.removeTask(parseRunnableGpio1);
        ThreadPoolWrapper.removeTask(parseRunnableGpio2);
        ThreadPoolWrapper.removeTask(parseRunnableGpio3);
        ThreadPoolWrapper.shutdown();

        intent = new Intent(AnalysisActivity.this, PatientInfomationActivity.class);
        startActivity(intent);
        finish();
    }

    /**
     * 上传信息
     */
    private void uploadInfo() {
        System.out.println("点击重新分析的上传");
        if (ecgMonitorBean == null) {
            return;
        }

        ecgMonitorBean.setIDL(idL);
        ecgMonitorBean.setIDR(idR);
        ecgMonitorBean.setIdl(idL);
        ecgMonitorBean.setIdr(idR);
        if(ecgMonitorBean.getHypecgs() == null || ecgMonitorBean.getHypecgs().length == 0) {
            MockDataUtil.initFourteenDataByMock(ecgMonitorBean, AnalysisActivity.this);
        }
        mPresenter.UploadEcghttp(ecgMonitorBean);
    }

    private void initView() {
        tv_title = findViewById(R.id.tv_title);
        tv_heart_sounds = findViewById(R.id.tv_heart_sounds_an);
        tv_pulse = findViewById(R.id.tv_pulse_an);
        tv_ecg = findViewById(R.id.tv_ecg_an);
        bt_next = findViewById(R.id.bt_next);
        bt_previous = findViewById(R.id.bt_previous);
        tx_zhione = findViewById(R.id.tx_zhione);
        tx_zhitwo = findViewById(R.id.tx_zhitwo);
        tx_zhithr = findViewById(R.id.tx_zhithr);
        path_text5 = findViewById(R.id.path_text5);
        firstview = findViewById(R.id.firstview);
        secondview = findViewById(R.id.secondview);
        thirdview = findViewById(R.id.thirdview);
        fifthview = findViewById(R.id.fifthview);
        fourthview = findViewById(R.id.fourthview);
        firstview2 = findViewById(R.id.firstview2);
        secondview2 = findViewById(R.id.secondview2);
        thirdview2 = findViewById(R.id.thirdview2);
        fifthview2 = findViewById(R.id.fifthview2);
        fourthview2 = findViewById(R.id.fourthview2);
        bt_newtext = findViewById(R.id.bt_newtext);

        analysisView0 = findViewById(R.id.path_view0);
        analysisView1 = findViewById(R.id.path_view1);
        analysisView2 = findViewById(R.id.path_view2);

        analysisView3 = findViewById(R.id.path_view3);
        analysisView4 = findViewById(R.id.path_view4);
        analysisView5 = findViewById(R.id.path_view5);

        layoutThird = findViewById(R.id.layout_third);
        layoutMergeViews = findViewById(R.id.layout_merge_views);

        setEachTitle();
    }

    /**
     * 初始化bean
     */
    private void initAnalysisBean() {

        ecgMonitorBean = MyApplication.ecgMonitorBean;
        analysisBean = MockDataUtil.initByEcgMonitorBean(ecgMonitorBean, AnalysisActivity.this);

//        ecgMonitorBean = MockDataUtil.getEcgMonitorBean(this);
//        mockEcgMonitorBean = MockDataUtil.getEcgMonitorBean(this);
//        analysisBean = MockDataUtil.initByEcgMonitorBean(ecgMonitorBean,mockEcgMonitorBean); //兼容模式/**/

//        analysisBean = MockDataUtil.initByMockData(mContext); //全mock
        setEachData();

    }


    public void setViewStatus() {
        if (step == 0) {
            bt_next.setText("下一步");
            bt_previous.setText("上一步");
            bt_newtext.setText("新建");
            analysisView0.setClickable(false);
            analysisView1.setClickable(false);

            showMergeViews(false);
        }

        if (step == 1) {
            bt_next.setText("下一步");
            bt_previous.setText("上一步");
            bt_newtext.setText("新建");

            analysisView0.setClickable(false);
            analysisView1.setClickable(false);

            showMergeViews(false);
        }
        if (step == 2) {
            bt_next.setText("结果");
            bt_newtext.setText("重新分析");

            analysisView0.setClickable(true);
            analysisView1.setClickable(true);

            showMergeViews(true);
        }

        setEachTitle();
    }

    private void showMergeViews(boolean isShown) {
        if (isShown) {
            layoutMergeViews.setVisibility(View.VISIBLE);
            layoutThird.setVisibility(View.GONE);
        } else {
            layoutMergeViews.setVisibility(View.GONE);
            layoutThird.setVisibility(View.VISIBLE);
        }
    }

    private void setEachTitle() {
        if (step < 0) {
            step = 0;
        }
        if (step > 2) {
            step = 2;
        }
        tv_ecg.setText(ANALYSIS_ECGTITLE[step][0]);
        tv_title.setText(ANALYSIS_TITLE[step]);
        tv_heart_sounds.setText(ANALYSIS_ECGTITLE[step][1]);
        tv_pulse.setText(ANALYSIS_ECGTITLE[step][2]);
    }


    /**
     * 执行画图逻辑
     */
    private void drawGraphView() {
//        ThreadPoolWrapper.getThreadPool().executeTask(new Runnable() {
//            @Override
//            public void run() {
//                runOnUiThread(new Runnable() {
//                    @Override
//                    public void run() {
//                        setEachData();
//                    }
//                });
//            }
//        });
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                setEachData();
            }
        });
    }

    /**
     * 为每个pathGraphView填充数据
     */
    private void setEachData() {
        analysisView0.initViewSize(widthPerPointone, viewHeight);
        analysisView1.initViewSize(widthPerPointone, viewHeight1);
        analysisView2.initViewSize(widthPerPointone, viewHeight);
        analysisView3.initViewSize(widthBuff, viewHeight);
        analysisView4.initViewSize(widthBuff, viewHeight);
        analysisView5.initViewSize(widthBuff, viewHeight);

        switch (step) {
            case 0: //超收缩压
                analysisView0.setMainData(analysisBean.getHypecgs(), R.color.lawngreen);
                analysisView1.setMainData(analysisBean.getHyppcgs(), R.color.ecgline);
                analysisView2.setMainData(analysisBean.getHypapgs(), R.color.yellow);

                analysisView0.setrPointList(analysisBean.getHypecgsRtops()); // 设置心电 Rtops
                analysisView0.setUsPointList(null);
                analysisView0.setUtPointList(null);
                analysisView0.setDnPointList(null);
                analysisView0.setRtopPointList(null);


                analysisView1.setiPointList(analysisBean.getHyppcgsIstarts());
                analysisView1.setIiPointList(analysisBean.getHyppcgsIIstarts());
                analysisView1.setUsPointList(null);
                analysisView1.setUtPointList(null);
                analysisView1.setDnPointList(null);
                analysisView1.setRtopPointList(null);


                analysisView2.setUsPointList(analysisBean.getHypapgsUSpnts());
                analysisView2.setDnPointList(analysisBean.getHypapgsDNpnts());
                analysisView2.setUtPointList(analysisBean.getHypapgsUTpnts());


                break;

            case 1: //低舒张压

                analysisView0.setMainData(analysisBean.getHypecgd(), R.color.lawngreen);
                analysisView1.setMainData(analysisBean.getHyppcgd(), R.color.ecgline);
                analysisView2.setMainData(analysisBean.getHypapgd(), R.color.yellow);

                analysisView0.setrPointList(analysisBean.getHypecgdRtops()); // 设置心电 Rtops
                analysisView0.setUsPointList(null);
                analysisView0.setUtPointList(null);
                analysisView0.setDnPointList(null);
                analysisView0.setRtopPointList(null);

                analysisView1.setiPointList(analysisBean.getHyppcgdIstarts());
                analysisView1.setIiPointList(analysisBean.getHyppcgdIIstarts());
                analysisView1.setUsPointList(null);
                analysisView1.setUtPointList(null);
                analysisView1.setDnPointList(null);
                analysisView1.setRtopPointList(null);

                //TODO sublist num
                analysisView2.setUsPointList(analysisBean.getHypapgdUSpnts());
                analysisView2.setDnPointList(analysisBean.getHypapgdDNpnts());
                analysisView2.setUtPointList(analysisBean.getHypapgdUTpnts());

                break;

            case 2: //左右脉搏
//                analysisView0.setMainData(Hello.filter(analysisBean.getApgL()),widthPerPointone,viewHeight);

                analysisView0.setMainData1(analysisBean.getApgL());
                analysisView1.setMainData1(analysisBean.getApgR());

                analysisView0.setRtopPointList(analysisBean.getRtopL());
                analysisView0.setUtPointList(analysisBean.getUTL());
                analysisView0.setDnPointList(analysisBean.getTsL());
                analysisView0.setrPointList(null);


                analysisView1.setiPointList(null);
                analysisView1.setIiPointList(null);
                analysisView1.setRtopPointList(analysisBean.getRtopR());
                analysisView1.setUtPointList(analysisBean.getUTR());
                analysisView1.setDnPointList(analysisBean.getTsR());

                //TODO 需要计算analysisView的宽度，根据宽度来绘制
                //通过IDL和IDR计算两个view的leftIndex和rightIndex

                idL = (int) analysisBean.getIDL();
                idR = (int) analysisBean.getIDR();
                int lStart = 0, lEnd = 0;
                if (analysisBean.getRtopL() != null) {
                    if (idL >= 0) {
                        lStart = analysisBean.getRtopL().get(idL);
                    }

                    if (idL < analysisBean.getRtopL().size()) {
                        lEnd = analysisBean.getRtopL().get(idL + 1);
                    }


                    analysisView0.setStartX(lStart);
                    analysisView0.setEndX(lEnd);
                }

                int rStart = 0, rEnd = 0;
                if (analysisBean.getRtopR() != null) {
                    if (idR >= 0) {
                        rStart = analysisBean.getRtopR().get(idR);
                    } else {
                        rStart = 0;
                    }
                    if (idR < analysisBean.getRtopR().size()) {
                        rEnd = analysisBean.getRtopR().get(idR + 1);
                    } else {
                        rEnd = 5000;
                    }

                    analysisView1.setStartX(rStart);
                    analysisView1.setEndX(rEnd);
                }
                if (analysisBean.getApgL() != null) {
                    analysisView3.setMainDataL(analysisBean.getApgL().subList(lStart, lEnd), analysisBean.getTsL().get(idL) - analysisBean.getRtopL().get(idL) - 1, analysisBean.getUTL().get(idL) - analysisBean.getRtopL().get(idL) - 1);
                }
                if (analysisBean.getApgR() != null) {
                    analysisView4.setMainDataR(analysisBean.getApgR().subList(rStart, rEnd), analysisBean.getTsR().get(idR) - analysisBean.getRtopR().get(idR) - 1, analysisBean.getUTR().get(idR) - analysisBean.getRtopR().get(idR) - 1);
                }
                if (analysisBean.getBuf() != null) {
                    analysisView5.setMainData(analysisBean.getBuf());
                }

                path_text5.setText(String.format("CSBP = %.2f", analysisBean.getCBPs()));
                firstview.setText(String.format("脉波周期 = %.2fs", analysisBean.getjTL()));
                secondview.setText(String.format("收 缩 期 = %.2fs", analysisBean.getjTsL()));
                thirdview.setText(String.format("Pss = %.2f", analysisBean.getjPssL()));
                fifthview.setText(String.format("As = %.2f", analysisBean.getjAsL()));
                fourthview.setText(String.format("Ad = %.2f", analysisBean.getjAdL()));
                firstview2.setText(String.format("脉波周期 = %.2fs", analysisBean.getjTR()));
                secondview2.setText(String.format("收 缩 期 = %.2fs", analysisBean.getjTsR()));
                thirdview2.setText(String.format("Pss = %.2f", analysisBean.getjPssR()));
                fifthview2.setText(String.format("As = %.2f", analysisBean.getjAsR()));
                fourthview2.setText(String.format("Ad = %.2f", analysisBean.getjAdR()));

                break;
        }

        analysisView0.performDraw();
        analysisView1.performDraw();
        analysisView2.performDraw();

        analysisView3.performDraw();
        analysisView4.performDraw();
        analysisView5.performDraw();
    }


    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        viewHeight = analysisView0.getMeasuredHeight();
        viewWidth = analysisView0.getMeasuredWidth();

        viewHeight1 = analysisView1.getMeasuredHeight();
        widthPerPointone = (float) viewWidth / 5000;

        widthBuff = (float) viewWidth / 3;
        Log.e("huang", "onWindowFocusChanged----");
        drawGraphView();
    }

    @Override
    public void onMainSuccess(BaseJsonMsg<EcgMonitorBean> o) {

        ecgMonitorBean.setBuf(o.getData().getBuf()); //分析出来的波形结果
        ecgMonitorBean.setIDL(o.getData().getIDL()); //左颈动脉选择波形
        ecgMonitorBean.setIDR(o.getData().getIDR()); //右颈动脉选择波形


        analysisBean.setBuf(Arrays.asList(o.getData().getBuf()));
        analysisBean.setCBPs(o.getData().getCBPs());
        analysisBean.setjAdL(o.getData().getjAdL());
        analysisBean.setjAdR(o.getData().getjAdR());
        analysisBean.setjAsL(o.getData().getjAsL());
        analysisBean.setjAsR(o.getData().getjAsR());
        analysisBean.setjPssL(o.getData().getjPssL());
        analysisBean.setjPssR(o.getData().getjPssR());
        analysisBean.setjTL(o.getData().getjTL());
        analysisBean.setjTR(o.getData().getjTR());
        analysisBean.setjTsL(o.getData().getjTsL());
        analysisBean.setjTsR(o.getData().getjTsR());

        MyApplication.ecgMonitorBean = ecgMonitorBean;
//        drawGraphView();  //later than up method

        analysisView5.setMainData(analysisBean.getBuf());
        analysisView5.performDraw();
        path_text5.setText(String.format("CSBP = %.2f", o.getData().getCBPs()));
        firstview.setText(String.format("脉波周期 = %.2fs", o.getData().getjTL()));
        secondview.setText(String.format("收 缩 期 = %.2fs", o.getData().getjTsL()));
        thirdview.setText(String.format("Pss = %.2f", o.getData().getjPssL()));
        fifthview.setText(String.format("As = %.2f", o.getData().getjAsL()));
        fourthview.setText(String.format("Ad = %.2f", o.getData().getjAdL()));
        firstview2.setText(String.format("脉波周期 = %.2fs", o.getData().getjTR()));
        secondview2.setText(String.format("收 缩 期 = %.2fs", o.getData().getjTsR()));
        thirdview2.setText(String.format("Pss = %.2f", o.getData().getjPssR()));
        fifthview2.setText(String.format("As = %.2f", o.getData().getjAsR()));
        fourthview2.setText(String.format("Ad = %.2f", o.getData().getjAdR()));
    }


}
