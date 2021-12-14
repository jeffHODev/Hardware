package com.hospital.nivihospital.modules.hospital.service.impl;

import com.hospital.nivihospital.common.redis.RedisService;
import com.hospital.nivihospital.common.util.Servlets;
import com.hospital.nivihospital.common.util.ValidateUtil;
import com.hospital.nivihospital.modules.hospital.dao.AppAccountMapper;
import com.hospital.nivihospital.modules.hospital.entity.Accountpass;
import com.hospital.nivihospital.modules.hospital.entity.AppAccount;
import com.hospital.nivihospital.modules.hospital.service.IHospitalService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

/**
 * @author dong
 * @title: HospitalService
 * @description: 医院相关业务实现
 * @date 2021/12/2 16:15
 */
@Service
public class HospitalService implements IHospitalService {
    @Autowired
    private AppAccountMapper appAccountMapper;

    @Autowired
    private RedisService redisService;

    @Override
    public AppAccount selectByLoginName(String loginName) {
        return appAccountMapper.selectByLoginName(loginName);
    }

    @Override
    public void registered(String pass) {
        appAccountMapper.registered(pass);
    }

    @Override
    public void uppass(Accountpass account) {
        account.setNewPass(ValidateUtil.entryptPassword(account.getNewPass()));
        appAccountMapper.uppass(account);
    }

    @Override
    public void loginOut() {
        String token = Servlets.getRequest().getHeader("token");
        redisService.remove("appAccount:"+token);
    }
}
