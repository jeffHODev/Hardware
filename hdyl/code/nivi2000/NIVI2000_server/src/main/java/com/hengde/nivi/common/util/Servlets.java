package com.hengde.nivi.common.util;

import org.springframework.web.context.request.RequestContextHolder;
import org.springframework.web.context.request.ServletRequestAttributes;
import javax.servlet.http.HttpServletRequest;

/**
 * Http与Servlet工具类.
 */
public class Servlets {
	/**
	 * 获取当前请求对象
	 * @return
	 */
	public static HttpServletRequest getRequest() {
		try{
			return ((ServletRequestAttributes) RequestContextHolder.getRequestAttributes()).getRequest();
		}catch(Exception e){
			return null;
		}
	}
}
