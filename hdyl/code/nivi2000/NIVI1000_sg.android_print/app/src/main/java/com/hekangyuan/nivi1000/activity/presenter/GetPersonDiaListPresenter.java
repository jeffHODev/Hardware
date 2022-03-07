package com.hekangyuan.nivi1000.activity.presenter;





import com.hekangyuan.nivi1000.activity.api.GetPersonaDiaView;
import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.base.mvp.BaseObserver;
import com.hekangyuan.nivi1000.base.mvp.BasePresenter;
import com.hekangyuan.nivi1000.model.DiagnosisBean;
import com.hekangyuan.nivi1000.model.EcgMonitorBean;
import com.hekangyuan.nivi1000.model.PatientBaseBean;

import java.util.List;


public class GetPersonDiaListPresenter extends BasePresenter<GetPersonaDiaView> {
    public GetPersonDiaListPresenter(GetPersonaDiaView baseView) {
        super(baseView);
    }

    /**
     * 获取单个患者诊断列表
     */
    public void GetPersonDiaListhttp(String  aa) {
        addDisposable(apiServer.GetPersonDiaListhttp(aa), new BaseObserver<List<DiagnosisBean>>(baseView) {
            @Override
            public void onSuccess(BaseJsonMsg<List<DiagnosisBean>> o) {

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
     * 上传心电数据
     */
    public void UploadHisEcghttp(EcgMonitorBean ecgMonitorBean) {

        addDisposable(apiServer.UploadEcghttp(ecgMonitorBean), new BaseObserver<EcgMonitorBean>(baseView) {
            @Override
            public void onSuccess(BaseJsonMsg<EcgMonitorBean> o) {

                baseView.onUpLoadSuccess(o);
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
