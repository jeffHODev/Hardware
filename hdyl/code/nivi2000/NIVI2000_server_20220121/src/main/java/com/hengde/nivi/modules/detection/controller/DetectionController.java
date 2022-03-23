package com.hengde.nivi.modules.detection.controller;

import com.hengde.nivi.common.response.Result;
import com.hengde.nivi.modules.create.dao.BasisMeasurementMapper;
import com.hengde.nivi.modules.detection.dao.OriginalWaveformMapper;
import com.hengde.nivi.modules.detection.entity.OriginalWaveform;
import com.hengde.nivi.modules.detection.entity.OriginalWaveformReq;
import com.hengde.nivi.modules.detection.service.IDetectionService;
import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.HashMap;
import java.util.Map;

/**
 * @author dong
 * @title: DetectionController
 * @description: 检测相关控制器
 * @date 2019/6/6 17:30
 */
@RestController
@RequestMapping("/api/detection")
public class DetectionController {
    protected static Logger logger = LoggerFactory.getLogger(DetectionController.class);

    @Autowired
    private IDetectionService detectionService;
    @Autowired
    private BasisMeasurementMapper basisMeasurementMapper;

    /**
    　* @description: 保存原始检测波形
    　* @author dong
    　* @version: V1.0
    　* @date 2019/6/6 17:42
    　* @param originalWaveform
    　* @return 剩余检测次数
    　*/
    @PostMapping("/saveOriginalWaveform")
    public Result saveOriginalWaveform(@RequestBody OriginalWaveformReq originalWaveformReq){
        logger.info("前端请求保存原始数据波形，basisMeasurementId：【{}】，IDL：【{}】，IDR：【{}】，model：【{}】",originalWaveformReq.getBasisMeasurementId(),originalWaveformReq.getIDL(),originalWaveformReq.getIDR(),originalWaveformReq.getModel());
        Result result = new Result();
        if(StringUtils.isBlank(originalWaveformReq.getBasisMeasurementId())){
//            return result.error("测试号为空").setData(new HashMap<>());
            return result.setStatus(2, "测试号参数为空").setData(new HashMap<>());
        }

        Integer model = basisMeasurementMapper.selectModel(originalWaveformReq.getBasisMeasurementId());
        if(model == null || model == 0){
            if(originalWaveformReq.getModel() == null || originalWaveformReq.getModel() == 0){
                logger.info("basisMeasurementId1111111++++++++++++++++++++", originalWaveformReq.getModel());
                return result.setStatus(2, "请设置测量模式").setData(new HashMap<>());
            }
        }else{
            originalWaveformReq.setModel(model);
            logger.info("basisMeasurementId22222++++++++++++++++++++", originalWaveformReq.getModel());
        }



        Map resultMap = detectionService.saveOriginalWaveform(originalWaveformReq, originalWaveformReq.getIDL(), originalWaveformReq.getIDR());
        if(resultMap.containsKey("staCode")){
            if((int)resultMap.get("staCode") == 519){
                return result.setStatus(2, "该测量记录没有对应检测结果数据").setData(new HashMap<>());
            }
            if((int)resultMap.get("staCode") == 500000){
                return result.setStatus(2, "左颈动脉脉搏数据不规范，请从新采集").setData(new HashMap<>());
            }
            if((int)resultMap.get("staCode") == 500001){
                return result.setStatus(2, "右颈动脉脉搏数据不规范，请从新采集").setData(new HashMap<>());
            }
        }
        return result.ok().setData(resultMap);
    }

    /**
    　* @description: 根据测试号获取原始检测波形
    　* @author dong
    　* @version: V1.0
    　* @date 2019/6/13 11:02
    　* @param
    　* @return
    　*/
    @GetMapping("/getOriginalWaveform")
    public Result getOriginalWaveform(@RequestParam String basisMeasurementId){
        Result result = new Result();
        return result.ok().setData(detectionService.getOriginalWaveform(basisMeasurementId));
    }
}