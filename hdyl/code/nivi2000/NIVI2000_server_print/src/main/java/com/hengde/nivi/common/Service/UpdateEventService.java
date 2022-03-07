package com.hengde.nivi.common.Service;

import com.hengde.nivi.common.config.HospitalConfig;
import com.hengde.nivi.modules.hospital.entity.Device;
import com.hengde.nivi.modules.hospital.entity.Hospital;
import com.hengde.nivi.modules.patient.dao.PatientMapper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

/**
 * @author dong
 * @title: UpdateEventService
 * @description: 修改记录公共服务
 * @date 2019/8/29 16:44
 */
@Component
public class UpdateEventService {
    @Autowired
    private HospitalConfig hospitalConfig;

    @Autowired
    private PatientMapper patientMapper;

    /**
    　* @description: 记录修改记录信息公共方法
    　* @author dong
    　* @version: V1.0
    　* @date 2019/8/30 13:52
    　* @param
                ：version        版本号
                ：objectType     源数据的表名
                ：objectId       源数据表主键
                ：fieldName      修改的源数据表的字段名
                ：fieldOldValue  该字段原来的值
                ：fieldNewValue  该字段最新的值
    　* @return VOID
    　*/
    public void updateEvent(Integer version, String objectType, String objectId, String fieldName, String fieldOldValue, String fieldNewValue){
        Device device = hospitalConfig.getHospitalDevice();
        String hospitalDeviceId = device.getId();

        Map parMap = new HashMap();
        parMap.put("objectType", objectType);
        parMap.put("objectId", objectId);
        parMap.put("fieldName", fieldName);
        parMap.put("fieldOldValue", fieldOldValue);
        parMap.put("fieldNewValue", fieldNewValue);
        parMap.put("updateTime", new Date());
        parMap.put("updateUser", hospitalDeviceId);

        if(version != null){
            parMap.put("version", version + 1);
            patientMapper.addUpdateEvent(parMap); //添加/更新记录
        }else{
            parMap.put("version", 1);
            patientMapper.addUpdateEvent(parMap); //添加/更新记录
        }
    }
}
