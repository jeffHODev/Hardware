package com.hekangyuan.nivi1000.model;

import java.io.Serializable;

/**
 *         "hyppcgdIstarts": + [51]                //类型：Array  必有字段  备注：低舒张第一心音
 *         "prResult":1.1096381,                //类型：Number  必有字段  备注：冠状动脉页面PR
 *         "jsvResult":1.3179045,                //类型：Number  必有字段  备注：心脏搏出量结果
 *         "hypapgdDNpnts": + [51]                //类型：Array  必有字段  备注：低舒张脉搏波DN
 *         "hypapgdUTpnts": + [51]                //类型：Array  必有字段  备注：低舒张脉搏波UT
 *         "IDL":2,                //类型：Number  必有字段  备注：左颈动脉选择波形
 *         "jpaResult":89,                //类型：Number  必有字段  备注：jpm平均压的结果
 *         "jpsResult":123,                //类型：Number  必有字段  备注：ps收缩压结果
 *         "IDR":2,                //类型：Number  必有字段  备注：右颈动脉选择波形
 *         "gNPSType":4,                //类型：Number  必有字段  备注：18个方格闪烁提示
 *         "cResult":1.0206256,                //类型：Number  必有字段  备注：冠状动脉页面C
 *         "hypapgsUTpnts": + [51]                //类型：Array  必有字段  备注：超收缩脉搏波UT
 *         "TsL": + [30]                //类型：Array  必有字段  备注：左颈动脉画图TS  DN 拐点
 *         "TsR": + [30]                //类型：Array  必有字段  备注：右颈动脉画图TS  DN 拐点
 *         "UTL": + [30]                //类型：Array  必有字段  备注：左颈动脉画图UT  UT 最高点
 *         "UTR": + [30]                //类型：Array  必有字段  备注：右颈动脉画图UT  UT 最高点
 *         "jpdResult":72,                //类型：Number  必有字段  备注：舒张压结果
 *         "RtopL": + [30]                //类型：Array  必有字段  备注：左颈动脉画图Rtop  US 起始点
 *         "RtopR": + [30]                //类型：Array  必有字段  备注：右颈动脉画图Rtop  US 起始点
 *         "hyppcgsIstarts": + [51]                //类型：Array  必有字段  备注：超收缩第一心音
 *         "hyppcgdIIstarts": + [51]                //类型：Array  必有字段  备注：低舒张第二心音
 *         "hyppcgsIIstarts": + [51]                //类型：Array  必有字段  备注：超收缩第二心音
 *         "hypapgsUSpnts": + [51]                //类型：Array  必有字段  备注：超收缩脉搏波US
 *         "hypecgdRtops": + [51]                //类型：Array  必有字段  备注：低舒张心电Rtops
 *         "hypapgsDNpnts": + [51]                //类型：Array  必有字段  备注：超收缩脉搏波DN
 *         "buf": + [2000]                //类型：Array  必有字段  备注：分析出来的波形结果
 *         "hypecgsRtops": + [51]                //类型：Array  必有字段  备注：超收缩心电Rtops
 *         "jbrResult":1.0031527,                //类型：Number  必有字段  备注：血流总阻力结果
 *         "hypapgdUSpnts": + [51]                //类型：Array  必有字段  备注：低舒张脉搏波US
 *         "pssResult":158.83612,                //类型：Number  必有字段  备注：拐点压结果
 *         "rect2":9,                //类型：Number  必有字段  备注：对应十个风险等级中的一个
 *         "qResult":0.93341786,                //类型：Number  必有字段  备注：冠状动脉页面Q
 *         "ascResult":0.9799036                //类型：Number  必有字段  备注：冠状动脉页面ASC
 */
