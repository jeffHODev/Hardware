package com.nivi.nivifilter;

import android.util.Log;

public class SerialBeanFilterEcg {
//    private static final double a[][] = { //ecgfilter bandpass 0.5 - 35Hz fs833
//            {1.00, -1.857112290799973, 0.923673561988853},
//            {1.00, -1.998838025856335, 0.998852260007740},
//            {1.00, -1.996625711558520, 0.996640071891598},
//            {1.00, -1.732024013452928, 0.793477506546787},
//            {1.00, -1.994670447777048, 0.994685035929571},
//            {1.00, -1.638672312316859, 0.695832782340597},
//            {1.00, -1.993175835335868, 0.993190671423068},
//            {1.00, -1.577256414065743, 0.631298143689783},
//            {1.00, -1.992354928490805, 0.992369928012028},
//            {1.00, -1.546907205895733, 0.599299836547364}
//    };

//        private static final double a[][] = { //ecgfilter bandpass 0.05 - 35Hz fs833
//            {1.00f, -1.85528235172459f, 0.921803530454676f},
//            {1.00f, -1.99988223383094f, 0.999882375965657f},
//            {1.00f, -1.99965818715830f, 0.999658329423239f},
//            {1.00f, -1.72691679565251f, 0.788770663148063f},
//            {1.00f, -1.99946685583871f, 0.999466998327878f},
//            {1.00f, -1.63097286708356f, 0.689291033787842f},
//            {1.00f, -1.99932715629519f, 0.999327299013740f},
//            {1.00f, -1.56771935555782f, 0.623680093393453f},
//            {1.00f, -1.99925341382676f, 0.999253556688763f},
//            {1.00f, -1.53640266617756f, 0.591187419087585f}
//    };

//            private static final double a[][] = { //ecgfilter bandpass 0.05 - 100Hz fs833
//                    {1,	-0.745307245910660	,0.752090763165312 },
//                    {1,	-1.99988193536143	  ,0.999882077593241 },
//                    {1,	-1.99965759822452	  ,0.999657740471045 },
//                    {1,	-0.603223157117436	,0.417780921119459 },
//                    {1,	-1.99946664082011	  ,0.999466783102716 },
//                    {1,	-0.519143154270895	,0.219754271490670 },
//                    {1,	-1.99932777705478	  ,0.999327919377247 },
//                    {1,	-0.471459845782329	,0.107347519980578 },
//                    {1,	-1.99925471673139	  ,0.999254859079431 },
//                    {1,	-0.449761982822255	,0.0561648344614722}
//
//
//            };
//
//
    private static final double ax[][] = { //ecgfilter not 50Hz fs833
           {1,-1.85588028994461,0.996169181161760}
//
    };
    private static final double a[][] = { //ecgfilter bandpass 0.5 - 25Hz fs833
            {1.00f, -1.91076936924490f, 0.945189798837126f},
            {1.00f, -1.99885057321829f, 0.998864814456533f},
            {1.00f, -1.99665018958606f, 0.996664611752781f},
            {1.00f, -1.81656309867549f, 0.848836452502025f},
            {1.00f, -1.99467470424797f, 0.994689454167301f},
            {1.00f, -1.74387407358110f, 0.774131381559491f},
            {1.00f, -1.99313105372667f, 0.993146168909269f},
            {1.00f, -1.69491403838299f, 0.723582806041133f},
            {1.00f, -1.99226579526166f, 0.992281157153817f},
            {1.00f, -1.67038656338464f, 0.698170765073687f}
    };
    private static final double b[] = { 1.00, 0.00, -1.00 }; //第一级二阶滤波，分子
    private static final double bx[] = { 0.998084590580880,-1.85588028994461,0.998084590580880 }; //第一级二阶滤波，分子
//    private static final double gain[] = { //ecgfilter bandpass 0.5 - 35Hz fs833
//            0.127212226178017,
//            0.127212226178017,
//            0.122774394865851,
//            0.122774394865851,
//            0.119343920884194,
//            0.119343920884194,
//            0.117024438900214,
//            0.117024438900214,
//            0.115858008627607,
//            0.115858008627607,
//    };

//
//private static final double gain[] = { //ecgfilter bandpass 0.05 - 100Hz fs833
//        0.501536501222180,
//        0.501536501222180,
//        0.451146819896531,
//        0.451146819896531,
//        0.418456723537331,
//        0.418456723537331,
//        0.398715444293324,
//        0.398715444293324,
//        0.389396895604631,
//        0.389396895604631
//};

