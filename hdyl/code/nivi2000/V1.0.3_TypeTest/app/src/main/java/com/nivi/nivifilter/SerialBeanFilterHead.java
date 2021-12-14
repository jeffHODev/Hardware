package com.nivi.nivifilter;

public class SerialBeanFilterHead {
    private static final float a[][] = { //headfilter bandpass 0.1 - 41Hz fs833
            {1.00f, -1.81906014896170f, 0.909557385274609f},
            {1.00f, -1.99976464429993f, 0.999765212860320f},
            {1.00f, -1.99931701616457f, 0.999317585591491f},
            {1.00f, -1.67511908267858f, 0.758306276328882f},
            {1.00f, -1.99893372627553f, 0.998934297215784f},
            {1.00f, -1.56957926483588f, 0.647298310881119f},
            {1.00f, -1.99865290220653f, 0.998653474707490f},
            {1.00f, -1.50093729510077f, 0.575039198331044f},
            {1.00f, -1.99850424058147f, 0.998504814063449f},
            {1.00f, -1.46722518897265f, 0.539530236394589f}
            
//            {1.00f, -1.99959239543219f, 0.999592829558627f}, //0.1Hz high pass
//            {1.00f, -1.99893393099028f, 0.998934364973767f},
//            {1.00f, -0.999341267198615f, 0.0f}

    };

    private static final float b[] = { 1.00f, 0.00f, -1.00f }; // //headfilter bandpass 0.1 - 41Hz fs833
//	private static final float b[][] = {//0.1Hz highpass
//	{ 1.00f, -2.00f, 1.00f },
//	{ 1.00f, -2.00f, 1.00f },
//	{ 1.00f, -1.00f, 0.00f }
//	};
    private static final float gain[] = { //headfilter bandpass 0.1 - 41Hz fs833
//            0.999796306247703f,//0.1Hz highpass
//            0.999467073991013f,
//            0.999670633599308f,
            0.150116312211629f,
        0.150116312211629f,
        0.144033280138869f,
        0.144033280138869f,
        0.139402335587071f,
        0.139402335587071f,
        0.136304680484915f,
        0.136304680484915f,
        0.134756797877989f,
        0.134756797877989f,
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

//           float y0 = filterOrder2(x, m00, m01, a[0], b, gain[0]);  //第一级二阶滤波
//           float y1 = filterOrder2(y0, m10, m11, a[1], b, gain[1]);  //第二级二阶滤波
//           float y2 = filterOrder2(y1, m20, m21, a[2], b, gain[2]);  //第二级二阶滤波

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

        //return y2;
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