public class EcgMonitorBean   implements Serializable {
    private Float []   hypecgs; //超收缩压心电
    private Float []   hyppcgs;//超收缩压心音
    private Float  []  hypapgs;//超收缩压脉搏
    private Float  []  hypecgd;//低舒张压心电
    private Float  []  hyppcgd;//低舒张压心音
    private Float  []  hypapgd;//低舒张压脉搏
    private Float  []  ecgPl;//左颈动脉心电
    private Float  []  pcgPl;//左颈动脉心音
    private Float  []  apgL;//左颈动脉脉搏
    private Float  []  ecgPr;//右颈动脉心电
    private Float  []  pcgPr;//右颈动脉心音
    private Float  []  apgR;//右颈动脉脉搏
    private Float  []  ecgTh;//股动脉心电
    private Float  []  apgTh;//股动脉脉搏

    //返回
    private Float [] hyppcgdIstarts;
    private Float [] hyppcgsIstarts;
    private Float [] hyppcgdIIstarts;
    private Float [] hyppcgsIIstarts;
    private Float [] hypapgdDNpnts;
    private Float [] hypapgsUSpnts;
    private Float [] hypapgdUTpnts;
    private Float [] hypecgdRtops;
    private Float [] hypapgsDNpnts;
    private Float [] hypecgsRtops;
    private Float [] hypapgdUSpnts;
    private Float [] hypapgsUTpnts;
    private String    basisMeasurementId;//测试号
    private String detectCount;// 剩余检测次数

    private float prResult;
    private float prResultR;
    private float jsvResult;

    private float IDL;
    private float IDR;
    private float jpaResult;
    private float jpsResult;
    private float gNPSType;

    private float cResult;
    private float cResultR;
    private float jpdResult;
    private float jbrResult;
    private float pssResult;
    private float qResult;
    private float qResultR;
    private float ascResult;
    private float ascResultR;

    private float rect2;

    private Float[] TsL;
    private Float[] TsR;
    private Float[] UTL;
    private Float[] UTR;
    private Integer[] RtopL;
    private Integer[] RtopR;

    private Float[] buf;

    private int RtopNumR;
    private int RtopNumL;
    private int hypecgsRtopsNum;
    private int hypecgdRtopsNum;

    private float PTV;
    private float PTT;
    private int age;

    private float idl;
    private float idr;
    private int model;
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

    public float getCBPs() {
        return CBPs;
    }

    public void setCBPs(float CBPs) {
        this.CBPs = CBPs;
    }

    public int getModel() {
        return model;
    }

    public void setModel(int model) {
        this.model = model;
    }

    public float getPrResultR() {
        return prResultR;
    }

    public void setPrResultR(float prResultR) {
        this.prResultR = prResultR;
    }

    public float getcResultR() {
        return cResultR;
    }

    public void setcResultR(float cResultR) {
        this.cResultR = cResultR;
    }

    public float getqResultR() {
        return qResultR;
    }

    public void setqResultR(float qResultR) {
        this.qResultR = qResultR;
    }

    public float getAscResultR() {
        return ascResultR;
    }

    public void setAscResultR(float ascResultR) {
        this.ascResultR = ascResultR;
    }

    public float getIdl() {
        return idl;
    }

    public void setIdl(float idl) {
        this.idl = idl;
    }

    public float getIdr() {
        return idr;
    }

