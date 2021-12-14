package com.hengde.nivi.modules.analysis.service.impl;

import com.hengde.nivi.common.global.AnalHPA_Data;
import com.hengde.nivi.common.global.GlobalClass;
import com.hengde.nivi.common.global.TypeData;
import com.hengde.nivi.common.global.TypeDataR;
import com.hengde.nivi.common.util.GetAge;
import com.hengde.nivi.common.util.jnatojavasdk.Apg;
import com.hengde.nivi.modules.analysis.service.IAnalysisApgService;
import com.hengde.nivi.modules.create.dao.BasisMeasurementMapper;
import com.hengde.nivi.modules.create.entity.BasisMeasurement;
import com.hengde.nivi.modules.detection.entity.OriginalWaveformReq;
import com.hengde.nivi.modules.detection.service.IDetectionService;
import com.hengde.nivi.modules.patient.dao.PatientMapper;
import com.hengde.nivi.modules.patient.entity.Patient;
import com.sun.jna.*;
import lombok.Data;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.awt.*;
import java.io.File;
import java.text.SimpleDateFormat;
import java.util.*;
import java.util.List;

/**
 * @author dong
 * @title: AnalysisApgService
 * @description: 左右颈动脉、结果算法相关业务实现
 * @date  2020/5/12 10:40
 */
@Service
public class AnalysisApgService implements IAnalysisApgService {

    @Autowired
    private IDetectionService detectionService;

    @Autowired
    private BasisMeasurementMapper basisMeasurementMapper;

    @Autowired
    private TypeData.ByReference mainData;

    @Autowired
    private TypeDataR.ByReference mainDataR;

    @Autowired
    private PatientMapper patientMapper;

    @Autowired
    private GlobalClass globalClass;

    @Autowired
    private AnalHPA_Data.ByReference analHPADataStruct;

    public interface CLibrary extends Library {
        CLibrary INSTANCE2 = (CLibrary) Native.loadLibrary("ecg",CLibrary.class);
        CLibrary INSTANCE3 = (CLibrary) Native.loadLibrary("sub",CLibrary.class);
        CLibrary INSTANCE6 = (CLibrary) Native.loadLibrary("ComLib",CLibrary.class);
        CLibrary INSTANCE7 = (CLibrary) Native.loadLibrary("hpassysstr",CLibrary.class);
        CLibrary INSTANCE8 = (CLibrary) Native.loadLibrary("modelstr",CLibrary.class);
        CLibrary INSTANCE9 = (CLibrary) Native.loadLibrary("reshpastr",CLibrary.class);
        CLibrary INSTANCE10 = (CLibrary) Native.loadLibrary("apgstr",CLibrary.class);

        short get_R_tops(float[] signal, short size , Pointer RTops_x, Pointer QP, Pointer TP);
        void Signal_Move(short[] olds, Pointer pointer, short num);
        void filter_mibun(float[] signal, Pointer pointer, short N, short NP );
        void filter( float[] signal, Pointer pointer, short N, short NP );
        short AbI_IIsoPointGet (float[] fl_pcg, short sig_count, short[] rr,
                                short chnumber, Pointer Iso, Pointer IIso);
        short AbApgStTsGetonPcg (float[] fl_apg, short sig_count,
                                 short[] Iso, short[] IIso, short chnumber,
                                 Pointer St, Pointer Ts);
//        short AbOneGetbyAuto (short[] St, short[] Ts, short[] Ut, short chnumber);
//        short AbOneGetbyAuto (short[] St, short chnumber);
        short DetectAbsMax(float[] signals, short startPos, short endPos );
        float DetectMaxValue(float[] buff, short start, short end );
        void Filter(float[] signals, Pointer smooth, short allNum, short smNum );
        void FilterMibun(float[] signals, Pointer mibun, short allNum, short smNum );
        short RQT_Analysis(float[] k_snl, float[] j_snl, short  Size, AnalHPA.ByReference analHPASStruct, AnalHPA_Data.ByReference analHPADataStruct );
        short DetectUSpoint( float[] des ,short start, short end,
                             short ch, short size, AnalHPA.ByReference analHPAStruct );
        short load_std_data (String fileName);
        short OneOrderModel(float[] fl_apg, short sig_count, short Tts, short WGht, short HGht, TypeData.ByReference mainData);
        short OneOrderModel(float[] fl_apg, short sig_count, short Tts, short WGht, short HGht, TypeDataR.ByReference mainData);
        void AnalysisHpaMeasure (short pSx, short pBd, short[] Rtop, short[] Qpt,
                                        short[] Tpt, short[] PIp, short[] PIIp, short[] Usp, short[] Dnp, short Inx,
                                        AnalHPA_Data.ByReference analHPADataStruct, String RsFile, short[] Utp, float PTT1, float AF_Length);
        short AutoSelectCycle (ApgI.ByReference apgi, ApgII.ByReference apgii, float[] ecg, float[] pcg, float[] apg, short[] startp, short[] Ts, short[] PI, short[] PII, short num, TypeData.ByReference mainData);
        short Cut_One_APG(float[] APGS, short size, Pointer apg, short[] startp, short[] Ts, short s, TypeData.ByReference mainData);
        short Cut_One_APG(float[] APGS, short size, Pointer apg, short[] startp, short[] Ts, short s, TypeDataR.ByReference mainData);
        short signal_load( Pointer buff, short size, String file_name);
        short MinID(float[] src, short t1, short t2);
        short MaxID(float[] src, short t1, short t2);

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
        public class AnalHPA extends Structure {
            @Override
            protected List getFieldOrder() {
                return Arrays.asList(new String[] { "R_top", "Q_start", "T_end", "I_start", "II_start", "US_point",
                        "DN_point", "UT_point", "Leg", "Rpos", "RtopNum", "Rtops", "Qstarts", "QendNum", "Tends",
                        "Istarts", "IIstarts", "USpnts", "DNpnts", "UTpnts"});
            }

            public static class ByReference extends AnalHPA implements Structure.ByReference { }

            public short	R_top;        /*  in ECG ** R_wave  */
            public short	Q_start;	   /*  in ECG ** Q_wave  */
            public short	T_end;        /*  in ECG ** T_wave  */
            public short	I_start;      /*  in PCG ** I_sound */
            public short	II_start;     /*  in PCG ** II_sound*/
            public short	US_point;     /*  in APG ** US_point*/
            public short	DN_point;     /*  in APG ** DN_point*/
            public short	UT_point;	   /*  in APG ** UT_point*/
            public float    Leg;
            public short	Rpos;
            public short	RtopNum;
            public short[] Rtops = new short[51];
            public short[]	Qstarts = new short[51];
            public short	QendNum;
            public short[]	Tends = new short[51];
            public short[]	Istarts = new short[51];
            public short[]	IIstarts = new short[51];
            public short[]	USpnts = new short[51];
            public short[]	DNpnts = new short[51];
            public short[]	UTpnts = new short[51];
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
        public class CG extends Structure {
            @Override
            protected List getFieldOrder() {
                return Arrays.asList(new String[] { "box_Num", "width", "height", "samplenum", "newFlag", "Ynum",
                        "step_time", "stepX", "stepY"});
            }

            public static class ByReference extends CG implements Structure.ByReference { }

            //HDC			dc;
            public short box_Num;
            public short width;
            public short height;
            public short samplenum;
            public short newFlag;
            public short Ynum;
            public float step_time;
            public short stepX;
            public short stepY;
        }
    }

