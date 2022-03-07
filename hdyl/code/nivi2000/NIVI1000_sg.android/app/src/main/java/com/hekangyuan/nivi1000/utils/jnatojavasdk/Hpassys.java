package com.hekangyuan.nivi1000.utils.jnatojavasdk;

import com.hekangyuan.nivi1000.jnastructure.AnalHPA_Data;
import com.hekangyuan.nivi1000.jnastructure.CLibrary;

public class Hpassys {
    public static short DetectUSpoint( float[] des ,short start, short end, short ch, short size, CLibrary.AnalHPA.ByReference tmpHpa )
    {

        float	buf,stak;
        short   i, j , nn = 0, mm = 0;

        buf = des[start];
        for( i = start; i < end; i++)
        {
            if( (i <= size - 20) && ( buf > des[i] ))
            {
                buf = des[i]; nn = i;
            }
        }
        stak = des[nn] ;
        for( j = nn; j > nn - 100; j--)
        {
            if( stak < des[j] )
            {
                stak = des[j]; mm = j;
            }
        }
        if( mm == 0 )
            return (short)(tmpHpa.Rtops[ch]+10);
        else
            return (mm);
    }

    public static short RQT_Analysis(float[] k_snl, float[] j_snl, short  Size, CLibrary.AnalHPA.ByReference tmpHpa, AnalHPA_Data.ByReference analHpa)
    {
        float  min_point=0.0f, min_det=0.0f, buf=0.0f, md_value_R_R=0.0f,T=0.0f,T1=0.0f;
        short	k = 0,p = 0, nn = 0, mm = 0, min2=0;
        short[] pok = new short[50];
        short[] meanErr = new short[50];
        short	i = 90, j = 0;
        short	one_null,ryu,t_m,one_null1;

        tmpHpa.RtopNum = 0;
        tmpHpa.QendNum = 0;

//        memset(pok, 0x00, MAXPOINTS * sizeof(short));

        min2 = DetectMin( k_snl, (short) (Size/3), (short) (Size-500) );
        min_point =  k_snl[ min2 ];


        min_det = min_point * 0.5f;
        while( i < Size - 150 )
        {
            if( k_snl[i] < min_det )
            {
                pok[j] = i;
                j++;
                while( k_snl[i] < min_det )
                {
                    if( i  > Size - 150)
                        break;
                    i++;
                }
                pok[j] = i;
                j++;
                if( j > 28 )
                    return(0);
            }
            i++;
        }

        for( i = 0; i < j+1; i+=2 )
        {
            buf = k_snl[pok[i]];
            if( pok[i+1] == 0 ) break;
            for( k = pok[i]; k < pok[i+1]; k++)
            {
                if( buf > k_snl[k] )
                {
                    buf = k_snl[k];
                    nn = k;
                }
            }
            tmpHpa.Rtops[p] = (short) (nn+5);
            if( p > 0 )
            {
                ryu = (short) (tmpHpa.Rtops[p] - tmpHpa.Rtops[p-1]);
                if( ryu > 400 )
                    p++;
            }
            else
                p++;

        }

        tmpHpa.RtopNum = (short) (p - 1);
        tmpHpa.QendNum = (short) (tmpHpa.RtopNum-1) ;
        nn = 0;
        for( i = 0; i < tmpHpa.RtopNum; i++ )
        {
            buf = k_snl[tmpHpa.Rtops[i]] - 3;
            for( j = tmpHpa.Rtops[i]; j > tmpHpa.Rtops[i] - 100; j--)
            {
                if( buf < k_snl[j] )
                {
                    buf = k_snl[j];
                    nn = j;
                }
            }
            tmpHpa.Qstarts[i] = nn;
        }

//        memset(pok, 0x00, MAXPOINTS * sizeof(short));
        i = 0;
/*	while( i <= tmpHpa->RtopNum)
	{
		if( tmpHpa->Rtops[i+1] == 0 )
   			break;
		t_m = tmpHpa->Rtops[i+1] - tmpHpa->Rtops[i];
		T = j_snl[tmpHpa->Rtops[i]+100];
		T1 = j_snl[tmpHpa->Rtops[i]+100];
		for( j = tmpHpa->Rtops[i]+100; j < tmpHpa->Rtops[i+1]-(t_m /2) + 60 ; j++)
		{	if( T > j_snl[j]  )
			{
				T = j_snl[j];
				one_null = j;
			}
			else if( T1 < j_snl[j] )
			{
				T1 = j_snl[j];
				one_null1 = j;
			}
		}
		if(fabs(j_snl[one_null]) > fabs(j_snl[one_null1]))
		{
			for(j = one_null; j < one_null + 150; j++)
			{
				if(j_snl[j - 1 ] <= 0.0001f && j_snl[j] >= 0.0001f)
				{
					tmpHpa->Tends[i] = j - 25 ;
					break;
				}
			}
		}
		else
		{
			for( j = one_null1; j < one_null1 + 150; j++)
			{
				if(j_snl[j - 1 ] >= 0.0001f && j_snl[j] <= 0.0001f)
				{

					tmpHpa->Tends[i] = j - 25 ;
					break;
				}
			}
		}
		if( tmpHpa->Tends[i] <= 0 )
			tmpHpa->Tends[i] = tmpHpa->Rtops[i]+(t_m / 2 -70 );
		i++;
	}
*/
        short TT , TN1,T2;

        while( i <= tmpHpa.RtopNum)
        {
            if( tmpHpa.Rtops[i+1] == 0 )
                break;
            t_m = (short) (tmpHpa.Rtops[i+1] - tmpHpa.Rtops[i]);
            T2 = (short) (tmpHpa.Rtops[i]+150);
            TT = MinID(j_snl, T2, (short) (tmpHpa.Rtops[i+1]-(t_m /2) + 60));
            TN1 = MaxID(j_snl, T2, (short) (tmpHpa.Rtops[i+1]-(t_m /2) + 60));
            for( j = TT; j < tmpHpa.Rtops[i+1]; j++)
            {
                if( j_snl[j] > -2.0f )
                {
                    tmpHpa.Tends[i] = (short) (j-25); break;
                }
            }
            if( tmpHpa.Tends[i] <= 0 )                                 //
                tmpHpa.Tends[i] = (short) (tmpHpa.Rtops[i]+(t_m / 2 -70 ));     //
            i++;
        }

        mm = 0;
        if( tmpHpa.RtopNum < 2 || tmpHpa.RtopNum > 20 )
        {
            return 0;
        }
        md_value_R_R = (float)(tmpHpa.Rtops[tmpHpa.RtopNum]
                - tmpHpa.Rtops[0]) / (float)(tmpHpa.RtopNum-1);

        for(i = 0; i <= tmpHpa.RtopNum-1; i++)
            meanErr[i] = (short) Math.abs( (tmpHpa.Rtops[i+1] - tmpHpa.Rtops[i]) - (short)md_value_R_R);

        buf = meanErr[0];
        for( i = 0; i <= tmpHpa.RtopNum-1; i++)
        {
            if( buf >= meanErr[i] )
            {
                buf = meanErr[i];
                mm = i;
            }
        }
        analHpa.R_R = (float)(tmpHpa.Rtops[mm+1] - tmpHpa.Rtops[mm] ) * 0.0012f;
        tmpHpa.Rpos = mm;
        tmpHpa.R_top = tmpHpa.Rtops[tmpHpa.Rpos];
        tmpHpa.Q_start = tmpHpa.Qstarts[tmpHpa.Rpos];
        tmpHpa.T_end = tmpHpa.Tends[tmpHpa.Rpos];
        return 1;
    }

