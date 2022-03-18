package com.nivi.nivifilter.iir;

public class IIRFilter {

    static float[] den = {(float) 1, (float) -2.324424097758703, (float) 1.858649336960407, (float) -0.505633977530266};
    static float[] num = {(float) 0.003573907708930, (float) 0.010721723126789, (float) 0.010721723126789, (float) 0.003573907708930};
    public static float[] y = new float[5000];


    public static void resp(double data_in[], int m, double data_out[], int n) {

    }

    public static float[] filter(float[] data) {
        for (int k = 0; k < 4997; k++) {
            for (int i = 0; i <= 3; i++) {
                y[k + 0] = y[k + 0] - den[i] * data[k + i];
            }
            for (int i = 0; i <= 3; i++) {
                y[k + 0] = y[k + 0] + num[i] * data[k + i];
            }
            for (int i = 2; i >= 1; i--) {
                y[k + i] = y[k + i - 1];
            }

        }
        return y;
    }
}