    @Override
    public Map preTranslateMessage(String basisMeasurementId, short IDL, short IDR, Map resMap, int model) {

        //定义最终返回的Map
        //Map resMap = new HashMap();

        CLibrary.ApgI.ByReference apgI = new CLibrary.ApgI.ByReference();

        //根据测试号获取对应患者基本检测信息
        BasisMeasurement basisMeasurement = basisMeasurementMapper.getPatientBasisMeasurementApg(basisMeasurementId);

        if(basisMeasurement.getArPs() == 1 || basisMeasurement.getArPs() == 3 )
        {
            basisMeasurement.setPs(basisMeasurement.getBrPs());
            basisMeasurement.setPd(basisMeasurement.getBrPd());
        }

        //根据患者id查询患者基本信息
        Patient patient = patientMapper.selectPatientById(basisMeasurement.getPatientId());

        //根据测试号获取所有原始检测波形
        OriginalWaveformReq originalWaveform = detectionService.getOriginalWaveform(basisMeasurementId);

        float PTT1 = 0.0f;
        float AF_Length = 0.0f;
//        float Apg_Length = 0.2437f * basisMeasurement.getHeight() - 18.999f; //源码
//        float Femoral_Length =  0.5643f * basisMeasurement.getHeight() - 18.381f; //源码
        float Apg_Length = 0.1837f * basisMeasurement.getHeight() - 18.999f; //源码
        float Femoral_Length =  0.4643f * basisMeasurement.getHeight() - 18.381f; //源码

        if (patient.getSex() == 1){ //woman
            AF_Length = 2.382f + 0.1767f * basisMeasurement.getHeight() + 0.238f * basisMeasurement.getWeight();
        }
        if(patient.getSex() == 0){ //man
            AF_Length = 1.382f + 0.1884f * basisMeasurement.getHeight() + 0.1205f * basisMeasurement.getWeight();
        }

//        float Apg_Length = 0.0f; //Distance measurements for the assessment of carotid to femoral pulse wave velocity Sebastian J. Vermeersch
//        float Femoral_Length = 0.2f * basisMeasurement.getHeight() + 10.0f - 3.0f;

        //定义需要使用的全局数组
        short[]	Utop = new short[30];
        Pointer PCGIL = new Memory(30 * 2);
        Pointer PCGIIL = new Memory(30 * 2);
        Pointer PCGIR = new Memory(30 * 2);
        Pointer PCGIIR = new Memory(30 * 2);
        short[]	Qpoint = new short[30];
        short[]	Tpoint = new short[30];

        Map parMap = new HashMap();
        parMap.put("AF_Length", AF_Length);
        parMap.put("Apg_Length", Apg_Length);
        parMap.put("Femoral_Length", Femoral_Length);
        parMap.put("PTT1", PTT1);

        /*if(this.ReadSignal(originalWaveform, ECGS, APGS, parMap, (short) sFlg) == false)
        {
            resMap.put("staCode", 500);
            return resMap;
        }*/

        //GetCycleInfo start
        //Pointer utopPL = new Memory(30 * 2);
        //Pointer qpointPL = new Memory(30 * 2);
        //Pointer tpointPL = new Memory(30 * 2);
        Pointer RtopL = new Memory(30 * 2);
        Pointer TsL = new Memory(30 * 2);
//        Pointer oneAPGL = new Memory(2000 * 4);
        float[] oneAPGL = new float[2000];

        //Pointer utopPR = new Memory(30 * 2);
        //Pointer qpointPR = new Memory(30 * 2);
        //Pointer tpointPR = new Memory(30 * 2);
        Pointer RtopR = new Memory(30 * 2);
        Pointer TsR = new Memory(30 * 2);
//        Pointer oneAPGR = new Memory(2000 * 4);
        float[] oneAPGR = new float[2000];

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
        apgII.Pd = basisMeasurement.getPd().shortValue();
        apgII.Ps = basisMeasurement.getPs().shortValue();
        apgII.Age = (short) age;
        apgII.ALPd = basisMeasurement.getAlPd().shortValue();
        apgII.ALPs = basisMeasurement.getAlPs().shortValue();
        apgII.ARPd = basisMeasurement.getArPd().shortValue();
        apgII.ARPs = basisMeasurement.getArPs().shortValue();
        apgII.BRPd = basisMeasurement.getBrPd().shortValue();
        apgII.BRPs = basisMeasurement.getBrPs().shortValue();
        apgII.Height = basisMeasurement.getHeight().shortValue();
        apgII.Weight = basisMeasurement.getWeight().shortValue();
        apgII.Sex = patient.getSex().shortValue();
        //CLibrary.AnalHPA_Data.ByReference analHPADataStruct = new CLibrary.AnalHPA_Data.ByReference();
        CLibrary.AnalHPA.ByReference analHPAStruct = new CLibrary.AnalHPA.ByReference();
        CLibrary.apgD.ByReference d = new CLibrary.apgD.ByReference();

//        getWaveformL(parMap, originalWaveform); //获取左波形数据
//
//        //开始调用左波形数据公共模块代码
//        publicCallL(resMap, parMap, apgI, basisMeasurement, age, RtopL, TsL, PCGIL, PCGIIL, originalWaveform,
//                analHPADataStruct, analHPAStruct, apgII, oneAPGL, d, IDL, model);
//        //左波形数据公共模块代码调用结束

        getWaveformR(parMap, originalWaveform); //获取右波形数据

        //开始调用右波形数据公共模块代码
        publicCallR(resMap, parMap, apgI, basisMeasurement, age, RtopR, TsR, PCGIR, PCGIIR, analHPADataStruct, apgII, oneAPGR, d, IDR, model);
        //右波形数据公共模块代码调用结束

        getWaveformL(parMap, originalWaveform); //获取左波形数据

        //开始调用左波形数据公共模块代码
        publicCallL(resMap, parMap, apgI, basisMeasurement, age, RtopL, TsL, PCGIL, PCGIIL, originalWaveform,
                analHPADataStruct, analHPAStruct, apgII, oneAPGL, d, IDL, model);
        //左波形数据公共模块代码调用结束

        CLibrary.CG.ByReference CG = new CLibrary.CG.ByReference();
        this.Paint_CardioVascular(CG, resMap);

        //freePointer(utopPL);
        //freePointer(qpointPL);
        //freePointer(tpointPL);
        freePointer(RtopL);
        freePointer(TsL);
        freePointer(PCGIL);
        freePointer(PCGIIL);
//        freePointer(oneAPGL);

        //freePointer(utopPR);
        //freePointer(qpointPR);
        //freePointer(tpointPR);
        freePointer(RtopR);
        freePointer(TsR);
        freePointer(PCGIR);
        freePointer(PCGIIR);
//        freePointer(oneAPGR);
        return resMap;
    }

    public void getWaveformL(Map parMap, OriginalWaveformReq originalWaveform){
        float[]	ECGS = new float[5000];
        float[]	PCGS = new float[5000];
        float[]	APGS = new float[5000];

        //获取左颈动脉心电波
        int[] ecgPL = originalWaveform.getEcgPl();
        short apgSampNum = (short) ecgPL.length;

        //将其转为float数组
        for (int e = 0; e < apgSampNum; e++) {
            ECGS[e] = (float) ecgPL[e];
        }

        //获取左颈动脉脉搏波
        int[] apgL = originalWaveform.getApgL();
        apgSampNum = (short) apgL.length;

        //将其转为float数
        for (int a = 0; a < apgSampNum; a++) {
            APGS[a] = (float) apgL[a];
        }

        //todo 以下代码是自己添加
        //获取左颈动脉心音波
        int[] pcgPL = originalWaveform.getPcgPl();
        apgSampNum = (short) pcgPL.length;

        //将其转为float数
        for (int b = 0; b < apgSampNum; b++) {
            PCGS[b] = (float) pcgPL[b];
        }

        parMap.put("apgSampNum", apgSampNum);

        parMap.put("ECGS", ECGS);
        parMap.put("APGS", APGS);
        //todo 以下代码为自己初始化值
        parMap.put("PCGS", PCGS);
    }

    public void getWaveformR(Map parMap, OriginalWaveformReq originalWaveform){
        float[]	ECGS = new float[5000];
        float[]	PCGS = new float[5000];
        float[]	APGS = new float[5000];

        //获取左颈动脉心电波
        int[] ecgPR = originalWaveform.getEcgPr();
        short apgSampNum = (short) ecgPR.length;

        //将其转为float数组
        for (int e = 0; e < apgSampNum; e++) {
            ECGS[e] = (float) ecgPR[e];
        }

        //获取左颈动脉脉搏波
        int[] apgR = originalWaveform.getApgR();
        apgSampNum = (short) apgR.length;

        //将其转为float数
        for (int a = 0; a < apgSampNum; a++) {
            APGS[a] = (float) apgR[a];
        }

        //todo 以下代码是自己添加
        //获取左颈动脉心音波
        int[] pcgPR = originalWaveform.getPcgPr();
        apgSampNum = (short) pcgPR.length;

        //将其转为float数
        for (int b = 0; b < apgSampNum; b++) {
            PCGS[b] = (float) pcgPR[b];
        }

        parMap.put("apgSampNum", apgSampNum);

        parMap.put("ECGS", ECGS);
        parMap.put("APGS", APGS);
        //todo 以下代码为自己初始化值
        parMap.put("PCGS", PCGS);
    }

