package com.nivi.nivifilter;

public class SerialBeanFilterPcg {
//    private static final float a[][] = { //bandpass 0.8-45HZ FS = 208.33HZ order = 20
//            {1.00f, -0.370318446587311f, 0.741478258957100f},
//            {1.00f, -1.99211045270079f, 0.992691251097329f},
//            {1.00f, -1.97813735918714f, 0.978720138504178f},
//            {1.00f, -0.303836010245745f, 0.396141510323030f},
//            {1.00f, -1.96589499787464f, 0.966484463319507f},
//            {1.00f, -0.269347771736051f, 0.191811440530334f},
//            {1.00f, -1.95659325720995f, 0.957190946668250f},
//            {1.00f, -0.252425806991956f, 0.0750553756709697f},
//            {1.00f, -1.95150152828687f, 0.952104884031380f},
//            {1.00f, -0.245649105385984f, 0.0214847444052265f}
//    }; //共十级二阶滤波，分母

//    private static final float a[][] = { //bandpass 0.8-35HZ FS = 208.33HZ order = 20
//            {1.00f, -0.872797284332393f, 0.768870757759665f},
//            {1.00f, -1.99218980957446f, 0.992770914722481f},
//            {1.00f, -1.97828928636842f, 0.978874599519219f},
//            {1.00f, -0.722630142967546f, 0.448798633178355f},
//            {1.00f, -1.96591726119019f, 0.966513250522468f},
//            {1.00f, -0.636480681929170f, 0.253241104144337f},
//            {1.00f, -1.95630826689472f, 0.956917127644367f},
//            {1.00f, -0.589721191597952f, 0.139454521090381f},
//            {1.00f, -1.95094030474106f, 0.951558121539833f},
//            {1.00f, -0.569316299566317f, 0.0867536194006823f}
//    };

//    private static final float a[][] = { //pcgfiltr highpass 19 fs833
//            {1.00f, -1.95756015186814f, 0.977836695002051f},
//            {1.00f, -1.91562625424199f, 0.935468443170652f},
//            {1.00f, -1.87691116794895f, 0.896352343342266f},
//            {1.00f, -1.84203396540033f, 0.861113880322958f},
//            {1.00f, -1.81147026768147f, 0.830233601714087f},
//            {1.00f, -1.78557539310957f, 0.804070506244508f},
//            {1.00f, -1.76460704140067f, 0.782884962901690f},
//            {1.00f, -1.74874566646993f, 0.766859294754906f},
//            {1.00f, -1.73811143670161f, 0.756114914900829f},
//            {1.00f, -1.73277720113503f, 0.750725426954171f},
//    };

//    private static final float a[][] = { //pcgfiltr bandpass 19-416 fs833
//            {1.00f, 1.99840792555584f, 0.998433046463819f},
//            {1.00f, -1.93628236815276f, 0.956322850680709f},
//            {1.00f, 1.99543361203535f, 0.995458698831731f},
//            {1.00f, -1.85904093820964f, 0.878284500157908f},
//            {1.00f, 1.99290915193587f, 0.992934212293507f},
//            {1.00f, -1.79799433653619f, 0.816609941081766f},
//            {1.00f, 1.99107784474175f, 0.991102887369783f},
//            {1.00f, -1.75606991889349f, 0.774255324998761f},
//            {1.00f, 1.99011591593121f, 0.990140949736062f},
//            {1.00f, -1.73479223598667f, 0.752759658909486f},
//    };
      private static final float a[][] = { //pcgfiltr bandSTOP 399-401 fs833
        {1.00f, 1.94625779061870f, 0.949710813289325f}
};
    private static final float a1[][] = { //pcgfiltr bandSTOP 40 （49 51） 60 fs833
            {1.00f, -1.75834603034150f, 0.890682237629067f}
    };
    private static final float a2[][] = { //pcgfiltr bandSTOP 75 （85 98） 105 fs833
            {1.00f, -1.53501608443046f, 0.934444780716876f}
    };
//    private static final float b[] = { 1.00f, 0.00f, -1.00f }; //第一级二阶滤波，分子

