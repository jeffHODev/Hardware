package com.hekangyuan.nivi1000.model;
import java.io.Serializable;
import java.util.Date;

public class BloodPressureBean implements Serializable {

    private int   height;//身高
    private int   weight;//体重
    private int   ps;//收缩压
    private int   pd;//舒张压
    private int   brPs;//药前收缩压
    private int   brPd;//药前舒张压
    private int   alPs;//左下肢收缩压
    private int   alPd;//右下肢收缩压
    private int   arPs;//冠脉药效等级
    private int   arPd;
    private int   pcmpt;//收缩压
    private int   pexpd;//舒张压
    private int   pavrg;//平均值
    private int   phr;//脉搏

    @Override
    public String toString() {
        return "BloodPressureBean{" +
                "height=" + height +
                ", weight=" + weight +
                ", ps=" + ps +
                ", pd=" + pd +
                ", brPs=" + brPs +
                ", brPd=" + brPd +
                ", alPs=" + alPs +
                ", alPd=" + alPd +
                ", arPs=" + arPs +
                ", arPd=" + arPd +
                ", pcmpt=" + pcmpt +
                ", pexpd=" + pexpd +
                ", pavrg=" + pavrg +
                ", phr=" + phr +
                ", diag_date=" + diag_date +
                ", patientId='" + patientId + '\'' +
                ", basisMeasurementId='" + basisMeasurementId + '\'' +
                '}';
    }

    private Date diag_date;
    private String patientId;//患者id
    private String basisMeasurementId; //测试号
    public int getPhr() {
        return phr;
    }

    public void setPhr(int phr) {
        this.phr = phr;
    }
    public int getHeight() {
        return height;
    }

    public void setHeight(int height) {
        this.height = height;
    }

    public int getWeight() {
        return weight;
    }

    public void setWeight(int weight) {
        this.weight = weight;
    }

    public int getPs() {
        return ps;
    }

    public void setPs(int ps) {
        this.ps = ps;
    }

    public int getPd() {
        return pd;
    }

    public void setPd(int pd) {
        this.pd = pd;
    }

    public int getBrPs() {
        return brPs;
    }

    public void setBrPs(int brPs) {
        this.brPs = brPs;
    }

    public int getBrPd() {
        return brPd;
    }

    public void setBrPd(int brPd) {
        this.brPd = brPd;
    }

    public int getAlPs() {
        return alPs;
    }

    public void setAlPs(int alPs) {
        this.alPs = alPs;
    }

    public int getAlPd() {
        return alPd;
    }

    public void setAlPd(int alPd) {
        this.alPd = alPd;
    }

    public int getArPs() {
        return arPs;
    }

    public void setArPs(int arPs) {
        this.arPs = arPs;
    }

    public int getArPd() {
        return arPd;
    }

    public void setArPd(int arPd) {
        this.arPd = arPd;
    }

    public int getPcmpt() {
        return pcmpt;
    }

    public void setPcmpt(int pcmpt) {
        this.pcmpt = pcmpt;
    }

    public int getPexpd() {
        return pexpd;
    }

    public void setPexpd(int pexpd) {
        this.pexpd = pexpd;
    }

    public int getPavrg() {
        return pavrg;
    }

    public void setPavrg(int pavrg) {
        this.pavrg = pavrg;
    }

    public String getPatientId() {
        return patientId;
    }

    public void setPatientId(String patientId) {
        this.patientId = patientId;
    }

    public String getBasisMeasurementId() {
        return basisMeasurementId;
    }

    public void setBasisMeasurementId(String basisMeasurementId) {
        this.basisMeasurementId = basisMeasurementId;
    }





}