    public void publicCallL(Map resMap, Map parMap, CLibrary.ApgI.ByReference apgI,
                            BasisMeasurement basisMeasurement, int age, Pointer Rtop, Pointer Ts, Pointer PCGI, Pointer PCGII,
                            OriginalWaveformReq originalWaveform, AnalHPA_Data.ByReference analHPADataStruct,
                            CLibrary.AnalHPA.ByReference analHPAStruct, CLibrary.ApgII.ByReference apgII, float[] oneAPG, CLibrary.apgD.ByReference d, short IDL, int model){
        short i;
        short dd =0;
        short ss=0;
        short RTopNum;
        float stepTime = 0.0012f;
        short ID;
        short ko;
        short[]	UT = new short[30];


        //RTopNum = CLibrary.INSTANCE2.get_R_tops((float[]) parMap.get("ECGS"), (short) parMap.get("apgSampNum"), utopP, qpointP, tpointP);

        Pointer jSnl = new Memory((short)parMap.get("apgSampNum") * 4);
        Pointer kSnlP = new Memory((short)parMap.get("apgSampNum") * 4);
        CLibrary.AnalHPA.ByReference analHPAStructsRQT = new CLibrary.AnalHPA.ByReference();
        AnalHPA_Data.ByReference analHPADataStructRQT = new AnalHPA_Data.ByReference();
        CLibrary.INSTANCE2.filter((float[]) parMap.get("ECGS"), jSnl, (short)parMap.get("apgSampNum"), (short) 25);
        CLibrary.INSTANCE2.filter_mibun(jSnl.getFloatArray(0, (short)parMap.get("apgSampNum")), kSnlP, (short) parMap.get("apgSampNum"), (short) 25);
        CLibrary.INSTANCE2.filter_mibun(kSnlP.getFloatArray(0, (short)parMap.get("apgSampNum")), jSnl, (short)parMap.get("apgSampNum"), (short) 25);
        CLibrary.INSTANCE7.RQT_Analysis(jSnl.getFloatArray(0, (short)parMap.get("apgSampNum")), kSnlP.getFloatArray(0, (short)parMap.get("apgSampNum")), (short)parMap.get("apgSampNum"), analHPAStructsRQT, analHPADataStructRQT);
        RTopNum = analHPAStructsRQT.getRtopNum();
        if(RTopNum <= 2){
            resMap.put("staCode", 512);
            freePointer(jSnl);
            freePointer(kSnlP);
            return;
        }

        if (RTopNum > 2) {
            i = autoSelectCycleL(apgI, basisMeasurement, age, (float[]) parMap.get("ECGS"), (float[]) parMap.get("PCGS"), (float[]) parMap.get("APGS"), Rtop, Ts, PCGI, PCGII, (short) parMap.get("apgSampNum"), stepTime);
            if ( i == 0 ) {
                resMap.put("staCode", 513);
                freePointer(jSnl);
                freePointer(kSnlP);
                return;
            }

            for (i = 0; i < RTopNum; i++) {
                dd = 0; ss = 0;
                dd = Ts.getShortArray(0, 30)[i]; ss = Rtop.getShortArray(0, 30)[i];
                UT[i] = CLibrary.INSTANCE6.DetectAbsMax((float[]) parMap.get("APGS"), ss, dd); //todo 这里与源码有差别，第一个参数是float数组，源码中是float类型的数字；
            }

            resMap.put("TsL", Ts.getShortArray(0, 30));
            resMap.put("RtopL", Rtop.getShortArray(0, 30)); //Rtop是起始点，Ts是拐点，UT最高点
            resMap.put("UTL", UT);
            resMap.put("RtopNumL", RTopNum);

            if(IDL != -1){
                ID = IDL;
            }else{
                ID = apgI.No; //可能需要存DB；ID是选的第几段波形
            }

            resMap.put("IDL", ID);

            //todo  这里返回给前端
        }else{
            resMap.put("staCode", 514);
            freePointer(jSnl);
            freePointer(kSnlP);
            return;
        }
        ko = 0; //todo ko=0是手动选择波形的控制器，逻辑需要修改
        //GetCycleInfo end

        System.out.println("测试输出IDR参数：" + (short) resMap.get("IDR") + "， 测试输出ID参数：" + ID);
        //PTV_Dectect start
        this.PTV_Dectect(originalWaveform, analHPADataStruct, analHPAStruct, parMap, resMap, model);
        //PTV_Dectect end

        System.out.println("测试同步模式之前：" + (float)parMap.get("AF_Length"));
        if (model == 1){ //同步模式
            parMap.put("AF_Length", (float)parMap.get("AF_Length")+15);
        }
        System.out.println("测试同步模式之后AF_Length：" + (float)parMap.get("AF_Length"));

        System.out.println("测试输出PTT1：" + (float)parMap.get("PTT1"));

        CLibrary.INSTANCE9.AnalysisHpaMeasure (apgII.Sex, apgII.Pd, analHPAStructsRQT.getRtops(), analHPAStructsRQT.getQstarts(),
                analHPAStructsRQT.getTends(), PCGI.getShortArray(0, 30), PCGII.getShortArray(0, 30), Rtop.getShortArray(0, 30), Ts.getShortArray(0, 30), ID,
                analHPADataStruct, "", UT, (float)parMap.get("PTT1"), (float)parMap.get("AF_Length"));

        float ppp = 0;
        if (model == 2){ //异步模式
            ppp = (float)parMap.get("AF_Length") / analHPADataStruct.PTV;
            ppp = ppp + 0.0475f;
            analHPADataStruct.PTV = (float)parMap.get("AF_Length") / ppp;
        }

        System.out.println("测试同步模式PTV之前：" + analHPADataStruct.PTV);
        if (model == 1){ //同步模式
            analHPADataStruct.PTV = (float) (analHPADataStruct.PTV * 1.1); //simple conversion from atCor for thighcuff adjustment
        }
        System.out.println("测试同步模式PTV之后：" + analHPADataStruct.PTV);

        float tempPTV = analHPADataStruct.PTV; //输出temp变量，达到能观察temp的效果
        System.out.println("输出tempPTV的值：" + tempPTV);

        if(analHPADataStruct.PTV > 2000.0f)
            analHPADataStruct.PTV = 2000.0f;
        else if (analHPADataStruct.PTV < 420.0f)
            analHPADataStruct.PTV = 420.0f;

        resMap.put("PTT", analHPADataStruct.PTT); //测试查看用的输出
        resMap.put("PTV", analHPADataStruct.PTV); //测试查看用的输出

        try {
            Apg.Cut_One_APG((float[]) parMap.get("APGS"), (short) ((float[]) parMap.get("APGS")).length, oneAPG, Rtop.getShortArray(0, 30), Ts.getShortArray(0, 30), ID, mainData);
        }catch (Exception e){
            e.printStackTrace();
            resMap.put("staCode", 500000);
            freePointer(jSnl);
            freePointer(kSnlP);
            return;
        }

//        CLibrary.INSTANCE10.Cut_One_APG((float[]) parMap.get("APGS"), (short) ((float[]) parMap.get("APGS")).length, oneAPG, Rtop.getShortArray(0, 30), Ts.getShortArray(0, 30), ID, mainData);
        /*short one_SizeRight = (short) (Rtop.getShortArray(0, 30)[ID + 1] - Rtop.getShortArray(0, 30)[ID]);
        float minV = oneAPG.getFloatArray(0,2000)[CLibrary.INSTANCE7.MinID(oneAPG.getFloatArray(0,2000), (short) 0, one_SizeRight)];
        float[] buf = new float[2000];
        for (i = 0; i < one_SizeRight; i++) {
            buf[i] = oneAPG.getFloatArray(0,2000)[i] - minV;
        }*/

        //MOD_SUB start
        short oneSize = (short) (Rtop.getShortArray(0, 30)[ID + 1] - Rtop.getShortArray(0, 30)[ID]);
        this.MOD_SUBL(oneAPG, Rtop.getShortArray(0, 30), Ts.getShortArray(0, 30), ID, d, apgI, oneSize, analHPADataStruct, apgII, resMap);

        freePointer(jSnl);
        freePointer(kSnlP);
    }

