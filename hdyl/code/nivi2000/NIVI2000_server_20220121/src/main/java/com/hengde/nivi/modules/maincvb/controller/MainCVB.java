package com.hengde.nivi.modules.maincvb.controller;

import com.hengde.nivi.common.response.Result;
import com.hengde.nivi.modules.maincvb.service.IMainCVBService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

/**
 * @author dong
 * @title: MainCVB
 * @description: 初始化、画图、结果相关业务
 * @date 2020/8/5 14:52
 */
@RestController
@RequestMapping("/api/maincvb")
public class MainCVB {

    /*@Autowired
    private IMainCVBService mainCVBService;

    @GetMapping("/analdatainit")
    public Result analDataInit(@RequestParam String basisMeasurementId){
        Result result = new Result();
        return result.ok().setData(mainCVBService.AnalData_Init(basisMeasurementId));
    }*/
}
