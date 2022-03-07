package com.hekangyuan.nivi1000.db;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.util.Log;
import com.hekangyuan.nivi1000.model.dbbean.PatientCheckDataBean;
import com.hekangyuan.nivi1000.model.dbbean.PatientdbBean;
import com.hekangyuan.nivi1000.model.dbbean.WaveformBean;
import com.hekangyuan.nivi1000.utils.ArrayUtils;

import java.util.List;


/**
 * @author huang
 *
 * 操作数据库帮助类
 *
 * 数据库名：patient_db
 *
 * 表: Patientdbbean,PatientCheckDataBean
 *
 *
 *
 */
public class PatientDaoHelper {

  private static final String DB_BAME = "patient_db";

  private static DaoMaster.DevOpenHelper mHelper;
  private static SQLiteDatabase mDb;
  //管理数据库
  private static DaoMaster mDaoMaster;
  //管理各种实体Dao,不让业务层拿到session直接去操作数据库，统一由此类提供方法
  private static DaoSession mDaoSession;

  /**
   * 设置greenDao
   */
  public static void initDatabase(Context context) {
    mHelper = new DaoMaster.DevOpenHelper(context, DB_BAME, null);
    mDb = mHelper.getWritableDatabase();
    mDaoMaster = new DaoMaster(mDb);
    mDaoSession = mDaoMaster.newSession();

//    QueryBuilder.LOG_SQL = true;        // 显示sql语句
//    QueryBuilder.LOG_VALUES = true; // 显示sql语句中的值
  }

  /**
   * 添加病人
   */
  public static void addPatient(PatientdbBean patientdbBean) {
    PatientdbBeanDao dao = mDaoSession.getPatientdbBeanDao();
    dao.insertOrReplace(patientdbBean);
  }

  /**
   * 移除病人
   */
  public static void removePatient(PatientdbBean patientdbBean) {
    PatientdbBeanDao dao = mDaoSession.getPatientdbBeanDao();

    PatientdbBean patient =
        dao.queryBuilder().where(PatientdbBeanDao.Properties.Id.eq(patientdbBean.getId())).unique();
    dao.delete(patient);
  }

  /**
   * 查找病人
   */
  public static PatientdbBean loadPatient(PatientdbBean patientdbBean) {
    PatientdbBeanDao dao = mDaoSession.getPatientdbBeanDao();

    PatientdbBean patient =
            dao.queryBuilder().where(PatientdbBeanDao.Properties.Id.eq(patientdbBean.getId())).unique();
    return patient;
  }

  /**
   * 查询所有病人
   */
  public static List<PatientdbBean> loadAllPatient(){
    PatientdbBeanDao dao = mDaoSession.getPatientdbBeanDao();

    return dao.queryBuilder().list();
  }


  /**
   * 添加检查
   */
  public static void addCheck(PatientCheckDataBean patientCheckDataBean) {
    PatientCheckDataBeanDao dao = mDaoSession.getPatientCheckDataBeanDao();
    dao.insertOrReplace(patientCheckDataBean);
  }

  /**
   * 添加检查
   */
  public static void addCheckByPatientId(PatientCheckDataBean patientCheckDataBean,long patientId) {
    patientCheckDataBean.setPatientId(patientId);
    PatientCheckDataBeanDao dao = mDaoSession.getPatientCheckDataBeanDao();
    dao.insertOrReplace(patientCheckDataBean);
  }

  /**
   * 移除检查
   */
  public static void removeCheckt(PatientCheckDataBean patientCheckDataBean) {
    PatientCheckDataBeanDao dao = mDaoSession.getPatientCheckDataBeanDao();

    PatientCheckDataBean patientCheck =
            dao.queryBuilder().where(PatientCheckDataBeanDao.Properties.Id.eq(patientCheckDataBean.getId())).unique();
    dao.delete(patientCheck);
  }

  /**
   * 查找检查
   */
  public static PatientCheckDataBean loadCheck(PatientCheckDataBean patientCheckDataBean) {
    PatientCheckDataBeanDao dao = mDaoSession.getPatientCheckDataBeanDao();

    PatientCheckDataBean patientCheck =
            dao.queryBuilder().where(PatientCheckDataBeanDao.Properties.Id.eq(patientCheckDataBean.getId())).unique();
    return patientCheck;
  }


  /**
   * 查询所有检查记录
   */
  public static List<PatientCheckDataBean> loadAllChecks(){
    PatientCheckDataBeanDao dao = mDaoSession.getPatientCheckDataBeanDao();

    return dao.queryBuilder().list();
  }

  /**
   * 根据病人id查找记录
   */
  public static List<PatientCheckDataBean> loadChecksByPatientId(Long patientId){
    PatientCheckDataBeanDao dao = mDaoSession.getPatientCheckDataBeanDao();

    return dao.queryBuilder().where(PatientCheckDataBeanDao.Properties.PatientId.eq(patientId)).list();
  }

  /**
   * 添加波形图
   */
  public static void addWaveForm(WaveformBean waveformBean) {
    WaveformBeanDao dao = mDaoSession.getWaveformBeanDao();
    dao.insertOrReplace(waveformBean);
  }

  /**
   * 查找波形图
   */
  public static WaveformBean loadWaveForm(Long checkId) {
    Log.e("id",""+checkId);

    WaveformBeanDao dao = mDaoSession.getWaveformBeanDao();

    WaveformBean waveformBean =
            dao.queryBuilder().where(WaveformBeanDao.Properties.CheckId.eq(checkId)).unique();
    Log.e("id",""+waveformBean.getId()+"  "+waveformBean.getCheckId());
    waveformBean.setApg_th(ArrayUtils.unzipString(waveformBean.getApg_th()));
    return waveformBean;
  }
}
