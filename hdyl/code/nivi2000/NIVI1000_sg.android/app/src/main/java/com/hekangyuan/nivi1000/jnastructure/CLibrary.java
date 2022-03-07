package com.hekangyuan.nivi1000.jnastructure;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;

import java.util.Arrays;
import java.util.List;

public interface CLibrary extends Library {
    CLibrary INSTANCE2 = (CLibrary) Native.loadLibrary("ComLib",CLibrary.class);
    CLibrary INSTANCE3 = (CLibrary) Native.loadLibrary("hpassysstr",CLibrary.class);
    CLibrary INSTANCE4 = (CLibrary) Native.loadLibrary("hpassys",CLibrary.class);
    CLibrary INSTANCE5 = (CLibrary) Native.loadLibrary("ecg",CLibrary.class);
    CLibrary INSTANCE6 = (CLibrary) Native.loadLibrary("sub",CLibrary.class);
    CLibrary INSTANCE7 = (CLibrary) Native.loadLibrary("modelstr",CLibrary.class);
    CLibrary INSTANCE8 = (CLibrary) Native.loadLibrary("reshpastr",CLibrary.class);
    CLibrary INSTANCE9 = (CLibrary) Native.loadLibrary("apgstr",CLibrary.class);

    float DetectMaxValue(float[] buff, short start, short end );
    void Filter(float[] signals, Pointer smooth, short allNum, short smNum );
    void FilterMibun(float[] signals, Pointer mibun, short allNum, short smNum );
    void FFT_Filter( short modeFlg, Pointer signals, short sigNum, float lowF,
                     float highF, float sampleInt );
    short PcgIanalysis( float[] p_snl, short size, AnalHPA.ByReference analHPAStruct );
    short PcgIIanalysis( float[] p_snl, short size, AnalHPA.ByReference analHPAStruct );
    short FilterLow( float[] signal, Pointer result, short size, float limT);
    short DetectUSpoint( float[] des ,short start, short end,
                         short ch, short size, AnalHPA.ByReference analHPAStruct );
    short DN_point( float[] des ,short start, short end, short ch, short size, AnalHPA.ByReference analHPAStruct );
    short DetectMax( float[] signals, short startPos, short endPos );
    float max_value(float[] buff, short size );
    float min_value(float[] buff, short size );
    void Spp_Tpp(float[] Ps_snl, float[] Pd_snl,short[] specp,
                 short size1,Pointer retval, Pointer Pga_snl );
    void Dpp_anal( float[] Pd_snl, short T8,float samp, short size2, Pointer retval);
    short RQT_Analysis(float[] k_snl, float[] j_snl, short  Size, AnalHPA.ByReference analHPASStruct, AnalHPA_Data.ByReference analHPADataStruct );
    short get_R_tops(float[] signal, short size , Pointer RTops_x, Pointer QP, Pointer TP);
    void Signal_Move(short[] olds, Pointer pointer, short num);
    void filter_mibun(float[] signal, Pointer pointer, short N, short NP );
    void filter( float[] signal, Pointer pointer, short N, short NP );
    short AbI_IIsoPointGet (float[] fl_pcg, short sig_count, short[] rr,
                            short chnumber, Pointer Iso, Pointer IIso);
    short AbApgStTsGetonPcg (float[] fl_apg, short sig_count,
                             short[] Iso, short[] IIso, short chnumber,
                             Pointer St, Pointer Ts);
    short AbOneGetbyAuto (short[] St, short chnumber);
    short DetectAbsMax(float[] signals, short startPos, short endPos );
    short load_std_data (String fileName);
    void AnalysisHpaMeasure (short pSx, short pBd, short[] Rtop, short[] Qpt,
                             short[] Tpt, short[] PIp, short[] PIIp, short[] Usp, short[] Dnp, short Inx,
                             AnalHPA_Data.ByReference analHPADataStruct, String RsFile, short[] Utp, float PTT1, float AF_Length );
    short signal_load( Pointer buff, short size, String file_name);
    short MinID(float[] src, short t1, short t2);
    short MaxID(float[] src, short t1, short t2);

