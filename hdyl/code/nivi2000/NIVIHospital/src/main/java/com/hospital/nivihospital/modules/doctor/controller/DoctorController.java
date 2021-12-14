package com.hospital.nivihospital.modules.doctor.controller;

import com.hospital.nivihospital.common.response.Result;
import org.springframework.web.bind.annotation.*;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * @author dong
 * @title: DoctorController
 * @description: 医生端登录相关业务控制器
 * @date 2021/11/19 11:36
 */
@RestController
@RequestMapping("/api/doctor")
public class DoctorController {
    /**
    　* @description: 医生登录--设备详情
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/19 11:38
    　* @param
    　* @return
    　*/
    @GetMapping("/getdeviceinfo")
    public Result getDeviceInfo(@RequestParam String deviceId){
        Result result = new Result();

        //todo 根据医生查询对应医院下的某台设备剩余次数
        Map hospitalDevice = new HashMap();
        hospitalDevice.put("deviceId", "sadfadsfadsfadsf");
        hospitalDevice.put("detectCount", 100);
        hospitalDevice.put("deviceNo", "42W4D1KUIH");
        return result.ok().setData(hospitalDevice);
    }

    /**
    　* @description: 获取当前医生对应的所有待评估报告
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/19 12:11
    　* @param 
    　* @return 
    　*/
    @GetMapping("/getnotevaluation")
    public Result getNotEvaluation(@RequestParam(value = "pageNo",defaultValue = "1") int pageNo,
                                   @RequestParam(value = "pageSize",defaultValue ="10") int pageSize){
        Result result = new Result();
        
        Map hospitalDevice = new HashMap();
        hospitalDevice.put("basisMeasurementId", "sadfadsfadsfadsf");
        hospitalDevice.put("date", "2021-08-25");
        hospitalDevice.put("evaluationState", 1);
        hospitalDevice.put("patientName", "王小二");

        Map hospitalDevice1 = new HashMap();
        hospitalDevice1.put("basisMeasurementId", "sadfadsfadsfadsf");
        hospitalDevice1.put("date", "2021-08-25");
        hospitalDevice1.put("evaluationState", 1); //1已评估 2待评估
        hospitalDevice1.put("patientName", "王小二");

        List list = new ArrayList();
        list.add(hospitalDevice);
        list.add(hospitalDevice1);

        Map resMap = new HashMap();
        resMap.put("dataList", list);
        resMap.put("allCount", 2);
        return result.ok().setData(resMap);
    }

    /**
    　* @description: 报告评估
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/22 10:24
    　* @param
    　* @return
    　*/
    @PostMapping("/reportAssess")
    public Result reportAssess(@RequestParam String basisMeasurementId, @RequestParam String assessment, @RequestParam int state){
        Result result = new Result();
        // todo 获取当前登录医生id---数据库保存评估信息
        return result.ok();
    }

    /**
    　* @description: 获取当前医生对应的评估列表
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/22 11:57
    　* @param
    　* @return
    　*/
    @GetMapping("/getassessmentlist")
    public Result getAssessmentList(@RequestParam(value = "pageNo",defaultValue = "1") int pageNo,
                                    @RequestParam(value = "pageSize",defaultValue ="10") int pageSize, @RequestParam String date){
        Result result = new Result();
//todo 按照date日期进行查询
        Map hospitalDevice = new HashMap();
        hospitalDevice.put("id", "sadfadsfadsfadsf");
        hospitalDevice.put("date", "2021-08-25");
        hospitalDevice.put("age", 1);
        hospitalDevice.put("patientName", "王小二");
        hospitalDevice.put("sex", 1); //0男 1女
        hospitalDevice.put("patientId", "asdfadsfasf");
        hospitalDevice.put("doctorName", "李大庄");

        Map hospitalDevice1 = new HashMap();
        hospitalDevice1.put("id", "sadfadsfadsfadsf");
        hospitalDevice1.put("date", "2021-08-25");
        hospitalDevice1.put("age", 1);
        hospitalDevice1.put("patientName", "王小二");
        hospitalDevice1.put("sex", 1); //0男 1女
        hospitalDevice1.put("patientId", "asdfadsfasf");
        hospitalDevice1.put("doctorName", "李大庄");

        List list = new ArrayList();
        list.add(hospitalDevice);
        list.add(hospitalDevice1);

        Map resMap = new HashMap();
        resMap.put("dataList", list);
        resMap.put("allCount", 2);
        return result.ok().setData(resMap);
    }

