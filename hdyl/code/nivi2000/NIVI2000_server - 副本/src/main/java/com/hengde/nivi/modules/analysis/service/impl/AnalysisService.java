package com.hengde.nivi.modules.analysis.service.impl;

import com.hengde.nivi.common.global.AnalHPA_Data;
import com.hengde.nivi.common.global.GlobalClass;
import com.hengde.nivi.common.global.HYP_RST;
import com.hengde.nivi.modules.analysis.service.IAnalysisService;
import com.hengde.nivi.modules.create.dao.BasisMeasurementMapper;
import com.hengde.nivi.modules.create.entity.BasisMeasurement;
import com.hengde.nivi.modules.detection.entity.OriginalWaveformReq;
import com.hengde.nivi.modules.detection.service.IDetectionService;
import com.sun.jna.*;
import lombok.Data;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.awt.*;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * @author dong
 * @title: AnalysisService
 * @description: 分析算法相关业务实现
 * @date 2020/4/26 17:11
 */
@Service
public class AnalysisService implements IAnalysisService {
    @Autowired
    private IDetectionService detectionService;

    @Autowired
    private BasisMeasurementMapper basisMeasurementMapper;

    @Autowired
    private AnalHPA_Data.ByReference analHPADataStruct;

    @Autowired
    private GlobalClass globalClass;

    @Autowired
    private HYP_RST.ByReference hypRr;

    public interface CLibrary extends Library {
        CLibrary INSTANCE2 = (CLibrary) Native.loadLibrary("ComLib",CLibrary.class);
        CLibrary INSTANCE3 = (CLibrary) Native.loadLibrary("hpassysstr",CLibrary.class);
        CLibrary INSTANCE4 = (CLibrary) Native.loadLibrary("hpassys",CLibrary.class);
        float DetectMaxValue(float[] buff, short start, short end );
        void Filter(float[] signals, Pointer smooth, short allNum, short smNum );
        void FilterMibun(float[] signals, Pointer mibun, short allNum, short smNum );
        void FFT_Filter( short modeFlg, Pointer signals, short sigNum, float lowF,
                         float highF, float sampleInt );
        short PcgIanalysis( float[] p_snl, short size, AnalHPA.ByReference analHPAStruct );
        short PcgIIanalysis( float[] p_snl, short size, AnalHPA.ByReference analHPAStruct );
        short FilterLow( float[] signal, Pointer result, short size, float limT);
        short DetectUSpoint( float[] des ,short start, short end,
                             short ch, short size, AnalHPA.ByReference analHPAStruct );
        short DN_point( float[] des ,short start, short end, short ch, short size, AnalHPA.ByReference analHPAStruct );
        short DetectMax( float[] signals, short startPos, short endPos );
        float max_value(float[] buff, short size );
        float min_value(float[] buff, short size );
        void Spp_Tpp(float[] Ps_snl, float[] Pd_snl,short[] specp,
                     short size1,Pointer retval, Pointer Pga_snl );
        void Dpp_anal( float[] Pd_snl, short T8,float samp, short size2, Pointer retval);

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

        short RQT_Analysis(float[] k_snl, float[] j_snl, short  Size, AnalHPA.ByReference analHPASStruct, AnalHPA_Data.ByReference analHPADataStruct );
    }

