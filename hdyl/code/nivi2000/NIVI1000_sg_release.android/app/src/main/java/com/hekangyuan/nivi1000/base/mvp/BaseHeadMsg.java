package com.hekangyuan.nivi1000.base.mvp;

import java.io.Serializable;

public class BaseHeadMsg implements Serializable {
    private int status;
    private String message;
    public BaseHeadMsg(String message, int code) {
        this.message = message;
        this.status = code;
    }

    public int getErrcode() {
        return status;
    }

    public void setErrcode(int code) {
        this.status = code;
    }

    public String getErrmsg() {
        return message;
    }

    public void setErrmsg(String message) {
        this.message = message;
    }

}
