package com.hekangyuan.nivi1000.mock;

import android.content.Context;
import android.content.res.AssetManager;

import com.google.gson.Gson;
import com.hekangyuan.nivi1000.activity.AnalysisActivity;
import com.hekangyuan.nivi1000.mock.MockEcgAnalysis;
import com.hekangyuan.nivi1000.model.AnalysisBean;
import com.hekangyuan.nivi1000.model.EcgMonitorBean;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Arrays;

public class MockDataUtil {

    public static MockEcgData getEcgData(Context context){
        MockEcgData mockEcgData = new MockEcgData();

        String analysisData = getJson(context,"data1.json");
        Gson gson = new Gson();

       mockEcgData = gson.fromJson(analysisData, MockEcgData.class);


        return mockEcgData;
    }


    public static MockEcgAnalysis getEcgAnalysis(Context context){
        MockEcgAnalysis mockEcgAnalysis = new MockEcgAnalysis();

        String analysisData = getJson(context,"analysis.json");
        Gson gson = new Gson();

        mockEcgAnalysis = gson.fromJson(analysisData, MockEcgAnalysis.class);


        return mockEcgAnalysis;
    }

    public static EcgMonitorBean getEcgMonitorBean(Context context){
        EcgMonitorBean ecgMonitorBean = new EcgMonitorBean();

//        String analysisData = getJson(context,"ecgMonitor.json");
//        Gson gson = new Gson();
//
//        ecgMonitorBean = gson.fromJson(analysisData, EcgMonitorBean.class);

        ecgMonitorBean.setHypecgs(getEcgData(context).getData().getHypecgs().toArray(
                new Float[getEcgData(context).getData().getHypecgs().size()]));
        ecgMonitorBean.setHyppcgs(getEcgData(context).getData().getHyppcgs().toArray(
                new Float[getEcgData(context).getData().getHyppcgs().size()]));
        ecgMonitorBean.setHypapgs(getEcgData(context).getData().getHypapgs().toArray(
                new Float[getEcgData(context).getData().getHypapgs().size()]
        ));
        ecgMonitorBean.setHypecgd(getEcgData(context).getData().getHypecgd().toArray(
                new Float[getEcgData(context).getData().getHypecgd().size()]
        ));
        ecgMonitorBean.setHyppcgd(getEcgData(context).getData().getHyppcgd().toArray(
                new Float[getEcgData(context).getData().getHyppcgd().size()]
        ));
        ecgMonitorBean.setHypapgd(getEcgData(context).getData().getHypapgd().toArray(
                new Float[getEcgData(context).getData().getHypapgd().size()]
        ));
        ecgMonitorBean.setEcgPl(getEcgData(context).getData().getEcgPl().toArray(
                new Float[getEcgData(context).getData().getEcgPl().size()]
        ));
        ecgMonitorBean.setPcgPl(getEcgData(context).getData().getPcgPl().toArray(
                new Float[getEcgData(context).getData().getPcgPl().size()]
        ));
        ecgMonitorBean.setApgL(getEcgData(context).getData().getApgL().toArray(
                new Float[getEcgData(context).getData().getApgL().size()]
        ));
        ecgMonitorBean.setEcgPr(getEcgData(context).getData().getEcgPr().toArray(
                new Float[getEcgData(context).getData().getEcgPr().size()]
        ));
        ecgMonitorBean.setPcgPr(getEcgData(context).getData().getPcgPr().toArray(
                new Float[getEcgData(context).getData().getPcgPr().size()]
        ));
        ecgMonitorBean.setApgR(getEcgData(context).getData().getApgR().toArray(
                new Float[getEcgData(context).getData().getApgR().size()]
        ));
        ecgMonitorBean.setEcgTh(getEcgData(context).getData().getEcgTh().toArray(
                new Float[getEcgData(context).getData().getEcgTh().size()]
        ));
        ecgMonitorBean.setApgTh(getEcgData(context).getData().getApgTh().toArray(
                new Float[getEcgData(context).getData().getApgTh().size()]
        ));

        /**
         * private Float [] hyppcgdIstarts;
         *     private Float [] hyppcgsIstarts;
         *     private Float [] hyppcgdIIstarts;
         *     private Float [] hyppcgsIIstarts;
         *
         *     private Float [] hypapgdDNpnts;
         *     private Float [] hypapgsUSpnts;
         *     private Float [] hypapgdUTpnts;
         *
         *     private Float [] hypecgdRtops;
         *     private Float [] hypapgsDNpnts;
         *     private Float [] hypecgsRtops;
         *
         *     private Float [] hypapgdUSpnts;
         *     private Float [] hypapgsUTpnts;
         */
        ecgMonitorBean.setHyppcgdIstarts(getEcgAnalysis(context).getData().getHyppcgdIstarts().toArray(
                new Float[getEcgAnalysis(context).getData().getHyppcgdIstarts().size()]
        ));
        ecgMonitorBean.setHyppcgsIstarts(getEcgAnalysis(context).getData().getHyppcgsIstarts().toArray(
                new Float[getEcgAnalysis(context).getData().getHyppcgsIstarts().size()]
        ));

        ecgMonitorBean.setHyppcgdIIstarts(getEcgAnalysis(context).getData().getHyppcgdIIstarts().toArray(
                new Float[getEcgAnalysis(context).getData().getHyppcgdIIstarts().size()]
        ));
        ecgMonitorBean.setHyppcgsIIstarts(getEcgAnalysis(context).getData().getHyppcgsIIstarts().toArray(
                new Float[getEcgAnalysis(context).getData().getHyppcgsIIstarts().size()]
        ));

        ecgMonitorBean.setHypapgdDNpnts(getEcgAnalysis(context).getData().getHypapgdDNpnts().toArray(
                new Float[getEcgAnalysis(context).getData().getHypapgdDNpnts().size()]
        ));
        ecgMonitorBean.setHypapgsDNpnts(getEcgAnalysis(context).getData().getHypapgsDNpnts().toArray(
                new Float[getEcgAnalysis(context).getData().getHypapgsDNpnts().size()]
        ));

        ecgMonitorBean.setHypapgsUSpnts(getEcgAnalysis(context).getData().getHypapgsUSpnts().toArray(
                new Float[getEcgAnalysis(context).getData().getHypapgsUSpnts().size()]
        ));
        ecgMonitorBean.setHypapgdUSpnts(getEcgAnalysis(context).getData().getHypapgdUSpnts().toArray(
                new Float[getEcgAnalysis(context).getData().getHypapgdUSpnts().size()]
        ));

        ecgMonitorBean.setHypapgdUTpnts(getEcgAnalysis(context).getData().getHypapgdUTpnts().toArray(
                new Float[getEcgAnalysis(context).getData().getHypapgdUTpnts().size()]
        ));
        ecgMonitorBean.setHypapgsUTpnts(getEcgAnalysis(context).getData().getHypapgsUTpnts().toArray(
                new Float[getEcgAnalysis(context).getData().getHypapgsUTpnts().size()]
        ));

        ecgMonitorBean.setHypecgdRtops(getEcgAnalysis(context).getData().getHypecgdRtops().toArray(
                new Float[getEcgAnalysis(context).getData().getHypecgdRtops().size()]
        ));
        ecgMonitorBean.setHypecgsRtops(getEcgAnalysis(context).getData().getHypecgsRtops().toArray(
                new Float[getEcgAnalysis(context).getData().getHypecgsRtops().size()]
        ));

        /**
         * nt dNum = ecgMonitorBean.getHypecgdRtopsNum();
         *         int sNum = ecgMonitorBean.getHypecgsRtopsNum();
         *         int lNum = ecgMonitorBean.getRtopNumL();
         *         int rNum = ecgMonitorBean.getRtopNumR();
         */
        ecgMonitorBean.setHypecgdRtopsNum(getEcgAnalysis(context).getData().getHypecgdRtopsNum());
        ecgMonitorBean.setHypecgsRtopsNum(getEcgAnalysis(context).getData().getHypecgsRtopsNum());
        ecgMonitorBean.setRtopNumL(getEcgAnalysis(context).getData().getRtopNumL());
        ecgMonitorBean.setRtopNumR(getEcgAnalysis(context).getData().getRtopNumR());

        /**
         * analysisBean.setTsL(Arrays.asList(ecgMonitorBean.getTsL()).subList(0, lNum));
         *         analysisBean.setUTL(Arrays.asList(ecgMonitorBean.getUTL()).subList(0, lNum));
         *         analysisBean.setRtopL(Arrays.asList(ecgMonitorBean.getRtopL()).subList(0, lNum));
         *
         *         analysisBean.setTsR(Arrays.asList(ecgMonitorBean.getTsR()).subList(0, rNum));
         *         analysisBean.setUTR(Arrays.asList(ecgMonitorBean.getUTR()).subList(0, rNum));
         *         analysisBean.setRtopR(Arrays.asList(ecgMonitorBean.getRtopR()).subList(0, rNum));
         */
        ecgMonitorBean.setTsL(getEcgAnalysis(context).getData().getTsL().toArray(
                new Float[getEcgAnalysis(context).getData().getTsL().size()]

        ));
        ecgMonitorBean.setUTL(getEcgAnalysis(context).getData().getUTL().toArray(
                new Float[getEcgAnalysis(context).getData().getUTL().size()]

        ));
        ecgMonitorBean.setRtopL(getEcgAnalysis(context).getData().getRtopL().toArray(
                new Integer[getEcgAnalysis(context).getData().getRtopL().size()]
        ));

        ecgMonitorBean.setTsR(getEcgAnalysis(context).getData().getTsR().toArray(
                new Float[getEcgAnalysis(context).getData().getTsR().size()]

        ));
        ecgMonitorBean.setUTR(getEcgAnalysis(context).getData().getUTR().toArray(
                new Float[getEcgAnalysis(context).getData().getUTR().size()]

        ));
        ecgMonitorBean.setRtopR(getEcgAnalysis(context).getData().getRtopR().toArray(
                new Integer[getEcgAnalysis(context).getData().getRtopR().size()]

        ));

        /**
         * analysisBean.setIDL((int)ecgMonitorBean.getIDL());
         *         analysisBean.setIDR((int)ecgMonitorBean.getIDR());
         */
        ecgMonitorBean.setIDL(getEcgAnalysis(context).getData().getIDL());
        ecgMonitorBean.setIDR(getEcgAnalysis(context).getData().getIDR());

        ecgMonitorBean.setBuf(getEcgAnalysis(context).getData().getBuf().toArray(
                new Float[getEcgAnalysis(context).getData().getBuf().size()]
        ));

        return ecgMonitorBean;
    }