    @Override
    public Map ecgPcgApgSysDia(String basisMeasurementId, Map resMap) {
        //定义最终返回的Map
        //Map resMap = new HashMap();

        //根据测试号获取对应患者基本检测信息
        BasisMeasurement basisMeasurement = basisMeasurementMapper.getPatientBasisMeasurement(basisMeasurementId);

        if(basisMeasurement.getArPs() == 1 || basisMeasurement.getArPs() == 3 )
        {
            basisMeasurement.setPs(basisMeasurement.getBrPs());
            basisMeasurement.setPd(basisMeasurement.getBrPd());
        }

        //根据测试号获取所有原始检测波形
        OriginalWaveformReq originalWaveform = detectionService.getOriginalWaveform(basisMeasurementId);
        if(originalWaveform == null){
            resMap.put("staCode", 0);
            return resMap;
        }

        //第一次计算ECG相关的波形时需要new出来相关结构体
//        CLibrary.AnalHPA_Data.ByReference analHPADataStruct = new CLibrary.AnalHPA_Data.ByReference();
        //初始化超收缩相关结构体
        CLibrary.AnalHPA.ByReference analHPAStructs = new CLibrary.AnalHPA.ByReference();
        //初始化低舒张相关结构体
        CLibrary.AnalHPA.ByReference analHPAStructd = new CLibrary.AnalHPA.ByReference();
        //定义需要使用的变量
        short passBand	= 3;
        float stepHyp = 0.0012f;
        float Wss = 1.0f;
        float Wsd = 0.4f;
        float Wd = 0.56f;
        float TOPMax = 0;
        float TOPBtw = 0;
        float lcuff = 4.0f;
        float PI = 3.141592f;
        float A0 = 0.67f;
        Map<String, Short> parMap = new HashMap();
        parMap.put("UTd", (short) 0);
        parMap.put("DNd", (short) 0);
        parMap.put("apg1size", (short) 0);
        parMap.put("apg2size", (short) 0);

        //定义需要使用的全局数组
        short[] T1 = new short[5];
        short[] T2 = new short[5];
        float[] apg1Snl = new float[2500];
        float[] apg2Snl = new float[2500];
        float[] apg3Snl = new float[2500];
        float[] PssSnl = new float[2500];
        float[] PddSnl = new float[2500];
        float[] PgaSnl = new float[2500];

        //最后一次使用的全局结构体
//        CLibrary.HYP_RST.ByReference hypRr = new CLibrary.HYP_RST.ByReference();

        /*****************首先获取心电的超收缩波和低舒张波*****************/
        int[] ecgs = originalWaveform.getHypecgs(); //心电的超收缩波
        //将其转为float数
        int ecgsSize = ecgs.length;
        float[] ecgsf = new float[ecgsSize];
        for (int i = 0; i < ecgsSize; i++){
            ecgsf[i] = (float)ecgs[i];
        }

        int ecgsSta = this.ecgSysDia(ecgsf, (short) ecgsSize, analHPAStructs, analHPADataStruct); //计算心电超收缩波R相关的值
        if (ecgsSta == 0){
            resMap.put("staCode", 500);
            return resMap;
        }
        if (ecgsSta == 2){
            resMap.put("staCode", 501);
            return resMap;
        }
        //获取心电超收缩心电Rtops的值
        short[] rtopss = analHPAStructs.getRtops();
        resMap.put("hypecgsRtops", rtopss);
        resMap.put("hypecgsRtopsNum", analHPAStructs.getRtopNum());

        int[] ecgd = originalWaveform.getHypecgd(); //心电的低舒张波
        int ecgdSize = ecgd.length;
        float[] ecgdf = new float[ecgdSize];
        //将其转为float数
        for (int i = 0; i < ecgdSize; i++){
            ecgdf[i] = (float)ecgd[i];
        }

        int ecgdSta = this.ecgSysDia(ecgdf, (short) ecgdSize, analHPAStructd, analHPADataStruct); //计算心电低舒张波R相关的值
        if (ecgdSta == 0){
            resMap.put("staCode", 502);
            return resMap;
        }
        if (ecgdSta == 2){
            resMap.put("staCode", 503);
            return resMap;
        }
        //获取心电低舒张心电Rtops的值
        short[] rtopsd = analHPAStructd.getRtops();
        resMap.put("hypecgdRtops", rtopsd);
        resMap.put("hypecgdRtopsNum", analHPAStructd.getRtopNum());

        /*****************然后获取心音的超收缩波和低舒张波*****************/
        int[] pcgs = originalWaveform.getHyppcgs(); //心音的超收缩波
        int pcgsSize = pcgs.length;
        float[] pcgsf = new float[pcgsSize];
        //将其转为float数
        for (int i = 0; i < pcgsSize; i++){
            pcgsf[i] = (float)pcgs[i];
        }
        int pcgsSta = this.pcgSysDia(pcgsf, (short) pcgsSize, passBand, stepHyp, analHPAStructs);
        if (pcgsSta == 0){
            resMap.put("staCode", 504);
            return resMap;
        }
        if (pcgsSta == 2){
            resMap.put("staCode", 505);
            return resMap;
        }
        //获取心音超收缩第一心音和第二心音的值
        short[] istartss = analHPAStructs.getIstarts();
        short[] iIstartss = analHPAStructs.getIIstarts();
        resMap.put("hyppcgsIstarts", istartss);
        resMap.put("hyppcgsIIstarts", iIstartss);

        int[] pcgd = originalWaveform.getHyppcgd(); //心音的低舒张波
        int pcgdSize = pcgd.length;
        float[] pcgdf = new float[pcgdSize];
        //将其转为float数
        for (int i = 0; i < pcgdSize; i++){
            pcgdf[i] = (float)pcgd[i];
        }
        int pcgdSta = this.pcgSysDia(pcgdf, (short) pcgdSize, passBand, stepHyp, analHPAStructd);
        if (pcgdSta == 0){
            resMap.put("staCode", 506);
            return resMap;
        }
        if (pcgdSta == 2){
            resMap.put("staCode", 507);
            return resMap;
        }
        //获取心音低舒张第一心音和第二心音的值
        short[] istartsd = analHPAStructd.getIstarts();
        short[] iIstartsd = analHPAStructd.getIIstarts();
        resMap.put("hyppcgdIstarts", istartsd);
        resMap.put("hyppcgdIIstarts", iIstartsd);

        /*****************最后获取脉搏的超收缩波和低舒张波*****************/
        int[] apgs = originalWaveform.getHypapgs(); //脉搏的超收缩波
        int apgsSize = apgs.length;
        float[] apgsf = new float[apgsSize];
        float[] bsnlapgsf = new float[apgsSize];
        //将其转为float数
        for (int i = 0; i < apgsSize; i++){
            apgsf[i] = (float)apgs[i];
            bsnlapgsf[i] = (float)apgs[i];
        }
        Pointer filterLowsPointer = new Memory(apgsSize * 4);
        filterLowsPointer.write(0, bsnlapgsf, 0, apgsSize);

        //开始调用脉搏公共方法
        int apgsSta = this.apgSysDia(apgs, apgsf, filterLowsPointer, (short) apgsSize, analHPAStructs);
        if (apgsSta == 0){
            resMap.put("staCode", 508);
            freePointer(filterLowsPointer);
            return resMap;
        }
        //获取apg的USpnts、DNpnts、UTpnts值；
        short[] uSpntss = analHPAStructs.getUSpnts();
        short[] dNpntss = analHPAStructs.getDNpnts();
        short[] uTpntss = analHPAStructs.getUTpnts();
        resMap.put("hypapgsUSpnts", uSpntss);
        resMap.put("hypapgsDNpnts", dNpntss);
        resMap.put("hypapgsUTpnts", uTpntss);

        this.tApgs(filterLowsPointer.getFloatArray(0, apgsSize), apg1Snl, parMap, T1, analHPAStructs);

        int[] apgd = originalWaveform.getHypapgd(); //脉搏的低舒张波
        int apgdSize = apgd.length;
        float[] apgdf = new float[apgdSize];
        float[] bsnlapgdf = new float[apgdSize];
        //将其转为float数
        for (int i = 0; i < apgdSize; i++){
            apgdf[i] = (float)apgd[i];
            bsnlapgdf[i] = (float)apgd[i]; //原始波形int[] 转float[]
        }
        Pointer filterLowdPointer = new Memory(apgdSize * 4);
        filterLowdPointer.write(0, bsnlapgdf, 0, apgdSize);

        //开始调用脉搏公共方法
        int apgdSta = this.apgSysDia(apgd, apgdf, filterLowdPointer, (short) apgdSize, analHPAStructd);
        if (apgdSta == 0){
            resMap.put("staCode", 509);
            freePointer(filterLowsPointer);
            freePointer(filterLowdPointer);
            return resMap;
        }
        //获取apg的USpnts、DNpnts、UTpnts值；
        short[] uSpntsd = analHPAStructd.getUSpnts();
        short[] dNpntsd = analHPAStructd.getDNpnts();
        short[] uTpntsd = analHPAStructd.getUTpnts();
        resMap.put("hypapgdUSpnts", uSpntsd);
        resMap.put("hypapgdDNpnts", dNpntsd);
        resMap.put("hypapgdUTpnts", uTpntsd);

        this.tApgd(filterLowdPointer.getFloatArray(0, apgsSize), apg2Snl, T2, parMap, analHPAStructd);

        //最后调用HypAnalResult方法
        int hypAnalRes = hypAnalResult(apg1Snl, apg2Snl, apg3Snl, parMap.get("apg1size"), parMap.get("apg2size"), parMap.get("UTd"), parMap.get("DNd"),
        PssSnl, PddSnl, PgaSnl, Wss, Wsd, Wd, T1, T2, stepHyp, TOPMax, TOPBtw, PI, lcuff, A0, analHPADataStruct, basisMeasurement, hypRr, analHPAStructd, resMap);

        if(hypAnalRes == 0){
            resMap.put("staCode", 510);
            freePointer(filterLowsPointer);
            freePointer(filterLowdPointer);
            return resMap;
        }

        if(hypAnalRes == 2){
            resMap.put("staCode", 511);
            freePointer(filterLowsPointer);
            freePointer(filterLowdPointer);
            return resMap;
        }

        freePointer(filterLowsPointer);
        freePointer(filterLowdPointer);

        return resMap;
    }

