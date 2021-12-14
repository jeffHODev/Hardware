package com.hengde.nivi.modules.analysis.controller;

import com.hengde.nivi.common.response.Result;
import com.hengde.nivi.modules.analysis.service.IAnalysisApgService;
import com.hengde.nivi.modules.analysis.service.IAnalysisService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import java.util.HashMap;
import java.util.Map;

/**
 * @author dong
 * @title: AnalysisApgController
 * @description: 左右颈动脉、结果算法相关控制器
 * @date 2020/5/12 10:40
 */
@RestController
@RequestMapping("/api/analysisapg")
public class AnalysisApgController {
    @Autowired
    private IAnalysisApgService analysisApgService;

    /**
    　* @description: 根据测试号获取左右颈动脉、结果相关的值；
    　* @author dong
    　* @version: V1.0
    　* @date 2020/5/12 10:54
    　* @param
    　* @return
    　*/
//    @GetMapping("/analysisapg")
//    public Result analysisApg(@RequestParam String basisMeasurementId, Short IDL, Short IDR){
//        Result result = new Result();
//        if(IDL == null || IDL == 0){
//            return result.error("IDL is null or 0").setData(new HashMap<>());
//        }
//        if(IDR == null || IDR == 0){
//            return result.error("IDR is null or 0").setData(new HashMap<>());
//        }
//
//        Map resMap = new HashMap();
//        return result.ok().setData(analysisApgService.preTranslateMessage(basisMeasurementId, IDL, IDR, resMap));
//    }
}
