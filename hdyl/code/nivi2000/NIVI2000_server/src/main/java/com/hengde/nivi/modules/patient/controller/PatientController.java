package com.hengde.nivi.modules.patient.controller;

import com.hengde.nivi.common.config.HospitalConfig;
import com.hengde.nivi.common.response.Result;
import com.hengde.nivi.modules.hospital.entity.Device;
import com.hengde.nivi.modules.hospital.entity.Hospital;
import com.hengde.nivi.modules.patient.dao.PatientMapper;
import com.hengde.nivi.modules.patient.entity.Patient;
import com.hengde.nivi.modules.patient.service.IPatientService;
import org.apache.commons.lang3.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static org.apache.commons.lang3.StringUtils.isBlank;

/**
 * @author dong
 * @title: PatientController
 * @description: 患者相关控制器
 * @date 2019/6/6 17:24
 */
@RestController
@RequestMapping("/api/patient")
public class PatientController {
    @Autowired
    private IPatientService patientService;

    @Autowired
    private HospitalConfig hospitalConfig;

    @Autowired
    private PatientMapper patientMapper;

    /**
    　* @description: 必填信息校验公共方法
    　* @author dong
    　* @version: V1.0
    　* @date 2021/3/30 19:28
    　* @param
    　* @return
    　*/
    public String check(Patient patient){
        if(isBlank(patient.getName())){
            return "患者姓名为空";
        }
//        if(isBlank(patient.getPhone())){
//            return "患者手机号为空";
//        }
        if(isBlank(patient.getBirthday())){
            return "患者出生日期为空";
        }
        if(patient.getSex() == null){
            return "患者性别为空";
        }
        if(isBlank(patient.getUniqueId())){
            return "患者身份证号为空";
        }
        return null;
    }

    /**
     　* @description: 创建患者基本信息
     　* @author dong
     　* @version: V1.0
     　* @date 2019/6/6 17:20
     　* @param
     　* @return
     　*/
    @PostMapping("/create")
    public Result createPatient(@RequestBody Patient patient){
        Result result = new Result();

        String msg = check(patient);
        if(msg != null){
            return result.setStatus(2, msg);
        }

//        Map parMap = new HashMap();
//        parMap.put("name", patient.getName());
//        parMap.put("phone", patient.getPhone());
//        Patient p = patientMapper.getPatientInfoByNP(parMap);
//        if(p != null){
//            return result.setStatus(2, "患者信息已存在");
//        }

        if(!isBlank(patient.getPhone())){
            Patient pp = patientMapper.getPatientInfoByPhone(patient.getPhone());
            if(pp != null){
                return result.setStatus(2, "该手机号对应的患者信息已存在");
            }
        }

        Patient pu = patientMapper.getPatientInfoByUniqueId(patient.getUniqueId());
        if(pu != null){
            return result.setStatus(2, "该身份证号对应的患者信息已存在");
        }

        return result.ok().setData(patientService.createPatient(patient));
    }

    /**
    　* @description: 查询患者基本信息(身份证号)
    　* @author dong
    　* @version: V1.0
    　* @date 2019/6/6 17:27
    　* @param uniqueID
    　* @return
    　*/
    @GetMapping("/info")
    public Result patientInfo(@RequestParam String uniqueId){
        Result result = new Result();
        Patient p = patientService.getPatientInfo(uniqueId);
        if(null == p){
            return result.setStatus(2, "没有查询到相关患者记录");
        }
        List<Patient> list = new ArrayList<>();
        list.add(p);
        Map resMap = new HashMap();
        resMap.put("patientList", list);
        return result.ok().setData(resMap);
    }

    /**
    　* @description: 根据患者姓名、手机号获取患者基本信息
    　* @author dong
    　* @version: V1.0
    　* @date 2019/8/21 17:27
    　* @param
    　* @return
    　*/
    @GetMapping("/getinfo")
    public Result getPatientInfo(@RequestParam String name, @RequestParam String phone){
        Result result = new Result();
        Patient p = patientService.getPatientInfoByNP(name, phone);
        if(p == null){
            result.setData(new HashMap());
            return result.okdata();
        }
        return result.ok().setData(p);
    }