    public void freePointer(Pointer p){
        long peer = Pointer.nativeValue(p);
        Native.free(peer);//手动释放内存
        Pointer.nativeValue(p, 0);//避免Memory对象被GC时重复执行Nativ.free()方法
    }

    //心电公共调用方法
    public int ecgSysDia(float[] ecgf, short sampleNum, CLibrary.AnalHPA.ByReference analHPAStruct, AnalHPA_Data.ByReference analHPADataStruct){
        float max_num = CLibrary.INSTANCE2.DetectMaxValue(ecgf, (short) 0, sampleNum);
//        if (max_num < 0.5f)
//        {
//            return 0;
//        }

        Pointer pecg = new Memory(sampleNum * 4);
        CLibrary.INSTANCE2.Filter(ecgf, pecg, sampleNum, (short) 25);
        Pointer ecgfp = new Memory(sampleNum * 4);
        CLibrary.INSTANCE2.FilterMibun(pecg.getFloatArray(0, sampleNum), ecgfp, sampleNum, (short) 25);

//        ecgfp.write(0, ecgf, 0, sampleNum);
        CLibrary.INSTANCE2.FilterMibun(ecgfp.getFloatArray(0, sampleNum), pecg, sampleNum, (short) 25);

        short res = CLibrary.INSTANCE3.RQT_Analysis(pecg.getFloatArray(0, sampleNum), ecgfp.getFloatArray(0, sampleNum), sampleNum, analHPAStruct, analHPADataStruct);

        freePointer(pecg);
        freePointer(ecgfp);

        if (res == 0)
        {
            return 2;
        }

        return 1;
    }

