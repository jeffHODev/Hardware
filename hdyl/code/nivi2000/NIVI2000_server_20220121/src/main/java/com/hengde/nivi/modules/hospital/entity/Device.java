package com.hengde.nivi.modules.hospital.entity;

import lombok.Data;

import java.util.Date;

/**
 * @author dong
 * @title: Device
 * @description: 设备实体
 * @date 2020/11/30 14:55
 */
@Data
public class Device {
    private String id;
    private String deviceNo;
    private String password;
    private Date createDate;
    private Short delFlag; //是否删除（0正常 1删除）
}
