package com.hengde.nivi.common.global;

import lombok.Data;
import org.springframework.boot.context.properties.ConfigurationProperties;

/**
 * @author dong
 * @title: GlobalClass
 * @description: 自定义全局变量
 * @date 2020/7/29 15:36
 */
@ConfigurationProperties(prefix = "globalclass")
@Data
public class GlobalClass {
    private float[] pgaSnl;
}