    public void setIdr(float idr) {
        this.idr = idr;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    public Float[] getHyppcgdIstarts() {
        return hyppcgdIstarts;
    }

    public void setHyppcgdIstarts(Float[] hyppcgdIstarts) {
        this.hyppcgdIstarts = hyppcgdIstarts;
    }

    public Float[] getHyppcgsIstarts() {
        return hyppcgsIstarts;
    }

    public void setHyppcgsIstarts(Float[] hyppcgsIstarts) {
        this.hyppcgsIstarts = hyppcgsIstarts;
    }

    public Float[] getHyppcgdIIstarts() {
        return hyppcgdIIstarts;
    }

    public void setHyppcgdIIstarts(Float[] hyppcgdIIstarts) {
        this.hyppcgdIIstarts = hyppcgdIIstarts;
    }

    public Float[] getHyppcgsIIstarts() {
        return hyppcgsIIstarts;
    }

    public void setHyppcgsIIstarts(Float[] hyppcgsIIstarts) {
        this.hyppcgsIIstarts = hyppcgsIIstarts;
    }

    public Float[] getHypapgdDNpnts() {
        return hypapgdDNpnts;
    }

    public void setHypapgdDNpnts(Float[] hypapgdDNpnts) {
        this.hypapgdDNpnts = hypapgdDNpnts;
    }

    public Float[] getHypapgsUSpnts() {
        return hypapgsUSpnts;
    }

    public void setHypapgsUSpnts(Float[] hypapgsUSpnts) {
        this.hypapgsUSpnts = hypapgsUSpnts;
    }

    public Float[] getHypapgdUTpnts() {
        return hypapgdUTpnts;
    }

    public void setHypapgdUTpnts(Float[] hypapgdUTpnts) {
        this.hypapgdUTpnts = hypapgdUTpnts;
    }

    public Float[] getHypecgdRtops() {
        return hypecgdRtops;
    }

    public void setHypecgdRtops(Float[] hypecgdRtops) {
        this.hypecgdRtops = hypecgdRtops;
    }

    public Float[] getHypapgsDNpnts() {
        return hypapgsDNpnts;
    }

    public void setHypapgsDNpnts(Float[] hypapgsDNpnts) {
        this.hypapgsDNpnts = hypapgsDNpnts;
    }

    public Float[] getHypecgsRtops() {
        return hypecgsRtops;
    }

    public void setHypecgsRtops(Float[] hypecgsRtops) {
        this.hypecgsRtops = hypecgsRtops;
    }

    public Float[] getHypapgdUSpnts() {
        return hypapgdUSpnts;
    }

    public void setHypapgdUSpnts(Float[] hypapgdUSpnts) {
        this.hypapgdUSpnts = hypapgdUSpnts;
    }

    public Float[] getHypapgsUTpnts() {
        return hypapgsUTpnts;
    }

    public void setHypapgsUTpnts(Float[] hypapgsUTpnts) {
        this.hypapgsUTpnts = hypapgsUTpnts;
    }

    public String getDetectCount() {
        return detectCount;
    }

    public void setDetectCount(String detectCount) {
        this.detectCount = detectCount;
    }


    public Float[] getHypecgs() {
        return hypecgs;
    }

    public void setHypecgs(Float[] hypecgs) {
        this.hypecgs = hypecgs;
    }

    public Float[] getHyppcgs() {
        return hyppcgs;
    }

    public void setHyppcgs(Float[] hyppcgs) {
        this.hyppcgs = hyppcgs;
    }

    public Float[] getHypapgs() {
        return hypapgs;
    }

    public void setHypapgs(Float[] hypapgs) {
        this.hypapgs = hypapgs;
    }

    public Float[] getHypecgd() {
        return hypecgd;
    }

    public void setHypecgd(Float[] hypecgd) {
        this.hypecgd = hypecgd;
    }

    public Float[] getHyppcgd() {
        return hyppcgd;
    }

    public void setHyppcgd(Float[] hyppcgd) {
        this.hyppcgd = hyppcgd;
    }

    public Float[] getHypapgd() {
        return hypapgd;
    }

    public void setHypapgd(Float[] hypapgd) {
        this.hypapgd = hypapgd;
    }

    public Float[] getEcgPl() {
        return ecgPl;
    }

    public void setEcgPl(Float[] ecgPl) {
        this.ecgPl = ecgPl;
    }

    public Float[] getPcgPl() {
        return pcgPl;
    }

    public void setPcgPl(Float[] pcgPl) {
        this.pcgPl = pcgPl;
    }

    public Float[] getApgL() {
        return apgL;
    }

    public void setApgL(Float[] apgL) {
        this.apgL = apgL;
    }

    public Float[] getEcgPr() {
        return ecgPr;
    }

    public void setEcgPr(Float[] ecgPr) {
        this.ecgPr = ecgPr;
    }

    public Float[] getPcgPr() {
        return pcgPr;
    }

    public void setPcgPr(Float[] pcgPr) {
        this.pcgPr = pcgPr;
    }

    public Float[] getApgR() {
        return apgR;
    }

    public void setApgR(Float[] apgR) {
        this.apgR = apgR;
    }

    public Float[] getEcgTh() {
        return ecgTh;
    }

    public void setEcgTh(Float[] ecgTh) {
        this.ecgTh = ecgTh;
    }

    public Float[] getApgTh() {
        return apgTh;
    }

    public void setApgTh(Float[] apgTh) {
        this.apgTh = apgTh;
    }

    public String getBasisMeasurementId() {
        return basisMeasurementId;
    }

    public void setBasisMeasurementId(String basisMeasurementId) {
        this.basisMeasurementId = basisMeasurementId;
    }


    public float getPrResult() {
        return prResult;
    }

    public void setPrResult(float prResult) {
        this.prResult = prResult;
    }

    public float getJsvResult() {
        return jsvResult;
    }

    public void setJsvResult(float jsvResult) {
        this.jsvResult = jsvResult;
    }

    public float getIDL() {
        return IDL;
    }

    public void setIDL(float IDL) {
        this.IDL = IDL;
    }

    public float getIDR() {
        return IDR;
    }

    public void setIDR(float IDR) {
        this.IDR = IDR;
    }

    public float getJpaResult() {
        return jpaResult;
    }

    public void setJpaResult(float jpaResult) {
        this.jpaResult = jpaResult;
    }

    public float getJpsResult() {
        return jpsResult;
    }

    public void setJpsResult(float jpsResult) {
        this.jpsResult = jpsResult;
    }

    public float getgNPSType() {
        return gNPSType;
    }

    public void setgNPSType(float gNPSType) {
        this.gNPSType = gNPSType;
    }

    public void setRect2(float rect2) {
        this.rect2 = rect2;
    }

    public float getcResult() {
        return cResult;
    }

    public void setcResult(float cResult) {
        this.cResult = cResult;
    }

    public float getJpdResult() {
        return jpdResult;
    }

    public void setJpdResult(float jpdResult) {
        this.jpdResult = jpdResult;
    }

    public float getJbrResult() {
        return jbrResult;
    }

    public void setJbrResult(float jbrResult) {
        this.jbrResult = jbrResult;
    }

    public float getPssResult() {
        return pssResult;
    }

    public void setPssResult(float pssResult) {
        this.pssResult = pssResult;
    }

    public float getqResult() {
        return qResult;
    }

    public void setqResult(float qResult) {
        this.qResult = qResult;
    }

    public float getAscResult() {
        return ascResult;
    }

    public void setAscResult(float ascResult) {
        this.ascResult = ascResult;
    }

    public float getRect2() {
        return rect2;
    }

    public Float[] getTsL() {
        return TsL;
    }

    public void setTsL(Float[] tsL) {
        TsL = tsL;
    }

    public Float[] getTsR() {
        return TsR;
    }

    public void setTsR(Float[] tsR) {
        TsR = tsR;
    }

    public Float[] getUTL() {
        return UTL;
    }

    public void setUTL(Float[] UTL) {
        this.UTL = UTL;
    }

    public Float[] getUTR() {
        return UTR;
    }

    public void setUTR(Float[] UTR) {
        this.UTR = UTR;
    }

    public Integer[] getRtopL() {
        return RtopL;
    }

    public void setRtopL(Integer[] rtopL) {
        RtopL = rtopL;
    }

    public Integer[] getRtopR() {
        return RtopR;
    }

    public void setRtopR(Integer[] rtopR) {
        RtopR = rtopR;
    }

    public Float[] getBuf() {
        return buf;
    }

    public void setBuf(Float[] buf) {
        this.buf = buf;
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

    public float getPTV() {
        return PTV;
    }

    public void setPTV(float PTV) {
        this.PTV = PTV;
    }

    public float getPTT() {
        return PTT;
    }

    public void setPTT(float PTT) {
        this.PTT = PTT;
    }
}
