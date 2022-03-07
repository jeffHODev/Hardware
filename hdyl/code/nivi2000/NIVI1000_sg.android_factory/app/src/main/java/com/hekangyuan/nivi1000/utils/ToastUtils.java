package com.hekangyuan.nivi1000.utils;

import android.content.Context;
import android.view.Gravity;
import android.widget.Toast;

import java.util.Timer;
import java.util.TimerTask;

/**
 * Created by liuchao on 2020/2/5.
 */
public class ToastUtils {
    private static String oldMsg;
    protected static Toast toast = null;
    private static long oneTime = 0;
    private static long twoTime = 0;

    //设置弹窗位置和必要设置，传递当前参数，设置显示时长
    public static void showToast(Context context, String s) {
        if (toast == null) {
            toast = Toast.makeText(context, s, Toast.LENGTH_SHORT);
            toast.setGravity(Gravity.CENTER, 0, 150);
            toast.show();
            oneTime = System.currentTimeMillis();
        } else {
            twoTime = System.currentTimeMillis();
            if (s.equals(oldMsg)) {
                if (twoTime - oneTime > Toast.LENGTH_SHORT) {
                    toast.show();
                }
            } else {
                oldMsg = s;
                toast.setText(s);
                toast.show();
            }
        }
        oneTime = twoTime;
    }

    public static void showToastLong(Context context, String s) {
        if (toast == null) {
            toast = Toast.makeText(context, s, Toast.LENGTH_LONG);
            toast.setGravity(Gravity.CENTER, 0, 150);
            toast.show();
        } else {
            if (s.equals(oldMsg)) {
                    toast.show();
            } else {
                oldMsg = s;
                toast = Toast.makeText(context, oldMsg, Toast.LENGTH_LONG);
                toast.setGravity(Gravity.CENTER, 0, 150);
                toast.show();
            }
        }
    }

    public static void showToast(Context context, int resId) {
        showToast(context, context.getString(resId));
    }

}
