package com.hengde.nivi.common.global;

import com.sun.jna.Structure;
import lombok.Data;
import org.springframework.stereotype.Component;

import java.util.Arrays;
import java.util.List;

/**
 * @author dong
 * @title: HYP_RST
 * @description: 全局结构体
 * @date 2020/8/21 10:33
 */
@Data
@Component
public class HYP_RST extends Structure {

    @Override
    protected List getFieldOrder() {
        return Arrays.asList(new String[] { "Lv", "Bpgrad_max", "Bpgrad_min", "Bpgrad_pp", "Bpgrad_dw", "Tpp",
                "PR1", "PR2", "CO1", "CO2", "Ke", "V0", "Ps", "Pd", "Pm", "C", "SV", "CBPs"});
    }

    @Component
    public static class ByReference extends HYP_RST implements Structure.ByReference { }

    public float Lv;
    public float Bpgrad_max;
    public float Bpgrad_min;
    public float Bpgrad_pp;
    public float Bpgrad_dw;
    public float Tpp;
    public float PR1;
    public float PR2;
    public float CO1;
    public float CO2;
    public float Ke;
    public float V0;
    public float Ps;
    public float Pd;
    public float Pm;
    public float C;
    public float SV;
    public float CBPs;
}
