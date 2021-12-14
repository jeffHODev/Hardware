package com.hospital.nivihospital.modules.doctorevaluation.contorller;

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
 * @title: EvaluationController
 * @description: 医生评估相关业务控制器
 * @date 2021/11/16 10:07
 */
@RestController
@RequestMapping("/api/evaluation")
public class EvaluationController {
    /**
    　* @description: 数据查询
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/16 10:46
    　* @param
    　* @return
    　*/
    @GetMapping("/getevaluationdata")
    public Result getEvaluationData(@RequestParam String deviceId, @RequestParam(value = "pageNo",defaultValue = "1") int pageNo,
                                    @RequestParam(value = "pageSize",defaultValue ="10") int pageSize){
        Result result = new Result();
        Map evaluationCount = new HashMap();
        evaluationCount.put("evaluationCountAll", 8);
        evaluationCount.put("notEvaluationCount", 3);

        Map evaluationData1 = new HashMap();
        evaluationData1.put("doctor", "李大庄");
        evaluationData1.put("doctorId", "sadfasdfsadfads");
        evaluationData1.put("selectDate", "2021-08-05");
        evaluationData1.put("evaluationCount", 2);

        Map evaluationData2 = new HashMap();
        evaluationData2.put("doctor", "李大庄");
        evaluationData2.put("doctorId", "sadfasdfsadfads");
        evaluationData2.put("selectDate", "2021-08-06");
        evaluationData2.put("evaluationCount", 2);

        List list = new ArrayList();
        list.add(evaluationData1);
        list.add(evaluationData2);

        evaluationCount.put("evaluationList", list);
        evaluationCount.put("allCount", 2);
        return result.ok().setData(evaluationCount);
    }

    /**
    　* @description: 开始日期结束日期对应设备的历史数据
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/16 11:44
    　* @param
    　* @return
    　*/
    @GetMapping("/getevaluationhisdata")
    public Result getEvaluationHisData(@RequestParam String deviceId, @RequestParam String startDate, String endDate, @RequestParam(value = "pageNo",defaultValue = "1") int pageNo,
                                       @RequestParam(value = "pageSize",defaultValue ="10") int pageSize){
        Result result = new Result();

        if(null == endDate || endDate.equals("") || endDate.length() == 0){ //endDate没传默认查询startDate当天数据

        }

        Map evaluationHisCount = new HashMap();
        evaluationHisCount.put("evaluationHisCountAll", 8);
        evaluationHisCount.put("notEvaluationHisCount", 3);

        Map evaluationHisData1 = new HashMap();
        evaluationHisData1.put("id", "asdfsadfas1");
        evaluationHisData1.put("patientName", "王小二");
        evaluationHisData1.put("age", 23);
        evaluationHisData1.put("sex", 1);
        evaluationHisData1.put("patientId", "sadfsdfdasf");
        evaluationHisData1.put("date", "2021-08-25");
        evaluationHisData1.put("doctorName", "李大庄");

        Map evaluationHisData2 = new HashMap();
        evaluationHisData2.put("id", "asdfsadfas1");
        evaluationHisData2.put("patientName", "王小二");
        evaluationHisData2.put("age", 23);
        evaluationHisData2.put("sex", 1);
        evaluationHisData2.put("patientId", "sadfsdfdasf");
        evaluationHisData2.put("date", "2021-08-25");
        evaluationHisData2.put("doctorName", "李大庄");

        List list = new ArrayList();
        list.add(evaluationHisData1);
        list.add(evaluationHisData2);

        evaluationHisCount.put("evaluationHisList", list);
        evaluationHisCount.put("allCount", 2);
        return result.ok().setData(evaluationHisCount);
    }

