package com.hekangyuan.nivi1000.api;

import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.model.BloodPressureBean;
import com.hekangyuan.nivi1000.model.DiagnosisBean;
import com.hekangyuan.nivi1000.model.EcgMonitorBean;
import com.hekangyuan.nivi1000.model.LoginBean;
import com.hekangyuan.nivi1000.model.PDFBean;
import com.hekangyuan.nivi1000.model.PasswordBean;
import com.hekangyuan.nivi1000.model.PatientBaseBean;
import com.hekangyuan.nivi1000.model.PatientInformationBean;
import com.hekangyuan.nivi1000.utils.Constants;

import java.util.List;

import io.reactivex.Observable;
import retrofit2.http.Body;
import retrofit2.http.GET;
import retrofit2.http.POST;
import retrofit2.http.PUT;
import retrofit2.http.Query;

public interface ApiServer {

//    @POST("api/Activity/get_activities?")
//    Observable<BaseModel<List<MainBean>>> getMain(@Query("time") String requestType);
//
//
//    @FormUrlEncoded
//    @POST("api/Activity/get_activities?")
//    Observable<BaseModel<List<MainBean>>> getMain2(@Field("time") String requestType);


//    @POST("api/Activity/get_activities?")
String  url = Constants.url;
    //登录
    @POST(url+"login")
    Observable<BaseJsonMsg<LoginBean>> Loginhttp (@Body LoginBean loginBean);

    @POST(url+"api/uppass")
    Observable<BaseJsonMsg<PasswordBean>> modifyPasswordhttp (@Body PasswordBean passwordBean);
    //创建患者信息
    @POST(url+"api/patient/create")
    Observable<BaseJsonMsg<PatientInformationBean>> PatientInformationhttp (@Body PatientInformationBean patientInformationBean);
   //创建血压
    @POST(url+"api/create/bloodpressure")
    Observable<BaseJsonMsg<BloodPressureBean>> NewBloodPressurePresenterhttp (@Body BloodPressureBean bloodPressureBean);
//    //获取单个患者信息
    @GET(url+"api/patient/info?")
    Observable<BaseJsonMsg<PatientBaseBean>> GetPersonalInformationhttp (@Query("uniqueId") String uniqueId);
    //上传心电记录
//    @GET(url+"api/analysis/analysis")
    @POST(url+"api/detection/saveOriginalWaveform?") //done
    Observable<BaseJsonMsg<EcgMonitorBean>> UploadEcghttp (@Body EcgMonitorBean ecgMonitorBean);
    //获取多个患者信息
    @GET(url+"api/patient/patientlist?") //done
    Observable<BaseJsonMsg<PatientBaseBean>> GetPersonalInformationListhttp (@Query("pageNo") int pageNo, @Query("pageSize") int pageSize);
    //
    //获取单个患者信息
    @GET(url+"api/patient/getinfo?") //done
    Observable<BaseJsonMsg<PatientInformationBean>> QueryPatInfohttp (@Query("name") String name, @Query("phone") String phone);

    //获取单个患者信息扩展接口-根据身份证号
    @GET(url+"api/patient/getinfobyuniqueid?") //done
    Observable<BaseJsonMsg<PatientInformationBean>> QueryPatInfoByUniqueIdhttp (@Query("uniqueId") String uniqueId);

    //修改单个患者信息
    @POST(url+"api/patient/updatepatient") //done
    Observable<BaseJsonMsg<PatientInformationBean>> UpPatientInformationhttp (@Body PatientInformationBean patientInformationBean);

    //获取单人诊断列表
    @GET(url+"api/patient/patienthistorylist") //done
    Observable<BaseJsonMsg<List<DiagnosisBean>>> GetPersonDiaListhttp (@Query("patientId") String patientId);

    //上传数据
    //TODO
    @POST(url+"api/upload")
    Observable<BaseJsonMsg<EcgMonitorBean>> Ecghttp (@Body EcgMonitorBean loginBean);

    //测试上传心电记录
//    @GET(url+"api/analysis/analysis?")
//    @POST(url+"api/detection/saveOriginalWaveform?") //done
//    Observable<BaseJsonMsg<EcgMonitorBean>> UploadEcghttp (@Query("basisMeasurementId") String basisMeasurementId);

    @GET(url+"retStamper?")
//    @GET("http://123.57.185.1:8080/retStamper?")
//    @GET("http://192.168.0.154:8080/retStamper")
    Observable<BaseJsonMsg<PDFBean>> GetPdfHttp (@Query("basisMeasurementId") String basisMeasurementId);
//    /**
//     * 第三种写法
//     *
//     * @param params
//     * @return
//     */
//    @FormUrlEncoded
//    @POST("api/Activity/get_activities?")
//    Observable<BaseModel<HashMap<String, String>>> getMain3(@FieldMap HashMap<String, String> params);

//    /**
//     * 演示 单图上传
//     *
//     * @param parts
//     * @return
//     */
//    @Multipart
//    @POST("api/Company/register")
//    Observable<BaseModel<Object>> upLoadImg(@Part MultipartBody.Part parts);
//
//    /**
//     * 演示 多图上传
//     *
//     * @param parts
//     * @return
//     */
//    @Multipart
//    @POST("api/user_info/update_headimg")
//    Observable<BaseModel<Object>> upHeadImg(@Part List<MultipartBody.Part> parts);
//
//    /**
//     * 演示 图片字段一起上传
//     *
//     * @param parts
//     * @return
//     */
//    @Multipart
//    @POST("api/Express/add")
//    Observable<BaseModel<Object>> expressAdd(@PartMap Map<String, RequestBody> map,
//                                             @Part List<MultipartBody.Part> parts);



//    /**
//     * 演示特殊结构写法
//     *
//     * @param requestType
//     * @return
//     */
//    @POST("api/Activity/get_activities?")
//    Observable<com.lp.mvp_network.second2demo.mvp.BaseModel<List<Bean1>, Bean2, List<Bean3>>> getMain2Demo(@Query("time") String requestType);
}
