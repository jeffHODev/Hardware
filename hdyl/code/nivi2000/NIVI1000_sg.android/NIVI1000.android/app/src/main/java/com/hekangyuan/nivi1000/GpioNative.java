package com.hekangyuan.nivi1000;

/**
 * Created by xxs on 17-11-3.
 */

public class GpioNative {

    static {
        System.loadLibrary("native-lib");
    }

    public native static boolean openGpioDev();
    public native static void closeGpioDev();
    public native static void setGpioMode(int index, int mode);
    public native static int getGpioLevel(int index);
    public native static void setGpioLevel(int index, boolean level);
}
