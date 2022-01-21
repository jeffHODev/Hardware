package com.hengde.nivi.modules.detection.entity;

import lombok.Data;

import java.util.Date;

@Data
public class OriginalWaveformReq {
    private String id;

    private Date createDate;

    private String basisMeasurementId;

    private Byte delFlag;

    private int[] hypecgs;

    private int[] hyppcgs;

    private int[] hypapgs;

    private int[] hypecgd;

    private int[] hyppcgd;

    private int[] hypapgd;

    private int[] ecgPl;

    private int[] pcgPl;

    private int[] apgL;

    private int[] ecgPr;

    private int[] pcgPr;

    private int[] apgR;

    private int[] ecgTh;

    private int[] apgTh;

    private short IDL;

    private short IDR;

    private Integer model; //1 同步模式； 2 异步模式；
}