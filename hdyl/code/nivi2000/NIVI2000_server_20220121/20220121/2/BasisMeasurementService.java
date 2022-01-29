package com.hengde.nivi.modules.create.service.impl;

import com.hengde.nivi.common.config.HospitalConfig;
import com.hengde.nivi.common.util.IdGenerate;
import com.hengde.nivi.modules.create.dao.BasisMeasurementMapper;
import com.hengde.nivi.modules.create.entity.BasisMeasurement;
import com.hengde.nivi.modules.create.service.IBasisMeasurementService;
import com.hengde.nivi.modules.hospital.entity.Device;
import com.hengde.nivi.modules.pdftest.Entity;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.Date;
import java.util.HashMap;
import java.util.Map;

/**
 * @author dong
 * @title: BasisMeasurementService
 * @description: 血压数据业务实现
 * @date 2019/7/3 14:55
 */
@Service
public class BasisMeasurementService implements IBasisMeasurementService {
    @Autowired
    private BasisMeasurementMapper basisMeasurementMapper;

    @Autowired
    private HospitalConfig hospitalConfig;

    @Override
    public Map saveBloodPressure(BasisMeasurement basisMeasurement) {
        String basisMeasurementId = IdGenerate.uuid();
        basisMeasurement.setId(basisMeasurementId);
        basisMeasurement.setDelFlag((byte)0);
        basisMeasurement.setDiagDate(new Date());
        Device device = hospitalConfig.getHospitalDevice();
        basisMeasurement.setDeviceId(device.getId());
        basisMeasurementMapper.insertSelective(basisMeasurement);
        Map resultMap = new HashMap();
        resultMap.put("basisMeasurementId", basisMeasurementId);
        return resultMap;
    }

    @Override
    public BasisMeasurement getPatientBasisMeasurement(String basisMeasurementId) {
        return basisMeasurementMapper.getPatientBasisMeasurement(basisMeasurementId);
    }

    @Override
    public Entity selectPDFInfoBybasisMeasurementId(String id) {
        return basisMeasurementMapper.selectPDFInfoBybasisMeasurementId(id);
    }
}
