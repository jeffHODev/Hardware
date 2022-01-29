package com.hengde.nivi.modules.pdftest;

import com.fasterxml.jackson.annotation.JsonFormat;
import lombok.Data;

/**
 * @author dong
 * @title: Entity
 * @description: TODO
 * @date 2022/1/21 14:09
 */
@Data
public class Entity {
    private String patientName;
    @JsonFormat(pattern = "yyyy-MM-dd", timezone = "GMT+8")
    private String age;
    private String weight;
    private String doctoreName;
    private String height;
    private String sex;
    @JsonFormat(pattern = "yyyy-MM-dd", timezone = "GMT+8")
    private String diagDate;
}
