package com.hengde.nivi.modules.hospital.service.impl;

import com.hengde.nivi.common.redis.RedisService;
import com.hengde.nivi.common.response.Result;
import com.hengde.nivi.common.util.IdGenerate;
import com.hengde.nivi.common.util.Servlets;
import com.hengde.nivi.common.util.ValidateUtil;
import com.hengde.nivi.modules.hospital.dao.HospitalMapper;
import com.hengde.nivi.modules.hospital.entity.Device;
import com.hengde.nivi.modules.hospital.entity.Devpass;
import com.hengde.nivi.modules.hospital.entity.Hospital;
import com.hengde.nivi.modules.hospital.service.IHospitalService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import java.util.Date;

/**
 * @author dong
 * @title: HospitalService
 * @description: 医院相关业务实现
 * @date 2019/7/2 16:15
 */
@Service
public class HospitalService implements IHospitalService {
    @Autowired
    private HospitalMapper hospitalMapper;

    @Autowired
    private RedisService redisService;

    @Override
    public Device selectByLoginName(String loginName) {
        return hospitalMapper.selectByLoginName(loginName);
    }

    @Override
    public void loginOut() {
        String token = Servlets.getRequest().getHeader("token");
        redisService.remove("hosptialdevicelogin:"+token);
    }

    @Override
    public void deviceinit(String deviceNo, String password) {
        Device device = new Device();
        String deviceId = IdGenerate.uuid();
        device.setId(deviceId);
        device.setDeviceNo(deviceNo);
        device.setPassword(ValidateUtil.entryptPassword(password)); //默认初始密码
        device.setCreateDate(new Date());
        hospitalMapper.insertDevice(device);
    }

    @Override
    public void uppass(Devpass devpass) {
        devpass.setNewPass(ValidateUtil.entryptPassword(devpass.getNewPass()));
        hospitalMapper.updateDevicePass(devpass);
    }
}