    //心音公共调用方法
    public int pcgSysDia(float[] pcgf, short sampleNum, short passBand, float stepHyp, CLibrary.AnalHPA.ByReference analHPAStruct){
        float max_num = CLibrary.INSTANCE2.DetectMaxValue(pcgf, (short) 0, sampleNum);
//        if (max_num < 0.5f)
//        {
//            return 0;
//        }
        Pointer ppcg = new Memory(sampleNum * 4);
        CLibrary.INSTANCE2.Filter(pcgf, ppcg, sampleNum, (short) 25);
        max_num = CLibrary.INSTANCE2.DetectMaxValue(ppcg.getFloatArray(0, sampleNum), (short) 0, sampleNum);
//        if (max_num < 0.2f)
//        {
//            freePointer(ppcg);
//            return 2;
//        }
        Pointer ppcgf = new Memory(sampleNum * 4);
        ppcgf.write(0, pcgf,0, sampleNum);
        CLibrary.INSTANCE2.FFT_Filter(passBand, ppcgf, sampleNum, 5.0f, 500.0f, stepHyp);
        analHPAStruct.setI_start(CLibrary.INSTANCE3.PcgIanalysis(ppcgf.getFloatArray(0, sampleNum), sampleNum, analHPAStruct));
        analHPAStruct.setII_start(CLibrary.INSTANCE3.PcgIIanalysis(ppcgf.getFloatArray(0, sampleNum), sampleNum, analHPAStruct));

        freePointer(ppcg);
        freePointer(ppcgf);

        return 1;
    }

