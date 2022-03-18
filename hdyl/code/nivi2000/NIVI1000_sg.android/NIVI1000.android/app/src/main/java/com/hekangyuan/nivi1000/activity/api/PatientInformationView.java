package com.hekangyuan.nivi1000.activity.api;


import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.base.mvp.BaseView;
import com.hekangyuan.nivi1000.model.PatientInformationBean;

/**
 * File descripition:
 */

public interface PatientInformationView extends BaseView {

    void onCreatInfoSuccess(BaseJsonMsg<PatientInformationBean> o);
    void onQueryInfoSuccess(BaseJsonMsg<PatientInformationBean> o);
    void onUpInfoSuccess(BaseJsonMsg<PatientInformationBean> o);

}
