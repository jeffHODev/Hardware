package com.hospital.nivihospital.modules.hospital.dao;

import com.hospital.nivihospital.modules.hospital.entity.Accountpass;
import com.hospital.nivihospital.modules.hospital.entity.AppAccount;

public interface AppAccountMapper {

    /**
    　* @description: 根据账号查询对应账户信息
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/29 11:48
    　* @param
    　* @return
    　*/
    AppAccount selectByLoginName(String loginName);

    /**
    　* @description: 更新密码
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/29 12:25
    　* @param
    　* @return
    　*/
    int registered(String pass);

    /**
    　* @description: 修改密码
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/30 11:18
    　* @param
    　* @return
    　*/
    int uppass(Accountpass account);

    int deleteByPrimaryKey(String id);

    int insert(AppAccount record);

    int insertSelective(AppAccount record);

    AppAccount selectByPrimaryKey(String id);

    int updateByPrimaryKeySelective(AppAccount record);

    int updateByPrimaryKey(AppAccount record);
}