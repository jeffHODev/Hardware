package com.hekangyuan.nivi1000.model;

public class SerialBean {

    private static final int SERIAL_LENGTH = 200;

    private float[] ecgdata ;
    private float[] headata ;
    private float[] pcgdata ;
    private float[] tstdata ;

    public SerialBean() {
        ecgdata = new float[SERIAL_LENGTH];
        headata = new float[SERIAL_LENGTH];
        pcgdata = new float[SERIAL_LENGTH];
        tstdata = new float[SERIAL_LENGTH];
    }

    public float[] getEcgdata() {
        return ecgdata;
    }

    public void setEcgdata(float[] ecgdata) {
        this.ecgdata = ecgdata;
    }

    public float[] getHeadata() {
        return headata;
    }

    public void setHeadata(float[] headata) {
        this.headata = headata;
    }

    public float[] getPcgdata() {
        return pcgdata;
    }

    public void setPcgdata(float[] pcgdata) {
        this.pcgdata = pcgdata;
    }

    public float[] getTstdata() {
        return tstdata;
    }

    public void setTstdata(float[] tstdata) {
        this.tstdata = tstdata;
    }
}
