package com.hospital.nivihospital.modules.dealers.service.impl;

import com.hospital.nivihospital.common.redis.RedisService;
import com.hospital.nivihospital.common.util.Servlets;
import com.hospital.nivihospital.common.util.ValidateUtil;
import com.hospital.nivihospital.modules.dealers.dao.SysDealersMapper;
import com.hospital.nivihospital.modules.dealers.service.IDealersService;
import com.hospital.nivihospital.modules.hospital.dao.AppAccountMapper;
import com.hospital.nivihospital.modules.hospital.entity.Accountpass;
import com.hospital.nivihospital.modules.hospital.entity.AppAccount;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.HashMap;
import java.util.Map;

/**
 * @author dong
 * @title: HospitalService
 * @description: 医院相关业务实现
 * @date 2019/7/2 16:15
 */
@Service
public class DealersService implements IDealersService {
    @Autowired
    private SysDealersMapper sysDealersMapper;

    @Override
    public Map getDealersHospitalList(AppAccount account, int pageNo, int pageSize) {
        Map resMap = new HashMap();
        int basisMeasurementAllCount = sysDealersMapper.selectBasisMeasurementAllCount(account.getEntityId()); //当前经销商所有检测记录总数
        int evaluationAllCount = sysDealersMapper.selectEvaluationAllCount(account.getEntityId()); //当前经销商下所有已评估总数
        resMap.put("evaluationCountAll", evaluationAllCount);
        resMap.put("notEvaluationCount", basisMeasurementAllCount - evaluationAllCount);

        Map parMap = new HashMap();
        parMap.put("entityId", account.getEntityId());
        parMap.put("pageNo", (pageNo - 1) * pageSize);
        parMap.put("pageSize", pageSize);

        resMap.put("dataList", sysDealersMapper.selectHospitalList(parMap));
        resMap.put("allCount", sysDealersMapper.selectHospitalListCount(account.getEntityId()));

        return resMap;
    }

    @Override
    public Map getHospitalDeviceList(String hospitalId, int pageNo, int pageSize) {
        Map resMap = new HashMap();
        int basisMeasurementallCountByHospital = sysDealersMapper.selectBasisMeasurementAllCountByHospital(hospitalId); //某医院下的所有检测记录总数
        int evaluationAllCountByHospita = sysDealersMapper.selectEvaluationAllCountByHospital(hospitalId); //某医院下所有已评估总数
        resMap.put("evaluationCountAll", evaluationAllCountByHospita);
        resMap.put("notEvaluationCount", basisMeasurementallCountByHospital - evaluationAllCountByHospita);

        Map parMap = new HashMap();
        parMap.put("hospitalId", hospitalId);
        parMap.put("pageNo", (pageNo - 1) * pageSize);
        parMap.put("pageSize", pageSize);

        resMap.put("dataList", sysDealersMapper.selectDeviceListByHospital(parMap));
        resMap.put("allCount", sysDealersMapper.selectDeviceListByHospitalCount(hospitalId));
        return resMap;
    }
}
