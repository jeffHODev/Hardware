package com.hengde.nivi.modules.create.dao;

import com.hengde.nivi.modules.create.entity.BasisMeasurement;

import java.util.Map;

public interface BasisMeasurementMapper {
    int deleteByPrimaryKey(String id);

    int insert(BasisMeasurement record);

    int insertSelective(BasisMeasurement record);

    BasisMeasurement selectByPrimaryKey(String id);

    int updateByPrimaryKeySelective(BasisMeasurement record);

    int updateByPrimaryKey(BasisMeasurement record);

    /**
    　* @description: 根据测试号获取对应患者相关检测的基本信息
    　* @author dong
    　* @version: V1.0
    　* @date 2020/5/6 20:40
    　* @param basisMeasurementId
    　* @return BasisMeasurement
    　*/
    BasisMeasurement getPatientBasisMeasurement(String basisMeasurementId);

    BasisMeasurement getPatientBasisMeasurementnew(String basisMeasurementId);

    BasisMeasurement getPatientBasisMeasurementnew1(String basisMeasurementId);

    BasisMeasurement getPatientBasisMeasurementApg(String basisMeasurementId);

    /**
    　* @description: 查询测量模式（1 同步；2 异步）
    　* @author dong
    　* @version: V1.0
    　* @date 2021/4/12 15:03
    　* @param
    　* @return
    　*/
    Integer selectModel(String basisMeasurementId);

    /**
    　* @description: 更新测量模式
    　* @author dong
    　* @version: V1.0
    　* @date 2021/4/12 15:13
    　* @param
    　* @return
    　*/
    int updateModel(Map map);
}