    public static short PcgIanalysis(float[] p_snl, short  size, CLibrary.AnalHPA.ByReference tmpHpa )
    {
        short   d, i,  ss;
        float  wrk,  max;

        ss = 0;
//        memset(tmpHpa->Istarts, 0x00, MAXPOINTS * sizeof(short));

        for( d = 0; d <= tmpHpa.RtopNum; d++)
        {
            i = (short) (tmpHpa.Rtops[d] - 20);
            ss = det_max ( p_snl, (short) (tmpHpa.Rtops[d] - 20), (short) (tmpHpa.Rtops[d] + 120));
            max = (float) Math.abs(p_snl[ss]);
            wrk = 0.5f * max;
            while ( i < tmpHpa.Rtops[d] + 120 )
            {
                if(  p_snl[i] > wrk  )

                {
                    tmpHpa.Istarts[d] = i;
                    break;
                }
                else	i++;
            }
            if( tmpHpa.Istarts[d] == 0 )
                tmpHpa.Istarts[d] = tmpHpa.Rtops[d];
        }

        return( tmpHpa.Istarts[tmpHpa.Rpos] );
    }

    public static short PcgIIanalysis(float[] p_snl, short  size, CLibrary.AnalHPA.ByReference tmpHpa )
    {
        short   d, i,  ss;
        float  wrk,  max;
        ss = 0; wrk = 0.0f;

//        memset(tmpHpa->IIstarts, 0x00, MAXPOINTS * sizeof(short));


        for( d = 0; d <= tmpHpa.QendNum; d++)
        {

            i = (short) (tmpHpa.Istarts[d] +200) ;
            ss = det_max( p_snl, i,(short) (tmpHpa.Rtops[d+1] - 100));

            max = (float) Math.abs(p_snl[ss]);
            wrk = 0.5f * max;
            while ( i < tmpHpa.Rtops[d+1] - 100 )
            {
                if(  p_snl[i] > wrk )

                {
                    tmpHpa.IIstarts[d] = i;
                    break;
                } else i++;
            }
            if( tmpHpa.IIstarts[d] == 0 )
                tmpHpa.IIstarts[d] = tmpHpa.Tends[d] ;
        }
        return( tmpHpa.IIstarts[tmpHpa.Rpos] );
    }

    private static short DetectMin ( float[] signals, short startPos, short endPos )
    {
        short	idx, minPos;
        float	minValue;

        minPos = startPos;
        minValue = signals[startPos];
        for( idx = startPos; idx < endPos; idx++ )
        {
            if( signals[idx] < minValue )
            {
                minValue = signals[idx];
                minPos = idx;
            }
        }
        return minPos;
    }

    private static short MinID(float[] src, short t1, short t2)
    {
        short ID,i;

        ID = t1;
        for(i = t1; i < t2; i++)
        {
            if (src[ID] > src[i]) ID = i;
        }

        return (ID);
    }

    private static short MaxID(float[] src, short t1, short t2)
    {
        short ID,i;

        ID = t1;
        for( i = t1;i < t2;i++ )
        {
            if (src[ID] < src[i]) ID = i;
        }
        return (ID);
    }

    private static short det_max( float[] signals, short startPos, short endPos )
    {
        short	idx, maxPos;
        float	maxValue;

        maxPos = startPos;
        maxValue = (float) Math.abs(signals[startPos]);

        for( idx = startPos; idx < endPos; idx++ )
        {
            if( maxValue < (float)Math.abs(signals[ idx ]) )
            {
                maxValue = (float)Math.abs(signals[ idx ]);
                maxPos = idx;
            }
        }
        return maxPos;
    }
}