    public void publicCallR(Map resMap, Map parMap, CLibrary.ApgI.ByReference apgI,
                            BasisMeasurement basisMeasurement, int age, Pointer Rtop, Pointer Ts, Pointer PCGI, Pointer PCGII,
                            AnalHPA_Data.ByReference analHPADataStruct, CLibrary.ApgII.ByReference apgII, float[] oneAPG, CLibrary.apgD.ByReference d, short IDR, int model){
        short i;
        short dd =0;
        short ss=0;
        short RTopNum;
        float stepTime = 0.0012f;
        short ID;
        short ko;
        short[]	UT = new short[30];

        //RTopNum = CLibrary.INSTANCE2.get_R_tops((float[]) parMap.get("ECGS"), (short) parMap.get("apgSampNum"), utopP, qpointP, tpointP);

        Pointer jSnl = new Memory((short)parMap.get("apgSampNum") * 4);
        Pointer kSnlP = new Memory((short)parMap.get("apgSampNum") * 4);
        CLibrary.AnalHPA.ByReference analHPAStructsRQT = new CLibrary.AnalHPA.ByReference();
        AnalHPA_Data.ByReference analHPADataStructRQT = new AnalHPA_Data.ByReference();
        CLibrary.INSTANCE2.filter((float[]) parMap.get("ECGS"), jSnl, (short)parMap.get("apgSampNum"), (short) 25);
        CLibrary.INSTANCE2.filter_mibun(jSnl.getFloatArray(0, (short)parMap.get("apgSampNum")), kSnlP, (short) parMap.get("apgSampNum"), (short) 25);
        CLibrary.INSTANCE2.filter_mibun(kSnlP.getFloatArray(0, (short)parMap.get("apgSampNum")), jSnl, (short)parMap.get("apgSampNum"), (short) 25);
        CLibrary.INSTANCE7.RQT_Analysis(jSnl.getFloatArray(0, (short)parMap.get("apgSampNum")), kSnlP.getFloatArray(0, (short)parMap.get("apgSampNum")), (short)parMap.get("apgSampNum"), analHPAStructsRQT, analHPADataStructRQT);
        RTopNum = analHPAStructsRQT.getRtopNum();

        if(RTopNum <= 2){
            resMap.put("staCode", 515);
            freePointer(jSnl);
            freePointer(kSnlP);
            return;
        }

        if (RTopNum > 2) {
            i = autoSelectCycleR(apgI, basisMeasurement, age, (float[]) parMap.get("ECGS"), (float[]) parMap.get("PCGS"), (float[]) parMap.get("APGS"), Rtop, Ts, PCGI, PCGII, (short) parMap.get("apgSampNum"), stepTime, model, resMap);
            if ( i == 0 ) {
                resMap.put("staCode", 516);
                freePointer(jSnl);
                freePointer(kSnlP);
                return;
            }

            for (i = 0; i < RTopNum; i++) {
                dd = 0; ss = 0;
                dd = Ts.getShortArray(0, 30)[i]; ss = Rtop.getShortArray(0, 30)[i];
                UT[i] = CLibrary.INSTANCE6.DetectAbsMax((float[]) parMap.get("APGS"), ss, dd); //todo 这里与源码有差别，第一个参数是float数组，源码中是float类型的数字；
            }

            resMap.put("TsR", Ts.getShortArray(0, 30));
            resMap.put("RtopR", Rtop.getShortArray(0, 30));
            resMap.put("UTR", UT);
            resMap.put("RtopNumR", RTopNum);

            if(IDR != -1){
                ID = IDR;
            }else{
                ID = apgI.No; //可能需要存DB；
            }
            resMap.put("IDR", ID);

        }else{
            resMap.put("staCode", 517);
            freePointer(jSnl);
            freePointer(kSnlP);
            return;
        }
        ko = 0; //todo ko=0是手动选择波形的控制器，逻辑需要修改
        //GetCycleInfo end
        try {
            Apg.Cut_One_APGR((float[]) parMap.get("APGS"), (short) ((float[]) parMap.get("APGS")).length, oneAPG, Rtop.getShortArray(0, 30), Ts.getShortArray(0, 30), ID, mainDataR);
        }catch (Exception e){
            e.printStackTrace();
            resMap.put("staCode", 500001);
            freePointer(jSnl);
            freePointer(kSnlP);
            return;
        }

//        CLibrary.INSTANCE10.Cut_One_APG((float[]) parMap.get("APGS"), (short) ((float[]) parMap.get("APGS")).length, oneAPG, Rtop.getShortArray(0, 30), Ts.getShortArray(0, 30), ID, mainDataR);
        /*short one_SizeRight = (short) (Rtop.getShortArray(0, 30)[ID + 1] - Rtop.getShortArray(0, 30)[ID]);
        float minV = oneAPG.getFloatArray(0,2000)[CLibrary.INSTANCE7.MinID(oneAPG.getFloatArray(0,2000), (short) 0, one_SizeRight)];
        float[] buf = new float[2000];
        for (i = 0; i < one_SizeRight; i++) {
            buf[i] = oneAPG.getFloatArray(0,2000)[i] - minV;
        }*/

        //MOD_SUB start
        short oneSize = (short) (Rtop.getShortArray(0, 30)[ID + 1] - Rtop.getShortArray(0, 30)[ID]);
        this.MOD_SUBR(oneAPG, Rtop.getShortArray(0, 30), Ts.getShortArray(0, 30), ID, d, apgI, oneSize, analHPADataStruct, apgII, resMap);

        freePointer(jSnl);
        freePointer(kSnlP);
    }

    public void freePointer(Pointer p){
        long peer = Pointer.nativeValue(p);
        Native.free(peer);//手动释放内存
        Pointer.nativeValue(p, 0);//避免Memory对象被GC时重复执行Nativ.free()方法
    }

    public boolean ReadSignal(OriginalWaveformReq originalWaveform, float[]	ECGS, float[] APGS, Map parMap, short no){ //这个方法暂时没有使用
        short[] s = new short[5000];
        short apgSampNum;
        if (no == 0) {
            //获取左颈动脉心电波
            int[] ecgPL = originalWaveform.getEcgPl();
            apgSampNum = (short) ecgPL.length;

            //将其转为short数组
            for (int i = 0; i < apgSampNum; i++) {
                s[i] = (short) ecgPL[i];
            }

            Pointer ecgsP = new Memory(apgSampNum * 4);
//            ecgsP.write(0, ECGS, 0, apgSampNum);
            CLibrary.INSTANCE3.Signal_Move(s, ecgsP, apgSampNum);
            ECGS = ecgsP.getFloatArray(0, apgSampNum);

            //获取左颈动脉脉搏波
            int[] apgL = originalWaveform.getApgL();
            apgSampNum = (short) apgL.length;

            //将其转为short数
            for (int i = 0; i < apgSampNum; i++) {
                s[i] = (short) apgL[i];
            }

            Pointer apgsP = new Memory(apgSampNum * 4);
//            ecgsP.write(0, APGS, 0, apgSampNum);
            CLibrary.INSTANCE3.Signal_Move(s, apgsP, apgSampNum);
            APGS = apgsP.getFloatArray(0, apgSampNum);

            if(apgSampNum == 0){
                freePointer(ecgsP);
                freePointer(apgsP);
                return false;
            }

            parMap.put("apgSampNum", apgSampNum);
            parMap.put("ECGS", ECGS);
            parMap.put("APGS", APGS);

            freePointer(ecgsP);
            freePointer(apgsP);
        }
        if(no == 1){
            //获取右颈动脉心电波
            int[] ecgPr = originalWaveform.getEcgPr();
            apgSampNum = (short) ecgPr.length;

            //将其转为short数组
            for (int i = 0; i < apgSampNum; i++) {
                s[i] = (short) ecgPr[i];
            }

            Pointer ecgsP = new Memory(apgSampNum * 4);
//            ecgsP.write(0, ECGS, 0, apgSampNum);
            CLibrary.INSTANCE3.Signal_Move(s, ecgsP, apgSampNum);
            ECGS = ecgsP.getFloatArray(0, apgSampNum);

            //获取右颈动脉脉搏波
            int[] apgR = originalWaveform.getApgR();
            apgSampNum = (short) apgR.length;

            //将其转为short数组
            for (int i = 0; i < apgSampNum; i++) {
                s[i] = (short) apgR[i];
            }

            Pointer apgsP = new Memory(apgSampNum * 4);
//            ecgsP.write(0, APGS, 0, apgSampNum);
            CLibrary.INSTANCE3.Signal_Move(s, apgsP, apgSampNum);
            APGS = apgsP.getFloatArray(0, apgSampNum);

            if(apgSampNum == 0){
                freePointer(ecgsP);
                freePointer(apgsP);
                return false;
            }

            parMap.put("apgSampNum", apgSampNum);
            parMap.put("ECGS", ECGS);
            parMap.put("APGS", APGS);

            freePointer(ecgsP);
            freePointer(apgsP);
        }

        return true;
    }