   // private static final float b[] = { 1.00f, -2.00f, 1.00f}; //pcgfiltr  fs833
     private static final float b[] = { 1.00f, 1.99645791299192f, 1.00f}; //pcgfiltr bandstop 399-401 fs833
     private static final float b1[] = { 1.00f, -1.86001221717061f, 1.00f}; //pcgfiltr bandSTOP 37 （40 60） 67 fs833
     private static final float b2[] = { 1.00f, -1.58703530825171f, 1.00f}; //pcgfiltr bandSTOP 75 （85 98） 105 fs833

//    private static final float gain[] = {
//            0.576024030153927f,
//            0.576024030153927f,
//            0.515000895704627f,
//            0.515000895704627f,
//            0.475978866002799f,
//            0.475978866002799f,
//            0.452609656011184f,
//            0.452609656011184f,
//            0.441626827758793f,
//            0.441626827758793f
//    }; //bandpass 0.8-45HZ FS = 208.33HZ order = 20

//    private static final float gain[] = {
//            0.463071375715916f,
//            0.463071375715916f,
//            0.418371031109980f,
//            0.418371031109980f,
//            0.389068666148473f,
//            0.389068666148473f,
//            0.371264810549292f,
//            0.371264810549292f,
//            0.362833597761529f,
//            0.362833597761529f
//    }; //bandpass 0.8-35HZ FS = 208.33HZ order = 20

//    private static final float gain[] = { //pcgfiltr bandpass 19-416 fs833
//            0.985925991368341f,
//            0.985925991368341f,
//            0.965404969494440f,
//            0.965404969494440f,
//            0.948921923642746f,
//            0.948921923642746f,
//            0.937462231059637f,
//            0.937462231059637f,
//            0.931601629937284f,
//            0.931601629937284f
//    };

//    private static final float gain[] = { //pcgfiltr highpass 19 fs833
//            0.983849211717548f,
//            0.962773674353160f,
//            0.943315877822804f,
//            0.925786961430822f,
//            0.910425967348889f,
//            0.897411474838521f,
//            0.886873001075590f,
//            0.878901240306209f,
//            0.873556587900609f,
//            0.870875657022301f
//    };
private static final float gain[] = { //pcgfiltr bandstop 399-401 fs83
        0.974855406644662f
};
private static final float gain1[] = { //pcgfiltr bandSTOP 40 （49 51） 60 fs833
        0.945341118814535f
};private static final float gain2[] = {//pcgfiltr bandSTOP 75 （85 98） 105 fs833
        0.967222390358438f
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


    public static float[] m001 = {0};
    public static float[] m011 = {0};

    public static float[] m002 = {0};
    public static float[] m012 = {0};

    public static float serialBeanFilter(float x) {
        float y0 = filterOrder2(x, m00, m01, a[0], b, gain[0]);  //第一级二阶滤波
//        float y1 = filterOrder2(y0, m10, m11, a[1], b, gain[1]);  //第二级二阶滤波
//        float y2 = filterOrder2(y1, m20, m21, a[2], b, gain[2]);  //第二级二阶滤波
//        float y3 = filterOrder2(y2, m30, m31, a[3], b, gain[3]);  //第二级二阶滤波
//        float y4 = filterOrder2(y3, m40, m41, a[4], b, gain[4]);  //第二级二阶滤波
//        float y5 = filterOrder2(y4, m50, m51, a[5], b, gain[5]);  //第二级二阶滤波
//        float y6 = filterOrder2(y5, m60, m61, a[6], b, gain[6]);  //第二级二阶滤波
//        float y7 = filterOrder2(y6, m70, m71, a[7], b, gain[7]);  //第二级二阶滤波
//        float y8 = filterOrder2(y7, m80, m81, a[8], b, gain[8]);  //第二级二阶滤波
//        float y9 = filterOrder2(y8, m90, m91, a[9], b, gain[9]);  //第二级二阶滤波

//        return y9;
        return y0;
    }
    public static float serialBeanFilter1(float x) {
        float y0 = filterOrder2(x, m001, m011, a1[0], b1, gain1[0]);  //第一级二阶滤波

        return y0;
    }
    public static float serialBeanFilter2(float x) {
        float y0 = filterOrder2(x, m002, m012, a2[0], b2, gain2[0]);  //第一级二阶滤波

        return y0;
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
