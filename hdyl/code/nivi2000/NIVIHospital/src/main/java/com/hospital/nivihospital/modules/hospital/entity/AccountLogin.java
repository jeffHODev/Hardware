package com.hospital.nivihospital.modules.hospital.entity;

import lombok.Data;

/**
 * @author dong
 * @title: AccountLogin
 * @description: 医院登录
 * @date 2021/10/10 18:18
 */
@Data
public class AccountLogin {
    private String loginName;
    private String password;
}
