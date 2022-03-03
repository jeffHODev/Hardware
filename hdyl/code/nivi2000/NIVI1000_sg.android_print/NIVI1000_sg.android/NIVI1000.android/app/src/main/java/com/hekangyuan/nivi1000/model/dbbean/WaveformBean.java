package com.hekangyuan.nivi1000.model.dbbean;

import org.greenrobot.greendao.annotation.Entity;
import org.greenrobot.greendao.annotation.Id;
import org.greenrobot.greendao.annotation.NotNull;
import org.greenrobot.greendao.annotation.Property;
import org.greenrobot.greendao.annotation.ToOne;
import org.greenrobot.greendao.annotation.Unique;
import org.greenrobot.greendao.annotation.Generated;

import java.io.File;
import org.greenrobot.greendao.DaoException;
import com.hekangyuan.nivi1000.db.DaoSession;
import com.hekangyuan.nivi1000.db.PatientCheckDataBeanDao;
import com.hekangyuan.nivi1000.db.WaveformBeanDao;

@Entity
public class WaveformBean {

    @Id(autoincrement = true)
    private Long id;

    @NotNull
    private Long checkId;

    @ToOne(joinProperty = "checkId")
    private PatientCheckDataBean check;

    private String idCard; //身份证
    private String hypecgs;//超收缩压心电波
    private String hyppcgs;//超收缩压心音波

    private String hypapgs;//超收缩压脉搏波
    private String hypecgd;//低舒张压心电波
    private String hyppcgd;//低舒张压心音波
    private String hypapgd;//低舒张压脉搏波
    private String ecg_pl; //左颈动脉心电波
    private String pcg_pl; //左颈动脉心音波
    private String apg_l;  //左颈动脉脉搏波
    private String ecg_pr; //右颈动脉心电波
    private String pcg_pr; //右颈动脉心音波
    private String arg_r;  //右颈动脉脉搏波
    private String ecg_th; //股动脉心电波
    private String apg_th; //股动脉脉搏波

    /** Used to resolve relations */
    @Generated(hash = 2040040024)
    private transient DaoSession daoSession;

    /** Used for active entity operations. */
    @Generated(hash = 1435338099)
    private transient WaveformBeanDao myDao;
    @Generated(hash = 123074644)
    public WaveformBean(Long id, @NotNull Long checkId, String idCard, String hypecgs,
            String hyppcgs, String hypapgs, String hypecgd, String hyppcgd, String hypapgd,
            String ecg_pl, String pcg_pl, String apg_l, String ecg_pr, String pcg_pr,
            String arg_r, String ecg_th, String apg_th) {
        this.id = id;
        this.checkId = checkId;
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
    @Generated(hash = 1159567395)
    public WaveformBean() {
    }
    public Long getId() {
        return this.id;
    }
    public void setId(Long id) {
        this.id = id;
    }
    public Long getCheckId() {
        return this.checkId;
    }
    public void setCheckId(Long checkId) {
        this.checkId = checkId;
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
    @Generated(hash = 1930343756)
    private transient Long check__resolvedKey;
    /** To-one relationship, resolved on first access. */
    @Generated(hash = 932481980)
    public PatientCheckDataBean getCheck() {
        Long __key = this.checkId;
        if (check__resolvedKey == null || !check__resolvedKey.equals(__key)) {
            final DaoSession daoSession = this.daoSession;
            if (daoSession == null) {
                throw new DaoException("Entity is detached from DAO context");
            }
            PatientCheckDataBeanDao targetDao = daoSession
                    .getPatientCheckDataBeanDao();
            PatientCheckDataBean checkNew = targetDao.load(__key);
            synchronized (this) {
                check = checkNew;
                check__resolvedKey = __key;
            }
        }
        return check;
    }
    /** called by internal mechanisms, do not call yourself. */
    @Generated(hash = 1688341344)
    public void setCheck(@NotNull PatientCheckDataBean check) {
        if (check == null) {
            throw new DaoException(
                    "To-one property 'checkId' has not-null constraint; cannot set to-one to null");
        }
        synchronized (this) {
            this.check = check;
            checkId = check.getId();
            check__resolvedKey = checkId;
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
    @Generated(hash = 1614073460)
    public void __setDaoSession(DaoSession daoSession) {
        this.daoSession = daoSession;
        myDao = daoSession != null ? daoSession.getWaveformBeanDao() : null;
    }

   
  
}
