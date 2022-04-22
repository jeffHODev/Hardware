package com.nivi.nivifilter;

import android.util.Log;

public class SerialBeanFilterHead {
    private static final double a[][] = { //headfilter bandpass 0.1 - 30Hz fs833
            {1.00, -1.823487434399284	, 0.871129966057866},
            {1.00, -1.999535870295009	, 0.999536439820794},
            {1.00, -1.998777249164059	, 0.998777822721504},
            {1.00, -1.651621587333797	, 0.694450026198119},
            {1.00, -1.796394348798264	, 0.796548320188404}
    };

    private static final double b[] = { 1.00, 0.00, -1.00 }; //第一级二阶滤波，分子

    private static final double gain[] = { //headfilter bandpass 0.1 - 30Hz fs833
            0.108829181059523,
            0.108829181059523,
            0.103548983618821,
            0.103548983618821,
            0.101725839905798
    };
//        private static final double a[][] = { //headfilter bandpass 0.1 - 30Hz fs833
//            {1.00, -1.883365066929218	, 0.932618544086676},
//            {1.00, -1.999764952860458	, 0.999765521920495},
//            {1.00, -1.999317477217735	, 0.999318047512131},
//            {1.00, -1.770046380336094	, 0.816224399174864},
//            {1.00, -1.998933325078590	, 0.998933897493982},
//            {1.00, -1.683945364232128	, 0.727703707747404},
//            {1.00, -1.998650948339432	, 0.998651522939602},
//            {1.00, -1.626513040424320	, 0.668610160093628},
//            {1.00, -1.998501060633446	, 0.998501636608796},
//            {1.00, -1.597879915073089	, 0.639132809867411}
//    };
//
//    private static final double b[] = { 1.00, 0.00, -1.00 }; //第一级二阶滤波，分子
//
//    private static final double gain[] = { //headfilter bandpass 0.1 - 30Hz fs833
//            0.110608563597083,
//            0.110608563597083,
//            0.107215714898926,
//            0.107215714898926,
//            0.104562836580281,
//            0.104562836580281,
//            0.102754377225928,
//            0.102754377225928,
//            0.101840476085343,
//            0.101840476085343
//    };

    public static double[] m00 = {0};
    public static double[] m01 = {0};

    public static double[] m10 = {0};
    public static double[] m11 = {0};

    public static double[] m20 = {0};
    public static double[] m21 = {0};

    public static double[] m30 = {0};
    public static double[] m31 = {0};

    public static double[] m40 = {0};
    public static double[] m41 = {0};

    public static double[] m50 = {0};
    public static double[] m51 = {0};

    public static double[] m60 = {0};
    public static double[] m61 = {0};

    public static double[] m70 = {0};
    public static double[] m71 = {0};

    public static double[] m80 = {0};
    public static double[] m81 = {0};

    public static double[] m90 = {0};
    public static double[] m91 = {0};

    public static float serialBeanFilter(float x) {
        double y0 = filterOrder2(x, m00, m01, a[0], b, gain[0]);  //第一级二阶滤波
        double y1 = filterOrder2(y0, m10, m11, a[1], b, gain[1]);  //第二级二阶滤波
        double y2 = filterOrder2(y1, m20, m21, a[2], b, gain[2]);  //第二级二阶滤波
        double y3 = filterOrder2(y2, m30, m31, a[3], b, gain[3]);  //第二级二阶滤波
        double y4 = filterOrder2(y3, m40, m41, a[4], b, gain[4]);  //第二级二阶滤波

        return (float) y4;
    }
//        public static float serialBeanFilter(float x) {
//        double y0 = filterOrder2(x, m00, m01, a[0], b, gain[0]);  //第一级二阶滤波
//        double y1 = filterOrder2(y0, m10, m11, a[1], b, gain[1]);  //第二级二阶滤波
//        double y2 = filterOrder2(y1, m20, m21, a[2], b, gain[2]);  //第二级二阶滤波
//        double y3 = filterOrder2(y2, m30, m31, a[3], b, gain[3]);  //第二级二阶滤波
//        double y4 = filterOrder2(y3, m40, m41, a[4], b, gain[4]);  //第二级二阶滤波
//        double y5 = filterOrder2(y4, m50, m51, a[5], b, gain[5]);  //第二级二阶滤波
//        double y6 = filterOrder2(y5, m60, m61, a[6], b, gain[6]);  //第二级二阶滤波
//        double y7 = filterOrder2(y6, m70, m71, a[7], b, gain[7]);  //第二级二阶滤波
//        double y8 = filterOrder2(y7, m80, m81, a[8], b, gain[8]);  //第二级二阶滤波
//        double y9 = filterOrder2(y8, m90, m91, a[9], b, gain[9]);  //第二级二阶滤波
//
//        return (float)y9;
//
//    }


    /*
    x: input
    m1:保存中间滤波器数据
    m2:保存中间滤波器数据
    a:滤波器系数
    b:滤波器系数
    gain:增益，对于只支持定点小数运算的，需要把增益分配到每个二阶IIR滤波器的系数中，使得每次中间的结果都不溢出，即使其频率响应的最大值最接近0dB
    */
    public static double filterOrder2(double x, double[] m1, double[] m2, double[] a, double[] b, double gain){
        double y, m;
        //计算没有增益的滤波输出，存于y_
        m = x - a[1] * m1[0] - a[2] * m2[0];    //求当前m，同时作为求y_的中间步骤
        y = m + b[1] * m1[0] + b[2] * m2[0];
        //更新*m_1和*m_2
        m2[0] = m1[0];
        m1[0] = m;
        //返回带增益的滤波输出
        return y * gain;
    }
}
