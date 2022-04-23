package com.hekangyuan.nivi1000.activity;

import android.content.Intent;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.GridView;
import android.widget.SimpleAdapter;
import android.widget.TextView;

import com.hekangyuan.nivi1000.GpioNative;
import com.hekangyuan.nivi1000.MyApplication;
import com.hekangyuan.nivi1000.R;
import com.hekangyuan.nivi1000.activity.api.GetPersonaDiaView;
import com.hekangyuan.nivi1000.activity.api.UploadEcgView;
import com.hekangyuan.nivi1000.activity.presenter.GetPersonDiaListPresenter;
import com.hekangyuan.nivi1000.activity.presenter.UploadEcgPresenter;
import com.hekangyuan.nivi1000.base.BaseActivity;
import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.mock.MockDataUtil;
import com.hekangyuan.nivi1000.model.DiagnosisBean;
import com.hekangyuan.nivi1000.model.EcgMonitorBean;
import com.hekangyuan.nivi1000.model.PatientInformationBean;
import com.hekangyuan.nivi1000.utils.Constants;
import com.hekangyuan.nivi1000.utils.NetWorkUtils;
import com.hekangyuan.nivi1000.utils.ThreadPoolWrapper;
import com.hekangyuan.nivi1000.utils.ToastUtils;
import com.hekangyuan.nivi1000.utils.Utils;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


/**
 * 显示患者记录
 */
public class ShowPerinfoActivity extends BaseActivity<GetPersonDiaListPresenter> implements GetPersonaDiaView, AdapterView.OnItemClickListener {
    private PatientInformationBean person;
    private List<DiagnosisBean> mainBeanslist;
    private TextView tx_name, tx_birthday, tx_uniqueId;


    private int[] icon = {R.mipmap.ic_launcher, R.mipmap.ic_launcher, R.mipmap.ic_launcher, R.mipmap.ic_launcher, R.mipmap.ic_launcher, R.mipmap.ic_launcher, R.mipmap.ic_launcher, R.mipmap.ic_launcher, R.mipmap.ic_launcher, R.mipmap.ic_launcher,

            R.mipmap.ic_launcher, R.mipmap.ic_launcher};


    private String[] month;
    private String[] diagDate;
    private SimpleAdapter adapter;
    private List<Map<String, Object>> dataList;
    private GridView gridView;
    private String basisMeasurementId;

    private boolean istrue = true;
    int level1 = 0;
    int lastLevel1 = -1;
    private Runnable parseRunnableGpio1;

    @Override
    protected GetPersonDiaListPresenter createPresenter() {
        return new GetPersonDiaListPresenter(this);
    }

    @Override
    protected int getLayoutId() {
        return R.layout.activity_showperinfo;
    }


    @Override
    protected void initData() {

//        TextView tv_title = findViewById(R.id.tv_title);
//        tv_title.setText("历史记录");
//
//        gridView = findViewById(R.id.gridView);
//        gridView.setOnItemClickListener(this);
//        person = (PatientInformationBean) getIntent().getSerializableExtra("person");
////        mPresenter.GetPersonDiaListhttp("basisMeasurementId");
//        mPresenter.GetPersonDiaListhttp(person.getId());
//        dataList = new ArrayList<Map<String, Object>>();
//
//        tx_name = findViewById(R.id.tx_name);
//        tx_name.setText(person.getName());
//        tx_birthday = findViewById(R.id.tx_birthday);
//        tx_birthday.setText(person.getBirthday() + "");
//        tx_uniqueId = findViewById(R.id.tx_uniqueId);
//        tx_uniqueId.setText(person.getUniqueId());

    }