    //脉搏公共调用方法
    public int apgSysDia(int[] apg, float[] apgf, Pointer bsnlapgf, short sampleNum, CLibrary.AnalHPA.ByReference analHPAStruct){
        short ss;
        short dd;
        float max_num = CLibrary.INSTANCE2.DetectMaxValue(apgf, (short) 0, sampleNum);
//        if (max_num < 0.5f)
//        {
//            return 0;
//        }
        float[] japgf = new float[sampleNum];
        //将其转为float数
        for (int i = 0; i < sampleNum; i++){
            japgf[i] = (float)apg[i];
        }
        Pointer japgfP = new Memory(sampleNum * 4);
        japgfP.write(0, japgf, 0, sampleNum);
        CLibrary.INSTANCE2.FilterLow(apgf, japgfP, sampleNum, 0.008f);
        Pointer papgf = new Memory(sampleNum * 4);
        CLibrary.INSTANCE2.Filter(japgfP.getFloatArray(0, sampleNum), papgf, sampleNum, (short) 25);
        CLibrary.INSTANCE2.FilterMibun(papgf.getFloatArray(0, sampleNum), japgfP, sampleNum, (short) 25);

        papgf.write(0, apgf,0, sampleNum);
        CLibrary.INSTANCE2.FilterMibun(japgfP.getFloatArray(0, sampleNum), papgf, sampleNum, (short) 25);

        for (int i = 0; i <= analHPAStruct.getRtopNum(); i++)
        {
            dd = 0;
            ss = 0;
            dd = analHPAStruct.getRtops()[i];
            ss = (short) (analHPAStruct.getRtops()[i] + (short) 250);
            analHPAStruct.USpnts[i] = CLibrary.INSTANCE3.DetectUSpoint(papgf.getFloatArray(0, sampleNum), dd, ss, (short) i, sampleNum, analHPAStruct);
        }

        analHPAStruct.setUS_point(analHPAStruct.getUSpnts()[analHPAStruct.getRpos()]);

        for (int i = 0; i <= analHPAStruct.getRtopNum(); i++)
        {
            dd = 0;
            ss = 0;
            dd = (short) (analHPAStruct.getIIstarts()[i] + (short) 5);
            ss = (short) (analHPAStruct.getIIstarts()[i] + (short) 160);
            analHPAStruct.DNpnts[i] = CLibrary.INSTANCE3.DN_point(papgf.getFloatArray(0, sampleNum), dd, ss, (short) i, sampleNum, analHPAStruct); //DNpnts函数有两个，目前返回的结果有问题；
            analHPAStruct.DNpnts[i] = (short) (analHPAStruct.DNpnts[i] - (short) 3);  //todo 此处与C源码结果不同，每个数组元素都多3，所以这里减掉3；
        }
        analHPAStruct.setDN_point(analHPAStruct.getDNpnts()[analHPAStruct.getRpos()]);

        for (int i = 0; i <= analHPAStruct.getRtopNum(); i++)
        {
            dd = 0;
            ss = 0;
            dd = analHPAStruct.getUSpnts()[i];
            ss = analHPAStruct.getDNpnts()[i];
            analHPAStruct.UTpnts[i] = (short)CLibrary.INSTANCE2.DetectMax(bsnlapgf.getFloatArray(0, sampleNum), dd, ss); //bsnlapgf = 脉搏原始波形apg[];
        }
        analHPAStruct.setUT_point(analHPAStruct.getUTpnts()[analHPAStruct.getRpos()]);

        for (int i = 0; i < sampleNum; i++)
        {
            japgf[i] = (float)apg[i];
        }
        CLibrary.INSTANCE2.FilterLow(japgf, bsnlapgf, sampleNum, 0.008f);

        freePointer(japgfP);
        freePointer(papgf);

        return 1;
    }