    /**
     　* @description: 开始日期结束日期对应设备下的指定医生的所有患者的历史数据
     　* @author dong
     　* @version: V1.0
     　* @date 2021/11/16 11:44
     　* @param
     　* @return
     　*/
    @GetMapping("/getevaluationhisdatabydoctor")
    public Result getEvaluationHisdataByDoctor(@RequestParam String deviceId, @RequestParam String doctorId, @RequestParam String startDate, @RequestParam String endDate, @RequestParam(value = "pageNo",defaultValue = "1") int pageNo,
                                               @RequestParam(value = "pageSize",defaultValue ="10") int pageSize){
        Result result = new Result();
        Map evaluationHisCount = new HashMap();
        evaluationHisCount.put("evaluationHisCountAll", 8);
        evaluationHisCount.put("notEvaluationHisCount", 3);

        Map evaluationHisData1 = new HashMap();
        evaluationHisData1.put("id", "asdfsadfas1");
        evaluationHisData1.put("patientName", "王小二");
        evaluationHisData1.put("age", 23);
        evaluationHisData1.put("sex", 1);
        evaluationHisData1.put("patientId", "sadfsdfdasf");
        evaluationHisData1.put("date", "2021-08-25");
        evaluationHisData1.put("doctorName", "李大庄");

        Map evaluationHisData2 = new HashMap();
        evaluationHisData2.put("id", "asdfsadfas1");
        evaluationHisData2.put("patientName", "王小二");
        evaluationHisData2.put("age", 23);
        evaluationHisData2.put("sex", 1);
        evaluationHisData2.put("patientId", "sadfsdfdasf");
        evaluationHisData2.put("date", "2021-08-25");
        evaluationHisData2.put("doctorName", "李大庄");

        List list = new ArrayList();
        list.add(evaluationHisData1);
        list.add(evaluationHisData2);

        evaluationHisCount.put("evaluationHisList", list);
        evaluationHisCount.put("allCount", 2);
        return result.ok().setData(evaluationHisCount);
    }

    /**
    　* @description: 2.2.3获取设备下所有医生评估信息
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/17 11:25
    　* @param 
    　* @return 
    　*/
    @GetMapping("/getpatientevalistbydevice")
    public Result getPatientEvaListByDevice(@RequestParam String deviceId, @RequestParam(value = "pageNo",defaultValue = "1") int pageNo,
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
        PatientDataInfo.put("evaluation", "该患者身体良好，没有发现异常");

        Map PatientDataInfo1 = new HashMap();
        PatientDataInfo1.put("id", "asdfsadfas");
        PatientDataInfo1.put("patientName", "王小二");
        PatientDataInfo1.put("age", 23);
        PatientDataInfo1.put("sex", 1);
        PatientDataInfo1.put("patientId", "sadfsdfdasf");
        PatientDataInfo1.put("date", "2021-08-25");
        PatientDataInfo1.put("doctorName", "李大庄");
        PatientDataInfo1.put("evaluation", "该患者身体良好，没有发现异常");

        List list = new ArrayList();
        list.add(PatientDataInfo);
        list.add(PatientDataInfo1);

        Map resMap = new HashMap();
        resMap.put("dataList", list);
        resMap.put("allCount", 2);

        return result.ok().setData(resMap);
    }

    /**
    　* @description: 查询当前医院下所有设备对应患者的评估信息
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/17 11:47
    　* @param
    　* @return
    　*/
    @GetMapping("/getpatientlistevalistbydevice")
    public Result getPatientListEvaListByDevice(String patientId, @RequestParam(value = "pageNo",defaultValue = "1") int pageNo,
                                            @RequestParam(value = "pageSize",defaultValue ="10") int pageSize, @RequestParam String startDate, String endDate){
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

        Map PatientDataInfo1 = new HashMap();
        PatientDataInfo1.put("id", "asdfsadfas");
        PatientDataInfo1.put("patientName", "王小二");
        PatientDataInfo1.put("age", 23);
        PatientDataInfo1.put("sex", 1);
        PatientDataInfo1.put("patientId", "sadfsdfdasf");
        PatientDataInfo1.put("date", "2021-08-25");
        PatientDataInfo1.put("doctorName", "李大庄");
        PatientDataInfo1.put("evaluation", "该患者身体良好，没有发现异常");

        List list = new ArrayList();
        list.add(PatientDataInfo);
        list.add(PatientDataInfo1);

        Map resMap = new HashMap();
        resMap.put("dataList", list);
        resMap.put("allCount", 2);

        return result.ok().setData(resMap);
    }

