package com.hekangyuan.nivi1000.base;

/**
 * File descripition:   异常处理基类
 *

 */

public class ApiException extends RuntimeException {
    private int errorCode;

    public ApiException(int code, String msg) {
        super(msg);
        this.errorCode = code;
    }

    public int getErrorCode() {
        return errorCode;
    }

}
