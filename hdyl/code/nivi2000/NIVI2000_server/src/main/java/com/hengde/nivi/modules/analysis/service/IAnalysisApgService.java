package com.hengde.nivi.modules.analysis.service;

import java.util.Map;

/**
 * @author dong
 * @title: IAnalysisApgService
 * @description: 左右颈动脉、结果算法相关业务接口定义
 * @date 2020/5/12 10:40
 */
public interface IAnalysisApgService {

    /**
    　* @description: 根据测试号获取左右颈动脉、结果相关的值；
    　* @author dong
    　* @version: V1.0
    　* @date 2020/5/12 10:57
    　* @param
    　* @return
    　*/
    Map preTranslateMessage(String basisMeasurementId, short IDL, short IDR, Map resMap, int model);
}
