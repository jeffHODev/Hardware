package com.hekangyuan.nivi1000.activity.presenter;





import com.hekangyuan.nivi1000.activity.api.PrintPDFView;
import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.base.mvp.BaseObserver;
import com.hekangyuan.nivi1000.base.mvp.BasePresenter;
import com.hekangyuan.nivi1000.model.PDFBean;


public class PringtPDFPresenter extends BasePresenter<PrintPDFView> {
    public PringtPDFPresenter(PrintPDFView baseView) {
        super(baseView);
    }

    /**
     * 登录
     */
    public void getPdf(PDFBean pdfBean,String basisMeasurementId) {

        addDisposable(apiServer.GetPdfHttp(basisMeasurementId), new BaseObserver<PDFBean>(baseView) {
            @Override
            public void onSuccess(BaseJsonMsg<PDFBean> o) {

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



//    /**
//     * 写法好多种  怎么顺手怎么来
//     */
//    public void getManApi() {
//        addDisposable(apiServer.getMain("year"), new BaseObserver(baseView) {
//            @Override
//            public void onSuccess(BaseModel o) {
//                baseView.onMainSuccess((BaseModel<List<MainBean>>) o);
//            }
//
//            @Override
//            public void onError(String msg) {
//                if (baseView != null) {
//                    baseView.showError(msg);
//                }
//            }
//        });
//    }
//
//    /**
//     * 写法好多种  怎么顺手怎么来
//     */
//    public void getMan2Api() {
//        addDisposable(apiServer.getMain2("year"), new BaseObserver(baseView) {
//            @Override
//            public void onSuccess(BaseModel o) {
//                baseView.onMainSuccess((BaseModel<List<MainBean>>) o);
//            }
//
//            @Override
//            public void onError(String msg) {
//                if (baseView != null) {
//                    baseView.showError(msg);
//                }
//            }
//        });
//    }
//
//
//
//    /**
//     * 写法好多种  怎么顺手怎么来
//     */
//    public void getMan3Api() {
//        HashMap<String, String> params = new HashMap<>();
//        params.put("time", "year");
//        addDisposable(apiServer.getMain3(params), new BaseObserver(baseView) {
//
//            @Override
//            public void onSuccess(BaseModel o) {
//                baseView.onMainSuccess((BaseModel<List<MainBean>>) o);
//            }
//
//            @Override
//            public void onError(String msg) {
//                if (baseView != null) {
//                    baseView.showError(msg);
//                }
//            }
//        });
//    }
//
//
//    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  图片上传  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
//
//    /**
//     * 演示单图上传
//     *
//     * @param parts
//     */
//    public void upLoadImgApi(MultipartBody.Part parts) {
//        addDisposable(apiServer.upLoadImg(parts), new BaseObserver(baseView) {
//
//            @Override
//            public void onSuccess(BaseModel o) {
//                baseView.onUpLoadImgSuccess((BaseModel<Object>) o);
//            }
//
//            @Override
//            public void onError(String msg) {
//                if (baseView != null) {
//                    baseView.showError(msg);
//                }
//            }
//        });
//    }
//
//
//    /**
//     * 演示多图上传
//     *
//     * @param parts
//     */
//    public void upLoadImgApi(List<MultipartBody.Part> parts) {
//        addDisposable(apiServer.upHeadImg(parts), new BaseObserver(baseView) {
//            @Override
//            public void onSuccess(BaseModel o) {
//                baseView.onUpLoadImgSuccess((BaseModel<Object>) o);
//            }
//
//            @Override
//            public void onError(String msg) {
//                if (baseView != null) {
//                    baseView.showError(msg);
//                }
//            }
//        });
//    }
//
//    /**
//     * 演示 图片和字段一起上传
//     *
//     * @param title
//     * @param content
//     * @param parts
//     */
//    public void upLoadImgApi(String title, String content, List<MultipartBody.Part> parts) {
//        HashMap<String, RequestBody> params = new HashMap<>();
//        params.put("title", RetrofitUtil.convertToRequestBody(title));
//        params.put("content", RetrofitUtil.convertToRequestBody(content));
//        addDisposable(apiServer.expressAdd(params, parts), new BaseObserver(baseView) {
//            @Override
//            public void onSuccess(BaseModel o) {
//                baseView.onUpLoadImgSuccess((BaseModel<Object>) o);
//            }
//
//            @Override
//            public void onError(String msg) {
//                if (baseView != null) {
//                    baseView.showError(msg);
//                }
//            }
//        });
//    }
}
