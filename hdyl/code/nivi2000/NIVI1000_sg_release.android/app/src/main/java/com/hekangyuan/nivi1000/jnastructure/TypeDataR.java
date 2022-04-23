package com.hekangyuan.nivi1000.jnastructure;

import com.sun.jna.Structure;

import java.util.Arrays;
import java.util.List;

public class TypeDataR extends Structure {
    @Override
    protected List getFieldOrder() {
        return Arrays.asList(new String[] { "Man", "Age", "Ps", "Pd", "Pv", "Ts",
                "Td", "T", "Pulse", "Te", "h1", "h2", "rate_h2h1", "Sv", "C0",
                "R0", "As", "Ad", "Ita", "Pss", "Pp", "Pba", "Ptop", "a1", "a2",
                "Cs", "Cd", "Cnew", "Cse", "Cr", "Csm", "Cdm1", "Cdm2", "Hs", "BG",
                "AG1", "Pwv1", "HI", "CO", "K", "H"});
    }

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

    public short getMan() {
        return Man;
    }

    public void setMan(short man) {
        Man = man;
    }

    public short getAge() {
        return Age;
    }

    public void setAge(short age) {
        Age = age;
    }

    public float getPs() {
        return Ps;
    }

    public void setPs(float ps) {
        Ps = ps;
    }

    public float getPd() {
        return Pd;
    }

    public void setPd(float pd) {
        Pd = pd;
    }

    public float getPv() {
        return Pv;
    }

    public void setPv(float pv) {
        Pv = pv;
    }

    public float getTs() {
        return Ts;
    }

    public void setTs(float ts) {
        Ts = ts;
    }

    public float getTd() {
        return Td;
    }

    public void setTd(float td) {
        Td = td;
    }

    public float getT() {
        return T;
    }

    public void setT(float t) {
        T = t;
    }

    public float getPulse() {
        return Pulse;
    }

    public void setPulse(float pulse) {
        Pulse = pulse;
    }

    public float getTe() {
        return Te;
    }

    public void setTe(float te) {
        Te = te;
    }

    public float getH1() {
        return h1;
    }

    public void setH1(float h1) {
        this.h1 = h1;
    }

    public float getH2() {
        return h2;
    }

    public void setH2(float h2) {
        this.h2 = h2;
    }

    public float getRate_h2h1() {
        return rate_h2h1;
    }

    public void setRate_h2h1(float rate_h2h1) {
        this.rate_h2h1 = rate_h2h1;
    }

    public float getSv() {
        return Sv;
    }

    public void setSv(float sv) {
        Sv = sv;
    }

    public float getC0() {
        return C0;
    }

    public void setC0(float c0) {
        C0 = c0;
    }

    public float getR0() {
        return R0;
    }

    public void setR0(float r0) {
        R0 = r0;
    }

    public float getAs() {
        return As;
    }

    public void setAs(float as) {
        As = as;
    }

    public float getAd() {
        return Ad;
    }

    public void setAd(float ad) {
        Ad = ad;
    }

    public float getIta() {
        return Ita;
    }

    public void setIta(float ita) {
        Ita = ita;
    }

    public float getPss() {
        return Pss;
    }

    public void setPss(float pss) {
        Pss = pss;
    }

    public float getPp() {
        return Pp;
    }

    public void setPp(float pp) {
        Pp = pp;
    }

    public float getPba() {
        return Pba;
    }

    public void setPba(float pba) {
        Pba = pba;
    }

    public float getPtop() {
        return Ptop;
    }

    public void setPtop(float ptop) {
        Ptop = ptop;
    }

    public float getA1() {
        return a1;
    }

    public void setA1(float a1) {
        this.a1 = a1;
    }

    public float getA2() {
        return a2;
    }

    public void setA2(float a2) {
        this.a2 = a2;
    }

    public float getCs() {
        return Cs;
    }

    public void setCs(float cs) {
        Cs = cs;
    }

    public float getCd() {
        return Cd;
    }

    public void setCd(float cd) {
        Cd = cd;
    }

    public float getCnew() {
        return Cnew;
    }

    public void setCnew(float cnew) {
        Cnew = cnew;
    }

    public float getCse() {
        return Cse;
    }

    public void setCse(float cse) {
        Cse = cse;
    }

    public float getCr() {
        return Cr;
    }

    public void setCr(float cr) {
        Cr = cr;
    }

    public float getCsm() {
        return Csm;
    }

    public void setCsm(float csm) {
        Csm = csm;
    }

    public float getCdm1() {
        return Cdm1;
    }

    public void setCdm1(float cdm1) {
        Cdm1 = cdm1;
    }

    public float getCdm2() {
        return Cdm2;
    }

    public void setCdm2(float cdm2) {
        Cdm2 = cdm2;
    }

    public float getHs() {
        return Hs;
    }

    public void setHs(float hs) {
        Hs = hs;
    }

    public float getBG() {
        return BG;
    }

    public void setBG(float BG) {
        this.BG = BG;
    }

    public float getAG1() {
        return AG1;
    }

    public void setAG1(float AG1) {
        this.AG1 = AG1;
    }

    public float getPwv1() {
        return Pwv1;
    }

    public void setPwv1(float pwv1) {
        Pwv1 = pwv1;
    }

    public float getHI() {
        return HI;
    }

    public void setHI(float HI) {
        this.HI = HI;
    }

    public float getCO() {
        return CO;
    }

    public void setCO(float CO) {
        this.CO = CO;
    }

    public float getK() {
        return K;
    }

    public void setK(float k) {
        K = k;
    }

    public float getH() {
        return H;
    }

    public void setH(float h) {
        H = h;
    }
}