    public class AnalHPA extends Structure {
        @Override
        protected List getFieldOrder() {
            return Arrays.asList(new String[] { "R_top", "Q_start", "T_end", "I_start", "II_start", "US_point",
                    "DN_point", "UT_point", "Leg", "Rpos", "RtopNum", "Rtops", "Qstarts", "QendNum", "Tends",
                    "Istarts", "IIstarts", "USpnts", "DNpnts", "UTpnts"});
        }

        public static class ByReference extends AnalHPA implements Structure.ByReference { }

        public short	R_top;        /*  in ECG ** R_wave  */
        public short	Q_start;	   /*  in ECG ** Q_wave  */
        public short	T_end;        /*  in ECG ** T_wave  */
        public short	I_start;      /*  in PCG ** I_sound */
        public short	II_start;     /*  in PCG ** II_sound*/
        public short	US_point;     /*  in APG ** US_point*/
        public short	DN_point;     /*  in APG ** DN_point*/
        public short	UT_point;	   /*  in APG ** UT_point*/
        public float    Leg;
        public short	Rpos;
        public short	RtopNum;
        public short[] Rtops = new short[51];
        public short[]	Qstarts = new short[51];
        public short	QendNum;
        public short[]	Tends = new short[51];
        public short[]	Istarts = new short[51];
        public short[]	IIstarts = new short[51];
        public short[]	USpnts = new short[51];
        public short[]	DNpnts = new short[51];
        public short[]	UTpnts = new short[51];

        public short getR_top() {
            return R_top;
        }

        public void setR_top(short r_top) {
            R_top = r_top;
        }

        public short getQ_start() {
            return Q_start;
        }

        public void setQ_start(short q_start) {
            Q_start = q_start;
        }

        public short getT_end() {
            return T_end;
        }

        public void setT_end(short t_end) {
            T_end = t_end;
        }

        public short getI_start() {
            return I_start;
        }

        public void setI_start(short i_start) {
            I_start = i_start;
        }

        public short getII_start() {
            return II_start;
        }

        public void setII_start(short II_start) {
            this.II_start = II_start;
        }

        public short getUS_point() {
            return US_point;
        }

        public void setUS_point(short US_point) {
            this.US_point = US_point;
        }

        public short getDN_point() {
            return DN_point;
        }

        public void setDN_point(short DN_point) {
            this.DN_point = DN_point;
        }

        public short getUT_point() {
            return UT_point;
        }

        public void setUT_point(short UT_point) {
            this.UT_point = UT_point;
        }

        public float getLeg() {
            return Leg;
        }

        public void setLeg(float leg) {
            Leg = leg;
        }

        public short getRpos() {
            return Rpos;
        }

        public void setRpos(short rpos) {
            Rpos = rpos;
        }

        public short getRtopNum() {
            return RtopNum;
        }

        public void setRtopNum(short rtopNum) {
            RtopNum = rtopNum;
        }

        public short[] getRtops() {
            return Rtops;
        }

        public void setRtops(short[] rtops) {
            Rtops = rtops;
        }

        public short[] getQstarts() {
            return Qstarts;
        }

        public void setQstarts(short[] qstarts) {
            Qstarts = qstarts;
        }

        public short getQendNum() {
            return QendNum;
        }

        public void setQendNum(short qendNum) {
            QendNum = qendNum;
        }

        public short[] getTends() {
            return Tends;
        }

        public void setTends(short[] tends) {
            Tends = tends;
        }

        public short[] getIstarts() {
            return Istarts;
        }

        public void setIstarts(short[] istarts) {
            Istarts = istarts;
        }

        public short[] getIIstarts() {
            return IIstarts;
        }

        public void setIIstarts(short[] IIstarts) {
            this.IIstarts = IIstarts;
        }

        public short[] getUSpnts() {
            return USpnts;
        }

        public void setUSpnts(short[] USpnts) {
            this.USpnts = USpnts;
        }

        public short[] getDNpnts() {
            return DNpnts;
        }

        public void setDNpnts(short[] DNpnts) {
            this.DNpnts = DNpnts;
        }

        public short[] getUTpnts() {
            return UTpnts;
        }

        public void setUTpnts(short[] UTpnts) {
            this.UTpnts = UTpnts;
        }
    }
}
