package com.hekangyuan.nivi1000.activity.presenter;





import android.util.Log;

import com.hekangyuan.nivi1000.activity.api.GetPersonalInformationListView;
import com.hekangyuan.nivi1000.activity.api.GetPersonalInformationView;
import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.base.mvp.BaseObserver;
import com.hekangyuan.nivi1000.base.mvp.BasePresenter;
import com.hekangyuan.nivi1000.model.PatientBaseBean;
import com.hekangyuan.nivi1000.model.PatientInformationBean;

import java.util.List;


public class GetPersonalInformationListPresenter extends BasePresenter<GetPersonalInformationListView> {
    public GetPersonalInformationListPresenter(GetPersonalInformationListView baseView) {
        super(baseView);
    }

    /**
     * 获取分页
     */
    public void GetPersonalInformationListhttp(int pageNo,int pageSize) {

        addDisposable(apiServer.GetPersonalInformationListhttp(pageNo,pageSize), new BaseObserver<PatientBaseBean>(baseView) {
            @Override
            public void onSuccess(BaseJsonMsg<PatientBaseBean> o) {
                baseView.onMainSuccess(o);
            }

            @Override
            public void onError(String msg) {
                if (baseView != null) {
                    baseView.showError(msg);
                }
            }
        });
    }

    /**
     * 获取单个
     */
    public void GetPersonalInformationhttp(String  cardid) {

        addDisposable(apiServer.GetPersonalInformationhttp(cardid), new BaseObserver<PatientBaseBean>(baseView) {
            @Override
            public void onSuccess(BaseJsonMsg<PatientBaseBean> o) {
                baseView.onPerSuccess(o);
            }

            @Override
            public void onError(String msg) {
                if (baseView != null) {
                    baseView.showError(msg);
                }
            }
        });
    }
}
