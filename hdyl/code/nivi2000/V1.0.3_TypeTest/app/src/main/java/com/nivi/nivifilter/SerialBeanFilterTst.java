package com.nivi.nivifilter;

public class SerialBeanFilterTst {
    private static final float a[][] = { //tstfilter lowpass 40Hz fs833


            {1.00f, -1.86614876342683f, 0.954433066928255f},
            {1.00f, -1.78577815925380f, 0.870260267051184f},
            {1.00f, -1.71467023844838f, 0.795788352235864f},
            {1.00f, -1.65300516651005f, 0.731206011303156f},
            {1.00f, -1.60073488922586f, 0.676462916677558f},
            {1.00f, -1.55768350001041f, 0.631374839934215f},
            {1.00f, -1.52362085633919f, 0.595700750899765f},
            {1.00f, -1.49831446706493f, 0.569197159668718f},
            {1.00f, -1.48156467236367f, 0.551654960852649f},
            {1.00f, -1.47322735435912f, 0.542923218605223f}
    };

    private static final float b[] = { 1.00f, 2.00f, 1.00f }; //第一级二阶滤波，分子

    private static final float gain[] = { //tstfilter lowpass 40Hz fs833
            0.0220710758753554f,
            0.0211205269493450f,
            0.0202795284468712f,
            0.0195502111982769f,
            0.0189320068629234f,
            0.0184228349809514f,
            0.0180199736401427f,
            0.0177206731509480f,
            0.0175225721222448f,
            0.0174239660615267f
    };

    public static float[] m00 = {0};
    public static float[] m01 = {0};

    public static float[] m10 = {0};
    public static float[] m11 = {0};

    public static float[] m20 = {0};
    public static float[] m21 = {0};

    public static float[] m30 = {0};
    public static float[] m31 = {0};

    public static float[] m40 = {0};
    public static float[] m41 = {0};

    public static float[] m50 = {0};
    public static float[] m51 = {0};

    public static float[] m60 = {0};
    public static float[] m61 = {0};

    public static float[] m70 = {0};
    public static float[] m71 = {0};

    public static float[] m80 = {0};
    public static float[] m81 = {0};

    public static float[] m90 = {0};
    public static float[] m91 = {0};

    public static float serialBeanFilter(float x) {
        float y0 = filterOrder2(x, m00, m01, a[0], b, gain[0]);  //第一级二阶滤波
        float y1 = filterOrder2(y0, m10, m11, a[1], b, gain[1]);  //第二级二阶滤波
        float y2 = filterOrder2(y1, m20, m21, a[2], b, gain[2]);  //第二级二阶滤波
        float y3 = filterOrder2(y2, m30, m31, a[3], b, gain[3]);  //第二级二阶滤波
        float y4 = filterOrder2(y3, m40, m41, a[4], b, gain[4]);  //第二级二阶滤波
        float y5 = filterOrder2(y4, m50, m51, a[5], b, gain[5]);  //第二级二阶滤波
        float y6 = filterOrder2(y5, m60, m61, a[6], b, gain[6]);  //第二级二阶滤波
        float y7 = filterOrder2(y6, m70, m71, a[7], b, gain[7]);  //第二级二阶滤波
        float y8 = filterOrder2(y7, m80, m81, a[8], b, gain[8]);  //第二级二阶滤波
        float y9 = filterOrder2(y8, m90, m91, a[9], b, gain[9]);  //第二级二阶滤波

        return y9;
    }

    /*
    x: input
    m1:保存中间滤波器数据
    m2:保存中间滤波器数据
    a:滤波器系数
    b:滤波器系数
    gain:增益，对于只支持定点小数运算的，需要把增益分配到每个二阶IIR滤波器的系数中，使得每次中间的结果都不溢出，即使其频率响应的最大值最接近0dB
    */
    public static float filterOrder2(float x, float[] m1, float[] m2, float[] a, float[] b, float gain){
        float y, m;
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
