package com.hospital.nivihospital.modules.hospital.service;

import com.hospital.nivihospital.modules.hospital.entity.Accountpass;
import com.hospital.nivihospital.modules.hospital.entity.AppAccount;

/**
 * @author dong
 * @title: IDealersService
 * @description: 医院相关业务接口
 * @date 2021/11/2 16:14
 */
public interface IHospitalService {
    /**
    　* @description: 根据账户查询账户信息
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/29 11:48
    　* @param
    　* @return
    　*/
    AppAccount selectByLoginName(String loginName);

    /**
    　* @description: 注册
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/29 12:24
    　* @param
    　* @return
    　*/
    void registered(String pass);

    /**
    　* @description: 修改密码
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/30 11:09
    　* @param
    　* @return
    　*/
    void uppass(Accountpass account);

    /**
    　* @description: 退出登录
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/30 11:28
    　* @param
    　* @return
    　*/
    void loginOut();
}
