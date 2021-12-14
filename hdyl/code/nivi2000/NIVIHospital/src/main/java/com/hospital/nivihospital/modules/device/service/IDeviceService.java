package com.hospital.nivihospital.modules.device.service;

import com.hospital.nivihospital.modules.hospital.entity.Accountpass;
import com.hospital.nivihospital.modules.hospital.entity.AppAccount;

import java.util.List;
import java.util.Map;

/**
 * @author dong
 * @title: IDealersService
 * @description: 设备相关业务接口
 * @date 2021/10/2 16:14
 */
public interface IDeviceService {
    /**
    　* @description: 获取医院对应设备列表
    　* @author dong
    　* @version: V1.0
    　* @date 2021/12/3 10:40
    　* @param
    　* @return
    　*/
    Map getdetectcount(String hospitalId, int pageNo, int pageSize);
}