    /**
    　* @description: 获取当前医生规定时间内的所有诊断报告
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/23 9:23
    　* @param
    　* @return
    　*/
    @GetMapping("/getdiagnosticreportlist")
    public Result getDiagnosticReportList(@RequestParam(value = "pageNo",defaultValue = "1") int pageNo,
                                    @RequestParam(value = "pageSize",defaultValue ="10") int pageSize, @RequestParam String startDate, String endDate){
        Result result = new Result();

        Map hospitalDevice = new HashMap();
        hospitalDevice.put("id", "sadfadsfadsfadsf");
        hospitalDevice.put("date", "2021-08-25");
        hospitalDevice.put("age", 1);
        hospitalDevice.put("patientName", "王小二");
        hospitalDevice.put("sex", 1); //0男 1女
        hospitalDevice.put("patientId", "asdfadsfasf");
        hospitalDevice.put("doctorName", "李大庄");
        hospitalDevice.put("evaluation", "该患者身体良好，没有发现异常");

        Map hospitalDevice1 = new HashMap();
        hospitalDevice1.put("id", "sadfadsfadsfadsf");
        hospitalDevice1.put("date", "2021-08-25");
        hospitalDevice1.put("age", 1);
        hospitalDevice1.put("patientName", "王小二");
        hospitalDevice1.put("sex", 1); //0男 1女
        hospitalDevice1.put("patientId", "asdfadsfasf");
        hospitalDevice1.put("doctorName", "李大庄");
        hospitalDevice1.put("evaluation", "该患者身体良好，没有发现异常");

        List list = new ArrayList();
        list.add(hospitalDevice);
        list.add(hospitalDevice1);

        Map resMap = new HashMap();
        resMap.put("dataList", list);
        resMap.put("allCount", 2);
        return result.ok().setData(resMap);
    }

    /**
    　* @description: 查询当前医生所评估的所有报告
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/23 10:10
    　* @param
    　* @return
    　*/
    @GetMapping("/getdiagnosticreportlistbydoctor")
    public Result getDiagnosticReportListByDoctor(@RequestParam(value = "pageNo",defaultValue = "1") int pageNo,
                                          @RequestParam(value = "pageSize",defaultValue ="10") int pageSize, @RequestParam String startDate, String endDate){
        Result result = new Result();
        //todo 缓存获取医生id，数据按照评估日期分组
        Map hospitalDevice = new HashMap();
        hospitalDevice.put("id", "sadfadsfadsfadsf");
        hospitalDevice.put("date", "2021-08-25");
        hospitalDevice.put("doctorName", "李大庄");
        hospitalDevice.put("evaluationCount", 25);

        Map hospitalDevice1 = new HashMap();
        hospitalDevice1.put("id", "sadfadsfadsfadsf");
        hospitalDevice1.put("date", "2021-08-25");
        hospitalDevice1.put("doctorName", "李大庄");
        hospitalDevice1.put("evaluationCount", 25);

        List list = new ArrayList();
        list.add(hospitalDevice);
        list.add(hospitalDevice1);

        Map resMap = new HashMap();
        resMap.put("dataList", list);
        resMap.put("allCount", 2);
        return result.ok().setData(resMap);
    }

    /**
    　* @description: 4.3.2 获取详细信息
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/23 10:41
    　* @param
    　* @return
    　*/
    @GetMapping("/getinfo")
    public Result getInfo(@RequestParam String id){
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
        PatientDataInfo.put("additionalEvaluation", "多注意饮食规律");
        PatientDataInfo.put("additionalEvaluationDoctor", "张小三");
        PatientDataInfo.put("additionalDate", "2021-08-25");

        return result.ok().setData(PatientDataInfo);
    }
}
