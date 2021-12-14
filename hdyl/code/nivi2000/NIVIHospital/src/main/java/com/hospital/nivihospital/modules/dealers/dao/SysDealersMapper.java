package com.hospital.nivihospital.modules.dealers.dao;


import com.hospital.nivihospital.modules.dealers.entity.SysDealers;

import java.util.List;
import java.util.Map;

public interface SysDealersMapper {

    /**
    　* @description: 查询当前经销商下的所有测试记录（正常可以出结果）
    　* @author dong
    　* @version: V1.0
    　* @date 2021/12/2 10:22
    　* @param
    　* @return
    　*/
    int selectBasisMeasurementAllCount(String entityId);

    /**
    　* @description: 查询当前经销商下的所有以评估的记录
    　* @author dong
    　* @version: V1.0
    　* @date 2021/12/2 10:25
    　* @param
    　* @return
    　*/
    int selectEvaluationAllCount(String entityId);

    /**
    　* @description: 某医院下的所有检测记录总数
    　* @author dong
    　* @version: V1.0
    　* @date 2021/12/3 9:37
    　* @param
    　* @return
    　*/
    int selectBasisMeasurementAllCountByHospital(String hospitalId);

    /**
    　* @description: 某医院下所有已评估总数
    　* @author dong
    　* @version: V1.0
    　* @date 2021/12/3 9:37
    　* @param
    　* @return
    　*/
    int selectEvaluationAllCountByHospital(String hospitalId);

    /**
    　* @description: 分页获取当前经销商下的所有医院列表
    　* @author dong
    　* @version: V1.0
    　* @date 2021/12/2 10:46
    　* @param 
    　* @return 
    　*/
    List<Map> selectHospitalList(Map parMap);

    /**
    　* @description: 分页获取某医院对应的设备列表信息
    　* @author dong
    　* @version: V1.0
    　* @date 2021/12/3 9:55
    　* @param
    　* @return
    　*/
    List<Map> selectDeviceListByHospital(Map parMap);

    /**
    　* @description: 分页获取当前经销商下的所有医院列表数据总个数
    　* @author dong
    　* @version: V1.0
    　* @date 2021/12/2 10:55
    　* @param
    　* @return
    　*/
    int selectHospitalListCount(String entityId);

    /**
    　* @description: 分页获取某医院对应的设备列表信息列表数据总个数
    　* @author dong
    　* @version: V1.0
    　* @date 2021/12/3 10:13
    　* @param
    　* @return
    　*/
    int selectDeviceListByHospitalCount(String hospitalId);

    int deleteByPrimaryKey(String id);

    int insert(SysDealers record);

    int insertSelective(SysDealers record);

    SysDealers selectByPrimaryKey(String id);

    int updateByPrimaryKeySelective(SysDealers record);

    int updateByPrimaryKey(SysDealers record);
}