package com.hospital.nivihospital.modules.dealers.service;

import com.hospital.nivihospital.modules.hospital.entity.Accountpass;
import com.hospital.nivihospital.modules.hospital.entity.AppAccount;

import java.util.Map;

/**
 * @author dong
 * @title: IDealersService
 * @description: 经销商相关业务接口
 * @date 2021/11/2 16:14
 */
public interface IDealersService {

    /**
    　* @description: 经销商登录，获取对应信息数据
    　* @author dong
    　* @version: V1.0
    　* @date 2021/12/2 10:09
    　* @param
    　* @return
    　*/
    Map getDealersHospitalList(AppAccount account, int pageNo, int pageSize);

    /**
    　* @description: 获取当前医院对应的设备列表
    　* @author dong
    　* @version: V1.0
    　* @date 2021/12/3 9:26
    　* @param
    　* @return
    　*/
    Map getHospitalDeviceList(String hospitalId, int pageNo, int pageSize);
}
