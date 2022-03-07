package com.hengde.nivi.modules.analysis.service;

import java.util.Map;

/**
 * @author dong
 * @title: IAnalysisService
 * @description: 分析算法相关业务接口定义
 * @date 2020/4/26 17:08
 */
public interface IAnalysisService {
    /**
    　* @description: 根据测试号获取心电、心音、脉搏的超收缩、低舒张R相关的值；
    　* @author dong
    　* @version: V1.0
    　* @date 2020/4/26 19:06
    　* @param basisMeasurementId
    　* @return Map
    　*/
    Map ecgPcgApgSysDia(String basisMeasurementId, Map resultMap);
}