    //脉搏下面代码中T数组相关代码超收缩波调用方法
    public int tApgs(float[] bSnl, float[] apg1Snl, Map<String, Short> parMap, short[] T1, CLibrary.AnalHPA.ByReference analHPAStruct){
        short apg1size = parMap.get("apg1size");
        for (int i = analHPAStruct.getRtops()[analHPAStruct.getRpos()]; i <= analHPAStruct.getRtops()[analHPAStruct.getRpos() + 1]; i++)
        {
            apg1Snl[i - analHPAStruct.getRtops()[analHPAStruct.getRpos()]] = bSnl[i];
            apg1size++;
        }
        T1[0] = (short) (analHPAStruct.getUS_point() - analHPAStruct.getRtops()[analHPAStruct.getRpos()]);
        T1[1] = (short) (analHPAStruct.getDN_point() - analHPAStruct.getUS_point() + T1[0]);
        T1[2] = (short) (analHPAStruct.getUT_point() - analHPAStruct.getUS_point() + T1[0]);
        T1[3] = apg1size;

        parMap.put("apg1size", apg1size);

        return 1;
    }

    //脉搏下面代码中T数组相关代码低舒张波调用方法
    public int tApgd(float[] bSnl, float[] apg2Snl, short[] T2, Map<String, Short> parMap, CLibrary.AnalHPA.ByReference analHPAStruct){
        short apg2size = parMap.get("apg2size");
        for (int i = analHPAStruct.getRtops()[analHPAStruct.getRpos()]; i <= analHPAStruct.getRtops()[analHPAStruct.getRpos() + 1]; i++)
        {
            apg2Snl[i - analHPAStruct.getRtops()[analHPAStruct.getRpos()]] = bSnl[i];
            apg2size++;
        }
        T2[0] = (short) (analHPAStruct.getUS_point() - analHPAStruct.getRtops()[analHPAStruct.getRpos()]);
        T2[1] = (short) (analHPAStruct.getDN_point() - analHPAStruct.getUS_point() + T2[0]);
        T2[2] = (short) (analHPAStruct.getUT_point() - analHPAStruct.getUS_point() + T2[0]);
        T2[3] = apg2size;
        parMap.put("apg2size", apg2size);
        parMap.put("UTd", T2[2]);
        parMap.put("DNd", T2[1]);

        return 1;
    }

