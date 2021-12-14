package com.hengde.nivi.modules.hospital.service;

import com.hengde.nivi.common.response.Result;
import com.hengde.nivi.modules.hospital.entity.Device;
import com.hengde.nivi.modules.hospital.entity.Devpass;
import com.hengde.nivi.modules.hospital.entity.Hospital;

/**
 * @author dong
 * @title: IHospitalService
 * @description: 医院相关业务接口
 * @date 2019/7/2 16:14
 */
public interface IHospitalService {
    /**
    　* @description: 根据医院设备账号查询医院设备信息
    　* @author dong
    　* @version: V1.0
    　* @date 2019/7/2 16:17
    　* @param
    　* @return
    　*/
    Device selectByLoginName(String loginName);

    /**
    　* @description: 医院退出登录
    　* @author dong
    　* @version: V1.0
    　* @date 2019/7/3 10:23
    　* @param
    　* @return
    　*/
    void loginOut();

    /**
    　* @description: 设备初始化
    　* @author dong
    　* @version: V1.0
    　* @date 2020/11/30 15:22
    　* @param
    　* @return
    　*/
    void deviceinit(String deviceNo, String password);

    void uppass(Devpass devpass);
}
