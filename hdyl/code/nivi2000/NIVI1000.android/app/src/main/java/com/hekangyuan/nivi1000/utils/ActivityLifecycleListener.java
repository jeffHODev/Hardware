package com.hekangyuan.nivi1000.utils;


import android.app.Activity;
import android.app.Application;
import android.os.Bundle;

import com.hekangyuan.nivi1000.MyApplication;

public class ActivityLifecycleListener implements Application.ActivityLifecycleCallbacks {
//    private int refCount = 0;

    @Override
    public void onActivityCreated(Activity activity, Bundle savedInstanceState) {

    }

    @Override
    public void onActivityStarted(Activity activity) {
        MyApplication.refCount++;
    }

    @Override
    public void onActivityResumed(Activity activity) {

    }

    @Override
    public void onActivityPaused(Activity activity) {

    }

    @Override
    public void onActivityStopped(Activity activity) {
        MyApplication.refCount--;
//        if(MyApplication.refCount == 0){
//            setAppGoBackGround(true);
//        }
    }

    @Override
    public void onActivitySaveInstanceState(Activity activity, Bundle outState) {

    }

    @Override
    public void onActivityDestroyed(Activity activity) {

    }
}
