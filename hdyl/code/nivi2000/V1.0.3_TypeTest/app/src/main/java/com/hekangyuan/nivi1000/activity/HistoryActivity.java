package com.hekangyuan.nivi1000.activity;

import android.content.Intent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.hekangyuan.nivi1000.GpioNative;
import com.hekangyuan.nivi1000.MyApplication;
import com.hekangyuan.nivi1000.R;
import com.hekangyuan.nivi1000.activity.api.GetPersonalInformationListView;
import com.hekangyuan.nivi1000.activity.presenter.GetPersonalInformationListPresenter;
import com.hekangyuan.nivi1000.adapter.PatientInfoAdapter;
import com.hekangyuan.nivi1000.base.BaseActivity;
import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.model.PatientBaseBean;
import com.hekangyuan.nivi1000.model.PatientInformationBean;
import com.hekangyuan.nivi1000.utils.NetWorkUtils;
import com.hekangyuan.nivi1000.utils.ThreadPoolWrapper;
import com.hekangyuan.nivi1000.utils.ToastUtils;
import com.hekangyuan.nivi1000.utils.Utils;
import com.hekangyuan.nivi1000.xview.XListView;

import java.util.ArrayList;
import java.util.List;


/**
 * 历史记录
 */
public class HistoryActivity extends BaseActivity<GetPersonalInformationListPresenter> implements GetPersonalInformationListView, XListView.IXListViewListener, View.OnClickListener {
    private XListView mListView;
    private PatientInfoAdapter adapter;
    private int start = 1;
//    private static int refreshCnt = 1;
    private PatientBaseBean patientBaseBean;
    private List<PatientInformationBean> patientInformationBeanList = new ArrayList<PatientInformationBean>();

    private RelativeLayout im_search;
    private EditText et_search_idcode;

    private boolean istrue = true;
    int level1 = 0;
    int lastLevel1 = -1;
    private Runnable parseRunnableGpio1;

    @Override
    protected GetPersonalInformationListPresenter createPresenter() {
        return new GetPersonalInformationListPresenter(this);
    }

    @Override
    protected int getLayoutId() {
        return R.layout.activity_main;
    }


    @Override
    public void onErrorCode(BaseJsonMsg model) {
        //super.onErrorCode(model);
        //我要自定义
        //实现自己的逻辑
    }

    @Override
    public void showError(String msg) {

    }


    @Override
    protected void initData() {

//        TextView tv_title = findViewById(R.id.tv_title);
//        tv_title.setText("历史记录");
//        im_search = findViewById(R.id.im_search);
//        im_search.setOnClickListener(this);
//        mListView = findViewById(R.id.xListView);
//        mListView.setPullLoadEnable(true);
//        mListView.setXListViewListener(this);
//        et_search_idcode = findViewById(R.id.et_search_idcode);
//
//        getdata();


    }

    @Override
    protected void onResume() {
        super.onResume();

        start = 1;
        patientInformationBeanList.clear();

        istrue = false;
        GpioNative.closeGpioDev();

        TextView tv_title = findViewById(R.id.tv_title);
        tv_title.setText("历史记录");
        im_search = findViewById(R.id.im_search);
        im_search.setOnClickListener(this);
        mListView = findViewById(R.id.xListView);
        mListView.setPullLoadEnable(true);
        mListView.setXListViewListener(this);
        et_search_idcode = findViewById(R.id.et_search_idcode);

        getdata();

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
                Utils.exitActivityAnimation(HistoryActivity.this);
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
                                Utils.exitActivityAnimation(HistoryActivity.this);
                            }
                        });
                    }

                }
            }
        };
        ThreadPoolWrapper.getThreadPool().executeTask(parseRunnableGpio1);
    }

    private void getdata() {

        if (NetWorkUtils.isNetworkAvailable(HistoryActivity.this)) {


            mPresenter.GetPersonalInformationListhttp(start, 3);
        } else {
            ToastUtils.showToast(HistoryActivity.this, R.string.no_wifi);
        }


    }


    private void geneItems() {
        adapter = new PatientInfoAdapter(HistoryActivity.this, patientInformationBeanList);
        mListView.setAdapter(adapter);
        mListView.setOnItemClickListener(new ListView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                istrue = false;
                GpioNative.closeGpioDev();
                ThreadPoolWrapper.removeTask(parseRunnableGpio1);
                ThreadPoolWrapper.shutdown();

                Intent intent = new Intent(HistoryActivity.this, ShowPerinfoActivity.class);
                intent.putExtra("person", patientInformationBeanList.get(position - 1));
                startActivity(intent);


            }
        });

    }

    private void onLoad() {
        start = start + 1;
        mListView.stopRefresh();
        mListView.stopLoadMore();
        mListView.setRefreshTime("刚刚");
    }
//
    @Override
    public void onRefresh() {
//        start = 1;
        start = start + 1;
        adapter.notifyDataSetChanged();
        getdata();

        onLoad();

    }

    @Override
    public void onLoadMore() {
//        start = ++refreshCnt;
        start = start + 1;
        getdata();
        adapter.notifyDataSetChanged();
        onLoad();
    }


    @Override
    public void onMainSuccess(BaseJsonMsg<PatientBaseBean> o) {
        if(o.getHeader().getErrcode() == 2){
            showToast(o.getHeader().getErrmsg());
            return;
        }

        patientBaseBean = o.getData();
        int count = patientBaseBean.getCount();
        if (patientInformationBeanList.size() > 0) {
            if (patientInformationBeanList.size() == count) {
                showToast("已经没有新的数据了");
            } else {
                patientInformationBeanList.addAll(patientBaseBean.getPatientList());
            }
        } else {
            patientInformationBeanList = patientBaseBean.getPatientList();
        }
        geneItems();
    }

    @Override
    public void onPerSuccess(BaseJsonMsg<PatientBaseBean> o) {
        if(o.getHeader().getErrcode() == 2){
//            patientInformationBeanList.clear();
            showToast(o.getHeader().getErrmsg());
            return;
        }

        patientInformationBeanList.clear();
        patientBaseBean = o.getData();
        patientInformationBeanList = patientBaseBean.getPatientList();
        geneItems();
    }


    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.im_search:
                if(et_search_idcode.getText().toString().trim().length() == 0){
                    ToastUtils.showToast(HistoryActivity.this,"请输入身份证号码进行查询");
                    break;
                }
                mPresenter.GetPersonalInformationhttp(et_search_idcode.getText().toString());
                break;
            default:
        }
    }
}
