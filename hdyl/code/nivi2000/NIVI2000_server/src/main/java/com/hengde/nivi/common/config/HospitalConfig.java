package com.hengde.nivi.common.config;

import com.hengde.nivi.common.redis.RedisService;
import com.hengde.nivi.common.util.Servlets;
import com.hengde.nivi.modules.hospital.entity.Device;
import com.hengde.nivi.modules.hospital.entity.Hospital;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

/**
 * @author dong
 * @title: Hospital
 * @description: 医院相关配置
 * @date 2019/8/6 11:22
 */
@Component
public class HospitalConfig {
    @Autowired
    private RedisService redisService;

    /**
    　* @description: 获取医院设备信息
    　* @author dong
    　* @version: V1.0
    　* @date 2019/8/6 11:29
    　* @param
    　* @return
    　*/
    public Device getHospitalDevice(){
        String token = Servlets.getRequest().getHeader("token");
        Device device = (Device) redisService.get("hosptialdevicelogin:"+token);
        return device;
    }
}
