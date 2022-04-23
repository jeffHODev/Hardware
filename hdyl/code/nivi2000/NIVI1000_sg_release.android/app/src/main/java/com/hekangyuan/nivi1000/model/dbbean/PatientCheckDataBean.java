package com.hekangyuan.nivi1000.model.dbbean;

import org.greenrobot.greendao.annotation.Entity;
import org.greenrobot.greendao.annotation.Id;
import org.greenrobot.greendao.annotation.NotNull;
import org.greenrobot.greendao.annotation.ToOne;
import org.greenrobot.greendao.annotation.Generated;
import org.greenrobot.greendao.DaoException;
import com.hekangyuan.nivi1000.db.DaoSession;
import com.hekangyuan.nivi1000.db.WaveformBeanDao;
import com.hekangyuan.nivi1000.db.PatientdbBeanDao;
import com.hekangyuan.nivi1000.db.PatientCheckDataBeanDao;

@Entity
public class PatientCheckDataBean {
    @Id(autoincrement = true)
    private Long id;

    private String   caseNo; //病历号
    private String   name;   //姓名
    private int   sex;       //性别
    private String birthday; //出生年月
    private String  nationality ;//国籍
    private String   address; //地址（详细地址）
    private String   phone;  //手机号
    private String   uniqueId;  //身份证号
    private String createDate;  //
    private String hospitalId;   //
    private int delFlag;      //

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
    private String   hypecgs; //超收缩压心电
    private String   hyppcgs;//超收缩压心音
    private String   hypapgs;//超收缩压脉搏
    private String   hypecgd;//低舒张压心电
    private String   hyppcgd;//低舒张压心音
    private String   hypapgd;//低舒张压脉搏
    private String   ecgPl;//左颈动脉心电
    private String   pcgPl;//左颈动脉心音
    private String   apgL;//左颈动脉脉搏
    private String   ecgPr;//右颈动脉心电
    private String   pcgPr;//右颈动脉心音
    private String   argR;//右颈动脉脉搏
    private String   ecgTh;//股动脉心电
    private String   apgTh;//股动脉脉搏
    private String     basisMeasurementId;//测试号



    @NotNull
    private Long patientId; //外键

    @ToOne(joinProperty = "patientId")
    private PatientdbBean patient;

    /** Used to resolve relations */
    @Generated(hash = 2040040024)
    private transient DaoSession daoSession;

    /** Used for active entity operations. */
    @Generated(hash = 681881650)
    private transient PatientCheckDataBeanDao myDao;

    @Generated(hash = 727351205)
    public PatientCheckDataBean(Long id, String caseNo, String name, int sex,
            String birthday, String nationality, String address, String phone,
            String uniqueId, String createDate, String hospitalId, int delFlag,
            int height, int weight, int ps, int pd, int brPs, int brPd, int alPs,
            int alPd, int arPs, int arPd, int pcmpt, int pexpd, int pavrg,
            String hypecgs, String hyppcgs, String hypapgs, String hypecgd,
            String hyppcgd, String hypapgd, String ecgPl, String pcgPl, String apgL,
            String ecgPr, String pcgPr, String argR, String ecgTh, String apgTh,
            String basisMeasurementId, @NotNull Long patientId) {
        this.id = id;
        this.caseNo = caseNo;
        this.name = name;
        this.sex = sex;
        this.birthday = birthday;
        this.nationality = nationality;
        this.address = address;
        this.phone = phone;
        this.uniqueId = uniqueId;
        this.createDate = createDate;
        this.hospitalId = hospitalId;
        this.delFlag = delFlag;
        this.height = height;
        this.weight = weight;
        this.ps = ps;
        this.pd = pd;
        this.brPs = brPs;
        this.brPd = brPd;
        this.alPs = alPs;
        this.alPd = alPd;
        this.arPs = arPs;
        this.arPd = arPd;
        this.pcmpt = pcmpt;
        this.pexpd = pexpd;
        this.pavrg = pavrg;
        this.hypecgs = hypecgs;
        this.hyppcgs = hyppcgs;
        this.hypapgs = hypapgs;
        this.hypecgd = hypecgd;
        this.hyppcgd = hyppcgd;
        this.hypapgd = hypapgd;
        this.ecgPl = ecgPl;
        this.pcgPl = pcgPl;
        this.apgL = apgL;
        this.ecgPr = ecgPr;
        this.pcgPr = pcgPr;
        this.argR = argR;
        this.ecgTh = ecgTh;
        this.apgTh = apgTh;
        this.basisMeasurementId = basisMeasurementId;
        this.patientId = patientId;
    }

