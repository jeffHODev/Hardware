package com.hekangyuan.nivi1000.model;

public class SBPBean {

    //收缩压
    private int sbp;
    //舒张压
    private int dbp;
    //平均压
    private int abp;
    //心率
    private int heartRate;

    public int getSbp() {
        return sbp;
    }

    public void setSbp(int sbp) {
        this.sbp = sbp;
    }

    public int getDbp() {
        return dbp;
    }

    public void setDbp(int dbp) {
        this.dbp = dbp;
    }

    public int getAbp() {
        return abp;
    }

    public void setAbp(int abp) {
        this.abp = abp;
    }

    public int getHeartRate() {
        return heartRate;
    }

    public void setHeartRate(int heartRate) {
        this.heartRate = heartRate;
    }
}
