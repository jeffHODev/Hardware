package com.hekangyuan.nivi1000.utils;

/**
 * Created by liuchao on 2020/2/5.
 * <p>
 * http://doclever.cn/controller/console/console.html
 */
public class Constants {
    public static String token = "";
//    public static final String url="http://192.168.0.105:8080/";
    //    public static final String url="http://192.168.0.154:8080/";
    public static final String url = "http://123.57.185.1:8080/";


    public static final String[][] ANALYSIS_ECGTITLE = {{"心电波", "心音波", "超收缩压脉搏"},
            {"心电波", "心音波", "低舒张压脉搏"},
            {"左颈动脉脉搏波", "右颈动脉脉搏波", "分析结果"}};

    public static final String[] ANALYSIS_TITLE = {"分析超收缩压", "分析低舒张压", "分析中央主动脉脉搏波"};

    public static final int MEASURE_MODE_SYNC = 1;
    public static final int MEASURE_MODE_ASYNC = 2;


    public static final String SP_MODE_MAP= "modeMap";
    public static final String SP_HOSPITAL_NAME_MAP = "nameMap";

    public static final String SP_HOSPITAL_NAME = "hospitalName";
    public static final String SP_MODE = "mode";

}
