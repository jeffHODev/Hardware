package com.nivi.nivifilter;

public class SerialBeanFilterTst {
    private static final double a[][] = { //tstfilter lowpass 40Hz fs833

// TODO: 2021/11/3 smooth 
            {1.00, -1.86614876342683, 0.954433066928255},
            {1.00, -1.78577815925380, 0.870260267051184},
            {1.00, -1.71467023844838, 0.795788352235864},
            {1.00, -1.65300516651005, 0.731206011303156},
            {1.00, -1.60073488922586, 0.676462916677558},
            {1.00, -1.55768350001041, 0.631374839934215},
            {1.00, -1.52362085633919, 0.595700750899765},
            {1.00, -1.49831446706493, 0.569197159668718},
            {1.00, -1.48156467236367, 0.551654960852649},
            {1.00, -1.47322735435912, 0.542923218605223}
    };

    private static final double b[] = { 1.00, 2.00, 1.00 }; //第一级二阶滤波，分子

    private static final double gain[] = { //tstfilter lowpass 40Hz fs833
            0.0220710758753554,
            0.0211205269493450,
            0.0202795284468712,
            0.0195502111982769,
            0.0189320068629234,
            0.0184228349809514,
            0.0180199736401427,
            0.0177206731509480,
            0.0175225721222448,
            0.0174239660615267
    };

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
        double y5 = filterOrder2(y4, m50, m51, a[5], b, gain[5]);  //第二级二阶滤波
        double y6 = filterOrder2(y5, m60, m61, a[6], b, gain[6]);  //第二级二阶滤波
        double y7 = filterOrder2(y6, m70, m71, a[7], b, gain[7]);  //第二级二阶滤波
        double y8 = filterOrder2(y7, m80, m81, a[8], b, gain[8]);  //第二级二阶滤波
        double y9 = filterOrder2(y8, m90, m91, a[9], b, gain[9]);  //第二级二阶滤波

        return (float)y9;
    }

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
