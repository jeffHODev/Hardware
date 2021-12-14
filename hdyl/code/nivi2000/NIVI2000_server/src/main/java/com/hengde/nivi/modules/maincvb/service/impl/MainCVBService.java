package com.hengde.nivi.modules.maincvb.service.impl;

import com.hengde.nivi.common.global.AnalHPA_Data;
import com.hengde.nivi.common.global.HYP_RST;
import com.hengde.nivi.common.global.TypeData;
import com.hengde.nivi.common.global.TypeDataR;
import com.hengde.nivi.common.util.GetAge;
import com.hengde.nivi.modules.analysis.service.impl.AnalysisApgService;
import com.hengde.nivi.modules.create.dao.BasisMeasurementMapper;
import com.hengde.nivi.modules.create.entity.BasisMeasurement;
import com.hengde.nivi.modules.maincvb.service.IMainCVBService;
import com.hengde.nivi.modules.patient.dao.PatientMapper;
import com.hengde.nivi.modules.patient.entity.Patient;
import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import lombok.Data;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.*;

/**
 * @author dong
 * @title: MainCVBService
 * @description: 初始化、画图、结果相关业务接口实现
 * @date 2020/8/5 14:57
 */
@Service
public class MainCVBService implements IMainCVBService {

    @Autowired
    private BasisMeasurementMapper basisMeasurementMapper;

    @Autowired
    private PatientMapper patientMapper;

    @Autowired
    private TypeData.ByReference mainData;

    @Autowired
    private TypeDataR.ByReference mainDataR;

    @Autowired
    private HYP_RST.ByReference hypRr;

    @Autowired
    private AnalHPA_Data.ByReference analHPADataStruct;

    public interface CLibrary extends Library {
        CLibrary INSTANCE2 = (CLibrary) Native.loadLibrary("dspres", CLibrary.class);
        CLibrary INSTANCE3 = (CLibrary) Native.loadLibrary("self", CLibrary.class);
        CLibrary INSTANCE4 = (CLibrary) Native.loadLibrary("maincvb", CLibrary.class);

        short DispResults(ApgI.ByReference apgi, ApgII.ByReference apgii, apgD.ByReference d, apgR.ByReference t, String sfile, TypeData.ByReference main_data, TypeData.ByReference main_data1, TypeDataR.ByReference main_data2);
        short GetBPType (BPTypeData.ByReference mBPtype );
        void BVS_TableView(ApgII.ByReference apgii, CvbPrn.ByReference pDat, apgR.ByReference m_apgR, short HFlag, short CFlag, AnalHPA_Data.ByReference HpaData);
        void Cardio_TableViewL(ApgII.ByReference apgii, HYP_RST.ByReference hypRr, apgD.ByReference d, ApgI.ByReference apgi, apgR.ByReference m_apgR, CvbPrn.ByReference pDat, short HFlag, AnalHPA_Data.ByReference HpaData);
        void Cardio_TableViewR(ApgII.ByReference apgii, apgR.ByReference m_apgR, apgD.ByReference d, HYP_RST.ByReference hypRr, CvbPrn.ByReference pDat, short HFlag, AnalHPA_Data.ByReference HpaData);
        void GetStateValue(CvbPrn.ByReference pDat);
        short OnTimer();

        @Data
        public class ApgI extends Structure {
            @Override
            protected List getFieldOrder() {
                return Arrays.asList(new String[] { "No", "Num", "Tsnum", "TsNo", "T", "Ts", "pulse"});
            }

            public static class ByReference extends ApgI implements Structure.ByReference { }

            public short No;
            public short Num;
            public short Tsnum;
            public short TsNo;
            public float T;
            public float Ts;
            public float pulse;
        }

        @Data
        public class ApgII extends Structure {
            @Override
            protected List getFieldOrder() {
                return Arrays.asList(new String[] { "NationID", "Sex", "Age", "Ps", "Pd", "Weight", "Height", "BRPs", "BRPd", "ALPs", "ALPd", "ARPs", "ARPd"});
            }

