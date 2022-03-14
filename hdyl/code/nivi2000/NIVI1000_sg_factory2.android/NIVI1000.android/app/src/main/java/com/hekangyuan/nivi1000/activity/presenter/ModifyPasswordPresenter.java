package com.hekangyuan.nivi1000.activity.presenter;





import com.hekangyuan.nivi1000.activity.api.LoginView;
import com.hekangyuan.nivi1000.activity.api.PasswordView;
import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.base.mvp.BaseObserver;
import com.hekangyuan.nivi1000.base.mvp.BasePresenter;
import com.hekangyuan.nivi1000.model.LoginBean;
import com.hekangyuan.nivi1000.model.PasswordBean;


public class ModifyPasswordPresenter extends BasePresenter<PasswordView> {
    public ModifyPasswordPresenter(PasswordView baseView) {
        super(baseView);
    }


    /**
     * 修改密码
     */
    public void modifyPassHttp(PasswordBean passwordBean) {
        addDisposable(apiServer.modifyPasswordhttp(passwordBean), new BaseObserver<PasswordBean>(baseView) {

            @Override
            public void onSuccess(BaseJsonMsg<PasswordBean> o) {
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