    private static final double gainx[] = { //ecgfilter bandpass 0.5 - 100Hz fs833
           1,
            1,
            1,
            1,
            1,
            1,
            1,
            1,
            1,
            1
    };
    private static final double gain[] = { //ecgfilter bandpass 0.5 - 25Hz fs833
            0.090970164778141,
            0.090970164778141,
            0.088645004698801,
            0.088645004698801,
            0.086801439460353,
            0.086801439460353,
            0.085531910527918,
            0.085531910527918,
            0.084886431736559,
            0.084886431736559
    };

//    public static double[] m00 = {0};
//    public static double[] m01 = {0};
//
//    public static double[] m10 = {0};
//    public static double[] m11 = {0};
//
//    public static double[] m20 = {0};
//    public static double[] m21 = {0};
//
//    public static double[] m30 = {0};
//    public static double[] m31 = {0};
//
//    public static double[] m40 = {0};
//    public static double[] m41 = {0};
//
//    public static double[] m50 = {0};
//    public static double[] m51 = {0};
//
//    public static double[] m60 = {0};
//    public static double[] m61 = {0};
//
//    public static double[] m70 = {0};
//    public static double[] m71 = {0};
//
//    public static double[] m80 = {0};
//    public static double[] m81 = {0};
//
//    public static double[] m90 = {0};
//    public static double[] m91 = {0};
//    public static double[] m100 = {0};
//    public static double[] m101 = {0};




    public static double[] m00 = {1568237.5825355668 };
    public static double[] m01 = {1568239.6322084349    };

    public static double[] m10 = {-11748.703206715894  };
    public static double[] m11 = {-11569.815158543102 };

    public static double[] m20 = {2786124.952732143     };
    public static double[] m21 = {2796846.669412349     };

    public static double[] m30 = {-16538.789895136426 };
    public static double[] m31 = {-16362.106231590504   };

    public static double[] m40 = {2403261.0236704536    };
    public static double[] m41 = {2416266.71276047      };

    public static double[] m50 = {-21305.867571020317 };
    public static double[] m51 = {-21133.546924953647};

    public static double[] m60 = {2922888.0253805923    };
    public static double[] m61 = {2939600.4534359183    };

    public static double[] m70 = {-28559.372728888357 };
    public static double[] m71 = {-28400.398126209486};

    public static double[] m80 = {1890237.156491996     };
    public static double[] m81 = {1910120.0580617948    };

    public static double[] m90 = {-34785.17857361987};
    public static double[] m91 = {-34669.27703204813};
    public static double[] m100 = {420.4649480452849     };
    public static double[] m101 = {1248.490678766529   };

//    public static double[] m00 = {2.987934559834893E7};
//    public static double[] m01 = {2.9879406798800383E7};
//
//    public static double[] m10 = {-5880.604449062418};
//    public static double[] m11 = {-5992.671579732362};
//
//    public static double[] m20 = {-204632.75677762998};
//    public static double[] m21 = {-204888.8509942126};
//
//    public static double[] m30 = {928.7368622895287};
//    public static double[] m31 = {928.7368622895287};
//
//    public static double[] m40 = {38368.9971912791};
//    public static double[] m41 = {37844.79574997408};
//
//    public static double[] m50 = {2645.7632978934976};
//    public static double[] m51 = {2586.0788048106056};
//
//    public static double[] m60 = {68458.03011134174};
//    public static double[] m61 = {68602.41522595365};
//
//    public static double[] m70 = {-1313.4442246167473};
//    public static double[] m71 = {-1348.548328044064};
//
//    public static double[] m80 = {-33383.47653239929};
//    public static double[] m81 = {-33245.9836132479};
//    public static double[] m90 = {-993.2886380413053};
//    public static double[] m91 = {-1000.2277270861545};
    //IIR
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
        double y10 = filterOrder2(x, m100, m101, ax[0], bx, gainx[0]);  //第一级二阶滤波

        System.out.println("ECG_Filter1：" +m00[0]);
        System.out.println("ECG_Filter：" +m01[0]);
        System.out.println("ECG_Filter：" +m10[0]);
        System.out.println("ECG_Filter：" +m11[0]);
        System.out.println("ECG_Filter：" +m20[0]);
        System.out.println("ECG_Filter：" +m21[0]);
        System.out.println("ECG_Filter：" +m30[0]);
        System.out.println("ECG_Filter：" +m31[0]);
        System.out.println("ECG_Filter：" +m40[0]);
        System.out.println("ECG_Filter：" +m41[0]);
        System.out.println("ECG_Filter：" +m50[0]);
        System.out.println("ECG_Filter：" +m51[0]);
        System.out.println("ECG_Filter：" +m60[0]);
        System.out.println("ECG_Filter：" +m61[0]);
        System.out.println("ECG_Filter：" +m70[0]);
        System.out.println("ECG_Filter：" +m71[0]);
        System.out.println("ECG_Filter：" +m80[0]);
        System.out.println("ECG_Filter：" +m81[0]);
        System.out.println("ECG_Filter：" +m90[0]);
        System.out.println("ECG_Filter20：" +m91[0]);
        System.out.println("ECG_Filter：" +m100[0]);
        System.out.println("ECG_Filter20：" +m101[0]);
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
