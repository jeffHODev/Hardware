package com.hospital.nivihospital.common.response;

import com.alibaba.fastjson.JSON;
import com.google.common.collect.Maps;

import java.io.Serializable;
import java.util.Map;

public class Result implements Serializable {

    private ResponseHeader header;

    private Map<String, Object> content;

    private Object data;


    public Result() {
        this(ResponseStatusEnum.OK.getStatus(), ResponseStatusEnum.OK.getMessage());
    }

    public Result(Integer status, String message) {
        this.header = new ResponseHeader();
        this.header.setStatus(status);
        this.header.setMessage(message);
        this.content = Maps.newHashMap();
    }

    public Result ok() {
        this.header.setStatus(ResponseStatusEnum.OK.getStatus());
        this.header.setMessage(ResponseStatusEnum.OK.getMessage());
        return this;
    }

    public Result ok(String message) {
        this.header.setStatus(ResponseStatusEnum.OK.getStatus());
        this.header.setMessage(message);
        return this;
    }

    public Result okdata() {
        this.header.setStatus(ResponseStatusEnum.OKDATA.getStatus());
        this.header.setMessage(ResponseStatusEnum.OKDATA.getMessage());
        return this;
    }

    public Result error() {
        this.header.setStatus(ResponseStatusEnum.ERROR.getStatus());
        this.header.setMessage(ResponseStatusEnum.ERROR.getMessage());
        return this;
    }

    public Result error(String message) {
        this.header.setStatus(ResponseStatusEnum.ERROR.getStatus());
        this.header.setMessage(message);
        return this;
    }

    public Result setStatus(ResponseStatusEnum status) {
        header.setStatus(status.getStatus());
        header.setMessage(status.getMessage());
        return this;
    }

    public Result setStatus(int status,String message) {
        this.header.setStatus(status);
        this.header.setMessage(message);
        return this;
    }

    public Result setMessage(String message) {
        header.setMessage(message);
        return this;
    }

    public Result setContent(String key, Object value) {
        content.put(key, value);
        return this;
    }

    public Result setContent(Map map) {
        content.putAll(map);
        return this;
    }

    public ResponseHeader getHeader() {
        return header;
    }

    public Map<String, Object> getContent() {
        return content;
    }

    public Object getData() {
        return data;
    }

    public Result setData(Object data) {
        this.data = data;
        return this;
    }

    public String toJSON() {
        String json = null;
        try {
            json = JSON.toJSONString(this);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return json;
    }
}
