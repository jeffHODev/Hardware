package com.hengde.nivi.modules.create.controller;

import com.hengde.nivi.common.config.HospitalConfig;
import com.hengde.nivi.common.response.Result;
import com.hengde.nivi.modules.create.entity.BasisMeasurement;
import com.hengde.nivi.modules.create.service.IBasisMeasurementService;
import com.hengde.nivi.modules.hospital.entity.Device;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.HashMap;

/**
 * @author dong
 * @title: CreatePatient
 * @projectName nivi
 * @description: 创建数据相关控制器
 * @date 2019/6/6 14:28
 */
@RestController
@RequestMapping("/api/create")
public class CreateController {
    @Autowired
    private IBasisMeasurementService basisMeasurementService;

    @Autowired
    private HospitalConfig hospitalConfig;

    /**
     　* @description: 新建患者血压测量(基本测量)
     　* @author dong
     　* @version: V1.0
     　* @date 2019/6/6 17:22
     　* @param basisMeasurement
     　* @return
     　*/
    @PostMapping("/bloodpressure")
    public Result bloodPressureInfo(@RequestBody BasisMeasurement basisMeasurement){
        Result result = new Result();

        Device device = hospitalConfig.getHospitalDevice();
        if(device == null){
            return result.setStatus(2, "设备登录信息为空，请从新登录");
        }

        //TODO 需要校验实体类中所有属性不为空
        if(basisMeasurement.getPatientId() == null){
//            return result.error("patientId is null").setData(new HashMap<>());
            return result.setStatus(2, "patientId is null").setData(new HashMap<>());
        }
        return result.ok().setData(basisMeasurementService.saveBloodPressure(basisMeasurement));
    }

    /**
     　* @description: 根据测试号获取对应患者相关检测的基本信息
     　* @author dong
     　* @version: V1.0
     　* @date 2020/5/6 20:32
     　* @param basisMeasurementId
     　* @return
     　*/
    @GetMapping("/patientbasismeasurement")
    public Result patientBasisMeasurement(@RequestParam String basisMeasurementId){
        Result result = new Result();
        return result.ok().setData(basisMeasurementService.getPatientBasisMeasurement(basisMeasurementId));
    }
}
