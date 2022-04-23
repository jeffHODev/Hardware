package com.nivi.nivifilter;

import com.sun.jna.Library;
import com.sun.jna.Native;

public class Max {
    static {
        System.loadLibrary("max");
    }
    public static native short MaxID1(float[] src, short t1, short t2);
    public static native short MaxID();
//    public interface CLibrary extends Library {
//        CLibrary INSTANCE = (CLibrary) Native.loadLibrary("max", CLibrary.class);
//
//        short MaxID(float[] src, short t1, short t2);
//    }

    /*public static void main(String[] args) {
        //System.loadLibrary("hello");
        //System.out.println(CLibrary.INSTANCE.MaxID(new float[]{1.3f,4.4f,6.3f,2.6f,6.9f}, (short)0, (short)5));
    }*/
}
