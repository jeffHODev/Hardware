package com.hengde.nivi.common.response;

public enum ResponseStatusEnum {

    /**
     * 请求成功
     */
    OK(1, "请求成功"),

    /**
     * 系统错误
     */
    ERROR(0, "系统错误"),

    /**
     * 请求成功
     */
    OKDATA(2, "请求成功，无数据返回"),

    /**
     * 系统错误
     */
    SMS_ERROR(-1, "短信验证失败"),

    /**
     * uid、token不匹配
     */
    INVALID_TOKEN(-2, "认证错误");

    private int status;

    private String message;

    ResponseStatusEnum(int status, String message) {
        this.status = status;
        this.message = message;
    }

    public int getStatus() {
        return status;
    }

    public void setStatus(int status) {
        this.status = status;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

}
