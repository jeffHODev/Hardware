package com.hekangyuan.nivi1000.base.mvp;

import com.hekangyuan.nivi1000.model.LoginBean;

import java.io.Serializable;

public class BaseJsonMsg <T> implements Serializable {
      private  BaseHeadMsg header;
      private  Object content;
      private T data;

      public BaseHeadMsg getHeader() {
            return header;
      }

      public void setHeader(BaseHeadMsg header) {
            this.header = header;
      }

      public Object getContent() {
            return content;
      }

      public void setContent(Object content) {
            this.content = content;
      }

      public T getData() {
            return data;
      }

      public void setData(T data) {
            this.data = data;
      }





}
