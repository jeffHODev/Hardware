package com.hekangyuan.nivi1000.model;

import org.greenrobot.greendao.annotation.Entity;
import org.greenrobot.greendao.annotation.Id;
import org.greenrobot.greendao.annotation.NotNull;
import org.greenrobot.greendao.annotation.Property;
import org.greenrobot.greendao.annotation.Unique;
import org.greenrobot.greendao.annotation.Generated;

import java.io.File;

@Entity
public class
Waveform_Bean {

    @Id(autoincrement = true)
//    @Unique
//    @Property(nameInDb = "waveform_id")
    private Long waveform_id;

    @NotNull
    private String idCard; //身份证
    @NotNull
    private String hypecgs;//超收缩压心电波
    @NotNull
    private String hyppcgs;//超收缩压心音波
    @NotNull
    private String hypapgs;//超收缩压脉搏波
    @NotNull
    private String hypecgd;//低舒张压心电波
    @NotNull
    private String hyppcgd;//低舒张压心音波
    @NotNull
    private String hypapgd;//低舒张压脉搏波
    @NotNull
    private String ecg_pl; //左颈动脉心电波
    @NotNull
    private String pcg_pl; //左颈动脉心音波
    @NotNull
    private String apg_l;  //左颈动脉脉搏波
    @NotNull
    private String ecg_pr; //右颈动脉心电波
    @NotNull
    private String pcg_pr; //右颈动脉心音波
    @NotNull
    private String arg_r;  //右颈动脉脉搏波
    @NotNull
    private String ecg_th; //股动脉心电波
    @NotNull
    private String apg_th; //股动脉脉搏波
    @Generated(hash = 873162686)
    public Waveform_Bean(Long waveform_id, @NotNull String idCard,
            @NotNull String hypecgs, @NotNull String hyppcgs,
            @NotNull String hypapgs, @NotNull String hypecgd,
            @NotNull String hyppcgd, @NotNull String hypapgd,
            @NotNull String ecg_pl, @NotNull String pcg_pl, @NotNull String apg_l,
            @NotNull String ecg_pr, @NotNull String pcg_pr, @NotNull String arg_r,
            @NotNull String ecg_th, @NotNull String apg_th) {
        this.waveform_id = waveform_id;
        this.idCard = idCard;
        this.hypecgs = hypecgs;
        this.hyppcgs = hyppcgs;
        this.hypapgs = hypapgs;
        this.hypecgd = hypecgd;
        this.hyppcgd = hyppcgd;
        this.hypapgd = hypapgd;
        this.ecg_pl = ecg_pl;
        this.pcg_pl = pcg_pl;
        this.apg_l = apg_l;
        this.ecg_pr = ecg_pr;
        this.pcg_pr = pcg_pr;
        this.arg_r = arg_r;
        this.ecg_th = ecg_th;
        this.apg_th = apg_th;
    }
    @Generated(hash = 869702944)
    public Waveform_Bean() {
    }
    public Long getWaveform_id() {
        return this.waveform_id;
    }
    public void setWaveform_id(Long waveform_id) {
        this.waveform_id = waveform_id;
    }
    public String getIdCard() {
        return this.idCard;
    }
    public void setIdCard(String idCard) {
        this.idCard = idCard;
    }
    public String getHypecgs() {
        return this.hypecgs;
    }
    public void setHypecgs(String hypecgs) {
        this.hypecgs = hypecgs;
    }
    public String getHyppcgs() {
        return this.hyppcgs;
    }
    public void setHyppcgs(String hyppcgs) {
        this.hyppcgs = hyppcgs;
    }
    public String getHypapgs() {
        return this.hypapgs;
    }
    public void setHypapgs(String hypapgs) {
        this.hypapgs = hypapgs;
    }
    public String getHypecgd() {
        return this.hypecgd;
    }
    public void setHypecgd(String hypecgd) {
        this.hypecgd = hypecgd;
    }
    public String getHyppcgd() {
        return this.hyppcgd;
    }
    public void setHyppcgd(String hyppcgd) {
        this.hyppcgd = hyppcgd;
    }
    public String getHypapgd() {
        return this.hypapgd;
    }
    public void setHypapgd(String hypapgd) {
        this.hypapgd = hypapgd;
    }
    public String getEcg_pl() {
        return this.ecg_pl;
    }
    public void setEcg_pl(String ecg_pl) {
        this.ecg_pl = ecg_pl;
    }
    public String getPcg_pl() {
        return this.pcg_pl;
    }
    public void setPcg_pl(String pcg_pl) {
        this.pcg_pl = pcg_pl;
    }
    public String getApg_l() {
        return this.apg_l;
    }
    public void setApg_l(String apg_l) {
        this.apg_l = apg_l;
    }
    public String getEcg_pr() {
        return this.ecg_pr;
    }
    public void setEcg_pr(String ecg_pr) {
        this.ecg_pr = ecg_pr;
    }
    public String getPcg_pr() {
        return this.pcg_pr;
    }
    public void setPcg_pr(String pcg_pr) {
        this.pcg_pr = pcg_pr;
    }
    public String getArg_r() {
        return this.arg_r;
    }
    public void setArg_r(String arg_r) {
        this.arg_r = arg_r;
    }
    public String getEcg_th() {
        return this.ecg_th;
    }
    public void setEcg_th(String ecg_th) {
        this.ecg_th = ecg_th;
    }
    public String getApg_th() {
        return this.apg_th;
    }
    public void setApg_th(String apg_th) {
        this.apg_th = apg_th;
    }
   
  
}
