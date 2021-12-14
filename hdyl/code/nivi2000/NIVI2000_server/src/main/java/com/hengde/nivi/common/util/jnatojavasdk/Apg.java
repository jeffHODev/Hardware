package com.hengde.nivi.common.util.jnatojavasdk;

import com.hengde.nivi.common.global.TypeData;
import com.hengde.nivi.common.global.TypeDataR;

public class Apg {
    public static short Cut_One_APG(float[] APGS, short size, float[] apg,
                      short[] startp, short[] Ts, short s, TypeData.ByReference main_data)
    {
        float[] buf,buf1;
        short i,sig_count;

        buf = new float[size+1];
        buf1 = new float[size+1];
        /////////////////////////
        main_data.T =(float)( (startp[s + 1] - startp[s])
                * 0.0012f );
        if (main_data.T == 0)
        {
            //delete []buf;
            //delete []buf1;
            return 0;
        }
        main_data.Pulse =(float)( 60.0f / main_data.T );
        main_data.Ts = (float)(Ts[s] - startp[s]) * 0.0012f;
        main_data.Td = main_data.T - main_data.Ts;
        ////////////////////////

        for(i = 0; i < size; i++)	   buf[i] = 0.0f;
        i = DbSOneDimLowPassFilt (APGS, buf, size, 0.01f);

        for(i = 0; i < size; i++)	   buf1[i] = buf[i];

        sig_count = (short) (startp[s+1] - startp[s]);

        for ( i = 0; i <= sig_count; i++ )
        {
            buf1[i] = buf[startp[s] + i];
            buf[i] = 0.0f;
        }

        sig_count = DbSigMatch ( buf1, buf, sig_count, main_data);

        for(i=0;i<sig_count;i++)	   apg[i] = buf[i];
        //delete []buf;
        //delete []buf1;
        return 1;
    }

    public static short Cut_One_APGR(float[] APGS, short size, float[] apg,
                                    short[] startp, short[] Ts, short s, TypeDataR.ByReference main_data)
    {
        float[] buf,buf1;
        short i,sig_count;

        buf = new float[size+1];
        buf1 = new float[size+1];
        /////////////////////////
        main_data.T =(float)( (startp[s + 1] - startp[s])
                * 0.0012f );
        if (main_data.T == 0)
        {
            //delete []buf;
            //delete []buf1;
            return 0;
        }
        main_data.Pulse =(float)( 60.0f / main_data.T );
        main_data.Ts = (float)(Ts[s] - startp[s]) * 0.0012f;
        main_data.Td = main_data.T - main_data.Ts;
        ////////////////////////

        for(i = 0; i < size; i++)	   buf[i] = 0.0f;
        i = DbSOneDimLowPassFilt (APGS, buf, size, 0.01f);

        for(i = 0; i < size; i++)	   buf1[i] = buf[i];

        sig_count = (short) (startp[s+1] - startp[s]);

        for ( i = 0; i <= sig_count; i++ )
        {
            buf1[i] = buf[startp[s] + i];
            buf[i] = 0.0f;
        }

        sig_count = DbSigMatchR ( buf1, buf, sig_count, main_data);

        for(i=0;i<sig_count;i++)	   apg[i] = buf[i];
        //delete []buf;
        //delete []buf1;
        return 1;
    }

    static short DbSOneDimLowPassFilt (float[] src, float[] ret,
                                short size, float limT )
    {
        short i;

        if ( (size <= 1) ) return 0;
        ret[0] = 0;
        for ( i =1; i <= size; i++ )
            ret[i] = DbDOneDimLowPassFilt ( src[i - 1], ret[i], limT );
        return (short) (i - 1);
    }

    static float DbDOneDimLowPassFilt (float uc, float yb,	float limT )
    {
        float dy, yc;

        if ( (limT <= 0) )  return 0;
        dy = (float)((uc - yb) / limT * 0.0012f);
        yc = dy + yb;
        return yc;
    }

    static short DbSigMatch ( float[] src, float[] ret, short size, TypeData.ByReference main_data)
    {
        short  i, maxi,	mini;
        float  max, min;

        if ( (size <= 0) ) return 0;
        i = DbBaseMatch ( src, ret, size );
        maxi = AbMaxVPointGet( ret, size, (short) 0);
        mini = AbMinVPointGet( ret, size, (short) 0);
        max = ret[maxi];
        min = ret[mini];
        if ( (max - min) <= 0 ) return 0;
        main_data.K = (main_data.Ps - main_data.Pd) / (max - min);
        if( main_data.K <= 0 )	return 0;
        main_data.H = main_data.Ps / main_data.K - max;
        for (i = 0;	i <= size;	i++ )
            ret[i] = (float)((main_data.Ps - main_data.Pd) / (max - min)
                    * (ret[i] - min) + main_data.Pd);
        return (short) (i - 1);
    }

