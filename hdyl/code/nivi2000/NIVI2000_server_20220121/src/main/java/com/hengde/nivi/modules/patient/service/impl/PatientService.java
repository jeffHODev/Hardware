package com.hengde.nivi.modules.patient.service.impl;

import com.hengde.nivi.common.Service.UpdateEventService;
import com.hengde.nivi.common.config.HospitalConfig;
import com.hengde.nivi.common.util.IdGenerate;
import com.hengde.nivi.modules.hospital.entity.Device;
import com.hengde.nivi.modules.hospital.entity.Hospital;
import com.hengde.nivi.modules.patient.dao.PatientMapper;
import com.hengde.nivi.modules.patient.entity.Patient;
import com.hengde.nivi.modules.patient.service.IPatientService;
import org.apache.commons.lang3.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * @author dong
 * @title: PatientService
 * @description: 患者相关业务接口实现
 * @date 2019/7/3 13:48
 */
@Service
public class PatientService implements IPatientService {
    @Autowired
    private PatientMapper patientMapper;

    @Autowired
    private HospitalConfig hospitalConfig;

    @Autowired
    private UpdateEventService updateEventService;

    @Override
    public Patient getPatientInfo(String uniqueID) {
        Map parMap = new HashMap();
        parMap.put("uniqueID", uniqueID);
        return patientMapper.getPatientInfo(parMap);
    }

    @Override
    public Patient getPatientInfoByNP(String name, String phone) {
        Map parMap = new HashMap();
        parMap.put("name", name);
        parMap.put("phone", phone);
        return patientMapper.getPatientInfoByNP(parMap);
    }

    @Override
    public Patient getPatientByUniqueId(String uniqueId) {
        return patientMapper.getPatientByUniqueId(uniqueId);
    }

    @Override
    public Map createPatient(Patient patient) {
        String patientId = IdGenerate.uuid();
        patient.setId(patientId);
        SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");//设置日期格式
        patient.setCreateDate(df.format(new Date()));
        patient.setDelFlag(0);
        patientMapper.insertSelective(patient);
        Map resultMap = new HashMap();
        resultMap.put("patientId", patientId);
        return resultMap;
    }

    @Override
    public Map getPatientList(int pageNo, int pageSize) {
        Map parMap = new HashMap();
        Device device = hospitalConfig.getHospitalDevice();
        String hospitalDeviceId = device.getId();
        parMap.put("deviceId", hospitalDeviceId);
        parMap.put("pageNo", (pageNo - 1) * pageSize);
        parMap.put("pageSize", pageSize);
        List<Patient> patientList = patientMapper.getPatientList(parMap);
        int count = patientMapper.getPatientListCount(hospitalDeviceId);
        Map resultMap = new HashMap();
        resultMap.put("patientList", patientList);
        resultMap.put("pageNo", pageNo);
        resultMap.put("pageSize", patientList.size());
        resultMap.put("count", count);
        return resultMap;
    }

    @Override
    @Transactional
    public void updatePatient(Patient patient) {
        String patientId = patient.getId();

        Patient oldPatient = patientMapper.selectPatientById(patientId); //根据患者id查询患者基本信息
        Map parMap = new HashMap();
        parMap.put("objectId", patientId);
        parMap.put("objectType", "patient");
        Integer version = patientMapper.selectVersionByTimeIdName(parMap); //查询当前最新的版本号

        if(!StringUtils.isBlank(patient.getCaseNo())){
            if(StringUtils.isBlank(oldPatient.getCaseNo()) || !patient.getCaseNo().equals(oldPatient.getCaseNo())){
                updateEventService.updateEvent(version, "patient", patientId, "case_no", oldPatient.getCaseNo(), patient.getCaseNo());
            }
        }

        if(!StringUtils.isBlank(patient.getNationality())){
            if(StringUtils.isBlank(oldPatient.getNationality()) || !patient.getNationality().equals(oldPatient.getNationality())){
                updateEventService.updateEvent(version, "patient", patientId, "nationality", oldPatient.getNationality(), patient.getNationality());
            }
        }

        if(!StringUtils.isBlank(patient.getAddress())){
            if(StringUtils.isBlank(oldPatient.getAddress()) || !patient.getAddress().equals(oldPatient.getAddress())){
                updateEventService.updateEvent(version, "patient", patientId, "address", oldPatient.getAddress(), patient.getAddress());
            }
        }

        if(!StringUtils.isBlank(patient.getBirthday())){
            if(StringUtils.isBlank(oldPatient.getBirthday()) || !patient.getBirthday().equals(oldPatient.getBirthday())){
                updateEventService.updateEvent(version, "patient", patientId, "birthday", oldPatient.getBirthday(), patient.getBirthday());
            }
        }

        if(patient.getSex() != null){
            if(oldPatient.getSex() == null || !patient.getSex().equals(oldPatient.getSex())){
                updateEventService.updateEvent(version, "patient", patientId, "sex", String.valueOf(oldPatient.getSex()), String.valueOf(patient.getSex()));
            }
        }

        if(!StringUtils.isBlank(patient.getUniqueId())){
            if(StringUtils.isBlank(oldPatient.getUniqueId()) || !patient.getUniqueId().equals(oldPatient.getUniqueId())){
                updateEventService.updateEvent(version, "patient", patientId, "unique_id", oldPatient.getUniqueId(), patient.getUniqueId());
            }
        }

        patientMapper.updatePatient(patient);
    }

    @Override
    public Map getHistoryPatientlist(int pageNo, int pageSize) {
        Device device = hospitalConfig.getHospitalDevice();
        String hospitalDeviceId = device.getId();
        Map parMap = new HashMap();
        parMap.put("deviceId", hospitalDeviceId);
        parMap.put("pageNo", (pageNo - 1) * pageSize);
        parMap.put("pageSize", pageSize);
        List<Map> patientList = patientMapper.getHistoryPatientlist(parMap);
        int count = patientMapper.getHistoryPatientListCount(hospitalDeviceId);
        Map resultMap = new HashMap();
        resultMap.put("historyPatientList", patientList);
        resultMap.put("pageNo", pageNo);
        resultMap.put("pageSize", patientList.size());
        resultMap.put("count", count);
        return resultMap;
    }

    @Override
    public List<Map> getPatientHistorylist(String patientId) {
        Device device = hospitalConfig.getHospitalDevice();
        String hospitalDeviceId = device.getId();
        Map parMap = new HashMap();
        parMap.put("patientId", patientId);
        parMap.put("deviceId", hospitalDeviceId);
        List<Map> ph = patientMapper.getPatientHistorylist(parMap);
        String[] CN_NUMBER_NAME = {"零", "一", "二", "三", "四", "五", "六", "七", "八", "九", "十"};
        for (Map m : ph){
            Date d = (Date) m.get("diagDate");
            int mon = d.getMonth() + 1;
            if(mon <= 10) {
                m.put("month", CN_NUMBER_NAME[mon] + "月");
            } else {
                m.put("month", "十" + CN_NUMBER_NAME[mon%10] + "月");
            }
        }
        return ph;
    }
}
