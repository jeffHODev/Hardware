package com.hengde.nivi.modules.detection.service;

import com.hengde.nivi.modules.detection.entity.OriginalWaveformReq;

import java.util.Map;

/**
 * @author dong
 * @title: IDetectionService
 * @description: 检测业务相关接口
 * @date 2019/6/12 11:33
 */
public interface IDetectionService {
    /**
    　* @description: 保存原始检测波形
    　* @author dong
    　* @version: V1.0
    　* @date 2019/6/12 11:48
    　* @param
    　* @return
    　*/
    Map saveOriginalWaveform(OriginalWaveformReq originalWaveformReq, short IDL, short IDR);

    /**
    　* @description: 根据测试号获取原始检测波形
    　* @author dong
    　* @version: V1.0
    　* @date 2019/6/13 10:27
    　* @param
    　* @return
    　*/
    OriginalWaveformReq getOriginalWaveform(String basisMeasurementId);
}