    /*public short autoSelectCycleL(CLibrary.ApgI.ByReference p, BasisMeasurement basisMeasurement, int age, float[] ecg , float[] pcg, float[] apg, Pointer startp, Pointer Ts, Pointer PCGI, Pointer PCGII, short num, float stepTime){
        short i;
        short pulseNumber;
        short chNumber;
        short sigCount;
        short[] RR = new short[20];
        short[] Iso = new short[20];
        short[] IIso = new short[20];
        short[] tr = new short[20];
        short[] qpoint = new short[20];
        short[] tpoint = new short[20];
        float[] kSnl = new float[num];
        float[] jSnl = new float[num];

        for(i=0; i<20; i++)
        {
            RR[i] = 0;
            Iso[i] = 0;
            IIso[0] = 0;
            tr[i] = 0;
            tpoint[i] = 0;
        }
        mainData.Age = (short) age;
        mainData.Ps = basisMeasurement.getPs();
        mainData.Pd = basisMeasurement.getPd();

        Pointer jSnlAUL = new Memory(num * 4);
        Pointer kSnlPAUL = new Memory(num * 4);
        CLibrary.AnalHPA.ByReference analHPAStructsRQT = new CLibrary.AnalHPA.ByReference();
        AnalHPA_Data.ByReference analHPADataStructRQT = new AnalHPA_Data.ByReference();
        CLibrary.INSTANCE2.filter(ecg, jSnlAUL, num, (short) 25);
        CLibrary.INSTANCE2.filter_mibun(jSnlAUL.getFloatArray(0, num), kSnlPAUL, num, (short) 25);
        CLibrary.INSTANCE2.filter_mibun(kSnlPAUL.getFloatArray(0, num), jSnlAUL, num, (short) 25);
        CLibrary.INSTANCE7.RQT_Analysis(jSnlAUL.getFloatArray(0, num), kSnlPAUL.getFloatArray(0, num), num, analHPAStructsRQT, analHPADataStructRQT);
        pulseNumber = analHPAStructsRQT.getRtopNum();

        sigCount = (short) (num - (short) 1);
        for(i=0; i<=pulseNumber; i++)
                RR[i] = analHPAStructsRQT.getRtops()[i];

        if ( (pulseNumber < 2) || (pulseNumber > 18) ){
            freePointer(jSnlAUL);
            freePointer(kSnlPAUL);
            return 0;
        }
        Pointer kSnlP = new Memory((num + 1) * 4);
//        kSnlP.write(0, kSnl, 0, num);
        CLibrary.INSTANCE3.filter(apg, kSnlP, num, (short) 25);
        Pointer jSnlP = new Memory((num + 1) * 4);
//        jSnlP.write(0, jSnl, 0, num);
        CLibrary.INSTANCE3.filter_mibun( kSnlP.getFloatArray(0, num + 1), jSnlP, num, (short) 25 );
        CLibrary.INSTANCE3.filter_mibun( jSnlP.getFloatArray(0, num + 1), kSnlP, num, (short) 25 );

        //todo 用真实数据测试AbI_IIsoPointGet方法注释掉
        pulseNumber = CLibrary.INSTANCE10.AbI_IIsoPointGet (pcg, sigCount, RR, pulseNumber, PCGI, PCGII);
        pulseNumber = CLibrary.INSTANCE10.AbApgStTsGetonPcg (kSnlP.getFloatArray(0, num + 1), sigCount, RR, analHPAStructsRQT.getTends(), pulseNumber, startp, Ts);
        chNumber = CLibrary.INSTANCE10.AbOneGetbyAuto (startp.getShortArray(0, 30), pulseNumber);

        p.setNo((short)chNumber);
        p.setTsnum((short)pulseNumber);
        p.setT((float)( (startp.getShortArray(0, 30)[chNumber+1] - startp.getShortArray(0, 30)[chNumber]) * stepTime ));
        p.setPulse((float)( 60.0f / p.T ));
        p.setTs((float)((Ts.getShortArray(0, 30)[chNumber] - startp.getShortArray(0, 30)[chNumber]) * stepTime));

        freePointer(jSnlAUL);
        freePointer(kSnlPAUL);
        freePointer(kSnlP);
        freePointer(jSnlP);
        return 1;
    }

    public short autoSelectCycleR(CLibrary.ApgI.ByReference p, BasisMeasurement basisMeasurement, int age, float[] ecg , float[] pcg, float[] apg, Pointer startp, Pointer Ts, Pointer PCGI, Pointer PCGII, short num, float stepTime, int model){
        short i;
        short pulseNumber;
        short chNumber;
        short sigCount;
        short[] RR = new short[20];
        short[] Iso = new short[20];
        short[] IIso = new short[20];
        short[] tr = new short[20];
        short[] qpoint = new short[20];
        short[] tpoint = new short[20];
        float[] kSnl = new float[num];
        float[] jSnl = new float[num];

        for(i=0; i<20; i++)
        {
            RR[i] = 0;
            Iso[i] = 0;
            IIso[0] = 0;
            tr[i] = 0;
            tpoint[i] = 0;
        }
        mainDataR.Age = (short) age;
        mainDataR.Ps = basisMeasurement.getPs();
        mainDataR.Pd = basisMeasurement.getPd();

        Pointer jSnlAUR = new Memory(num * 4);
        Pointer kSnlPAUR = new Memory(num * 4);
        CLibrary.AnalHPA.ByReference analHPAStructsRQT = new CLibrary.AnalHPA.ByReference();
        AnalHPA_Data.ByReference analHPADataStructRQT = new AnalHPA_Data.ByReference();
        CLibrary.INSTANCE2.filter(ecg, jSnlAUR, num, (short) 25);
        CLibrary.INSTANCE2.filter_mibun(jSnlAUR.getFloatArray(0, num), kSnlPAUR, num, (short) 25);
        CLibrary.INSTANCE2.filter_mibun(kSnlPAUR.getFloatArray(0, num), jSnlAUR, num, (short) 25);
        CLibrary.INSTANCE7.RQT_Analysis(jSnlAUR.getFloatArray(0, num), kSnlPAUR.getFloatArray(0, num), num, analHPAStructsRQT, analHPADataStructRQT);
        pulseNumber = analHPAStructsRQT.getRtopNum();

        sigCount = (short) (num - (short) 1);
        for(i=0; i<=pulseNumber; i++)
            RR[i] = analHPAStructsRQT.getRtops()[i];

        if ( (pulseNumber < 2) || (pulseNumber > 18) ){
            freePointer(jSnlAUR);
            freePointer(kSnlPAUR);
            return 0;
        }
        Pointer kSnlP = new Memory((num + 1) * 4);
//        kSnlP.write(0, kSnl, 0, num);
        CLibrary.INSTANCE3.filter(apg, kSnlP, num, (short) 25);
        Pointer jSnlP = new Memory((num + 1) * 4);
//        jSnlP.write(0, jSnl, 0, num);
        CLibrary.INSTANCE3.filter_mibun( kSnlP.getFloatArray(0, num + 1), jSnlP, num, (short) 25 );
        CLibrary.INSTANCE3.filter_mibun( jSnlP.getFloatArray(0, num + 1), kSnlP, num, (short) 25 );

        if(model == 2){ //异步模式右 调用一次，同步模式右 不调用
            pulseNumber = CLibrary.INSTANCE10.AbI_IIsoPointGet (pcg, sigCount, RR, pulseNumber, PCGI, PCGII);
        }

        pulseNumber = CLibrary.INSTANCE10.AbApgStTsGetonPcg (kSnlP.getFloatArray(0, num + 1), sigCount, RR, analHPAStructsRQT.getTends(), pulseNumber, startp, Ts);
        chNumber = CLibrary.INSTANCE10.AbOneGetbyAuto (startp.getShortArray(0, 30), pulseNumber);

        p.setNo((short)chNumber);
        p.setTsnum((short)pulseNumber);
        p.setT((float)( (startp.getShortArray(0, 30)[chNumber+1] - startp.getShortArray(0, 30)[chNumber]) * stepTime ));
        p.setPulse((float)( 60.0f / p.T ));
        p.setTs((float)((Ts.getShortArray(0, 30)[chNumber] - startp.getShortArray(0, 30)[chNumber]) * stepTime));

        freePointer(jSnlAUR);
        freePointer(kSnlPAUR);
        freePointer(kSnlP);
        freePointer(jSnlP);
        return 1;
    }*/