            public static class ByReference extends ApgII implements Structure.ByReference { }

            public short NationID;
            public short Sex;
            public short Age;
            public short Ps;
            public short Pd;
            public short Weight;
            public short Height;
            public short BRPs;
            public short BRPd;
            public short ALPs;
            public short ALPd;
            public short ARPs;
            public short ARPd;
        }

        @Data
        public class apgD extends Structure {

            @Override
            protected List getFieldOrder() {
                return Arrays.asList(new String[] { "C0", "R0", "Cd", "C1", "C2", "L",
                        "Ad", "As", "T", "Pss"});
            }

            public static class ByReference extends apgD implements Structure.ByReference { }

            public float C0;
            public float R0;
            public float Cd;
            public float C1;
            public float C2;
            public float L;
            public float Ad;
            public float As;
            public float T;
            public float Pss;
        }

        @Data
        public class apgR extends Structure {

            @Override
            protected List getFieldOrder() {
                return Arrays.asList(new String[] { "Sv", "BR", "Kw", "Ps", "Pd", "Pba",
                        "Pss", "Cs", "Cd", "Cnew", "Ptop", "BG", "C2", "HI", "V", "Rp", "Ca", "V0", "Tpp", "Lv", "PR1", "PR2",
                        "MAP", "Ke", "C", "dp", "dp1", "ABI", "BMI"});
            }

            public static class ByReference extends apgR implements Structure.ByReference { }

            public float[] Sv = new float[3];
            public float[] BR = new float[3];
            public float[] Kw = new float[3];
            public float[] Ps = new float[3];
            public float[] Pd = new float[3];
            public float[] Pba = new float[3];
            public float[] Pss = new float[3];
            public float[] Cs = new float[3];
            public float[] Cd = new float[3];
            public float[] Cnew = new float[3];
            public float[] Ptop = new float[3];
            public float[] BG = new float[3];
            public float[] C2 = new float[3];
            public float[] HI = new float[3];
            public float[] V = new float[3];
            public float[] Rp = new float[3];
            public float[] Ca = new float[3];
            public float[] V0 = new float[3];
            public float[] Tpp = new float[3];
            public float[] Lv = new float[3];
            public float[] PR1 = new float[3];
            public float[] PR2 = new float[3];
            public float[] MAP = new float[3];
            public float[] Ke = new float[3];
            public float[] C = new float[3];
            public float[] dp = new float[3];
            public float[] dp1 = new float[3];
            public float[] ABI = new float[3];
            public float[] BMI = new float[3];
        }

        @Data
        public class BPTypeData extends Structure {
            @Override
            protected List getFieldOrder() {
                return Arrays.asList(new String[] { "Pa", "SV", "R", "Ps1", "Pd1"});
            }

            public static class ByReference extends BPTypeData implements Structure.ByReference { }

            public float Pa ;
            public float SV ;
            public float R ;
            public short Ps1;
            public short Pd1;
        }

        @Data
        public class AnalAPG_Data extends Structure {
            @Override
            protected List getFieldOrder() {
                return Arrays.asList(new String[] { "Normal", "result"});
            }

            public static class ByReference extends AnalAPG_Data implements Structure.ByReference { }

            public float Normal;
            public float result;
        }

        @Data
        public class AnalCoro_Data extends Structure {
            @Override
            protected List getFieldOrder() {
                return Arrays.asList(new String[] { "Normal", "Left", "Right"});
            }

            public static class ByReference extends AnalCoro_Data implements Structure.ByReference { }

            public float Normal;
            public float Left;
            public float Right;
        }

