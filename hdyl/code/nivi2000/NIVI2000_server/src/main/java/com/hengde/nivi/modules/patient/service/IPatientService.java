package com.hengde.nivi.modules.patient.service;

import com.hengde.nivi.modules.patient.entity.Patient;

import java.util.List;
import java.util.Map;

/**
 * @author dong
 * @title: IPatientService
 * @description: 患者相关业务接口
 * @date 2019/7/3 13:47
 */
public interface IPatientService {
    /**
    　* @description: 根据身份证号获取患者详细信息
    　* @author dong
    　* @version: V1.0
    　* @date 2019/7/3 14:04
    　* @param
    　* @return
    　*/
    Patient getPatientInfo(String uniqueID);

    /**
    　* @description: 根据患者姓名、手机号获取患者基本信息
    　* @author dong
    　* @version: V1.0
    　* @date 2019/8/21 17:32
    　* @param
    　* @return
    　*/
    Patient getPatientInfoByNP(String name, String phone);

    /**
    　* @description: 快速查询页面根据身份证号
    　* @author dong
    　* @version: V1.0
    　* @date 2021/7/13 11:17
    　* @param 
    　* @return 
    　*/
    Patient getPatientByUniqueId(String uniqueId);

    /**
    　* @description: 新建患者
    　* @author dong
    　* @version: V1.0
    　* @date 2019/7/3 14:22
    　* @param
    　* @return
    　*/
    Map createPatient(Patient patient);

    /**
    　* @description: 分页获取医院对应患者列表
    　* @author dong
    　* @version: V1.0
    　* @date 2019/8/6 11:17
    　* @param
    　* @return
    　*/
    Map getPatientList(int pageNo, int pageSize);

    /**
    　* @description: 根据id修改患者基本信息
    　* @author dong
    　* @version: V1.0
    　* @date 2019/8/22 14:27
    　* @param
    　* @return
    　*/
    void updatePatient(Patient patient);

    /**
    　* @description: 根据医院获取对应历史患者检测记录
    　* @author dong
    　* @version: V1.0
    　* @date 2019/9/6 10:42
    　* @param
    　* @return
    　*/
    Map getHistoryPatientlist(int pageNo, int pageSize);

    /**
    　* @description: 根据患者id获取对应历史检测记录
    　* @author dong
    　* @version: V1.0
    　* @date 2019/12/27 10:53
    　* @param
    　* @return
    　*/
    List<Map> getPatientHistorylist(String patientId);
}