    //ECG、PCG、APG都执行完毕之后，最后执行一次的方法
    public int hypAnalResult(float[] apg1Snl, float[] apg2Snl, float[] apg3Snl, short apg1size, short apg2size, short UTd, short DNd,
                             float[] PssSnl, float[] PddSnl, float[] PgaSnl, float Wss, float Wsd, float Wd,short[] T1, short[] T2,
                             float stepHyp, float TOPMax, float TOPBtw, float PI, float lcuff, float A0, AnalHPA_Data.ByReference analHPADataStruct,
                             BasisMeasurement basisMeasurement, HYP_RST.ByReference hypRr, CLibrary.AnalHPA.ByReference analHPAStruct, Map resMap){
        short i;
        short a;
        short fl;
        float k;
        float H1;
        float maxN;
        float minN;
        float[] ret1 = new float[8];
        float[] ret2 = new float[5];
        float llcuff;
        float hei;
        float aa;
        float tr;
        float tpp1;
        long rst;

        H1 = apg1Snl[0];
        for (i = 0; i <= apg1size - 1; i++)
        {
            apg1Snl[i] = apg1Snl[i] - (((apg1Snl[apg1size - 1] - H1) / apg1size) * i);
        }
        maxN = CLibrary.INSTANCE2.max_value(apg1Snl, apg1size);
        minN = CLibrary.INSTANCE2.min_value(apg1Snl, apg1size);

        // CSR added 2010-12-24 : for division by zero
        // begin
        if (maxN == minN) {
            return 0;
        }
        // end

        k = (float)(((basisMeasurement.getPs()) - (basisMeasurement.getPd())) / (maxN - minN));

        for (i = 0; i <= apg1size - 1; i++)
        {
            apg1Snl[i] = apg1Snl[i] - minN;
            apg1Snl[i] = apg1Snl[i] * k + (basisMeasurement.getPd());
        }

        H1 = apg2Snl[0];
        for (i = 0; i <= apg2size - 1; i++)
        {
            apg2Snl[i] = apg2Snl[i] - (((apg2Snl[apg2size - 1] - H1) / apg2size) * i);
        }
        maxN = CLibrary.INSTANCE2.max_value(apg2Snl, apg2size);
        minN = CLibrary.INSTANCE2.min_value(apg2Snl, apg2size);

        // CSR added 2010-12-24 : for division by zero
        // begin
        if (maxN == minN) {
            return 2;
        }

        k = (float)(((basisMeasurement.getPs()) - (basisMeasurement.getPd())) / (maxN - minN));

        for (i = 0; i <= apg2size - 1; i++)
        {
            apg2Snl[i] = apg2Snl[i] - minN;
            apg2Snl[i] = apg2Snl[i] * k + (basisMeasurement.getPd());
        }

        this.pCSDectect(apg2Snl, apg2size, UTd, DNd, basisMeasurement, hypRr, resMap);

        if (apg1size > apg2size)
            apg1size = apg2size;

        for (i = 0; i <= apg1size - 1; i++)
        {
            PssSnl[i] = (float)(((Wss * apg1Snl[i]) + (Wsd * apg2Snl[i])) / (Wss + Wsd));
            PddSnl[i] = (float)(apg2Snl[i] + Wd * (basisMeasurement.getPd() - apg1Snl[i]));
        }

        Pointer ret1P = new Memory(32);
        ret1P.write(0, ret1, 0, 8);
        Pointer pgaSnlP = new Memory(2500 * 4);
        pgaSnlP.write(0, PgaSnl, 0, 2500);
        CLibrary.INSTANCE4.Spp_Tpp(PssSnl, PddSnl, T1, apg1size, ret1P, pgaSnlP);
        globalClass.setPgaSnl(pgaSnlP.getFloatArray(0, 2500));
        Pointer ret2P = new Memory(20);
        ret2P.write(0, ret2, 0, 5);
        CLibrary.INSTANCE4.Dpp_anal(PddSnl, T2[1], stepHyp, apg2size, ret2P);

        //TODO 这里注意使用的最后一次的结构体--低舒张
        TOPMax = (analHPAStruct.UTpnts[analHPAStruct.Rpos] - analHPAStruct.USpnts[analHPAStruct.Rpos]) * 0.0012f;
        TOPBtw = (analHPAStruct.USpnts[analHPAStruct.Rpos] - analHPAStruct.Rtops[analHPAStruct.Rpos]) * 0.0012f;
        hypRr.Pm = (ret1P.getFloatArray(0, 8)[2] + ret2P.getFloatArray(0, 5)[1]) / (ret1P.getFloatArray(0, 8)[5] + ret2P.getFloatArray(0, 5)[2]);
        if( hypRr.Pm < basisMeasurement.getPd() )
            hypRr.Pm = (float)((basisMeasurement.getPs() - basisMeasurement.getPd()) / 3 + basisMeasurement.getPd());
        llcuff = lcuff;
        hypRr.V0 = (float)Math.abs(1.5f * ((4.0f * PI * llcuff) / ret1P.getFloatArray(0, 8)[1]) * (ret2P.getFloatArray(0, 5)[0] / ret1P.getFloatArray(0, 8)[0]));

        if( hypRr.V0 <= 7.05 )
            hypRr.V0 =  7.05f;
        else if ( hypRr.V0 >= 29 )
            hypRr.V0 = 29.1f;

        hypRr.PR1 = (0.8f * (((float)basisMeasurement.getPs()) - ((float)basisMeasurement.getPd())) / hypRr.V0);
        hypRr.PR2 = 0.8f * (((hypRr.Pm - ((float)basisMeasurement.getPd())) / hypRr.V0));
        System.out.println("测试输出PR2的结果" + hypRr.PR2);
        hypRr.CO1 = hypRr.Pm / hypRr.PR1;
        hypRr.CO2 = hypRr.Pm / hypRr.PR2;
        hei = (float)basisMeasurement.getHeight();
        aa = A0 * (hei / 152.4f);
        hypRr.Ke = 7.5f * (ret1P.getFloatArray(0, 8)[0] * ret1P.getFloatArray(0, 8)[1]) / (4.0f * PI * aa * llcuff);
        hypRr.C = 1.0f / hypRr.Ke;
        tr = 1.0f + (float)Math.pow(((float)basisMeasurement.getPd()) / ((((float)basisMeasurement.getPs()) - ((float)basisMeasurement.getPd()))) , (Math.exp(0.5f)));
        tr = (float)Math.pow(tr , -0.5f);

        hypRr.Lv = (float)(ret1P.getFloatArray(0, 8)[3] / tr * Math.exp(0.5f * (tr * tr - 1)));
        hypRr.SV = ((hypRr.Pm / hypRr.PR1) / (60.0f / analHPADataStruct.R_R)) * 50.0f; //hyp_rr.SV = ((hyp_rr.Pm / hyp_rr.PR1) / (60# / Hpa_data.R_R)) * 50#;
        hypRr.Bpgrad_max = ret1P.getFloatArray(0, 8)[3];
        hypRr.Bpgrad_min = ret1P.getFloatArray(0, 8)[4];
        hypRr.Bpgrad_pp = ret1P.getFloatArray(0, 8)[0];
        hypRr.Bpgrad_dw = ret2P.getFloatArray(0, 5)[0];
        hypRr.Ps = (float)basisMeasurement.getPs();
        hypRr.Pd = (float)basisMeasurement.getPd();
        hypRr.Tpp = ret1P.getFloatArray(0, 8)[1];
        tpp1 = hypRr.Tpp / tr;

        for (i = 0; i <= apg1size; i++)
        {
            apg3Snl[i] = ((float)basisMeasurement.getPs()) * (float) Math.exp(((-1) * (i * 0.0012f - TOPMax - TOPBtw) * (i * 0.0012f - TOPMax - TOPBtw)) / (2.0f * tpp1 * tpp1));
            //apg3Snl[i] = ((double)PsnInfo.Ps) * Exp(((-1#) * (i * 0.0012 - TOP_max - TOP_btw) * (i * 0.0012 - TOP_max - TOP_btw)) / (2 * Tpp1 * Tpp1));
        }

        //后面还有代码，不需要再转换；


        //返回之前free内存
        freePointer(ret1P);
        freePointer(pgaSnlP);
        freePointer(ret2P);
        return 1;
    }

