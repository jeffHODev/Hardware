package com.hengde.nivi.modules.analysis.controller;

import com.hengde.nivi.common.response.Result;
import com.hengde.nivi.modules.analysis.service.IAnalysisService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

/**
 * @author dong
 * @title: AnalysisController
 * @description: 分析算法相关控制器
 * @date 2020/4/26 16:40
 */
@RestController
@RequestMapping("/api/analysis")
public class AnalysisController {
    @Autowired
    private IAnalysisService analysisService;

    /**
    　* @description: 根据测试号获取心电、心音、脉搏的超收缩、低舒张R相关的值；
    　* @author dong
    　* @version: V1.0
    　* @date 2020/4/26 17:50
    　* @param
    　* @return
    　*/
    /*@GetMapping("/analysis")
    public Result analysis(@RequestParam String basisMeasurementId){
        Result result = new Result();
        return result.ok().setData(analysisService.ecgPcgApgSysDia(basisMeasurementId));
    }*/
}
