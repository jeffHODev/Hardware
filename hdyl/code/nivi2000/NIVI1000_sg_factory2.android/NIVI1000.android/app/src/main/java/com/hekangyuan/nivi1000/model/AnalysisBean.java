package com.hekangyuan.nivi1000.model;

import java.util.List;

/**
 * @author huang
 *
 * 分析页面绘制需要的数据类
 *
 * 包括超收缩压，低舒张压，左右脉搏8个波形数据
 *
 * 同时包括Rtops，I，II，
 *
 *     rPointList //心电Rtops
 *     iPointList  //第一
 *     iiPointList  //第二
 *     usPointList  //起始点
 *     dnPointList  //拐点
 *     utPointList  //终点
 */
public class AnalysisBean {

    private List<Float>   hypecgs; //超收缩压心电
    private List<Float>   hyppcgs;//超收缩压心音
    private List<Float>  hypapgs;//超收缩压脉搏
    private List<Float>  hypecgd;//低舒张压心电
    private List<Float>  hyppcgd;//低舒张压心音
    private List<Float>  hypapgd;//低舒张压脉搏
    private List<Float>  apgL;//左颈动脉脉搏
    private List<Float>  apgR;//右颈动脉脉搏


    private List<Float> hyppcgdIstarts;
    private List<Float> hypapgdDNpnts;
    private List<Float> hypapgdUTpnts;
    private List<Float> TsL;
    private List<Float> UTL;
    private List<Integer> RtopL;
    private List<Float> TsR;
    private List<Float> UTR;
    private List<Integer> RtopR;
    private List<Float> hypapgsUTpnts;
    private List<Float> hyppcgsIstarts;
    private List<Float> hyppcgdIIstarts;
    private List<Float> hyppcgsIIstarts;
    private List<Float> hypapgsUSpnts;
    private List<Float> hypecgdRtops;
    private List<Float> hypapgsDNpnts;
    private List<Float> buf;
    private List<Float> hypecgsRtops;
    private List<Float> hypapgdUSpnts;

    private int RtopNumR;
    private int RtopNumL;
    private int hypecgsRtopsNum;
    private int hypecgdRtopsNum;

    private int IDL;
    private int IDR;

    private float CBPs;
    private float jAdL;
    private float jAsL;
    private float jPssL;
    private float jTL;
    private float jTsL;
    private float jAdR;
    private float jAsR;
    private float jPssR;
    private float jTR;
    private float jTsR;

    public float getCBPs() {
        return CBPs;
    }

    public void setCBPs(float CBPs) {
        this.CBPs = CBPs;
    }

    public float getjAdL() {
        return jAdL;
    }

    public void setjAdL(float jAdL) {
        this.jAdL = jAdL;
    }

    public float getjAsL() {
        return jAsL;
    }

    public void setjAsL(float jAsL) {
        this.jAsL = jAsL;
    }

    public float getjPssL() {
        return jPssL;
    }

    public void setjPssL(float jPssL) {
        this.jPssL = jPssL;
    }

    public float getjTL() {
        return jTL;
    }

    public void setjTL(float jTL) {
        this.jTL = jTL;
    }

    public float getjTsL() {
        return jTsL;
    }

    public void setjTsL(float jTsL) {
        this.jTsL = jTsL;
    }

    public float getjAdR() {
        return jAdR;
    }

    public void setjAdR(float jAdR) {
        this.jAdR = jAdR;
    }

    public float getjAsR() {
        return jAsR;
    }

    public void setjAsR(float jAsR) {
        this.jAsR = jAsR;
    }

    public float getjPssR() {
        return jPssR;
    }

    public void setjPssR(float jPssR) {
        this.jPssR = jPssR;
    }

    public float getjTR() {
        return jTR;
    }

    public void setjTR(float jTR) {
        this.jTR = jTR;
    }

    public float getjTsR() {
        return jTsR;
    }

    public void setjTsR(float jTsR) {
        this.jTsR = jTsR;
    }

    public List<Float> getHypecgs() {
        return hypecgs;
    }

    public void setHypecgs(List<Float> hypecgs) {
        this.hypecgs = hypecgs;
    }

    public List<Float> getHyppcgs() {
        return hyppcgs;
    }

    public void setHyppcgs(List<Float> hyppcgs) {
        this.hyppcgs = hyppcgs;
    }

    public List<Float> getHypapgs() {
        return hypapgs;
    }

    public void setHypapgs(List<Float> hypapgs) {
        this.hypapgs = hypapgs;
    }

    public List<Float> getHypecgd() {
        return hypecgd;
    }

    public void setHypecgd(List<Float> hypecgd) {
        this.hypecgd = hypecgd;
    }

    public List<Float> getHyppcgd() {
        return hyppcgd;
    }

    public void setHyppcgd(List<Float> hyppcgd) {
        this.hyppcgd = hyppcgd;
    }

    public List<Float> getHypapgd() {
        return hypapgd;
    }

    public void setHypapgd(List<Float> hypapgd) {
        this.hypapgd = hypapgd;
    }

    public List<Float> getApgL() {
        return apgL;
    }

    public void setApgL(List<Float> apgL) {
        this.apgL = apgL;
    }

