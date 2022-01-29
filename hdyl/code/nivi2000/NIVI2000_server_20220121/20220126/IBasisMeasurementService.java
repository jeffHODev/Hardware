package com.hengde.nivi.modules.create.service;

import com.hengde.nivi.modules.create.entity.BasisMeasurement;
import com.hengde.nivi.modules.pdftest.Entity;

import java.util.HashMap;
import java.util.Map;

/**
 * @author dong
 * @title: IBasisMeasurementService
 * @description: 血压数据业务相关接口
 * @date 2019/7/3 14:55
 */
public interface IBasisMeasurementService {
    /**
    　* @description: 保存血压检测数据
    　* @author dong
    　* @version: V1.0
    　* @date 2019/7/3 14:59
    　* @param
    　* @return
    　*/
    Map saveBloodPressure(BasisMeasurement basisMeasurement);

    /**
    　* @description: 根据测试号获取对应患者相关检测的基本信息
    　* @author dong
    　* @version: V1.0
    　* @date 2020/5/6 20:37
    　* @param basisMeasurementId
    　* @return BasisMeasurement
    　*/
    BasisMeasurement getPatientBasisMeasurement(String basisMeasurementId);

    Entity selectPDFInfoBybasisMeasurementId(String id);
}
