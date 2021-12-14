package com.hekangyuan.nivi1000.model;

import java.io.Serializable;
import java.util.Date;

public class PatientInformationBean  implements Serializable {




    private String id;
    private String   caseNo; //病历号
    private String   name;   //姓名
    private int   sex;       //性别
    private String birthday; //出生年月
    private String  nationality ;//国籍
    private String   address; //地址（详细地址）
    private String   phone;  //手机号
    private String   uniqueId;  //身份证号
    private String createDate;  //

    private String patientId;

    @Override
    public String toString() {
        return "PatientInformationBean{" +
                "id='" + id + '\'' +
                ", caseNo='" + caseNo + '\'' +
                ", name='" + name + '\'' +
                ", sex=" + sex +
                ", birthday='" + birthday + '\'' +
                ", nationality='" + nationality + '\'' +
                ", address='" + address + '\'' +
                ", phone='" + phone + '\'' +
                ", uniqueId='" + uniqueId + '\'' +
                ", createDate='" + createDate + '\'' +
                ", hospitalId='" + hospitalId + '\'' +
                ", delFlag=" + delFlag +
                '}';
    }

    private String hospitalId;   //
    private int delFlag;      //

    public String getCreateDate() {
        return createDate;
    }

    public void setCreateDate(String createDate) {
        this.createDate = createDate;
    }


    public String getUniqueId() {
        return uniqueId;
    }

    public void setUniqueId(String uniqueId) {
        this.uniqueId = uniqueId;
    }

    public String getHospitalId() {
        return hospitalId;
    }

    public void setHospitalId(String hospitalId) {
        this.hospitalId = hospitalId;
    }

    public int getDelFlag() {
        return delFlag;
    }

    public void setDelFlag(int delFlag) {
        this.delFlag = delFlag;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getCaseNo() {
        return caseNo;
    }

    public void setCaseNo(String caseNo) {
        this.caseNo = caseNo;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getSex() {
        return sex;
    }

    public void setSex(int sex) {
        this.sex = sex;
    }

    public String getBirthday() {
        return birthday;
    }

    public void setBirthday(String birthday) {
        this.birthday = birthday;
    }

    public String getNationality() {
        return nationality;
    }

    public void setNationality(String nationality) {
        this.nationality = nationality;
    }

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public String getPhone() {
        return phone;
    }

    public void setPhone(String phone) {
        this.phone = phone;
    }

    public String getPatientId() {
        return patientId;
    }

    public void setPatientId(String patientId) {
        this.patientId = patientId;
    }
}
