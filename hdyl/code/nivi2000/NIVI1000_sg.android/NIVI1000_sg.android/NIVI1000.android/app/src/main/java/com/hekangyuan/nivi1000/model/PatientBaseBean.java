package com.hekangyuan.nivi1000.model;

import java.util.List;

public class PatientBaseBean {
    private List<PatientInformationBean> patientList;
    private int pageNo;
    private int count;
    private int pageSize;

    public List<PatientInformationBean> getPatientList() {
        return patientList;
    }

    public void setPatientList(List<PatientInformationBean> patientList) {
        this.patientList = patientList;
    }

    public int getPageNo() {
        return pageNo;
    }

    public void setPageNo(int pageNo) {
        this.pageNo = pageNo;
    }

    public int getCount() {
        return count;
    }

    public void setCount(int count) {
        this.count = count;
    }

    public int getPageSize() {
        return pageSize;
    }

    public void setPageSize(int pageSize) {
        this.pageSize = pageSize;
    }
}