        @Data
        public class CvbPrn extends Structure {
            @Override
            protected List getFieldOrder() {
                return Arrays.asList(new String[] { "jAso", "jKe", "jKs", "jKd", "jBG", "jABI", "jPWV", "jSv", "jBR"
                        , "jBV", "jHI", "jAsc", "jKcd", "jKca", "jSvc", "jPRp", "jRc", "jVc", "jRcd", "jPs", "jPd",
                        "jPa", "jPtop", "jPss", "jDPT", "jICT", "jPEP", "jET", "jUT", "jHIep", "jAIue", "jBPt"});
            }

            public static class ByReference extends CvbPrn implements Structure.ByReference { }

            public AnalAPG_Data jAso;
            public AnalAPG_Data jKe;
            public AnalAPG_Data jKs;
            public AnalAPG_Data jKd;
            public AnalAPG_Data jBG;
            public AnalAPG_Data jABI;
            public AnalAPG_Data jPWV;
            public AnalAPG_Data jSv;
            public AnalAPG_Data jBR;
            public AnalAPG_Data jBV;
            public AnalCoro_Data jHI;
            public AnalCoro_Data jAsc;
            public AnalCoro_Data jKcd;
            public AnalCoro_Data jKca;
            public AnalCoro_Data jSvc;
            public AnalCoro_Data jPRp;
            public AnalCoro_Data jRc;
            public AnalCoro_Data jVc;
            public AnalCoro_Data jRcd;
            public AnalAPG_Data jPs;
            public AnalAPG_Data jPd;
            public AnalAPG_Data jPa;
            public AnalAPG_Data jPtop;
            public AnalAPG_Data jPss;
            public AnalAPG_Data jDPT;
            public AnalAPG_Data jICT;
            public AnalAPG_Data jPEP;
            public AnalAPG_Data jET;
            public AnalAPG_Data jUT;
            public AnalAPG_Data jHIep;
            public AnalAPG_Data jAIue;
            public short jBPt;
        }
    }

    public void freePointer(Pointer p){
        long peer = Pointer.nativeValue(p);
        Native.free(peer);//手动释放内存
        Pointer.nativeValue(p, 0);//避免Memory对象被GC时重复执行Nativ.free()方法
    }

