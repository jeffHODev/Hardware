package com.hospital.nivihospital;

import com.hospital.nivihospital.common.auth.JwtAuthenticationFilter;
import org.mybatis.spring.annotation.MapperScan;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.context.properties.EnableConfigurationProperties;
import org.springframework.boot.web.servlet.FilterRegistrationBean;
import org.springframework.context.annotation.Bean;
import org.springframework.transaction.annotation.EnableTransactionManagement;

/**
 * @author dong
 * @title: NIVIHospitalApplication
 * @description: 医院端启动类
 * @date 2021/7/26 10:06
 */
@SpringBootApplication
@MapperScan("com.hospital.nivihospital.modules.**.dao")
@EnableTransactionManagement
public class NIVIHospitalApplication {

    @Bean
    public FilterRegistrationBean jwtFilter() {
        final FilterRegistrationBean registrationBean = new FilterRegistrationBean();
        JwtAuthenticationFilter filter = new JwtAuthenticationFilter();
        registrationBean.setFilter(filter);
        return registrationBean;
    }

    public static void main(String[] args) {
        SpringApplication.run(NIVIHospitalApplication.class, args);
    }
}
