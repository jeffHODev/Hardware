package com.hengde.nivi.modules.detection.dao;

import com.hengde.nivi.modules.detection.entity.OriginalWaveform;
import com.hengde.nivi.modules.detection.entity.OriginalWaveformReq;

import java.util.Map;

public interface OriginalWaveformMapper {
    int deleteByPrimaryKey(String id);

    int insert(Map record);

    int insertSelective(OriginalWaveform record);

    OriginalWaveform selectByPrimaryKey(String id);

    /**
    　* @description: 根据测试号获取检测原始波形
    　* @author dong
    　* @version: V1.0
    　* @date 2019/7/3 15:36
    　* @param
    　* @return
    　*/
    OriginalWaveform selectByBasisMeasurementId(String basisMeasurementId);

    /**
    　* @description: 更新医院剩余检测次数
    　* @author dong
    　* @version: V1.0
    　* @date 2019/8/1 15:24
    　* @param
    　* @return
    　*/
    int updateDetectCountByHospitalId(String deviceId);

    /**
    　* @description: 查询医院设备剩余检测次数
    　* @author dong
    　* @version: V1.0
    　* @date 2019/8/1 15:27
    　* @param
    　* @return
    　*/
    int selectDetectCountByHospitalId(String deviceId);

    int updateByPrimaryKeySelective(OriginalWaveform record);

    int updateByPrimaryKeyWithBLOBs(OriginalWaveform record);

    int updateByPrimaryKey(OriginalWaveform record);
}