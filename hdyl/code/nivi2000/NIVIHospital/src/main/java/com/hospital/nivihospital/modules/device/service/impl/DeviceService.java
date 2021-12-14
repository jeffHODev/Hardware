package com.hospital.nivihospital.modules.device.service.impl;

import com.hospital.nivihospital.modules.device.dao.DeviceMapper;
import com.hospital.nivihospital.modules.device.service.IDeviceService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * @author dong
 * @title: DeviceService
 * @description: TODO
 * @date 2021/12/3 10:37
 */
@Service
public class DeviceService implements IDeviceService {

    @Autowired
    private DeviceMapper deviceMapper;

    @Override
    public Map getdetectcount(String hospitalId, int pageNo, int pageSize) {
        Map resMap = new HashMap();
        Map parMap = new HashMap();
        parMap.put("hospitalId", hospitalId);
        parMap.put("pageNo", (pageNo - 1) * pageSize);
        parMap.put("pageSize", pageSize);

        resMap.put("dataList", deviceMapper.selectDetectCountByHospital(parMap));
        resMap.put("allCount", deviceMapper.selectDetectCountByHospitalCount(hospitalId));
        return resMap;
    }
}
