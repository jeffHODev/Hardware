package com.hekangyuan.nivi1000.model;

public class DiagnosisBean {

    private  String  patientId;
    private String month;
    private String diagDate;
    private String basisMeasurementId;

    public String getPatientId() {
        return patientId;
    }

    public void setPatientId(String patientId) {
        this.patientId = patientId;
    }

    public String getMonth() {
        return month;
    }

    public void setMonth(String month) {
        this.month = month;
    }

    public String getDiagDate() {
        return diagDate;
    }

    public void setDiagDate(String diagDate) {
        this.diagDate = diagDate;
    }

    public String getBasisMeasurementId() {
        return basisMeasurementId;
    }

    public void setBasisMeasurementId(String basisMeasurementId) {
        this.basisMeasurementId = basisMeasurementId;
    }
}