    static short DbSigMatchR ( float[] src, float[] ret, short size, TypeDataR.ByReference main_data)
    {
        short  i, maxi,	mini;
        float  max, min;

        if ( (size <= 0) ) return 0;
        i = DbBaseMatch ( src, ret, size );
        maxi = AbMaxVPointGet( ret, size, (short) 0);
        mini = AbMinVPointGet( ret, size, (short) 0);
        max = ret[maxi];
        min = ret[mini];
        if ( (max - min) <= 0 ) return 0;
        main_data.K = (main_data.Ps - main_data.Pd) / (max - min);
        if( main_data.K <= 0 )	return 0;
        main_data.H = main_data.Ps / main_data.K - max;
        for (i = 0;	i <= size;	i++ )
            ret[i] = (float)((main_data.Ps - main_data.Pd) / (max - min)
                    * (ret[i] - min) + main_data.Pd);
        return (short) (i - 1);
    }

    static short DbBaseMatch (float[] src, float[] ret, short size)
    {
        short i;
        float a, b;

        if (size <= 0) return 0;
        a = (float)((src[size] - src[0]) / (float)size);
        b = (float)((src[size] - src[0]) / 2.0f);
        for (i = 0 ; i < size ; i++ )
            ret[i] = (float)(src[i] - a * (float)i + b);
        return (short) (i - 1);
    }

    public static short AbMaxVPointGet ( float[] src, short size, short option )
    {
        short  i, maxvp;
        float  maxf, maxd, convertv;

        if ( size <= 0 ) return 0;
        maxvp = 0;
        if (option == 0)
        {
            maxf = src[maxvp];
            for ( i = 0; i <= size; i++ )
            {
                if ( maxf < src[i] )
                {
                    maxf = src[i];
                    maxvp = i;
                }
            }
        }
        if (option == 1)
        {
            convertv = src[maxvp];
            maxd = (float)Math.abs(convertv);
            for ( i = 0; i <= size; i++ )
            {
                convertv = src[i];
                if ( maxd < Math.abs(convertv) )
                {
                    maxd = (float)Math.abs(convertv);
                    maxvp = i;
                }
            }
        }
        return maxvp;
    }

    public static short AbMinVPointGet ( float[] src, short size, short option )
    {
        short  i, minvp;
        float  minf, mind, convertv;

        if ( size <= 0 ) return 0;
        minvp = 0;
        if (option == 0)
        {
            minf = src[minvp];
            for ( i = 0; i <= size; i++ )
            {
                if ( minf > src[i] )
                {
                    minf = src[i];
                    minvp = i;
                }
            }
        }
        if (option == 1)
        {
            convertv = src[minvp];
            mind = (float)Math.abs(convertv);
            for ( i = 0; i <= size; i++ )
            {
                convertv = src[minvp];
                if ( mind > Math.abs(convertv) )
                {
                    mind = (float)Math.abs(convertv);
                    minvp = i;
                }
            }
        }
        return minvp;
    }

    public static short AbOneGetbyAuto (short[] St, short[] Ts, short[] Ut,short chnumber)
    {
        short i, k, size, base;
        short[] Stdiff = new short[chnumber-1];
        short[] Tsdiff = new short[chnumber-1];
        short[] Utdiff = new short[chnumber-1];
        short sumStdiff = 0; short sumUtdiff = 0; short sumTsdiff = 0;
        float[] summeansqr = new float[chnumber - 1];

        //base = AbOptimalIntervalGet (&St[0], &St[1], chnumber-1 );
	/*	for ( k = 1; k <= chnumber - 1; k++ )
	{
        size = St[k + 1] - St[k];
		if ( (0.95f * base < size) && (1.05f * base > size) )	i = k;
	}*/
        for (i = 0; i <(chnumber-1); i++)
        {
            Stdiff[i] = (short) (St[i+1] - St[i]);
            Tsdiff[i] = (short) (Ts[i+1] - Ts[i]);
            Utdiff[i] = (short) (Ut[i+1] - Ut[i]);
        }
	/*for (i = 1; i <= chnumber ; i++)
	{
		Stdiff[i - 1] = St[i] - St[i - 1];
		Tsdiff[i - 1] = Ts[i] - Ts[i - 1];
		Utdiff[i - 1] = Ut[i] - Ut[i - 1];
	}*/
        for (i = 0; i < chnumber - 1; i++)  //for (i = 0; i <= chnumber-1; i++)
        {
            sumStdiff = (short) (sumStdiff+Stdiff[i]);
            sumTsdiff = (short) (sumTsdiff+Tsdiff[i]);
            sumUtdiff = (short) (sumUtdiff+Utdiff[i]);
        }
        float meanStdiff = sumStdiff / (chnumber - 1);
        float meanTsdiff = sumTsdiff / (chnumber - 1);
        float meanUtdiff = sumUtdiff / (chnumber - 1);
        for (i = 0; i < chnumber - 1; i++)  //for (i = 0; i <= chnumber-1; i++)
        {
            summeansqr[i] = (Stdiff[i] - meanStdiff)*(Stdiff[i] - meanStdiff) + (Tsdiff[i] - meanTsdiff)*(Tsdiff[i] - meanTsdiff) + (Utdiff[i] - meanUtdiff)*(Utdiff[i] - meanUtdiff);
        }
        short mini = AbMinVPointGet(summeansqr, (short) (chnumber - 1 - 1), (short) 0); //chnumber - 1 - 1

        return (mini);
    }
}