    public static String getJson(Context context, String fileName) {

        StringBuilder stringBuilder = new StringBuilder();
        try {
            AssetManager assetManager = context.getAssets();
            BufferedReader bf = new BufferedReader(new InputStreamReader(
                    assetManager.open(fileName)));
            String line;
            while ((line = bf.readLine()) != null) {
                stringBuilder.append(line);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return stringBuilder.toString();
    }

    /**
     * 根据mock数据初始化
     */
    public static AnalysisBean initByMockData(Context mContext) {
        AnalysisBean analysisBean = new AnalysisBean();
        MockEcgAnalysis mockEcgAnalysis = MockDataUtil.getEcgAnalysis(mContext);

        analysisBean.setHypecgs(MockDataUtil.getEcgData(mContext).getData().getHypecgs());
        analysisBean.setHyppcgs(MockDataUtil.getEcgData(mContext).getData().getHyppcgs());
        analysisBean.setHypapgs(MockDataUtil.getEcgData(mContext).getData().getHypapgs());

        analysisBean.setHypecgd(MockDataUtil.getEcgData(mContext).getData().getHypecgd());
        analysisBean.setHyppcgd(MockDataUtil.getEcgData(mContext).getData().getHyppcgd());
        analysisBean.setHypapgd(MockDataUtil.getEcgData(mContext).getData().getHypapgd());

        analysisBean.setApgL(MockDataUtil.getEcgData(mContext).getData().getApgL());
        analysisBean.setApgR(MockDataUtil.getEcgData(mContext).getData().getApgR());

        int dNum = mockEcgAnalysis.getData().getHypecgdRtopsNum();
        int sNum = mockEcgAnalysis.getData().getHypecgsRtopsNum();
        int lNum = mockEcgAnalysis.getData().getRtopNumL();
        int rNum = mockEcgAnalysis.getData().getRtopNumR();
        analysisBean.setHypecgsRtops(mockEcgAnalysis.getData().getHypecgsRtops().subList(0, sNum));
        analysisBean.setHypecgdRtops(mockEcgAnalysis.getData().getHypecgdRtops().subList(0, dNum));

        analysisBean.setHyppcgsIstarts(mockEcgAnalysis.getData().getHyppcgsIstarts().subList(0, sNum));
        analysisBean.setHyppcgsIIstarts(mockEcgAnalysis.getData().getHyppcgsIIstarts().subList(0, sNum));

        analysisBean.setHyppcgdIstarts(mockEcgAnalysis.getData().getHyppcgdIstarts().subList(0, dNum));
        analysisBean.setHyppcgdIIstarts(mockEcgAnalysis.getData().getHyppcgdIIstarts().subList(0, dNum));

        analysisBean.setHypapgsUSpnts(mockEcgAnalysis.getData().getHypapgsUSpnts().subList(0, sNum)); //起
        analysisBean.setHypapgsDNpnts(mockEcgAnalysis.getData().getHypapgsDNpnts().subList(0, sNum));
        analysisBean.setHypapgsUTpnts(mockEcgAnalysis.getData().getHypapgsUTpnts().subList(0, sNum)); //终点

        analysisBean.setHypapgdUSpnts(mockEcgAnalysis.getData().getHypapgdUSpnts().subList(0, dNum));
        analysisBean.setHypapgdDNpnts(mockEcgAnalysis.getData().getHypapgdDNpnts().subList(0, dNum));
        analysisBean.setHypapgdUTpnts(mockEcgAnalysis.getData().getHypapgdUTpnts().subList(0, dNum));

        analysisBean.setTsL(mockEcgAnalysis.getData().getTsL().subList(0, lNum));
        analysisBean.setUTL(mockEcgAnalysis.getData().getUTL().subList(0, lNum));
        analysisBean.setRtopL(mockEcgAnalysis.getData().getRtopL().subList(0, lNum));

        analysisBean.setTsR(mockEcgAnalysis.getData().getTsR().subList(0, rNum));
        analysisBean.setUTR(mockEcgAnalysis.getData().getUTR().subList(0, rNum));
        analysisBean.setRtopR(mockEcgAnalysis.getData().getRtopR().subList(0, rNum));

        analysisBean.setRtopNumL(mockEcgAnalysis.getData().getRtopNumL());
        analysisBean.setRtopNumR(mockEcgAnalysis.getData().getRtopNumR());
        analysisBean.setHypecgdRtopsNum(mockEcgAnalysis.getData().getHypecgdRtopsNum());
        analysisBean.setHypecgsRtopsNum(mockEcgAnalysis.getData().getHypecgsRtopsNum());

        analysisBean.setIDL(mockEcgAnalysis.getData().getIDL());
        analysisBean.setIDR(mockEcgAnalysis.getData().getIDR());

        analysisBean.setBuf(mockEcgAnalysis.getData().getBuf());
        return analysisBean;
    }

    public static AnalysisBean initByEcgMonitorBean(EcgMonitorBean ecgMonitorBean,EcgMonitorBean mockEcgMonitorBean){
        AnalysisBean analysisBean = new AnalysisBean();

        if (ecgMonitorBean.getHypecgs() == null) {
            analysisBean.setHypecgs(Arrays.asList(mockEcgMonitorBean.getHypecgs()));
            analysisBean.setHyppcgs(Arrays.asList(mockEcgMonitorBean.getHyppcgs()));
            analysisBean.setHypapgs(Arrays.asList(mockEcgMonitorBean.getHypapgs()));

            analysisBean.setHypecgd(Arrays.asList(mockEcgMonitorBean.getHypecgd()));
            analysisBean.setHyppcgd(Arrays.asList(mockEcgMonitorBean.getHyppcgd()));
            analysisBean.setHypapgd(Arrays.asList(mockEcgMonitorBean.getHypapgd()));

            analysisBean.setApgL(Arrays.asList(mockEcgMonitorBean.getApgL()));
            analysisBean.setApgR(Arrays.asList(mockEcgMonitorBean.getApgR()));
        }else{
            analysisBean.setHypecgs(Arrays.asList(ecgMonitorBean.getHypecgs()));
            analysisBean.setHyppcgs(Arrays.asList(ecgMonitorBean.getHyppcgs()));
            analysisBean.setHypapgs(Arrays.asList(ecgMonitorBean.getHypapgs()));

            analysisBean.setHypecgd(Arrays.asList(ecgMonitorBean.getHypecgd()));
            analysisBean.setHyppcgd(Arrays.asList(ecgMonitorBean.getHyppcgd()));
            analysisBean.setHypapgd(Arrays.asList(ecgMonitorBean.getHypapgd()));

            analysisBean.setApgL(Arrays.asList(ecgMonitorBean.getApgL()));
            analysisBean.setApgR(Arrays.asList(ecgMonitorBean.getApgR()));
        }

        int dNum = ecgMonitorBean.getHypecgdRtopsNum();
        int sNum = ecgMonitorBean.getHypecgsRtopsNum();
        int lNum = ecgMonitorBean.getRtopNumL();
        if(lNum <= 0 ){
            lNum = mockEcgMonitorBean.getRtopNumL();
        }
        int rNum = ecgMonitorBean.getRtopNumR();
        if(rNum <= 0){
            rNum = ecgMonitorBean.getRtopNumR();
        }
        analysisBean.setHypecgsRtops(Arrays.asList(ecgMonitorBean.getHypecgsRtops()).subList(0, sNum));
        analysisBean.setHypecgdRtops(Arrays.asList(ecgMonitorBean.getHypecgdRtops()).subList(0, dNum));

        analysisBean.setHyppcgsIstarts(Arrays.asList(ecgMonitorBean.getHyppcgsIstarts()).subList(0, sNum));
        analysisBean.setHyppcgsIIstarts(Arrays.asList(ecgMonitorBean.getHyppcgsIIstarts()).subList(0, sNum));

        analysisBean.setHyppcgdIstarts(Arrays.asList(ecgMonitorBean.getHyppcgdIstarts()).subList(0, dNum));
        analysisBean.setHyppcgdIIstarts(Arrays.asList(ecgMonitorBean.getHyppcgdIIstarts()).subList(0, dNum));

        analysisBean.setHypapgsUSpnts(Arrays.asList(ecgMonitorBean.getHypapgsUSpnts()).subList(0, sNum)); //起
        analysisBean.setHypapgsDNpnts(Arrays.asList(ecgMonitorBean.getHypapgsDNpnts()).subList(0, sNum));
        analysisBean.setHypapgsUTpnts(Arrays.asList(ecgMonitorBean.getHypapgsUTpnts()).subList(0, sNum)); //终点

        analysisBean.setHypapgdUSpnts(Arrays.asList(ecgMonitorBean.getHypapgdUSpnts()).subList(0, dNum));
        analysisBean.setHypapgdDNpnts(Arrays.asList(ecgMonitorBean.getHypapgdDNpnts()).subList(0, dNum));
        analysisBean.setHypapgdUTpnts(Arrays.asList(ecgMonitorBean.getHypapgdUTpnts()).subList(0, dNum));

        if(ecgMonitorBean.getTsL() != null) {
            analysisBean.setTsL(Arrays.asList(ecgMonitorBean.getTsL()).subList(0, lNum));
        }else{
            analysisBean.setTsL(Arrays.asList(mockEcgMonitorBean.getTsL()).subList(0, lNum));
        }
        if(ecgMonitorBean.getUTL() != null) {
            analysisBean.setUTL(Arrays.asList(ecgMonitorBean.getUTL()).subList(0, lNum));
        }else{
            analysisBean.setTsL(Arrays.asList(mockEcgMonitorBean.getUTL()).subList(0, lNum));
        }
        if(ecgMonitorBean.getRtopL() != null) {
            analysisBean.setRtopL(Arrays.asList(ecgMonitorBean.getRtopL()).subList(0, lNum));
        }else{
            analysisBean.setRtopL(Arrays.asList(mockEcgMonitorBean.getRtopL()).subList(0, lNum));
        }

        if(ecgMonitorBean.getTsR() != null) {
            analysisBean.setTsR(Arrays.asList(ecgMonitorBean.getTsR()).subList(0, rNum));
        }else{
            analysisBean.setTsR(Arrays.asList(mockEcgMonitorBean.getTsR()).subList(0, rNum));
        }

        if(ecgMonitorBean.getUTR() != null) {
            analysisBean.setUTR(Arrays.asList(ecgMonitorBean.getUTR()).subList(0, rNum));
        }else{
            analysisBean.setTsR(Arrays.asList(mockEcgMonitorBean.getUTR()).subList(0, rNum));
        }
        if(ecgMonitorBean.getRtopR() != null) {
            analysisBean.setRtopR(Arrays.asList(ecgMonitorBean.getRtopR()).subList(0, rNum));
        }else{
            analysisBean.setRtopL(Arrays.asList(mockEcgMonitorBean.getRtopR()).subList(0, lNum));
        }

        analysisBean.setRtopNumL(ecgMonitorBean.getRtopNumL());
        analysisBean.setRtopNumR(ecgMonitorBean.getRtopNumR());
        analysisBean.setHypecgdRtopsNum(ecgMonitorBean.getHypecgdRtopsNum());
        analysisBean.setHypecgsRtopsNum(ecgMonitorBean.getHypecgsRtopsNum());

        analysisBean.setIDL((int)ecgMonitorBean.getIDL());
        analysisBean.setIDR((int)ecgMonitorBean.getIDR());

        analysisBean.setBuf(Arrays.asList(ecgMonitorBean.getBuf()));

        return analysisBean;
    }


    /**
     * 将ecgMonitorBean转化为画图对象
     * @param ecgMonitorBean
     * @param mContext
     * @return
     */
    public static AnalysisBean initByEcgMonitorBean(EcgMonitorBean ecgMonitorBean,Context mContext){
        AnalysisBean analysisBean = new AnalysisBean();

        if (ecgMonitorBean.getHypecgs() != null) {
            analysisBean.setHypecgs(Arrays.asList(ecgMonitorBean.getHypecgs()));
            analysisBean.setHyppcgs(Arrays.asList(ecgMonitorBean.getHyppcgs()));
            analysisBean.setHypapgs(Arrays.asList(ecgMonitorBean.getHypapgs()));

            analysisBean.setHypecgd(Arrays.asList(ecgMonitorBean.getHypecgd()));
            analysisBean.setHyppcgd(Arrays.asList(ecgMonitorBean.getHyppcgd()));
            analysisBean.setHypapgd(Arrays.asList(ecgMonitorBean.getHypapgd()));

            analysisBean.setApgL(Arrays.asList(ecgMonitorBean.getApgL()));
            analysisBean.setApgR(Arrays.asList(ecgMonitorBean.getApgR()));
        }else{  // 模拟数据
            analysisBean.setHypecgs(MockDataUtil.getEcgData(mContext).getData().getHypecgs());
            analysisBean.setHyppcgs(MockDataUtil.getEcgData(mContext).getData().getHyppcgs());
            analysisBean.setHypapgs(MockDataUtil.getEcgData(mContext).getData().getHypapgs());

            analysisBean.setHypecgd(MockDataUtil.getEcgData(mContext).getData().getHypecgd());
            analysisBean.setHyppcgd(MockDataUtil.getEcgData(mContext).getData().getHyppcgd());
            analysisBean.setHypapgd(MockDataUtil.getEcgData(mContext).getData().getHypapgd());

            analysisBean.setApgL(MockDataUtil.getEcgData(mContext).getData().getApgL());
            analysisBean.setApgR(MockDataUtil.getEcgData(mContext).getData().getApgR());
        }

        int dNum = ecgMonitorBean.getHypecgdRtopsNum();
        int sNum = ecgMonitorBean.getHypecgsRtopsNum();
        int lNum = ecgMonitorBean.getRtopNumL();
        if(lNum <= 0 ){
            lNum = ecgMonitorBean.getRtopNumL();
        }
        int rNum = ecgMonitorBean.getRtopNumR();
        if(rNum <= 0){
            rNum = ecgMonitorBean.getRtopNumR();
        }

        if(ecgMonitorBean.getHypecgsRtops() != null) {
            analysisBean.setHypecgsRtops(Arrays.asList(ecgMonitorBean.getHypecgsRtops()).subList(0, sNum));
        }

        if(ecgMonitorBean.getHypecgdRtops() != null) {
            analysisBean.setHypecgdRtops(Arrays.asList(ecgMonitorBean.getHypecgdRtops()).subList(0, dNum));
        }

        if(ecgMonitorBean.getHyppcgsIstarts() != null) {
            analysisBean.setHyppcgsIstarts(Arrays.asList(ecgMonitorBean.getHyppcgsIstarts()).subList(0, sNum));
        }

        if(ecgMonitorBean.getHyppcgsIIstarts() != null) {
            analysisBean.setHyppcgsIIstarts(Arrays.asList(ecgMonitorBean.getHyppcgsIIstarts()).subList(0, sNum));
        }

        if(ecgMonitorBean.getHyppcgdIstarts() != null) {
            analysisBean.setHyppcgdIstarts(Arrays.asList(ecgMonitorBean.getHyppcgdIstarts()).subList(0, dNum));
        }

        if(ecgMonitorBean.getHyppcgdIIstarts() != null) {
            analysisBean.setHyppcgdIIstarts(Arrays.asList(ecgMonitorBean.getHyppcgdIIstarts()).subList(0, dNum));
        }

        if(ecgMonitorBean.getHypapgsUSpnts() != null) {
            analysisBean.setHypapgsUSpnts(Arrays.asList(ecgMonitorBean.getHypapgsUSpnts()).subList(0, sNum)); //起
        }

        if(ecgMonitorBean.getHypapgsDNpnts() != null) {
            analysisBean.setHypapgsDNpnts(Arrays.asList(ecgMonitorBean.getHypapgsDNpnts()).subList(0, sNum));
        }

        if(ecgMonitorBean.getHypapgsUTpnts() != null) {
            analysisBean.setHypapgsUTpnts(Arrays.asList(ecgMonitorBean.getHypapgsUTpnts()).subList(0, sNum)); //终点
        }

        if(ecgMonitorBean.getHypapgdUSpnts() != null) {
            analysisBean.setHypapgdUSpnts(Arrays.asList(ecgMonitorBean.getHypapgdUSpnts()).subList(0, dNum));
        }

        if(ecgMonitorBean.getHypapgdDNpnts() != null) {
            analysisBean.setHypapgdDNpnts(Arrays.asList(ecgMonitorBean.getHypapgdDNpnts()).subList(0, dNum));
        }

        if(ecgMonitorBean.getHypapgdUTpnts() != null) {
            analysisBean.setHypapgdUTpnts(Arrays.asList(ecgMonitorBean.getHypapgdUTpnts()).subList(0, dNum));
        }

        if(ecgMonitorBean.getTsL() != null) {
            analysisBean.setTsL(Arrays.asList(ecgMonitorBean.getTsL()).subList(0, lNum));
        }
        if(ecgMonitorBean.getUTL() != null) {
            analysisBean.setUTL(Arrays.asList(ecgMonitorBean.getUTL()).subList(0, lNum));
        }
        if(ecgMonitorBean.getRtopL() != null) {
            analysisBean.setRtopL(Arrays.asList(ecgMonitorBean.getRtopL()).subList(0, lNum));
        }

        if(ecgMonitorBean.getTsR() != null) {
            analysisBean.setTsR(Arrays.asList(ecgMonitorBean.getTsR()).subList(0, rNum));
        }

        if(ecgMonitorBean.getUTR() != null) {
            analysisBean.setUTR(Arrays.asList(ecgMonitorBean.getUTR()).subList(0, rNum));
        }
        if(ecgMonitorBean.getRtopR() != null) {
            analysisBean.setRtopR(Arrays.asList(ecgMonitorBean.getRtopR()).subList(0, rNum));
        }

        analysisBean.setRtopNumL(ecgMonitorBean.getRtopNumL());
        analysisBean.setRtopNumR(ecgMonitorBean.getRtopNumR());
        analysisBean.setHypecgdRtopsNum(ecgMonitorBean.getHypecgdRtopsNum());
        analysisBean.setHypecgsRtopsNum(ecgMonitorBean.getHypecgsRtopsNum());

        analysisBean.setIDL((int)ecgMonitorBean.getIDL());
        analysisBean.setIDR((int)ecgMonitorBean.getIDR());

        analysisBean.setBuf(Arrays.asList(ecgMonitorBean.getBuf()));

        analysisBean.setCBPs(ecgMonitorBean.getCBPs());
        analysisBean.setjAdL(ecgMonitorBean.getjAdL());
        analysisBean.setjAdR(ecgMonitorBean.getjAdR());
        analysisBean.setjAsL(ecgMonitorBean.getjAsL());
        analysisBean.setjAsR(ecgMonitorBean.getjAsR());
        analysisBean.setjPssL(ecgMonitorBean.getjPssL());
        analysisBean.setjPssR(ecgMonitorBean.getjPssR());
        analysisBean.setjTL(ecgMonitorBean.getjTL());
        analysisBean.setjTR(ecgMonitorBean.getjTR());
        analysisBean.setjTsL(ecgMonitorBean.getjTsL());
        analysisBean.setjTsR(ecgMonitorBean.getjTsR());

        return analysisBean;
    }


    public static EcgMonitorBean initFourteenDataByMock(EcgMonitorBean ecgMonitorBean,Context context){
        ecgMonitorBean.setHypecgs(getEcgData(context).getData().getHypecgs().toArray(
                new Float[getEcgData(context).getData().getHypecgs().size()]));
        ecgMonitorBean.setHyppcgs(getEcgData(context).getData().getHyppcgs().toArray(
                new Float[getEcgData(context).getData().getHyppcgs().size()]));
        ecgMonitorBean.setHypapgs(getEcgData(context).getData().getHypapgs().toArray(
                new Float[getEcgData(context).getData().getHypapgs().size()]
        ));
        ecgMonitorBean.setHypecgd(getEcgData(context).getData().getHypecgd().toArray(
                new Float[getEcgData(context).getData().getHypecgd().size()]
        ));
        ecgMonitorBean.setHyppcgd(getEcgData(context).getData().getHyppcgd().toArray(
                new Float[getEcgData(context).getData().getHyppcgd().size()]
        ));
        ecgMonitorBean.setHypapgd(getEcgData(context).getData().getHypapgd().toArray(
                new Float[getEcgData(context).getData().getHypapgd().size()]
        ));
        ecgMonitorBean.setEcgPl(getEcgData(context).getData().getEcgPl().toArray(
                new Float[getEcgData(context).getData().getEcgPl().size()]
        ));
        ecgMonitorBean.setPcgPl(getEcgData(context).getData().getPcgPl().toArray(
                new Float[getEcgData(context).getData().getPcgPl().size()]
        ));
        ecgMonitorBean.setApgL(getEcgData(context).getData().getApgL().toArray(
                new Float[getEcgData(context).getData().getApgL().size()]
        ));
        ecgMonitorBean.setEcgPr(getEcgData(context).getData().getEcgPr().toArray(
                new Float[getEcgData(context).getData().getEcgPr().size()]
        ));
        ecgMonitorBean.setPcgPr(getEcgData(context).getData().getPcgPr().toArray(
                new Float[getEcgData(context).getData().getPcgPr().size()]
        ));
        ecgMonitorBean.setApgR(getEcgData(context).getData().getApgR().toArray(
                new Float[getEcgData(context).getData().getApgR().size()]
        ));
        ecgMonitorBean.setEcgTh(getEcgData(context).getData().getEcgTh().toArray(
                new Float[getEcgData(context).getData().getEcgTh().size()]
        ));
        ecgMonitorBean.setApgTh(getEcgData(context).getData().getApgTh().toArray(
                new Float[getEcgData(context).getData().getApgTh().size()]
        ));

        return ecgMonitorBean;
    }


}