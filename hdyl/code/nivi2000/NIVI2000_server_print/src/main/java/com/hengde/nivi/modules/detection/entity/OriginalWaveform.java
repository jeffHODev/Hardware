package com.hengde.nivi.modules.detection.entity;

import lombok.Data;
import java.util.Date;

@Data
public class OriginalWaveform {
    private String id;

    private Date createDate;

    private String basisMeasurementId;

    private Byte delFlag;

    private String hypecgs;

    private String hyppcgs;

    private String hypapgs;

    private String hypecgd;

    private String hyppcgd;

    private String hypapgd;

    private String ecgPl;

    private String pcgPl;

    private String apgL;

    private String ecgPr;

    private String pcgPr;

    private String apgR;

    private String ecgTh;

    private String apgTh;
}