    public Map AnalData_Init(String basisMeasurementId, Map resMap){
        //定义最终返回的Map
        //Map resMap = new HashMap();

        //根据测试号获取对应患者基本检测信息
        BasisMeasurement basisMeasurementre = basisMeasurementMapper.getPatientBasisMeasurementnew(basisMeasurementId);

        //根据患者id查询患者基本信息
        Patient patient = patientMapper.selectPatientById(basisMeasurementre.getPatientId());

        CLibrary.ApgI.ByReference apgI = new CLibrary.ApgI.ByReference();

        CLibrary.ApgII.ByReference apgII = new CLibrary.ApgII.ByReference();
        //计算当前患者真实年龄
        SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd");
        Date date = null;
        try {
            date = format.parse(patient.getBirthday());
        } catch (Exception e) {
            e.printStackTrace();
        }
        int age = GetAge.getAge(date);
        resMap.put("age", age);
        //apgII.Pd = basisMeasurement.getPd().shortValue();
        //apgII.Ps = basisMeasurement.getPs().shortValue();
        apgII.Age = (short) age;
        apgII.ALPd = basisMeasurementre.getAlPd().shortValue();
        apgII.ALPs = basisMeasurementre.getAlPs().shortValue();
        apgII.ARPd = basisMeasurementre.getArPd().shortValue();
        apgII.ARPs = basisMeasurementre.getArPs().shortValue();
        apgII.BRPd = basisMeasurementre.getBrPd().shortValue();
        apgII.BRPs = basisMeasurementre.getBrPs().shortValue();
        apgII.Height = basisMeasurementre.getHeight().shortValue();
        apgII.Weight = basisMeasurementre.getWeight().shortValue();
        apgII.Sex = patient.getSex().shortValue();
        if (apgII.ARPs == 1 || apgII.ARPd == 1 || apgII.ARPs ==3 || apgII.ARPd ==3)
        {
            apgII.Ps = basisMeasurementre.getBrPs().shortValue();
            apgII.Pd = basisMeasurementre.getBrPd().shortValue();
        }
        else
        {
            apgII.Pd = basisMeasurementre.getPd().shortValue();
            apgII.Ps = basisMeasurementre.getPs().shortValue();
        }

        CLibrary.apgD.ByReference apgD = new CLibrary.apgD.ByReference();
        CLibrary.apgR.ByReference apgR = new CLibrary.apgR.ByReference();

        String sfile = "";
        try {
            //todo linux获取路径
            File directory = new File("");// 参数为空
            sfile = directory.getCanonicalPath();
        }catch (Exception e){
            e.printStackTrace();
        }

        sfile = sfile + "/WK-STD.RYU";

        TypeData.ByReference main_data = new TypeData.ByReference();
        boolean g_bApgR;
        short ret = CLibrary.INSTANCE2.DispResults(apgI, apgII, apgD, apgR, sfile, main_data, mainData, mainDataR);

        System.out.println("测试输出apgI结构体的值，No：" + apgI.No);
        System.out.println("测试输出apgI结构体的值，Num：" + apgI.Num);
        System.out.println("测试输出apgI结构体的值，Tsnum：" + apgI.Tsnum);
        System.out.println("测试输出apgI结构体的值，TsNo：" + apgI.TsNo);
        System.out.println("测试输出apgI结构体的值，T：" + apgI.T);
        System.out.println("测试输出apgI结构体的值，Ts：" + apgI.Ts);
        System.out.println("测试输出apgI结构体的值，pulse：" + apgI.pulse);

        System.out.println("测试输出apgII结构体的值，NationID：" + apgII.NationID);
        System.out.println("测试输出apgII结构体的值，Sex：" + apgII.Sex);
        System.out.println("测试输出apgII结构体的值，Age：" + apgII.Age);
        System.out.println("测试输出apgII结构体的值，Ps：" + apgII.Ps);
        System.out.println("测试输出apgII结构体的值，Pd：" + apgII.Pd);
        System.out.println("测试输出apgII结构体的值，Weight：" + apgII.Weight);
        System.out.println("测试输出apgII结构体的值，Height：" + apgII.Height);
        System.out.println("测试输出apgII结构体的值，BRPs：" + apgII.BRPs);
        System.out.println("测试输出apgII结构体的值，BRPd：" + apgII.BRPd);
        System.out.println("测试输出apgII结构体的值，ALPs：" + apgII.ALPs);
        System.out.println("测试输出apgII结构体的值，ALPd：" + apgII.ALPd);
        System.out.println("测试输出apgII结构体的值，ARPs：" + apgII.ARPs);
        System.out.println("测试输出apgII结构体的值，ARPd：" + apgII.ARPd);

        System.out.println("测试输出apgD结构体的值，C0：" + apgD.C0);
        System.out.println("测试输出apgD结构体的值，R0：" + apgD.R0);
        System.out.println("测试输出apgD结构体的值，Cd：" + apgD.Cd);
        System.out.println("测试输出apgD结构体的值，C1：" + apgD.C1);
        System.out.println("测试输出apgD结构体的值，C2：" + apgD.C2);
        System.out.println("测试输出apgD结构体的值，L：" + apgD.L);
        System.out.println("测试输出apgD结构体的值，Ad：" + apgD.Ad);
        System.out.println("测试输出apgD结构体的值，As：" + apgD.As);
        System.out.println("测试输出apgD结构体的值，T：" + apgD.T);
        System.out.println("测试输出apgD结构体的值，Pss：" + apgD.Pss);

        System.out.println("测试输出apgR结构体的值，Sv：" + Arrays.toString(apgR.Sv));
        System.out.println("测试输出apgR结构体的值，BR：" + Arrays.toString(apgR.BR));
        System.out.println("测试输出apgR结构体的值，Kw：" + Arrays.toString(apgR.Kw));
        System.out.println("测试输出apgR结构体的值，Ps：" + Arrays.toString(apgR.Ps));
        System.out.println("测试输出apgR结构体的值，Pd0：" + apgR.Pd[0] + " Pd1: " + apgR.Pd[1] + " Pd2: " + apgR.Pd[2]);
        System.out.println("测试输出apgR结构体的值，Pba：" + Arrays.toString(apgR.Pba));
        System.out.println("测试输出apgR结构体的值，Pss0：" + apgR.Pss[0] + " Pss1: " + apgR.Pss[1] + " Pss2: " + apgR.Pss[2]);
        System.out.println("测试输出apgR结构体的值，Cs：" + Arrays.toString(apgR.Cs));
        System.out.println("测试输出apgR结构体的值，Cd：" + Arrays.toString(apgR.Cd));
        System.out.println("测试输出apgR结构体的值，Cnew：" + Arrays.toString(apgR.Cnew));
        System.out.println("测试输出apgR结构体的值，Ptop：" + Arrays.toString(apgR.Ptop));
        System.out.println("测试输出apgR结构体的值，BG：" + Arrays.toString(apgR.BG));
        System.out.println("测试输出apgR结构体的值，C2：" + Arrays.toString(apgR.C2));
        System.out.println("测试输出apgR结构体的值，HI：" + Arrays.toString(apgR.HI));
        System.out.println("测试输出apgR结构体的值，V：" + Arrays.toString(apgR.V));
        System.out.println("测试输出apgR结构体的值，Rp：" + Arrays.toString(apgR.Rp));
        System.out.println("测试输出apgR结构体的值，Ca：" + Arrays.toString(apgR.Ca));
        System.out.println("测试输出apgR结构体的值，V0：" + Arrays.toString(apgR.V0));
        System.out.println("测试输出apgR结构体的值，Tpp：" + Arrays.toString(apgR.Tpp));
        System.out.println("测试输出apgR结构体的值，Lv：" + Arrays.toString(apgR.Lv));
        System.out.println("测试输出apgR结构体的值，PR1：" + Arrays.toString(apgR.PR1));
        System.out.println("测试输出apgR结构体的值，PR2：" + Arrays.toString(apgR.PR2));
        System.out.println("测试输出apgR结构体的值，MAP：" + Arrays.toString(apgR.MAP));
        System.out.println("测试输出apgR结构体的值，Ke：" + Arrays.toString(apgR.Ke));
        System.out.println("测试输出apgR结构体的值，C：" + Arrays.toString(apgR.C));
        System.out.println("测试输出apgR结构体的值，dp：" + Arrays.toString(apgR.dp));
        System.out.println("测试输出apgR结构体的值，dp1：" + Arrays.toString(apgR.dp1));
        System.out.println("测试输出apgR结构体的值，ABI：" + Arrays.toString(apgR.ABI));
        System.out.println("测试输出apgR结构体的值，BMI：" + Arrays.toString(apgR.BMI));

        if (ret == 0)
        {
            resMap.put("prResult", -10000.0);
            resMap.put("staCode", 518);
            g_bApgR = false;
            return resMap;
        }
        //g_bApgR = true; //这里判断之后，在后面的Cardio_TableViewR算法中用到的if (g_bApgR == True) 语句就可以不用判断；

        g_bApgR = true;
        float f1 = apgII.ALPs;
        float f2 = apgII.ALPd;
        float f3 = apgII.Ps;

        if ( f1 > f2)
        {
            if (f3 != 0)
                apgR.ABI[1] = f1 / f3;
        }
        else
        {	if (f3 != 0)
            apgR.ABI[1] = f2 / f3;
        }

        //todo 源码中后面还有对应代码业务，这里需要补充
        // 后面用到的 AnalHPA_Data HpaData的赋值来自于AnalysisApg.cpp中的AnalysisHpaMeasure 中的AnalHPA_Data mAnalHPA 结构体输出参数

        this.PSTypeEvaluation(basisMeasurementId, apgR, apgII, apgD, apgI, apgR, resMap);

        return resMap;
    }

