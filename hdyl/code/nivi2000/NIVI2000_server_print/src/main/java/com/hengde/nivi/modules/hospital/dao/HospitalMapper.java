package com.hengde.nivi.modules.hospital.dao;

import com.hengde.nivi.modules.hospital.entity.Device;
import com.hengde.nivi.modules.hospital.entity.Devpass;
import com.hengde.nivi.modules.hospital.entity.Hospital;

public interface HospitalMapper {
    int deleteByPrimaryKey(String id);

    int insert(Hospital record);

    int insertSelective(Hospital record);

    Hospital selectByPrimaryKey(String id);

    int updateByPrimaryKeySelective(Hospital record);

    int updateByPrimaryKey(Hospital record);

    /**
    　* @description: 根据医院设备账号查询医院设备信息
    　* @author dong
    　* @version: V1.0
    　* @date 2019/7/2 15:51
    　* @param
    　* @return
    　*/
    Device selectByLoginName(String loginName);

    /**
    　* @description: 初始化设备
    　* @author dong
    　* @version: V1.0
    　* @date 2020/11/30 15:37
    　* @param
    　* @return
    　*/
    int insertDevice(Device device);

    /**
    　* @description: 修改设备密码
    　* @author dong
    　* @version: V1.0
    　* @date 2020/11/30 15:57
    　* @param
    　* @return
    　*/
    int updateDevicePass(Devpass devpass);
}