    public short autoSelectCycleL(CLibrary.ApgI.ByReference p, BasisMeasurement basisMeasurement, int age, float[] ecg , float[] pcg, float[] apg, Pointer startp, Pointer Ts, Pointer PCGI, Pointer PCGII, short num, float stepTime){
        short i;
        short pulseNumber;
        short chNumber;
        short sigCount;
        short[] RR = new short[20];
        short[] Iso = new short[20];
        short[] IIso = new short[20];
        short[] tr = new short[20];
        short[] qpoint = new short[20];
        short[] tpoint = new short[20];
        float[] kSnl = new float[num];
        float[] jSnl = new float[num];

        for(i=0; i<20; i++)
        {
            RR[i] = 0;
            Iso[i] = 0;
            IIso[0] = 0;
            tr[i] = 0;
            tpoint[i] = 0;
        }
        mainData.Age = (short) age;
        mainData.Ps = basisMeasurement.getPs();
        mainData.Pd = basisMeasurement.getPd();

        Pointer jSnlAUL = new Memory(num * 4);
        Pointer kSnlPAUL = new Memory(num * 4);
        CLibrary.AnalHPA.ByReference analHPAStructsRQT = new CLibrary.AnalHPA.ByReference();
        AnalHPA_Data.ByReference analHPADataStructRQT = new AnalHPA_Data.ByReference();
        CLibrary.INSTANCE2.filter(ecg, jSnlAUL, num, (short) 25);
        CLibrary.INSTANCE2.filter_mibun(jSnlAUL.getFloatArray(0, num), kSnlPAUL, num, (short) 25);
        CLibrary.INSTANCE2.filter_mibun(kSnlPAUL.getFloatArray(0, num), jSnlAUL, num, (short) 25);
        CLibrary.INSTANCE7.RQT_Analysis(jSnlAUL.getFloatArray(0, num), kSnlPAUL.getFloatArray(0, num), num, analHPAStructsRQT, analHPADataStructRQT);
        pulseNumber = analHPAStructsRQT.getRtopNum();

        sigCount = (short) (num - (short) 1);
        for(i=0; i<=pulseNumber; i++)
            RR[i] = analHPAStructsRQT.getRtops()[i];

        if ( (pulseNumber < 2) || (pulseNumber > 18) ){
            freePointer(jSnlAUL);
            freePointer(kSnlPAUL);
            return 0;
        }
        Pointer kSnlP = new Memory((num + 1) * 4);
//        kSnlP.write(0, kSnl, 0, num);
        CLibrary.INSTANCE3.filter(apg, kSnlP, num, (short) 25);
        Pointer jSnlP = new Memory((num + 1) * 4);
//        jSnlP.write(0, jSnl, 0, num);
        CLibrary.INSTANCE3.filter_mibun( kSnlP.getFloatArray(0, num + 1), jSnlP, num, (short) 25 );
        CLibrary.INSTANCE3.filter_mibun( jSnlP.getFloatArray(0, num + 1), kSnlP, num, (short) 25 );

        //todo 用真实数据测试AbI_IIsoPointGet方法注释掉
        pulseNumber = CLibrary.INSTANCE10.AbI_IIsoPointGet (pcg, sigCount, RR, pulseNumber, PCGI, PCGII);
        pulseNumber = CLibrary.INSTANCE10.AbApgStTsGetonPcg (kSnlP.getFloatArray(0, num + 1), sigCount, RR, analHPAStructsRQT.getTends(), pulseNumber, startp, Ts);

        short[] ut = new short[pulseNumber];
        for (i = 0; i < pulseNumber; i++) {
            short dd = 0; short ss = 0;
            dd = Ts.getShortArray(0, 30)[i]; ss = startp.getShortArray(0, 30)[i];
            short maxPos = ss;
            float maxValue = (float)(Math.abs(apg[ss]));

            for (short idx = ss; idx < dd; idx++)
            {
                if (maxValue < Math.abs((apg[idx])))
                {
                    maxValue = (float)(Math.abs((apg[idx])));
                    maxPos = idx;
                }
            }
            ut[i] = maxPos;
        }

//        chNumber = CLibrary.INSTANCE10.AbOneGetbyAuto (startp.getShortArray(0, 30), Ts.getShortArray(0, 30), ut, pulseNumber);
        chNumber = Apg.AbOneGetbyAuto (startp.getShortArray(0, 30), Ts.getShortArray(0, 30), ut, pulseNumber);

        float[] sevrdenom = new float[pulseNumber - 2];
        float[] sevrnum = new float[pulseNumber - 1];
        float[] sevr = new float[pulseNumber - 2];
        for (i = 0; i < pulseNumber - 1; i++) {
            for (short n = Ts.getShortArray(0, 30)[i]; n < startp.getShortArray(0, 30)[i + 1]; n++) {
                sevrnum[i] = sevrnum[i] + apg[n]-startp.getShortArray(0, 30)[i];
            }
        }
        for (i = 0; i < pulseNumber - 2; i++) {
            for (short n = startp.getShortArray(0, 30)[i]; n < Ts.getShortArray(0, 30)[i]; n++) {
                sevrdenom[i] = sevrdenom[i] + apg[n]-startp.getShortArray(0, 30)[i];
            }
        }
        for (i = 0; i < pulseNumber - 2; i++) {
            sevr[i] = sevrnum[i] / sevrdenom[i];
        }
        short ch_number2 = Apg.AbMaxVPointGet(sevr, (short) (pulseNumber - 1 - 1 - 1), (short) 0);
        if (chNumber == ch_number2) {
            chNumber = ch_number2;
        }

        p.setNo((short)chNumber);
        p.setTsnum((short)pulseNumber);
        p.setT((float)( (startp.getShortArray(0, 30)[chNumber+1] - startp.getShortArray(0, 30)[chNumber]) * stepTime ));
        p.setPulse((float)( 60.0f / p.T ));
        p.setTs((float)((Ts.getShortArray(0, 30)[chNumber] - startp.getShortArray(0, 30)[chNumber]) * stepTime));

        freePointer(jSnlAUL);
        freePointer(kSnlPAUL);
        freePointer(kSnlP);
        freePointer(jSnlP);
        return 1;
    }

