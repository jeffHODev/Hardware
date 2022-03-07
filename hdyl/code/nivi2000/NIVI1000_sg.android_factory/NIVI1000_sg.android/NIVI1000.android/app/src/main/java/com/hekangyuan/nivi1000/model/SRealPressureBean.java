package com.hekangyuan.nivi1000.model;

public class SRealPressureBean {

    //实时压力 MMR901
    private int rPMMR;
    //实时压力 血压计
    private int rPCuff;

    public int getrPMMR() {
        return rPMMR;
    }

    public void setrPMMR(int rPMMR) {
        this.rPMMR = rPMMR;
    }

    public int getrPCuff() {
        return rPCuff;
    }

    public void setrPCuff(int rPCuff) {
        this.rPCuff = rPCuff;
    }
}
