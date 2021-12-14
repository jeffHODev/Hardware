package com.hengde.nivi.modules.patient.dao;


import com.hengde.nivi.modules.patient.entity.Patient;

import java.util.List;
import java.util.Map;

public interface PatientMapper {
    int deleteByPrimaryKey(String id);

    int insert(Patient record);

    int insertSelective(Patient record);

    Patient selectByPrimaryKey(String id);

    int updateByPrimaryKeySelective(Patient record);

    int updateByPrimaryKey(Patient record);

    /**
    　* @description: 根据身份证号获取患者详细信息
    　* @author dong
    　* @version: V1.0
    　* @date 2019/7/3 14:06
    　* @param uniqueID
                    ：身份证号
    　* @return
    　*/
    Patient getPatientInfo(Map map);

    /**
    　* @description: 根据患者姓名、手机号获取患者基本信息
    　* @author dong
    　* @version: V1.0
    　* @date 2019/8/21 17:43
    　* @param
    　* @return
    　*/
    Patient getPatientInfoByNP(Map map);

    /**
    　* @description: 快速查询页面根据身份证号
    　* @author dong
    　* @version: V1.0
    　* @date 2021/7/13 11:20
    　* @param
    　* @return
    　*/
    Patient getPatientByUniqueId(String uniqueId);

    /**
    　* @description: 根据医院id分页获取所有患者列表
    　* @author dong
    　* @version: V1.0
    　* @date 2019/8/6 13:47
    　* @param 
    　* @return 
    　*/
    List<Patient> getPatientList(Map map);

    /**
    　* @description: 根据医院id获取所有患者列表的数据总条数
    　* @author dong
    　* @version: V1.0
    　* @date 2019/8/6 13:57
    　* @param
    　* @return
    　*/
    int getPatientListCount(String deviceId);

    /**
    　* @description: 修改患者信息
    　* @author dong
    　* @version: V1.0
    　* @date 2019/8/22 14:34
    　* @param
    　* @return
    　*/
    int updatePatient(Patient record);

    /**
    　* @description: 根据患者id查询患者基本信息
    　* @author dong
    　* @version: V1.0
    　* @date 2019/8/27 11:31
    　* @param
    　* @return
    　*/
    Patient selectPatientById(String id);

    /**
    　* @description: 根据表名和该表主键id获取最新版本号
    　* @author dong
    　* @version: V1.0
    　* @date 2019/8/30 11:18
    　* @param
    　* @return
    　*/
    Integer selectVersionByTimeIdName(Map map);

    /**
    　* @description: 添加更新记录
    　* @author dong
    　* @version: V1.0
    　* @date 2019/8/30 14:03
    　* @param
    　* @return
    　*/
    int addUpdateEvent(Map map);

    /**
    　* @description: 获取患者历史检测记录数据列表
    　* @author dong
    　* @version: V1.0
    　* @date 2019/9/10 10:39
    　* @param
    　* @return
    　*/
    List<Map> getHistoryPatientlist(Map map);

    /**
    　* @description: 获取患者历史检测记录数据列表对应数据总条数
    　* @author dong
    　* @version: V1.0
    　* @date 2019/9/10 10:43
    　* @param
    　* @return
    　*/
    int getHistoryPatientListCount(String deviceId);

    /**
    　* @description: 根据患者id获取对应历史检测记录
    　* @author dong
    　* @version: V1.0
    　* @date 2019/12/27 10:55
    　* @param
    　* @return
    　*/
    List<Map> getPatientHistorylist(Map map);

    /**
    　* @description: 根据手机号查询对应患者
    　* @author dong
    　* @version: V1.0
    　* @date 2021/3/30 19:28
    　* @param
    　* @return
    　*/
    Patient getPatientInfoByPhone(String phone);

    /**
    　* @description: 根据身份证号查询对应患者
    　* @author dong
    　* @version: V1.0
    　* @date 2021/3/30 19:34
    　* @param
    　* @return
    　*/
    Patient getPatientInfoByUniqueId(String uniqueId);

    /**
    　* @description: 排除当前数据查询患者基本信息
    　* @author dong
    　* @version: V1.0
    　* @date 2021/3/30 19:37
    　* @param
    　* @return
    　*/
    Patient getPatientInfoByNPNotId(Map parMap);

    /**
    　* @description: 排除当前数据根据手机号查询患者基本信息
    　* @author dong
    　* @version: V1.0
    　* @date 2021/3/30 19:42
    　* @param
    　* @return
    　*/
    Patient getPatientInfoByPhoneNotId(Map parMap);

    /**
    　* @description: 排除当前数据根据身份证号查询患者基本信息
    　* @author dong
    　* @version: V1.0
    　* @date 2021/3/30 19:44
    　* @param
    　* @return
    　*/
    Patient getPatientInfoByUniqueIdNotId(Map parMap);
}