    public List<Float> getApgR() {
        return apgR;
    }

    public void setApgR(List<Float> apgR) {
        this.apgR = apgR;
    }

    public List<Float> getHyppcgdIstarts() {
        return hyppcgdIstarts;
    }

    public void setHyppcgdIstarts(List<Float> hyppcgdIstarts) {
        this.hyppcgdIstarts = hyppcgdIstarts;
    }

    public List<Float> getHypapgdDNpnts() {
        return hypapgdDNpnts;
    }

    public void setHypapgdDNpnts(List<Float> hypapgdDNpnts) {
        this.hypapgdDNpnts = hypapgdDNpnts;
    }

    public List<Float> getHypapgdUTpnts() {
        return hypapgdUTpnts;
    }

    public void setHypapgdUTpnts(List<Float> hypapgdUTpnts) {
        this.hypapgdUTpnts = hypapgdUTpnts;
    }

    public List<Float> getTsL() {
        return TsL;
    }

    public void setTsL(List<Float> tsL) {
        TsL = tsL;
    }

    public List<Float> getUTL() {
        return UTL;
    }

    public void setUTL(List<Float> UTL) {
        this.UTL = UTL;
    }

    public List<Integer> getRtopL() {
        return RtopL;
    }

    public void setRtopL(List<Integer> rtopL) {
        RtopL = rtopL;
    }

    public List<Float> getTsR() {
        return TsR;
    }

    public void setTsR(List<Float> tsR) {
        TsR = tsR;
    }

    public List<Float> getUTR() {
        return UTR;
    }

    public void setUTR(List<Float> UTR) {
        this.UTR = UTR;
    }

    public List<Integer> getRtopR() {
        return RtopR;
    }

    public void setRtopR(List<Integer> rtopR) {
        RtopR = rtopR;
    }

    public List<Float> getHypapgsUTpnts() {
        return hypapgsUTpnts;
    }

    public void setHypapgsUTpnts(List<Float> hypapgsUTpnts) {
        this.hypapgsUTpnts = hypapgsUTpnts;
    }

    public List<Float> getHyppcgsIstarts() {
        return hyppcgsIstarts;
    }

    public void setHyppcgsIstarts(List<Float> hyppcgsIstarts) {
        this.hyppcgsIstarts = hyppcgsIstarts;
    }

    public List<Float> getHyppcgdIIstarts() {
        return hyppcgdIIstarts;
    }

    public void setHyppcgdIIstarts(List<Float> hyppcgdIIstarts) {
        this.hyppcgdIIstarts = hyppcgdIIstarts;
    }

    public List<Float> getHyppcgsIIstarts() {
        return hyppcgsIIstarts;
    }

    public void setHyppcgsIIstarts(List<Float> hyppcgsIIstarts) {
        this.hyppcgsIIstarts = hyppcgsIIstarts;
    }

    public List<Float> getHypapgsUSpnts() {
        return hypapgsUSpnts;
    }

    public void setHypapgsUSpnts(List<Float> hypapgsUSpnts) {
        this.hypapgsUSpnts = hypapgsUSpnts;
    }

    public List<Float> getHypecgdRtops() {
        return hypecgdRtops;
    }

    public void setHypecgdRtops(List<Float> hypecgdRtops) {
        this.hypecgdRtops = hypecgdRtops;
    }

    public List<Float> getHypapgsDNpnts() {
        return hypapgsDNpnts;
    }

    public void setHypapgsDNpnts(List<Float> hypapgsDNpnts) {
        this.hypapgsDNpnts = hypapgsDNpnts;
    }

    public List<Float> getBuf() {
        return buf;
    }

    public void setBuf(List<Float> buf) {
        this.buf = buf;
    }

    public List<Float> getHypecgsRtops() {
        return hypecgsRtops;
    }

    public void setHypecgsRtops(List<Float> hypecgsRtops) {
        this.hypecgsRtops = hypecgsRtops;
    }

    public List<Float> getHypapgdUSpnts() {
        return hypapgdUSpnts;
    }

    public void setHypapgdUSpnts(List<Float> hypapgdUSpnts) {
        this.hypapgdUSpnts = hypapgdUSpnts;
    }

    public int getRtopNumR() {
        return RtopNumR;
    }

    public void setRtopNumR(int rtopNumR) {
        RtopNumR = rtopNumR;
    }

    public int getRtopNumL() {
        return RtopNumL;
    }

    public void setRtopNumL(int rtopNumL) {
        RtopNumL = rtopNumL;
    }

    public int getHypecgsRtopsNum() {
        return hypecgsRtopsNum;
    }

    public void setHypecgsRtopsNum(int hypecgsRtopsNum) {
        this.hypecgsRtopsNum = hypecgsRtopsNum;
    }

    public int getHypecgdRtopsNum() {
        return hypecgdRtopsNum;
    }

    public void setHypecgdRtopsNum(int hypecgdRtopsNum) {
        this.hypecgdRtopsNum = hypecgdRtopsNum;
    }

    public int getIDL() {
        return IDL;
    }

    public void setIDL(int IDL) {
        this.IDL = IDL;
    }

    public int getIDR() {
        return IDR;
    }

    public void setIDR(int IDR) {
        this.IDR = IDR;
    }
}
