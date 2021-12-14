package com.hengde.nivi.common.global;

import com.sun.jna.Structure;
import lombok.Data;
import org.springframework.stereotype.Component;

import java.util.Arrays;
import java.util.List;

/**
 * @author dong
 * @title: TypeData
 * @description: 全局结构体
 * @date 2020/5/18 16:56
 */
@Data
@Component
public class TypeDataR extends Structure {
    @Override
    protected List getFieldOrder() {
        return Arrays.asList(new String[] { "Man", "Age", "Ps", "Pd", "Pv", "Ts",
                "Td", "T", "Pulse", "Te", "h1", "h2", "rate_h2h1", "Sv", "C0",
                "R0", "As", "Ad", "Ita", "Pss", "Pp", "Pba", "Ptop", "a1", "a2",
                "Cs", "Cd", "Cnew", "Cse", "Cr", "Csm", "Cdm1", "Cdm2", "Hs", "BG",
                "AG1", "Pwv1", "HI", "CO", "K", "H"});
    }

    @Component
    public static class ByReference extends TypeDataR implements Structure.ByReference { }

    public short Man;
    public short Age;
    public float Ps;
    public float Pd;
    public float Pv;
    public float Ts;
    public float Td;
    public float T;
    public float Pulse;
    public float Te;
    public float h1;
    public float h2;
    public float rate_h2h1;
    public float Sv;
    public float C0;
    public float R0;
    public float As;
    public float Ad;
    public float Ita;
    public float Pss;
    public float Pp;
    public float Pba;
    public float Ptop;
    public float a1;
    public float a2;
    public float Cs;
    public float Cd;
    public float Cnew;
    public float Cse;
    public float Cr;
    public float Csm;
    public float Cdm1;
    public float Cdm2 ;
    public float Hs;
    public float BG;
    public float AG1;
    public float Pwv1;
    public float HI;
    public float CO;
    public float K;
    public float H;
}
