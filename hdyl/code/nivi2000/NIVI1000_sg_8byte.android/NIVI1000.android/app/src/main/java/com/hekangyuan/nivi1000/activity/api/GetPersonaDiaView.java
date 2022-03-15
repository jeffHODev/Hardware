package com.hekangyuan.nivi1000.activity.api;


import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.base.mvp.BaseView;
import com.hekangyuan.nivi1000.model.DiagnosisBean;
import com.hekangyuan.nivi1000.model.EcgMonitorBean;
import com.hekangyuan.nivi1000.model.PatientInformationBean;

import java.util.List;

/**
 * File descripition:
 */

public interface GetPersonaDiaView extends BaseView {
    void onMainSuccess(BaseJsonMsg<List<DiagnosisBean>> o);
    void onUpLoadSuccess(BaseJsonMsg<EcgMonitorBean> o);

//    void onUpLoadImgSuccess(BaseModel<Object> o);

}