    public boolean PSTypeEvaluation(String basisMeasurementId, CLibrary.apgR.ByReference apgR, CLibrary.ApgII.ByReference apgii, CLibrary.apgD.ByReference d, CLibrary.ApgI.ByReference apgi, CLibrary.apgR.ByReference m_apgR, Map resMap){
        //根据测试号获取对应患者基本检测信息
        BasisMeasurement basisMeasurementnew = basisMeasurementMapper.getPatientBasisMeasurementnew1(basisMeasurementId);

        CLibrary.CvbPrn.ByReference pDat = new CLibrary.CvbPrn.ByReference();

        //todo 以下赋值来自GetBPFlag方法 start
        short HFlag = basisMeasurementnew.getArPs().shortValue();
        short CFlag = basisMeasurementnew.getArPd().shortValue();
        //todo 以下赋值来自GetBPFlag方法 end

        CLibrary.INSTANCE4.BVS_TableView(apgii, pDat, m_apgR, HFlag, CFlag, analHPADataStruct);

        CLibrary.INSTANCE4.Cardio_TableViewL(apgii, hypRr, d, apgi, m_apgR, pDat, HFlag, analHPADataStruct);

        //返回结果页面第三页左的结果数据
        if(Float.isNaN(pDat.jPRp.Left) || Float.isInfinite(pDat.jPRp.Left)){
            resMap.put("prResult", -10000.0);
            resMap.put("staCode", 550);
        }else{
            resMap.put("prResult", pDat.jPRp.Left);
        }

        if(Float.isNaN(pDat.jKcd.Left) || Float.isInfinite(pDat.jKcd.Left)){
            resMap.put("cResult", -10000.0);
            resMap.put("staCode", 551);
        }else{
            resMap.put("cResult", pDat.jKcd.Left);
        }

        if(Float.isNaN(pDat.jAsc.Left) || Float.isInfinite(pDat.jAsc.Left)){
            resMap.put("ascResult", -10000.0);
            resMap.put("staCode", 552);
        }else{
            resMap.put("ascResult", pDat.jAsc.Left);
        }

        if(Float.isNaN(pDat.jSvc.Left) || Float.isInfinite(pDat.jSvc.Left)){
            resMap.put("qResult", -10000.0);
            resMap.put("staCode", 553);
        }else{
            resMap.put("qResult", pDat.jSvc.Left);
        }

        CLibrary.INSTANCE4.Cardio_TableViewR(apgii, m_apgR, d, hypRr, pDat, HFlag, analHPADataStruct);

        //返回结果页面第三页右的结果数据
        if(Float.isNaN(pDat.jPRp.Right) || Float.isInfinite(pDat.jPRp.Right)){
            resMap.put("prResultR", -10000.0);
            resMap.put("staCode", 554);
        }else{
            resMap.put("prResultR", pDat.jPRp.Right);
        }

        if(Float.isNaN(pDat.jKcd.Right) || Float.isInfinite(pDat.jKcd.Right)){
            resMap.put("cResultR", -10000.0);
            resMap.put("staCode", 555);
        }else{
            resMap.put("cResultR", pDat.jKcd.Right);
        }

        if(Float.isNaN(pDat.jAsc.Right) || Float.isInfinite(pDat.jAsc.Right)){
            resMap.put("ascResultR", -10000.0);
            resMap.put("staCode", 556);
        }else{
            resMap.put("ascResultR", pDat.jAsc.Right);
        }

        if(Float.isNaN(pDat.jSvc.Right) || Float.isInfinite(pDat.jSvc.Right)){
            resMap.put("qResultR", -10000.0);
            resMap.put("staCode", 557);
        }else{
            resMap.put("qResultR", pDat.jSvc.Right);
        }

        if(Float.isNaN(pDat.jSv.result) || Float.isInfinite(pDat.jSv.result)){
            resMap.put("jsvResult", -10000.0);
            resMap.put("staCode", 558);
        }else{
            resMap.put("jsvResult", pDat.jSv.result);
        }

        if(Float.isNaN(pDat.jBR.result) || Float.isInfinite(pDat.jBR.result)){
            resMap.put("jbrResult", -10000.0);
            resMap.put("staCode", 559);
        }else{
            resMap.put("jbrResult", pDat.jBR.result);
        }

        //todo BLS_TableView 算法中得赋值 start
        pDat.jBG.Normal = m_apgR.BG[0];
        //pDat.jBG.result = m_apgR.BG[1];
        pDat.jBG.result =  (float) Math.sqrt(pDat.jBR.result / pDat.jKe.result) ;
        //todo BLS_TableView 算法中得赋值 end

        //todo PSSize_TableView 算法中得赋值 start
        pDat.jPs.result = (float)basisMeasurementnew.getPs();
        pDat.jPd.result = (float)basisMeasurementnew.getPd();
        pDat.jPa.result = (float)((float)basisMeasurementnew.getPs() + (float)basisMeasurementnew.getPd() * 2.0f) / 3.0f;

        if(Float.isNaN(pDat.jPa.result) || Float.isInfinite(pDat.jPa.result)){
            resMap.put("jpaResult", -10000.0);
            resMap.put("staCode", 560);
        }else{
            resMap.put("jpaResult", pDat.jPa.result);
        }
        if(Float.isNaN(pDat.jPs.result) || Float.isInfinite(pDat.jPs.result)){
            resMap.put("jpsResult", -10000.0);
            resMap.put("staCode", 561);
        }else{
            resMap.put("jpsResult", pDat.jPs.result);
        }
        if(Float.isNaN(pDat.jPd.result) || Float.isInfinite(pDat.jPd.result)){
            resMap.put("jpdResult", -10000.0);
            resMap.put("staCode", 562);
        }else{
            resMap.put("jpdResult", pDat.jPd.result);
        }

        //todo PSSize_TableView 算法中得赋值 end

        //todo PSSize1_TableView 算法中得赋值 start
        pDat.jPss.Normal = m_apgR.Pss[0];
        pDat.jPss.result = m_apgR.Pss[1];

        if(Float.isNaN(pDat.jPss.result) || Float.isInfinite(pDat.jPss.result)){
            resMap.put("pssResult", -10000.0);
            resMap.put("staCode", 563);
        }else{
            resMap.put("pssResult", pDat.jPss.result);
        }

        //todo PSSize1_TableView 算法中得赋值 end

        boolean bRet = false;
        CLibrary.BPTypeData.ByReference mBPType = new CLibrary.BPTypeData.ByReference();
        short g_nPSType;
        mBPType.Pa = pDat.jPa.result;
        mBPType.R = pDat.jBR.result;
        mBPType.Ps1 =  (short)pDat.jPs.result;
        mBPType.Pd1 = (short)pDat.jPd.result;
        mBPType.SV = pDat.jSv.result / pDat.jSv.Normal;
        g_nPSType = CLibrary.INSTANCE3.GetBPType(mBPType);

        resMap.put("gNPSType", g_nPSType);
        pDat.jBPt = g_nPSType;

        CLibrary.INSTANCE4.GetStateValue(pDat);

        /* todo 返回计算得风险等级
        129 160 190 220 250 280 310 340 370 400
        0   1   2	3	4	5	6	7	8	9
         */
        short rect2 = CLibrary.INSTANCE4.OnTimer();

        resMap.put("rect2", rect2);

        if (g_nPSType == 0)
        {
            return bRet;
        }

        bRet = true;

        return bRet;

    }
}
