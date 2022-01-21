package com.hengde.nivi.modules.patient.entity;

import com.fasterxml.jackson.annotation.JsonFormat;
import lombok.Data;

@Data
public class Patient {
    private String id;

    private String caseNo;

    private String nationality;

    private String address;

    private String phone;

    @JsonFormat(pattern = "yyyy-MM-dd", timezone = "GMT+8")
    private String birthday;

    private Integer sex;

    private String name;

    private String uniqueId;

    @JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss", timezone = "GMT+8")
    private String createDate;

    private Integer delFlag;
}