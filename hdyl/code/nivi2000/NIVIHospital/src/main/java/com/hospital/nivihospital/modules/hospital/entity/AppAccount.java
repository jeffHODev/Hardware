package com.hospital.nivihospital.modules.hospital.entity;

import lombok.Data;

import java.util.Date;

/**
 * @author dong
 * @title: AppAccount
 * @description: 医院实体类
 * @date 2021/10/10 11:14
 */
@Data
public class AppAccount {
    private String id;
    private String entityId; //对应实体
    private String account; //登录账号
    private String password; //登录密码
    private Date createDate; //创建时间
    private Short entityType; //实体类型（1医院 2医生 3经销商）
    private Short delFlag; //是否删除（0正常 1删除）
}
