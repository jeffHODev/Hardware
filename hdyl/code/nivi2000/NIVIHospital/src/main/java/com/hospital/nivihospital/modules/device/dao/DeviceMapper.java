package com.hospital.nivihospital.modules.device.dao;

import com.hospital.nivihospital.modules.device.entity.Device;

import java.util.List;
import java.util.Map;

public interface DeviceMapper {

    /**
    　* @description: 获取医院对应的设备列表
    　* @author dong
    　* @version: V1.0
    　* @date 2021/12/3 10:54
    　* @param
    　* @return
    　*/
    List<Map> selectDetectCountByHospital(Map parMap);

    /**
    　* @description: 获取医院对应的设备列表数据总条数
    　* @author dong
    　* @version: V1.0
    　* @date 2021/12/3 11:00
    　* @param
    　* @return
    　*/
    int selectDetectCountByHospitalCount(String hospitalId);

    int deleteByPrimaryKey(String id);

    int insert(Device record);

    int insertSelective(Device record);

    Device selectByPrimaryKey(String id);

    int updateByPrimaryKeySelective(Device record);

    int updateByPrimaryKey(Device record);
}