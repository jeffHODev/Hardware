package com.hospital.nivihospital.modules.hospital.entity;

import lombok.Data;

/**
 * @author dong
 * @title:Hospass
 * @description: 修改设备密码
 * @date 2021/10/10 12:02
 */
@Data
public class Accountpass {
    private String account;
    private String oldPass;
    private String newPass;
}
