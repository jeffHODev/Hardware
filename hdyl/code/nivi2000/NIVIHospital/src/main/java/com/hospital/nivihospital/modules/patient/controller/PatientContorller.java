package com.hospital.nivihospital.modules.patient.controller;

import com.hospital.nivihospital.common.response.Result;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * @author dong
 * @title: PatientContorller
 * @description: 患者相关控制器
 * @date 2021/11/16 10:51
 */
@RestController
@RequestMapping("/api/patient")
public class PatientContorller {
    /**
    　* @description: 医院登录-设备详情-数据查询-详情
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/16 11:16
    　* @param 
    　* @return 
    　*/
    @GetMapping("/getpatientdatainfolist")
    public Result getPatientDataInfoList(@RequestParam String date, @RequestParam String doctorId, @RequestParam(value = "pageNo",defaultValue = "1") int pageNo,
                                         @RequestParam(value = "pageSize",defaultValue ="10") int pageSize){
        Result result = new Result();
        Map PatientDataInfo = new HashMap();
        PatientDataInfo.put("id", "asdfsadfas");
        PatientDataInfo.put("patientName", "王小二");
        PatientDataInfo.put("age", 23);
        PatientDataInfo.put("sex", 1);
        PatientDataInfo.put("patientId", "sadfsdfdasf");
        PatientDataInfo.put("date", "2021-08-25");
        PatientDataInfo.put("doctorName", "李大庄");

        Map PatientDataInfo1 = new HashMap();
        PatientDataInfo.put("id", "asdfsadfas1");
        PatientDataInfo1.put("patientName", "王小二");
        PatientDataInfo1.put("age", 23);
        PatientDataInfo1.put("sex", 1);
        PatientDataInfo1.put("patientId", "sadfsdfdasf");
        PatientDataInfo1.put("date", "2021-08-25");
        PatientDataInfo1.put("doctorName", "李大庄");

        List list = new ArrayList();
        list.add(PatientDataInfo);
        list.add(PatientDataInfo1);

        Map resMap = new HashMap();
        resMap.put("dataList", list);
        resMap.put("allCount", 2);
        return result.ok().setData(resMap);
    }

    /**
    　* @description: 医院登录-设备详情-数据查询-详情-PDF详情
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/16 11:19
    　* @param
    　* @return
    　*/
    @GetMapping("/getpatientdatainfoPDF")
    public Result getPatientDataInfoPDF(@RequestParam String id){
        Result result = new Result();
        Map PatientDataInfo = new HashMap();
        PatientDataInfo.put("id", "asdfsadfas");
        PatientDataInfo.put("patientName", "王小二");
        PatientDataInfo.put("age", 23);
        PatientDataInfo.put("sex", 1);
        PatientDataInfo.put("patientId", "sadfsdfdasf");
        PatientDataInfo.put("date", "2021-08-25");
        PatientDataInfo.put("doctorName", "李大庄");
        PatientDataInfo.put("evaluation", "该患者身体良好，没有发现异常");

        return result.ok().setData(PatientDataInfo);
    }
}
