package com.hengde.nivi.modules.create.entity;

import lombok.Data;

import java.util.Date;

@Data
public class BasisMeasurement {
    private String id;

    private Integer height;

    private Integer weight;

    private Date diagDate;

    private Integer ps;

    private Integer pd;

    private Integer brPs;

    private Integer brPd;

    private Integer alPs;

    private Integer alPd;

    private Integer arPs;

    private Integer arPd;

    private Integer pcmpt;

    private Integer pexpd;

    private Integer pavrg;

    private Integer pHr;

    private String patientId;

    private String deviceId;

    private Integer model;

    private Byte delFlag;
}