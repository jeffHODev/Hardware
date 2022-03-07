package com.hekangyuan.nivi1000.activity.presenter;

import com.hekangyuan.nivi1000.activity.api.DrawView;
import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.base.mvp.BaseObserver;
import com.hekangyuan.nivi1000.base.mvp.BasePresenter;
import com.hekangyuan.nivi1000.model.EcgMonitorBean;


public class DrawsPresenter extends BasePresenter<DrawView> {
    public DrawsPresenter(DrawView baseView) {
        super(baseView);
    }

    /**
     * 登录
     */
    public void Ecghttp(EcgMonitorBean ecgMonitorBean) {
        addDisposable(apiServer.Ecghttp(ecgMonitorBean), new BaseObserver<EcgMonitorBean>(baseView) {
            @Override
            public void onSuccess(BaseJsonMsg<EcgMonitorBean> o) {

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


}
