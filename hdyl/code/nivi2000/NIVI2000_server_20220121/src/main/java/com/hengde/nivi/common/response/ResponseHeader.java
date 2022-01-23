package com.hengde.nivi.common.response;

public class ResponseHeader {

    // 响应状态
    private int status;

    // 响应信息
    private String message;

    public Integer getStatus() {
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