    @Override
    protected void onResume() {
        super.onResume();

        istrue = false;
        GpioNative.closeGpioDev();

        TextView tv_title = findViewById(R.id.tv_title);
        tv_title.setText("历史记录");

        gridView = findViewById(R.id.gridView);
        gridView.setOnItemClickListener(this);
        person = (PatientInformationBean) getIntent().getSerializableExtra("person");
//        mPresenter.GetPersonDiaListhttp("basisMeasurementId");
        mPresenter.GetPersonDiaListhttp(person.getId());
        dataList = new ArrayList<Map<String, Object>>();

        tx_name = findViewById(R.id.tx_name);
        tx_name.setText(person.getName());
//        tx_birthday = findViewById(R.id.tx_birthday);
//        tx_birthday.setText(person.getBirthday() + "");
        tx_uniqueId = findViewById(R.id.tx_uniqueId);
        tx_uniqueId.setText(person.getUniqueId());

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
                ThreadPoolWrapper.shutdown();

                finish();
                Utils.exitActivityAnimation(ShowPerinfoActivity.this);
            }
        });

        istrue = true;
        level1 = 0;
        lastLevel1 = -1;
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
                                ThreadPoolWrapper.shutdown();

                                finish();
                                Utils.exitActivityAnimation(ShowPerinfoActivity.this);
                            }
                        });
                    }
                }
            }
        };
        ThreadPoolWrapper.getThreadPool().executeTask(parseRunnableGpio1);
    }


    //数据转换list<map>转数组
    private List<Map<String, Object>> getData() {
        for (int i = 0; i < month.length; i++) {
            Map<String, Object> map = new HashMap<String, Object>();
            map.put("month", month[i]);
            map.put("diagDate", diagDate[i]);
            dataList.add(map);

        }

        return dataList;

    }

//    //点击事件
//    @Override
//    public void onItemClick(AdapterView<?> arg0, android.view.View arg1, int position, long id) {
//        ToastUtils.showToast(this,"aa");
//        Intent intent = new Intent(ShowPerinfoActivity.this, DrawsActivity.class);
//        startActivity(intent);
//
//    }

    //网络请求成功
    @Override
    public void onMainSuccess(BaseJsonMsg<List<DiagnosisBean>> o) {
        if(o.getHeader().getErrcode() == 2){
            showToast(o.getHeader().getErrmsg());
            return;
        }

        mainBeanslist = o.getData();
//        Object[] strArray = mainBeanslist.toArray();
        month = new String[mainBeanslist.size()];
        diagDate = new String[mainBeanslist.size()];
        for (int i = 0; i < mainBeanslist.size(); i++) {
            month[i] = mainBeanslist.get(i).getMonth();
            diagDate[i] = mainBeanslist.get(i).getDiagDate().substring(0, 10);
        }
        adapter = new SimpleAdapter(this, getData(), R.layout.grid_item, new String[]{"month", "diagDate"}, new int[]{R.id.tv_month, R.id.tv_diagDate});
        gridView.setAdapter(adapter);

    }

    @Override
    public void onUpLoadSuccess(BaseJsonMsg<EcgMonitorBean> o) {
        if(o.getHeader().getErrcode() == 2){
            showToast("该测量记录没有对应检测结果数据");
            return;
        }

        EcgMonitorBean ecgMonitorBean = new EcgMonitorBean();
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

        MyApplication.ecgMonitorBean = ecgMonitorBean;
        MyApplication.ecgMonitorBean.setBasisMeasurementId(basisMeasurementId);

        istrue = false;
        GpioNative.closeGpioDev();
        ThreadPoolWrapper.removeTask(parseRunnableGpio1);
        ThreadPoolWrapper.shutdown();

        Intent intent;
        intent = new Intent(ShowPerinfoActivity.this, HistoryResultOneActivity.class);
        startActivity(intent);
    }

    @Override
    public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
//        ToastUtils.showToast(this, "aa");
        basisMeasurementId = mainBeanslist.get(i).getBasisMeasurementId(); //获取当前选择的组件中测试号数据项，用来传递给后台查询结果使用；
        System.out.println("历史记录点击查看结果，测试号：" + basisMeasurementId);
        EcgMonitorBean ecgMonitorBean = new EcgMonitorBean();
        ecgMonitorBean.setIdl(-1); //使用默认选择的ID；
        ecgMonitorBean.setIdr(-1);
        ecgMonitorBean.setBasisMeasurementId(basisMeasurementId);
//        ecgMonitorBean.setBasisMeasurementId("a1ea9e35ff654b159505145b1e3b48dc");
        mPresenter.UploadHisEcghttp(ecgMonitorBean);
    }
}
