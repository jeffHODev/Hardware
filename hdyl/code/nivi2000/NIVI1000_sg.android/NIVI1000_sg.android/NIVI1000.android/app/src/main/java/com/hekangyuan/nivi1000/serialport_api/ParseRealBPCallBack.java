package com.hekangyuan.nivi1000.serialport_api;

import com.hekangyuan.nivi1000.model.SBPBean;
import com.hekangyuan.nivi1000.model.SRealPressureBean;

public interface ParseRealBPCallBack {

    void onParseSuccess(SRealPressureBean sRealPressureBean);
}
