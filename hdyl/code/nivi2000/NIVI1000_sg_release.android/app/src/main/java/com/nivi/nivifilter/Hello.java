package com.nivi.nivifilter;

import com.hekangyuan.nivi1000.utils.Utils;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Hello {

    static {
        System.loadLibrary("hello");
    }

    public static native String sayHello();

    public static native void filter(float[] signals, float[] result, short allNum, short smNum);

    public static native void filterMibun(float[] signals, float[] result, short allNum, short smNum);

    public static native short DetectAbsMax(float[] signals, short start, short end);

//    public static List<Float> filter(List<Float> source) {
//        List<Float> result = new ArrayList<Float>();
//
//        int divideIndex = source.size() / 2;
//
//        /**
//         * 思路，将数组分成两部分，分别进行滤波，然后合并
//         */
//        List<Float> front = source.subList(0, divideIndex);
//        float[] frontArray = Utils.unWrap(front.toArray(new Float[front.size()]));
//        float[] frontResult = new float[frontArray.length];
//
//        filter(frontArray, frontResult, (short) frontArray.length, (short) 25);
//
//
//        List<Float> tail = source.subList(divideIndex, source.size());
//        float[] tailArray = Utils.unWrap(tail.toArray(new Float[tail.size()]));
//        float[] tailResult = new float[tailArray.length];
//
//        filter(tailArray, tailResult, (short) tailArray.length, (short) 25);
//
//
//        float[] resultArray = new float[frontArray.length + tailArray.length];
//        System.arraycopy(frontResult, 0, resultArray, 0, frontResult.length);
//        System.arraycopy(tailResult, 0, resultArray, frontResult.length, tailResult.length);
//
////        for (int i = 0; i < resultArray.length; i++) {
////            Log.e("huang", resultArray[i] + "");
////        }
//        return Arrays.asList(Utils.wrap(resultArray));
//    }


//                float f1 [] = {61.0f,4559.2f,922.4f,4665.2f,1753.7f,3599.2f,2266.2f,1477.7f,697.3f,4281.9f,2345.0f,4166.7f,1715.7f,4470.2f,111.4f,3370.8f,766.1f,4659.3f,4287.7f,3254.5f,2860.5f,1314.6f,4132.0f,4987.4f,1861.6f,3448.2f,1094.9f,
//                        4125.7f,1176.1f,3942.7f,2373.3f,1920.3f,2278.9f,778.6f,685.5f,3129.2f,1599.4f,255.6f,4974.7f,2965.5f,3020.0f,2226.6f,2547.9f,2551.9f,1326.1f,433.2f,4383.6f,141.6f,4890.4f,3877.5f,957.8f,4031.9f,3801.8f,830.5f,194.1f,1170.8f,3817.2f,
//                        2878.1f,1970.6f,4565.6f,154.1f,3113.1f,691.5f,7.7f,1344.9f,3411.7f,635.3f,248.4f,630.1f,1063.7f,1745.2f,3492.6f,1617.5f,1583.7f,1851.3f,1238.0f,259.6f,1485.7f,3413.9f,2886.3f,4400.7f,2324.1f,2193.5f,3924.3f,618.7f,2996.0f,4489.0f,
//                        139.7f,226.7f,1989.8f,1241.0f,4611.7f,3766.4f,656.7f,4193.2f,2885.5f,4388.7f,3568.6f,2114.7f,1062.4f};
//
//                float[] a4 = new float[100];
//                Hello.filter(f1,a4,(short)100,(short)25);
//                for(int i=0; i < a4.length; i++){
//                    Log.e("huang",a4[i]+"");
//                }


}
