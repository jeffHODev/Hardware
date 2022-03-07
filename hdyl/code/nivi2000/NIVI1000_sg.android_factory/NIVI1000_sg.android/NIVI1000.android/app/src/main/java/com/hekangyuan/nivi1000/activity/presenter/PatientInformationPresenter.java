package com.hekangyuan.nivi1000.activity.presenter;





import android.util.Log;

import com.hekangyuan.nivi1000.activity.api.PatientInformationView;
import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.base.mvp.BaseObserver;
import com.hekangyuan.nivi1000.base.mvp.BasePresenter;
import com.hekangyuan.nivi1000.model.PatientInformationBean;


public class PatientInformationPresenter extends BasePresenter<PatientInformationView> {
    public PatientInformationPresenter(PatientInformationView baseView) {
        super(baseView);
    }



    //创建患者
    public void PatientInformationhttp(PatientInformationBean patientInformationBean) {
        addDisposable(apiServer.PatientInformationhttp(patientInformationBean), new BaseObserver(baseView) {
            @Override
            public void onSuccess(BaseJsonMsg o) {

                        baseView.onCreatInfoSuccess(o);

            }

            @Override
            public void onError(String msg) {
                if (baseView != null) {
                    baseView.showError(msg);
                }
            }
        });
    }

    //获取患者信息
    public void QueryPatInfohttp(String name,String phone) {
        addDisposable(apiServer.QueryPatInfohttp(name,phone), new BaseObserver(baseView) {
            @Override
            public void onSuccess(BaseJsonMsg o) {

                baseView.onQueryInfoSuccess(o);

            }

            @Override
            public void onError(String msg) {
                if (baseView != null) {
                    baseView.showError(msg);
                }
            }
        });
    }

    //获取患者信息
    public void QueryPatInfoByUniqueIdhttp(String uniqueId) {
        addDisposable(apiServer.QueryPatInfoByUniqueIdhttp(uniqueId), new BaseObserver(baseView) {
            @Override
            public void onSuccess(BaseJsonMsg o) {

                baseView.onQueryInfoSuccess(o);

            }

            @Override
            public void onError(String msg) {
                if (baseView != null) {
                    baseView.showError(msg);
                }
            }
        });
    }

    //修改患者信息
    public void UpPatientInformationhttp(PatientInformationBean patientInformationBean) {
        addDisposable(apiServer.UpPatientInformationhttp(patientInformationBean), new BaseObserver(baseView) {
            @Override
            public void onSuccess(BaseJsonMsg o) {

                baseView.onUpInfoSuccess(o);

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