    /**
    　* @description: 获取患者报告详情含有追加评估
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/17 12:03
    　* @param
    　* @return
    　*/
    @GetMapping("/getpatientevainfoadditional")
    public Result getPatientEvaInfoAdditional(@RequestParam String id){
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

    /**
    　* @description: 当前医院日期范围内根据医生查询评估过的患者数据
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/17 12:15
    　* @param
    　* @return
    　*/
    @GetMapping("/getpatientlistevalistbydoctor")
    public Result getPatientListEvaListByDoctor(String doctorId, @RequestParam(value = "pageNo",defaultValue = "1") int pageNo,
                                                @RequestParam(value = "pageSize",defaultValue ="10") int pageSize, @RequestParam String startDate, String endDate){
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

        Map PatientDataInfo1 = new HashMap();
        PatientDataInfo1.put("id", "asdfsadfas");
        PatientDataInfo1.put("patientName", "王小二");
        PatientDataInfo1.put("age", 23);
        PatientDataInfo1.put("sex", 1);
        PatientDataInfo1.put("patientId", "sadfsdfdasf");
        PatientDataInfo1.put("date", "2021-08-25");
        PatientDataInfo1.put("doctorName", "李大庄");
        PatientDataInfo1.put("evaluation", "该患者身体良好，没有发现异常");

        List list = new ArrayList();
        list.add(PatientDataInfo);
        list.add(PatientDataInfo1);

        Map resMap = new HashMap();
        resMap.put("dataList", list);
        resMap.put("allCount", 2);

        return result.ok().setData(resMap);
    }

    /**
    　* @description: 3.6 根据设备id查询历史数据，医生、结束日期选填
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/19 10:43
    　* @param
    　* @return
    　*/
    @GetMapping("/gethistlistevalistbydoctor")
    public Result getHistListEvaListByDoctor(String doctorId, @RequestParam String deviceId, @RequestParam(value = "pageNo",defaultValue = "1") int pageNo,
                                                @RequestParam(value = "pageSize",defaultValue ="10") int pageSize, @RequestParam String startDate, String endDate){
        Result result = new Result();

        //todo 查询设备对应的评估记录，根据医生分组

        Map PatientDataInfo = new HashMap();
        PatientDataInfo.put("doctorId", "asdfsadfas");
        PatientDataInfo.put("date", "2021-08-25");
        PatientDataInfo.put("doctorName", "李大庄");
        PatientDataInfo.put("evaluationCount", 25);

        Map PatientDataInfo1 = new HashMap();
        PatientDataInfo1.put("doctorId", "asdfsadfas");
        PatientDataInfo1.put("date", "2021-08-25");
        PatientDataInfo1.put("doctorName", "李大庄");
        PatientDataInfo1.put("evaluationCount", 25);

        List list = new ArrayList();
        list.add(PatientDataInfo);
        list.add(PatientDataInfo1);

        Map resMap = new HashMap();
        resMap.put("dataList", list);
        resMap.put("allCount", 2);

        return result.ok().setData(resMap);
    }

    /**
    　* @description: 统计某医生数据
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/19 11:18
    　* @param
    　* @return
    　*/
    @GetMapping("/getdatastatistical")
    public Result getDataStatistical(@RequestParam String doctorId, @RequestParam(value = "pageNo",defaultValue = "1") int pageNo,
                                             @RequestParam(value = "pageSize",defaultValue ="10") int pageSize){
        Result result = new Result();

        Map PatientDataInfo = new HashMap();
        PatientDataInfo.put("id", "asdfsadfas");
        PatientDataInfo.put("doctorName", "李大庄");
        PatientDataInfo.put("evaluationAllCount", 25);
        PatientDataInfo.put("cooperationDays", 100);
        PatientDataInfo.put("diagnosisPatientCount", 50);

        Map PatientDataInfo1 = new HashMap();
        PatientDataInfo1.put("id", "asdfsadfas");
        PatientDataInfo1.put("doctorName", "李大庄");
        PatientDataInfo1.put("evaluationAllCount", 25);
        PatientDataInfo1.put("cooperationDays", 100);
        PatientDataInfo1.put("diagnosisPatientCount", 50);

        List list = new ArrayList();
        list.add(PatientDataInfo);
        list.add(PatientDataInfo1);

        Map resMap = new HashMap();
        resMap.put("dataList", list);
        resMap.put("allCount", 2);

        return result.ok().setData(resMap);
    }
}
