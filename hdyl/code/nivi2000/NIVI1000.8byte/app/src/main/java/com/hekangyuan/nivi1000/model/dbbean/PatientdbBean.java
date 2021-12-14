package com.hekangyuan.nivi1000.model.dbbean;

import org.greenrobot.greendao.annotation.Entity;
import org.greenrobot.greendao.annotation.Id;
import org.greenrobot.greendao.annotation.Generated;
import org.greenrobot.greendao.annotation.ToMany;

import java.util.List;
import org.greenrobot.greendao.DaoException;
import com.hekangyuan.nivi1000.db.DaoSession;
import com.hekangyuan.nivi1000.db.PatientCheckDataBeanDao;
import com.hekangyuan.nivi1000.db.PatientdbBeanDao;

@Entity
public class PatientdbBean {
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
    private String createDate;  //创建时间

    @ToMany(referencedJoinProperty = "patientId")   // 注意参数的值
    private List<PatientCheckDataBean> checks;
    /** Used to resolve relations */
    @Generated(hash = 2040040024)
    private transient DaoSession daoSession;
    /** Used for active entity operations. */
    @Generated(hash = 1929914438)
    private transient PatientdbBeanDao myDao;

    @Generated(hash = 356879248)
    public PatientdbBean(Long id, String caseNo, String name, int sex,
            String birthday, String nationality, String address, String phone,
            String uniqueId, String createDate) {
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
    }

    @Generated(hash = 1081246434)
    public PatientdbBean() {
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

    /**
     * To-many relationship, resolved on first access (and after reset).
     * Changes to to-many relations are not persisted, make changes to the target entity.
     */
    @Generated(hash = 144086592)
    public List<PatientCheckDataBean> getChecks() {
        if (checks == null) {
            final DaoSession daoSession = this.daoSession;
            if (daoSession == null) {
                throw new DaoException("Entity is detached from DAO context");
            }
            PatientCheckDataBeanDao targetDao = daoSession
                    .getPatientCheckDataBeanDao();
            List<PatientCheckDataBean> checksNew = targetDao
                    ._queryPatientdbBean_Checks(id);
            synchronized (this) {
                if (checks == null) {
                    checks = checksNew;
                }
            }
        }
        return checks;
    }

    /** Resets a to-many relationship, making the next get call to query for a fresh result. */
    @Generated(hash = 1527870304)
    public synchronized void resetChecks() {
        checks = null;
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
    @Generated(hash = 1321354537)
    public void __setDaoSession(DaoSession daoSession) {
        this.daoSession = daoSession;
        myDao = daoSession != null ? daoSession.getPatientdbBeanDao() : null;
    }




}
