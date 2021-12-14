package com.hengde.nivi.modules.hospital.entity;

import lombok.Data;
import java.util.Date;

/**
 * @author dong
 * @title: Hospital
 * @description: 医院实体类
 * @date 2019/7/2 11:14
 */
@Data
public class Hospital {
    private String id;
    private String name; //名称
    private Date createDate; //创建时间
    private String createUserId; //创建人
    private String updateUserId; //更新的人
    private String updateDate; //更新时间
    private String remark; //备注（客户信息）
    private Short delFlag; //是否删除（0正常 1删除）
}
