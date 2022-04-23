package com.hekangyuan.nivi1000.activity.api;


import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.base.mvp.BaseView;
import com.hekangyuan.nivi1000.model.EcgMonitorBean;
import com.hekangyuan.nivi1000.model.PDFBean;

/**
 * File descripition:
 */

public interface PrintPDFView extends BaseView {
    void onMainSuccess(BaseJsonMsg<PDFBean> o);

//    void onUpLoadImgSuccess(BaseModel<Object> o);

}
