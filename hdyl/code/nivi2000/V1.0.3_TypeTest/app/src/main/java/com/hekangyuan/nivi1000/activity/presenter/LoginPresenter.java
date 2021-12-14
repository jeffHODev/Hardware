package com.hekangyuan.nivi1000.activity.presenter;





import android.util.Log;

import com.hekangyuan.nivi1000.activity.api.LoginView;
import com.hekangyuan.nivi1000.activity.api.MainView;
import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.base.mvp.BaseObserver;
import com.hekangyuan.nivi1000.base.mvp.BasePresenter;
import com.hekangyuan.nivi1000.model.LoginBean;


public class LoginPresenter extends BasePresenter<LoginView> {
    public LoginPresenter(LoginView baseView) {
        super(baseView);
    }

    /**
     * 登录
     */
    public void Loginhttp(LoginBean loginBean) {
        addDisposable(apiServer.Loginhttp(loginBean), new BaseObserver<LoginBean>(baseView) {
            @Override
            public void onSuccess(BaseJsonMsg<LoginBean> o) {

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
     * 修改密码
     */
    public void modifyPassHttp(LoginBean loginBean) {
        addDisposable(apiServer.Loginhttp(loginBean), new BaseObserver<LoginBean>(baseView) {
            @Override
            public void onSuccess(BaseJsonMsg<LoginBean> o) {

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
