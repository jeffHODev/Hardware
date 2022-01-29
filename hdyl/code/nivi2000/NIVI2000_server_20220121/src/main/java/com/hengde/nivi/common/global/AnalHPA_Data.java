package com.hengde.nivi.common.global;

import com.sun.jna.Structure;
import lombok.Data;
import org.springframework.stereotype.Component;

import java.util.Arrays;
import java.util.List;

/**
 * @author dong
 * @title: AnalHPA_Data
 * @description: 全局结构体
 * @date 2020/6/16 17:40
 */
@Data
@Component
public class AnalHPA_Data extends Structure {
    @Override
    protected List getFieldOrder() {
        return Arrays.asList(new String[] { "R_R", "Q_II", "ET", "PEP", "Q_I", "ICT",
                "ME", "SE", "GOE", "Q_T", "I_II", "DEM", "UT", "UE", "R_Time",
                "DeltaQ_II", "DeltaET", "DeltaPEP", "R_Ao0", "PTT", "PT_us", "PTV", "IHS", "Gasok",
                "HR", "ME1", "EUT", "PssL", "PssR", "AsL", "AsR", "AdL", "AdR", "TsL", "TsR", "TL", "TR"});
    }

    @Component
    public static class ByReference extends AnalHPA_Data implements Structure.ByReference { }

    public float   R_R;	  //   Period of Heart_time
    public float   Q_II;    //  Total_Systolic_time
    public float   ET;      //  Ejection_time
    public float   PEP;     //  Pre_Ejection_Period
    public float   Q_I;	  //  Electro-mechanical interval
    public float   ICT;     //  Isovolumic_Contraction_time
    public float   ME;      //  Machine_index
    public float   SE;      //  Strain_exponent
    public float   GOE;     //  Ejection_exponent
    public float   Q_T;     //  Electric_Systolic_time
    public float   I_II;    //  Machine_Systolic_time
    public float   DEM;	  //  Difference of electric and machine
    public float	 UT;      //  Up storke time
    public float	 UE;	  //  Ascending index
    public float	 R_Time;  //  Reduced Ejection Time
    public float	 DeltaQ_II;
    public float	 DeltaET;
    public float	 DeltaPEP;
    public float	 R_Ao0;	  //  Systolic time
    public float 	 PTT;	  //  Pulse Transfer time i APG
    public float	 PT_us;   //  Pulse Transfer time in foot APG
    public float	 PTV;     //  Pulse Transfer velocity
    public float	 IHS;	  //  Index of heart strength
    public float	 Gasok;
    public short	 HR;	  //  Heart Pulse
    public float   ME1;
    public float   EUT;
    public float	PssL;
    public float	PssR;
    public float	AsL;
    public float	AsR;
    public float	AdL;
    public float	AdR;
    public float	TsL;
    public float	TsR;
    public float	TL;
    public float	TR;
}