    @Generated(hash = 618216344)
    public PatientCheckDataBean() {
    }

    public Long getId() {
        return this.id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getCaseNo() {
        return this.caseNo;
    }

    public void setCaseNo(String caseNo) {
        this.caseNo = caseNo;
    }

    public String getName() {
        return this.name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getSex() {
        return this.sex;
    }

    public void setSex(int sex) {
        this.sex = sex;
    }

    public String getBirthday() {
        return this.birthday;
    }

    public void setBirthday(String birthday) {
        this.birthday = birthday;
    }

    public String getNationality() {
        return this.nationality;
    }

    public void setNationality(String nationality) {
        this.nationality = nationality;
    }

    public String getAddress() {
        return this.address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public String getPhone() {
        return this.phone;
    }

    public void setPhone(String phone) {
        this.phone = phone;
    }

    public String getUniqueId() {
        return this.uniqueId;
    }

    public void setUniqueId(String uniqueId) {
        this.uniqueId = uniqueId;
    }

    public String getCreateDate() {
        return this.createDate;
    }

    public void setCreateDate(String createDate) {
        this.createDate = createDate;
    }

    public String getHospitalId() {
        return this.hospitalId;
    }

    public void setHospitalId(String hospitalId) {
        this.hospitalId = hospitalId;
    }

    public int getDelFlag() {
        return this.delFlag;
    }

    public void setDelFlag(int delFlag) {
        this.delFlag = delFlag;
    }

    public int getHeight() {
        return this.height;
    }

    public void setHeight(int height) {
        this.height = height;
    }

    public int getWeight() {
        return this.weight;
    }

    public void setWeight(int weight) {
        this.weight = weight;
    }

    public int getPs() {
        return this.ps;
    }

    public void setPs(int ps) {
        this.ps = ps;
    }

    public int getPd() {
        return this.pd;
    }

    public void setPd(int pd) {
        this.pd = pd;
    }

    public int getBrPs() {
        return this.brPs;
    }

    public void setBrPs(int brPs) {
        this.brPs = brPs;
    }

    public int getBrPd() {
        return this.brPd;
    }

    public void setBrPd(int brPd) {
        this.brPd = brPd;
    }

    public int getAlPs() {
        return this.alPs;
    }

    public void setAlPs(int alPs) {
        this.alPs = alPs;
    }

    public int getAlPd() {
        return this.alPd;
    }

    public void setAlPd(int alPd) {
        this.alPd = alPd;
    }

    public int getArPs() {
        return this.arPs;
    }

    public void setArPs(int arPs) {
        this.arPs = arPs;
    }

    public int getArPd() {
        return this.arPd;
    }

    public void setArPd(int arPd) {
        this.arPd = arPd;
    }

    public int getPcmpt() {
        return this.pcmpt;
    }

    public void setPcmpt(int pcmpt) {
        this.pcmpt = pcmpt;
    }

    public int getPexpd() {
        return this.pexpd;
    }

    public void setPexpd(int pexpd) {
        this.pexpd = pexpd;
    }

    public int getPavrg() {
        return this.pavrg;
    }

    public void setPavrg(int pavrg) {
        this.pavrg = pavrg;
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

    public String getEcgPl() {
        return this.ecgPl;
    }

    public void setEcgPl(String ecgPl) {
        this.ecgPl = ecgPl;
    }

    public String getPcgPl() {
        return this.pcgPl;
    }

    public void setPcgPl(String pcgPl) {
        this.pcgPl = pcgPl;
    }

    public String getApgL() {
        return this.apgL;
    }

    public void setApgL(String apgL) {
        this.apgL = apgL;
    }

    public String getEcgPr() {
        return this.ecgPr;
    }

    public void setEcgPr(String ecgPr) {
        this.ecgPr = ecgPr;
    }

    public String getPcgPr() {
        return this.pcgPr;
    }

    public void setPcgPr(String pcgPr) {
        this.pcgPr = pcgPr;
    }

    public String getArgR() {
        return this.argR;
    }

    public void setArgR(String argR) {
        this.argR = argR;
    }

    public String getEcgTh() {
        return this.ecgTh;
    }

    public void setEcgTh(String ecgTh) {
        this.ecgTh = ecgTh;
    }

    public String getApgTh() {
        return this.apgTh;
    }

    public void setApgTh(String apgTh) {
        this.apgTh = apgTh;
    }

    public String getBasisMeasurementId() {
        return this.basisMeasurementId;
    }

    public void setBasisMeasurementId(String basisMeasurementId) {
        this.basisMeasurementId = basisMeasurementId;
    }

    public Long getPatientId() {
        return this.patientId;
    }

    public void setPatientId(Long patientId) {
        this.patientId = patientId;
    }

    @Generated(hash = 391381774)
    private transient Long patient__resolvedKey;

    /** To-one relationship, resolved on first access. */
    @Generated(hash = 294721174)
    public PatientdbBean getPatient() {
        Long __key = this.patientId;
        if (patient__resolvedKey == null || !patient__resolvedKey.equals(__key)) {
            final DaoSession daoSession = this.daoSession;
            if (daoSession == null) {
                throw new DaoException("Entity is detached from DAO context");
            }
            PatientdbBeanDao targetDao = daoSession.getPatientdbBeanDao();
            PatientdbBean patientNew = targetDao.load(__key);
            synchronized (this) {
                patient = patientNew;
                patient__resolvedKey = __key;
            }
        }
        return patient;
    }

    /** called by internal mechanisms, do not call yourself. */
    @Generated(hash = 2116084684)
    public void setPatient(@NotNull PatientdbBean patient) {
        if (patient == null) {
            throw new DaoException(
                    "To-one property 'patientId' has not-null constraint; cannot set to-one to null");
        }
        synchronized (this) {
            this.patient = patient;
            patientId = patient.getId();
            patient__resolvedKey = patientId;
        }
    }

    /**
     * Convenient call for {@link org.greenrobot.greendao.AbstractDao#delete(Object)}.
     * Entity must attached to an entity context.
     */
    @Generated(hash = 128553479)
    public void delete() {
        if (myDao == null) {
            throw new DaoException("Entity is detached from DAO context");
        }
        myDao.delete(this);
    }

    /**
     * Convenient call for {@link org.greenrobot.greendao.AbstractDao#refresh(Object)}.
     * Entity must attached to an entity context.
     */
    @Generated(hash = 1942392019)
    public void refresh() {
        if (myDao == null) {
            throw new DaoException("Entity is detached from DAO context");
        }
        myDao.refresh(this);
    }

    /**
     * Convenient call for {@link org.greenrobot.greendao.AbstractDao#update(Object)}.
     * Entity must attached to an entity context.
     */
    @Generated(hash = 713229351)
    public void update() {
        if (myDao == null) {
            throw new DaoException("Entity is detached from DAO context");
        }
        myDao.update(this);
    }

    /** called by internal mechanisms, do not call yourself. */
    @Generated(hash = 1729386964)
    public void __setDaoSession(DaoSession daoSession) {
        this.daoSession = daoSession;
        myDao = daoSession != null ? daoSession.getPatientCheckDataBeanDao() : null;
    }






}