    public short autoSelectCycleR(CLibrary.ApgI.ByReference p, BasisMeasurement basisMeasurement, int age, float[] ecg , float[] pcg, float[] apg, Pointer startp, Pointer Ts, Pointer PCGI, Pointer PCGII, short num, float stepTime, int model, Map resMap){
        short i;
        short pulseNumber;
        short chNumber;
        short sigCount;
        short[] RR = new short[20];
        short[] Iso = new short[20];
        short[] IIso = new short[20];
        short[] tr = new short[20];
        short[] qpoint = new short[20];
        short[] tpoint = new short[20];
        float[] kSnl = new float[num];
        float[] jSnl = new float[num];

        for(i=0; i<20; i++)
        {
            RR[i] = 0;
            Iso[i] = 0;
            IIso[0] = 0;
            tr[i] = 0;
            tpoint[i] = 0;
        }
        mainDataR.Age = (short) age;
        mainDataR.Ps = basisMeasurement.getPs();
        mainDataR.Pd = basisMeasurement.getPd();

        Pointer jSnlAUR = new Memory(num * 4);
        Pointer kSnlPAUR = new Memory(num * 4);
        CLibrary.AnalHPA.ByReference analHPAStructsRQT = new CLibrary.AnalHPA.ByReference();
        AnalHPA_Data.ByReference analHPADataStructRQT = new AnalHPA_Data.ByReference();
        CLibrary.INSTANCE2.filter(ecg, jSnlAUR, num, (short) 25);
        CLibrary.INSTANCE2.filter_mibun(jSnlAUR.getFloatArray(0, num), kSnlPAUR, num, (short) 25);
        CLibrary.INSTANCE2.filter_mibun(kSnlPAUR.getFloatArray(0, num), jSnlAUR, num, (short) 25);
        CLibrary.INSTANCE7.RQT_Analysis(jSnlAUR.getFloatArray(0, num), kSnlPAUR.getFloatArray(0, num), num, analHPAStructsRQT, analHPADataStructRQT);
        pulseNumber = analHPAStructsRQT.getRtopNum();

        sigCount = (short) (num - (short) 1);
        for(i=0; i<=pulseNumber; i++)
            RR[i] = analHPAStructsRQT.getRtops()[i];

        if ( (pulseNumber < 2) || (pulseNumber > 18) ){
            freePointer(jSnlAUR);
            freePointer(kSnlPAUR);
            return 0;
        }
        Pointer kSnlP = new Memory((num + 1) * 4);
//        kSnlP.write(0, kSnl, 0, num);
        CLibrary.INSTANCE3.filter(apg, kSnlP, num, (short) 25);
        Pointer jSnlP = new Memory((num + 1) * 4);
//        jSnlP.write(0, jSnl, 0, num);
        CLibrary.INSTANCE3.filter_mibun( kSnlP.getFloatArray(0, num + 1), jSnlP, num, (short) 25 );
        CLibrary.INSTANCE3.filter_mibun( jSnlP.getFloatArray(0, num + 1), kSnlP, num, (short) 25 );

        if(model == 2){ //异步模式右 调用一次，同步模式右 不调用
            pulseNumber = CLibrary.INSTANCE10.AbI_IIsoPointGet (pcg, sigCount, RR, pulseNumber, PCGI, PCGII);
        }

        pulseNumber = CLibrary.INSTANCE10.AbApgStTsGetonPcg (kSnlP.getFloatArray(0, num + 1), sigCount, RR, analHPAStructsRQT.getTends(), pulseNumber, startp, Ts);
        resMap.put("analHPAStructsRQTRtops", analHPAStructsRQT.Rtops);

        short[] ut = new short[pulseNumber];
        for (i = 0; i < pulseNumber; i++) {
            short dd = 0; short ss = 0;
            dd = Ts.getShortArray(0, 30)[i]; ss = startp.getShortArray(0, 30)[i];
            short maxPos = ss;
            float maxValue = (float)(Math.abs(apg[ss]));

            for (short idx = ss; idx < dd; idx++)
            {
                if (maxValue < Math.abs((apg[idx])))
                {
                    maxValue = (float)(Math.abs((apg[idx])));
                    maxPos = idx;
                }
            }
            ut[i] = maxPos;
        }

//        chNumber = CLibrary.INSTANCE10.AbOneGetbyAuto (startp.getShortArray(0, 30), Ts.getShortArray(0, 30), ut, pulseNumber);
        chNumber = Apg.AbOneGetbyAuto (startp.getShortArray(0, 30), Ts.getShortArray(0, 30), ut, pulseNumber);

        float[] sevrdenom = new float[pulseNumber - 2];
        float[] sevrnum = new float[pulseNumber - 1];
        float[] sevr = new float[pulseNumber - 2];
        for (i = 0; i < pulseNumber - 1; i++) {
            for (short n = Ts.getShortArray(0, 30)[i]; n < startp.getShortArray(0, 30)[i + 1]; n++) {
                sevrnum[i] = sevrnum[i] + apg[n]-startp.getShortArray(0, 30)[i];
            }
        }
        for (i = 0; i < pulseNumber - 2; i++) {
            for (short n = startp.getShortArray(0, 30)[i]; n < Ts.getShortArray(0, 30)[i]; n++) {
                sevrdenom[i] = sevrdenom[i] + apg[n]-startp.getShortArray(0, 30)[i];
            }
        }
        for (i = 0; i < pulseNumber - 2; i++) {
            sevr[i] = sevrnum[i] / sevrdenom[i];
        }
        short ch_number2 = Apg.AbMaxVPointGet(sevr, (short) (pulseNumber - 1 - 1 - 1), (short) 0);
        if (chNumber == ch_number2) {
            chNumber = ch_number2;
        }

        p.setNo((short)chNumber);
        p.setTsnum((short)pulseNumber);
        p.setT((float)( (startp.getShortArray(0, 30)[chNumber+1] - startp.getShortArray(0, 30)[chNumber]) * stepTime ));
        p.setPulse((float)( 60.0f / p.T ));
        p.setTs((float)((Ts.getShortArray(0, 30)[chNumber] - startp.getShortArray(0, 30)[chNumber]) * stepTime));

        freePointer(jSnlAUR);
        freePointer(kSnlPAUR);
        freePointer(kSnlP);
        freePointer(jSnlP);
        return 1;
    }

    public boolean PTV_Dectect(OriginalWaveformReq originalWaveform, AnalHPA_Data.ByReference analHPADataStruct, CLibrary.AnalHPA.ByReference analHPAStruct, Map parMap, Map resMap, int model){
        short i;
        short ss;
        short dd;
        short rst1;
        float maxNum;
        long rst;
        float PTT1;

        //获取股动脉心电波
        int[] sSnl = originalWaveform.getEcgTh();
        short sampleNum = (short) sSnl.length;
        float[] kSnl = new float[sampleNum];

        for (i = 0; i < sampleNum; i++) {
            kSnl[i] = (float)sSnl[i];
        }
        maxNum = CLibrary.INSTANCE6.DetectMaxValue(kSnl, (short) 0, sampleNum);
//        if (maxNum < 0.2f) {
//            return false;
//        }

        Pointer jSnl = new Memory(sampleNum * 4);
        CLibrary.INSTANCE6.Filter(kSnl, jSnl, sampleNum, (short) 25);
        Pointer kSnlPp = new Memory(sampleNum * 4);
        CLibrary.INSTANCE6.FilterMibun(jSnl.getFloatArray(0, sampleNum), kSnlPp, sampleNum, (short) 25);
        Pointer kSnlP = new Memory(sampleNum * 4);
        CLibrary.INSTANCE6.FilterMibun(kSnlPp.getFloatArray(0, sampleNum), jSnl, sampleNum, (short) 25);

        rst1 = CLibrary.INSTANCE7.RQT_Analysis(jSnl.getFloatArray(0, sampleNum), kSnlPp.getFloatArray(0, sampleNum), sampleNum, analHPAStruct, analHPADataStruct);
        if (rst1 == 0) {
            freePointer(jSnl);
            freePointer(kSnlP);
            return false;
        }
        analHPAStruct.R_top = analHPAStruct.Rtops[analHPAStruct.Rpos];

        //获取股动脉脉搏波
        int[] sSnla = originalWaveform.getApgTh();
        short sampleNuma = (short) sSnla.length;
        float[] kSnla = new float[sampleNuma];

        for (i = 0; i < sampleNuma; i++) {
            kSnla[i] = (float)sSnla[i];
        }
        maxNum = CLibrary.INSTANCE6.DetectMaxValue(kSnla, (short) 0, sampleNuma);
//        if (maxNum < 0.2f) {
//            freePointer(jSnl);
//            freePointer(kSnlP);
//            return false;
//        }

        Pointer jSnla = new Memory(sampleNuma * 4);
        CLibrary.INSTANCE6.FilterMibun(kSnla, jSnla, sampleNuma, (short) 25);
        Pointer kSnlaP = new Memory(sampleNuma * 4);
        CLibrary.INSTANCE6.FilterMibun(jSnla.getFloatArray(0, sampleNuma), kSnlaP, sampleNuma, (short) 25);

        for (i = 0; i <= analHPAStruct.RtopNum; i++) {
            dd = 0; ss = 0; dd = analHPAStruct.Rtops[i];
            ss = (short) (analHPAStruct.Rtops[i] + (short) 320);
            analHPAStruct.USpnts[i] = CLibrary.INSTANCE7.DetectUSpoint(kSnlaP.getFloatArray(0, sampleNuma), dd, ss, i, sampleNuma, analHPAStruct);
        }
//        analHPAStruct.US_point = analHPAStruct.USpnts[analHPAStruct.Rpos];

        if (model == 1){ //同步模式
            analHPAStruct.US_point = analHPAStruct.USpnts[(short) resMap.get("IDR")];

            System.out.println("同步模式输出PTV_Dectect中analHPAStruct.US_point：" + analHPAStruct.US_point);

//        PTT1 = Math.abs((analHPAStruct.US_point - analHPAStruct.R_top) * 0.0012f);
            PTT1 = Math.abs((analHPAStruct.US_point - ((short[]) resMap.get("analHPAStructsRQTRtops"))[(short) resMap.get("IDR")]) * 0.0012f);
        }else{
            analHPAStruct.US_point = analHPAStruct.USpnts[(short) resMap.get("IDL")];

            System.out.println("异步模式输出PTV_Dectect中analHPAStruct.US_point：" + analHPAStruct.US_point);

//        PTT1 = Math.abs((analHPAStruct.US_point - analHPAStruct.R_top) * 0.0012f);
            PTT1 = Math.abs((analHPAStruct.US_point - analHPAStruct.Rtops[(short) resMap.get("IDL")]) * 0.0012f);
        }

//        PTT1 = (analHPAStruct.US_point - analHPAStruct.R_top) * 0.0012f;
        parMap.put("PTT1", PTT1);

        freePointer(jSnl);
        freePointer(kSnlP);
        freePointer(jSnla);
        freePointer(kSnlaP);

        if ( PTT1 <= 0.0f){
            return false;
        } else{
//            parMap.put("AF_Length", ((float) parMap.get("Femoral_Length") - (float) parMap.get("Apg_Length")) + 3);
            return true;
        }
    }

