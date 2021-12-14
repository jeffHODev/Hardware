package com.hospital.nivihospital.modules.hospital.dao;

import com.hospital.nivihospital.modules.hospital.entity.SysHospital;

import java.util.Map;

public interface SysHospitalMapper {

    /**
    　* @description: 连表查询医院名称
    　* @author dong
    　* @version: V1.0
    　* @date 2021/12/1 11:06
    　* @param
    　* @return
    　*/
    Map selectHospitalByPrimaryKey(String id);

    int deleteByPrimaryKey(String id);

    int insert(SysHospital record);

    int insertSelective(SysHospital record);

    SysHospital selectByPrimaryKey(String id);

    int updateByPrimaryKeySelective(SysHospital record);

    int updateByPrimaryKey(SysHospital record);
}