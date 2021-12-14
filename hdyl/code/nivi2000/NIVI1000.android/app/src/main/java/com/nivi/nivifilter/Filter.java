package com.nivi.nivifilter;

import static java.lang.Math.cos;
import static java.lang.Math.sin;

public class Filter {

    public static float[] filter(float[] signal, short N, short NP) {
        float[] smooth = new float[5000];
        float sum;
        short i, j;
        short[] NF = {35, 21, 231, 429, 143, 1105, 323, 2261, 3059, 805, 5175};
        short IWEIGHT[][] = {
                {17, 12, -3},
                {7, 6, 3, -2},
                {59, 54, 39, 14, -21},
                {89, 84, 69, 44, 9, -36},
                {25, 24, 21, 16, 9, 0, -11},
                {167, 162, 147, 122, 87, 42, -13, -78},
                {43, 42, 39, 34, 27, 18, 7, -6, -21},
                {269, 264, 249, 224, 189, 144, 89, 24, -51, -136},
                {329, 324, 309, 284, 249, 204, 149, 84, 9, -76, -171},
                {79, 78, 75, 70, 63, 54, 43, 30, 15, -2, -21, -42},
                {467, 462, 447, 422, 387, 343, 287, 222, 147, 62, -33, -138, -253}
        };

        short NQ = (short) (NP / 2);
        short k = (short) ((NP - 5) / 2);

        for (i = NQ; i < N - NQ; i++) {
            sum = (signal[i] * (float) IWEIGHT[k][0]);
            for (j = 1; j <= NQ; j++) {
                sum = sum + ((signal[i - j] + signal[i + j]) * (float) IWEIGHT[k][j]);
            }
            smooth[i] = (sum / (float) NF[k]);
        }
        for (i = 0; i < NQ; i++) {
            smooth[i] = signal[i];
        }
        for (i = (short) (N - NQ); i < N; i++) {
            smooth[i] = signal[i];
        }
        return smooth;
    }



    public static float[] filter2(float[] signal, short N, short NP) {
        float[] smooth = new float[5000];


        return smooth;
    }
}

//     private float pi = 3.1415926f;
//
//  private int Hamming = 1;
//
//
//private float[] Output_Data;
//
//   private float sinc(float n)
//    {
//        if(0==n)
//            return 1;
//        else
//            return (float) (sin(pi*n)/(pi*n));
//    }
//
//   private float[] Unit_Impulse_Response(int N, double w_c)
//    {
//        int Count = 0;
//
//        for(Count = -(N-1)/2;Count <= (N-1)/2;Count++)
//        {
//		Output_Data[Count+((N-1)/2)] = (float) ((w_c/pi)*sinc((float) ((w_c/pi)*(double)(Count))));
//
//        }
//
//
//        for(Count = -(N-1)/2;Count <= (N-1)/2;Count++)
//        {
//		Output_Data[Count+((N-1)/2)] *= (0.54 + 0.46 * cos((2*pi*Count)/(N-1)));
//
//        }
//
//
//        return Output_Data;
//    }
//
//
//    void Save_Input_Date (float Scand,int Depth,float [] Input_Data)
//    {
//        int Count;
//
//        for(Count = 0 ; Count < Depth-1 ; Count++)
//        {
//		Input_Data [ Count] = Input_Data [ Count + 1];
//        }
//
//    Input_Data [Depth-1] =  Scand;
//    }
//
//
//
//    float Real_Time_FIR_Filter(float [] b,int b_Lenth,float [] Input_Data)
//    {
//        int Count;
//        double Output_Data = 0;
//
//        for(Count = 0; Count < b_Lenth ;Count++)
//        {
//            Output_Data += b [ Count] *Input_Data[ b_Lenth - 1-Count];
//        }
//
//        return (float) Output_Data;
//    }
//
//
//
//    int main(void)
//    {
//        double w_p = pi/10;
//        double w_s = pi/5;
//        double w_c = (w_s + w_p)/2;
//
//
//        int N = 0;
//        N = (int) ((6.6*pi)/(w_s - w_p) + 0.5);
//        if(0 == N%2) N++;
//
//
//        double *Impulse_Response;
//        Impulse_Response = (double *) malloc(sizeof(double)*N);
//        memset(Impulse_Response,
//                0,
//                sizeof(double)*N);
//
//        Unit_Impulse_Response(N,w_c,
//                Hamming,
//                Impulse_Response);
//
//        double *Input_Buffer;
//        double Output_Data = 0;
//        Input_Buffer = (double *) malloc(sizeof(double)*N);
//        memset(Input_Buffer,
//                0,
//                sizeof(double)*N);
//        int Count = 0;
//
//        FILE *fs;
//        fs=fopen("FIR_Data.txt","w");
//
//        while(1)
//        {
//            if(Input_Data[Count] == 55) break;
//
//            Save_Input_Date (Input_Data[Count],
//                    N,
//                    Input_Buffer);
//
//            Output_Data = Real_Time_FIR_Filter(Impulse_Response,N,Input_Buffer);
//
//            fprintf(fs,"%lf,",Output_Data);
//
//            Count++;
//            if ( (Count % 10) == 0)
//                fprintf(fs,"\n");
//        }
//
//
//        fclose(fs);
//        printf("Finish \n");
//        return (int)0;
//    }
//}