    public boolean pCSDectect(float[] apg2Snl, short apg2size, short UTd, short DNd, BasisMeasurement basisMeasurement, HYP_RST.ByReference hypRr, Map resMap){
        float[] s1 = new float[2500];
        float[] s2 = new float[2500];

        Pointer s1P = new Memory(2500 * 4);
        s1P.write(0, s1, 0, 2500);
        CLibrary.INSTANCE2.FilterMibun(apg2Snl, s1P, apg2size, (short) 25);
        Pointer s2P = new Memory(2500 * 4);
        s2P.write(0, s2, 0, 2500);
        CLibrary.INSTANCE2.FilterMibun(s1P.getFloatArray(0, 2500), s2P, apg2size, (short) 25);

        for( short i = UTd; i< DNd; i++)
        {
            if(s2P.getFloatArray(0, 2500)[i - 1 ] <= 0.0001f && s2P.getFloatArray(0, 2500)[i] >= 0.0001f)
            {
                hypRr.CBPs = basisMeasurement.getPs() * ( apg2Snl[i] / apg2Snl[UTd]) * 0.98f; break;
            }
            else if(s2[i - 1 ] >= 0.0001f && s2[i] <= 0.0001f)
            {
                hypRr.CBPs = basisMeasurement.getPs() * ( apg2Snl[i] / apg2Snl[UTd]) * 0.98f; break;
            }
        }

        resMap.put("CBPs", hypRr.CBPs);

        freePointer(s1P);
        freePointer(s2P);
        return true;
    }
}