    public boolean MOD_SUBL(float[] oneAPG, short[] Rtop, short[] Ts, int ID, CLibrary.apgD.ByReference d, CLibrary.ApgI.ByReference p, short oneSize, AnalHPA_Data.ByReference analHPADataStruct, CLibrary.ApgII.ByReference r, Map resMap){
        short i;
        float jAd;
        float jAs;
        float jPss;
        float jT;
        float jTs;
        i = this.APG_ModelL(r, d, oneAPG, oneSize, (short)(Ts[ID] - Rtop[ID]), analHPADataStruct);
        jAd = d.Ad;
        jAs = d.As;
        jPss = d.Pss;
        jT = p.T;
        jTs = p.Ts;

        resMap.put("jAdL", jAd);
        resMap.put("jAsL", jAs);
        resMap.put("jPssL", jPss);
        resMap.put("jTL", jT);
        resMap.put("jTsL", jTs);

        if (i == 1) {
            return true;
        } else {
            return false;
        }
    }

    public short APG_ModelL(CLibrary.ApgII.ByReference r, CLibrary.apgD.ByReference d, float[] apg, short size, short Ts, AnalHPA_Data.ByReference analHPADataStruct){
        short WGht = r.Weight;
        short HGht = r.Height;
        String fileName = "";
        try {
            //todo linux获取路径
            File directory = new File("");// 参数为空
            fileName = directory.getCanonicalPath();
        }catch (Exception e){
            e.printStackTrace();
        }

        String s = fileName + "/WK-STD.RYU";
        short retl = CLibrary.INSTANCE8.load_std_data(s);
        if(retl == 0) {  return 0;}

        mainData.Pwv1 = analHPADataStruct.PTV;
        short ret = CLibrary.INSTANCE8.OneOrderModel(apg, size, Ts, WGht, HGht, mainData); //todo 这里与源码有异议，源码中没有mainData这个参数
        if(ret == 0) { return 0;}

        (d.C0) = (float)mainData.C0;
        (d.R0) = (float)mainData.R0;
        (d.Cd) = (float)mainData.Cd;
        (d.Ad) = (float)mainData.Ad;
        (d.As) = (float)mainData.As;
        (d.T)  = (float)mainData.T;
        (d.Pss) = (float)mainData.Pss;

        return 1;
    }

    public boolean MOD_SUBR(float[] oneAPG, short[] Rtop, short[] Ts, int ID, CLibrary.apgD.ByReference d, CLibrary.ApgI.ByReference p, short oneSize, AnalHPA_Data.ByReference analHPADataStruct, CLibrary.ApgII.ByReference r, Map resMap){
        short i;
        float jAd;
        float jAs;
        float jPss;
        float jT;
        float jTs;
        i = this.APG_ModelR(r, d, oneAPG, oneSize, (short)(Ts[ID] - Rtop[ID]), analHPADataStruct);
        jAd = d.Ad;
        jAs = d.As;
        jPss = d.Pss;
        jT = p.T;
        jTs = p.Ts;

        resMap.put("jAdR", jAd);
        resMap.put("jAsR", jAs);
        resMap.put("jPssR", jPss);
        resMap.put("jTR", jT);
        resMap.put("jTsR", jTs);

        if (i == 1) {
            return true;
        } else {
            return false;
        }
    }

    public short APG_ModelR(CLibrary.ApgII.ByReference r, CLibrary.apgD.ByReference d, float[] apg, short size, short Ts, AnalHPA_Data.ByReference analHPADataStruct){
        short WGht = r.Weight;
        short HGht = r.Height;
        String fileName = "";
        try {
            //todo linux获取路径
            File directory = new File("");// 参数为空
            fileName = directory.getCanonicalPath();
        }catch (Exception e){
            e.printStackTrace();
        }

        String s = fileName + "/WK-STD.RYU";
        short retl = CLibrary.INSTANCE8.load_std_data(s);
        if(retl == 0) {  return 0;}

        mainDataR.Pwv1 = analHPADataStruct.PTV;
        short ret = CLibrary.INSTANCE8.OneOrderModel(apg, size, Ts, WGht, HGht, mainDataR); //todo 这里与源码有异议，源码中没有mainData这个参数
        if(ret == 0) { return 0;}

        (d.C0) = (float)mainDataR.C0;
        (d.R0) = (float)mainDataR.R0;
        (d.Cd) = (float)mainDataR.Cd;
        (d.Ad) = (float)mainDataR.Ad;
        (d.As) = (float)mainDataR.As;
        (d.T)  = (float)mainDataR.T;
        (d.Pss) = (float)mainDataR.Pss;

        return 1;
    }

    public int Paint_CardioVascular(CLibrary.CG.ByReference CG, Map resMap){
//        short nRet;
        short i;
        float snMax, snMin;
        snMax = 0.0f;
        snMin = 10000.0f;

        /*try {
            //todo linux获取路径
            File directory = new File("");// 参数为空
            strHgaName = directory.getCanonicalPath();
        }catch (Exception e){
            e.printStackTrace();
        }

        String s = strHgaName + "/";

        Pointer SJ_snl1 = new Memory(2000 * 4);
        nRet = CLibrary.INSTANCE7.signal_load(SJ_snl1, (short) 2000, strHgaName);//uncommented out by NC on 202-7-22, should take value from Pga_snl(synthetic calculation from CUFF)
        if (nRet == 0) {
            return 0;
        }*/

        //System.out.println("测试PgaSnl数据：" + Arrays.toString(globalClass.getPgaSnl()) + "元素个数：" + globalClass.getPgaSnl().length);

        for (i = 0; i < 2000; i++) {
            if (snMax < globalClass.getPgaSnl()[i]) snMax = globalClass.getPgaSnl()[i];
            if (snMin > globalClass.getPgaSnl()[i]) snMin = globalClass.getPgaSnl()[i];
        }

        float[] buf = new float[2000];

        System.out.println("snMin:" + snMin);
        for (i = 0; i < 2000; i++) {
            buf[i] = (globalClass.getPgaSnl()[i] - snMin); // / 10
        }

        resMap.put("buf", buf);
        //todo 这里buf作为分析出来的波形结果返回给前端

        snMax = 0.0f;
        snMin = 10000.0f;
        for (i = 0; i < 2000; i++) {
            if (snMax < buf[i]) snMax = buf[i];
            if (snMin > buf[i]) snMin = buf[i];
        }
        short maxI = CLibrary.INSTANCE7.MaxID(buf, (short) 0, (short) 2000);
        float maxV = buf[maxI];
        //System.out.println("测试buf数据：" + Arrays.toString(buf));
        //System.out.println("测试maxV数据：" + maxV);
//        CG.dc = picture->GetHDC();
        CG.box_Num = 1; CG.Ynum = 6;
//        CG.height = rect.Height();
//        CG.width = rect.Width();
        CG.samplenum = 2000;
        CG.stepX = 45; CG.stepY = 30;
        String strTemp = "";

//        freePointer(SJ_snl1);
        return 1;
    }

}