    /**
     　* @description: 根据患者身份证号获取患者基本信息
     　* @author dong
     　* @version: V1.0
     　* @date 2021/7/13 11:27
     　* @param
     　* @return
     　*/
    @GetMapping("/getinfobyuniqueid")
    public Result getPatientByUniqueId(@RequestParam String uniqueId){
        Result result = new Result();
        Patient p = patientService.getPatientByUniqueId(uniqueId);
        if(p == null){
            result.setData(new HashMap());
            return result.okdata();
        }
        return result.ok().setData(p);
    }

    /**
    　* @description: 分页获取医院设备对应患者列表
    　* @author dong
    　* @version: V1.0
    　* @date 2019/8/6 11:08
    　* @param
    　* @return
    　*/
    @GetMapping("/patientlist")
    public Result patientList(@RequestParam(value = "pageNo",defaultValue = "1") int pageNo,
                              @RequestParam(value = "pageSize",defaultValue ="10") int pageSize){
        Result result = new Result();

        Device device = hospitalConfig.getHospitalDevice();
        if(device == null){
            return result.setStatus(2, "设备登录信息为空，请从新登录");
        }

        return result.ok().setData(patientService.getPatientList(pageNo, pageSize));
    }

    /**
    　* @description: 修改患者基本信息
    　* @author dong
    　* @version: V1.0
    　* @date 2019/8/21 18:00
    　* @param
    　* @return
    　*/
    @PostMapping("/updatepatient")
    public Result updatePatient(@RequestBody Patient patient){
        Result result = new Result();

        Device device = hospitalConfig.getHospitalDevice();
        if(device == null){
            return result.setStatus(2, "设备登录信息为空，请从新登录");
        }

        if(isBlank(patient.getId())){
            return result.setStatus(2, "患者id为空");
        }

        String msg = check(patient);
        if(msg != null){
            return result.setStatus(2, msg);
        }

//        Map parMap = new HashMap();
//        parMap.put("name", patient.getName());
//        parMap.put("phone", patient.getPhone());
//        parMap.put("id", patient.getId());
//        Patient p = patientMapper.getPatientInfoByNPNotId(parMap);
//        if(p != null){
//            return result.setStatus(2, "患者信息已存在");
//        }

        if(!isBlank(patient.getPhone())){
            Map phMap = new HashMap();
            phMap.put("phone", patient.getPhone());
            phMap.put("id", patient.getId());
            Patient pp = patientMapper.getPatientInfoByPhoneNotId(phMap);
            if(pp != null){
                return result.setStatus(2, "该手机号对应的患者信息已存在");
            }
        }

        Map unMap = new HashMap();
        unMap.put("uniqueId", patient.getUniqueId());
        unMap.put("id", patient.getId());
        Patient pu = patientMapper.getPatientInfoByUniqueIdNotId(unMap);
        if(pu != null){
            return result.setStatus(2, "该身份证号对应的患者信息已存在");
        }

        patientService.updatePatient(patient);
        return result.ok();
    }

    /**
    　* @description: 根据设备分页获取对应历史患者检测记录
    　* @author dong
    　* @version: V1.0
    　* @date 2019/9/6 10:40
    　* @param
    　* @return
    　*/
    @GetMapping("/historypatientlist")
    public Result historyPatientlist(@RequestParam(value = "pageNo",defaultValue = "1") int pageNo,
                              @RequestParam(value = "pageSize",defaultValue ="10") int pageSize){
        Result result = new Result();

        Device device = hospitalConfig.getHospitalDevice();
        if(device == null){
            return result.setStatus(2, "设备登录信息为空，请从新登录");
        }

        return result.ok().setData(patientService.getHistoryPatientlist(pageNo, pageSize));
    }

    /**
     　* @description: 根据患者id获取对应历史检测记录
     　* @author dong
     　* @version: V1.0
     　* @date 2019/12/27 10:50
     　* @param
     　* @return
     　*/
    @GetMapping("/patienthistorylist")
    public Result historyPatientlist(@RequestParam String patientId){
        Result result = new Result();

        Device device = hospitalConfig.getHospitalDevice();
        if(device == null){
            return result.setStatus(2, "设备登录信息为空，请从新登录");
        }

        return result.ok().setData(patientService.getPatientHistorylist(patientId));
    }
}
