package com.hekangyuan.nivi1000.jnastructure;

import com.sun.jna.Structure;

import java.util.Arrays;
import java.util.List;

public class AnalHPA_Data extends Structure {
    @Override
    protected List getFieldOrder() {
        return Arrays.asList(new String[] { "R_R", "Q_II", "ET", "PEP", "Q_I", "ICT",
                "ME", "SE", "GOE", "Q_T", "I_II", "DEM", "UT", "UE", "R_Time",
                "DeltaQ_II", "DeltaET", "DeltaPEP", "R_Ao0", "PTT", "PT_us", "PTV", "IHS", "Gasok",
                "HR", "ME1", "EUT", "PssL", "PssR", "AsL", "AsR", "AdL", "AdR", "TsL", "TsR", "TL", "TR"});
    }

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

    public float getR_R() {
        return R_R;
    }

    public void setR_R(float r_R) {
        R_R = r_R;
    }

    public float getQ_II() {
        return Q_II;
    }

    public void setQ_II(float q_II) {
        Q_II = q_II;
    }

    public float getET() {
        return ET;
    }

    public void setET(float ET) {
        this.ET = ET;
    }

    public float getPEP() {
        return PEP;
    }

    public void setPEP(float PEP) {
        this.PEP = PEP;
    }

    public float getQ_I() {
        return Q_I;
    }

    public void setQ_I(float q_I) {
        Q_I = q_I;
    }

    public float getICT() {
        return ICT;
    }

    public void setICT(float ICT) {
        this.ICT = ICT;
    }

    public float getME() {
        return ME;
    }

    public void setME(float ME) {
        this.ME = ME;
    }

    public float getSE() {
        return SE;
    }

    public void setSE(float SE) {
        this.SE = SE;
    }

    public float getGOE() {
        return GOE;
    }

    public void setGOE(float GOE) {
        this.GOE = GOE;
    }

    public float getQ_T() {
        return Q_T;
    }

    public void setQ_T(float q_T) {
        Q_T = q_T;
    }

    public float getI_II() {
        return I_II;
    }

    public void setI_II(float i_II) {
        I_II = i_II;
    }

    public float getDEM() {
        return DEM;
    }

    public void setDEM(float DEM) {
        this.DEM = DEM;
    }

    public float getUT() {
        return UT;
    }

    public void setUT(float UT) {
        this.UT = UT;
    }

    public float getUE() {
        return UE;
    }

    public void setUE(float UE) {
        this.UE = UE;
    }

    public float getR_Time() {
        return R_Time;
    }

    public void setR_Time(float r_Time) {
        R_Time = r_Time;
    }

    public float getDeltaQ_II() {
        return DeltaQ_II;
    }

    public void setDeltaQ_II(float deltaQ_II) {
        DeltaQ_II = deltaQ_II;
    }

    public float getDeltaET() {
        return DeltaET;
    }

    public void setDeltaET(float deltaET) {
        DeltaET = deltaET;
    }

    public float getDeltaPEP() {
        return DeltaPEP;
    }

    public void setDeltaPEP(float deltaPEP) {
        DeltaPEP = deltaPEP;
    }

    public float getR_Ao0() {
        return R_Ao0;
    }

    public void setR_Ao0(float r_Ao0) {
        R_Ao0 = r_Ao0;
    }

    public float getPTT() {
        return PTT;
    }

    public void setPTT(float PTT) {
        this.PTT = PTT;
    }

    public float getPT_us() {
        return PT_us;
    }

    public void setPT_us(float PT_us) {
        this.PT_us = PT_us;
    }

    public float getPTV() {
        return PTV;
    }

    public void setPTV(float PTV) {
        this.PTV = PTV;
    }

    public float getIHS() {
        return IHS;
    }

    public void setIHS(float IHS) {
        this.IHS = IHS;
    }

    public float getGasok() {
        return Gasok;
    }

    public void setGasok(float gasok) {
        Gasok = gasok;
    }

    public short getHR() {
        return HR;
    }

    public void setHR(short HR) {
        this.HR = HR;
    }

    public float getME1() {
        return ME1;
    }

    public void setME1(float ME1) {
        this.ME1 = ME1;
    }

    public float getEUT() {
        return EUT;
    }

    public void setEUT(float EUT) {
        this.EUT = EUT;
    }

    public float getPssL() {
        return PssL;
    }

    public void setPssL(float pssL) {
        PssL = pssL;
    }

    public float getPssR() {
        return PssR;
    }

    public void setPssR(float pssR) {
        PssR = pssR;
    }

    public float getAsL() {
        return AsL;
    }

    public void setAsL(float asL) {
        AsL = asL;
    }

    public float getAsR() {
        return AsR;
    }

    public void setAsR(float asR) {
        AsR = asR;
    }

    public float getAdL() {
        return AdL;
    }

    public void setAdL(float adL) {
        AdL = adL;
    }

    public float getAdR() {
        return AdR;
    }

    public void setAdR(float adR) {
        AdR = adR;
    }

    public float getTsL() {
        return TsL;
    }

    public void setTsL(float tsL) {
        TsL = tsL;
    }

    public float getTsR() {
        return TsR;
    }

    public void setTsR(float tsR) {
        TsR = tsR;
    }

    public float getTL() {
        return TL;
    }

    public void setTL(float TL) {
        this.TL = TL;
    }

    public float getTR() {
        return TR;
    }

    public void setTR(float TR) {
        this.TR = TR;
    }
}
