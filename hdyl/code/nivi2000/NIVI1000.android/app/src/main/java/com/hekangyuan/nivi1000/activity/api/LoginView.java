package com.hekangyuan.nivi1000.activity.api;


import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.base.mvp.BaseView;
import com.hekangyuan.nivi1000.model.LoginBean;

/**
 * File descripition:
 */

public interface LoginView extends BaseView {
    void onMainSuccess(BaseJsonMsg<LoginBean> o);

//    void onUpLoadImgSuccess(BaseModel<Object> o);

}
