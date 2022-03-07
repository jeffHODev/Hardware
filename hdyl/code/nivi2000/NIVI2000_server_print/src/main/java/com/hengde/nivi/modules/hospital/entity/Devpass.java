package com.hengde.nivi.modules.hospital.entity;

import lombok.Data;

/**
 * @author dong
 * @title: Devpass
 * @description: 修改设备密码
 * @date 2020/11/30 12:02
 */
@Data
public class Devpass {
    private String deviceNo;
    private String oldPass;
    private String newPass;
}
