package com.hekangyuan.nivi1000.activity.api;


import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.base.mvp.BaseView;
import com.hekangyuan.nivi1000.model.PasswordBean;

/**
 * File descripition:
 */

public interface PasswordView extends BaseView {
    void onMainSuccess(BaseJsonMsg<PasswordBean> o);
}
