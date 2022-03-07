package com.hengde.nivi.modules.maincvb.service;

import java.util.Map;

/**
 * @author dong
 * @title: IMainCVBService
 * @description: 初始化、画图、结果相关业务接口定义
 * @date 2020/8/5 14:57
 */
public interface IMainCVBService {
    Map AnalData_Init(String basisMeasurementId, Map resMap);
}
