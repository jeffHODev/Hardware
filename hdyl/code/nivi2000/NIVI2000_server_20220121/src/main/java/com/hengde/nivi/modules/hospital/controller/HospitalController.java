package com.hengde.nivi.modules.hospital.controller;

import com.hengde.nivi.common.redis.RedisService;
import com.hengde.nivi.common.response.Result;
import com.hengde.nivi.common.util.ValidateUtil;
import com.hengde.nivi.modules.auth.JwtUtil;
import com.hengde.nivi.modules.hospital.dao.HospitalMapper;
import com.hengde.nivi.modules.hospital.entity.Device;
import com.hengde.nivi.modules.hospital.entity.Devpass;
import com.hengde.nivi.modules.hospital.entity.Hospital;
import com.hengde.nivi.modules.hospital.entity.HospitalLogin;
import com.hengde.nivi.modules.hospital.service.IHospitalService;
import io.jsonwebtoken.Jwts;
import io.jsonwebtoken.SignatureAlgorithm;
import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.transaction.annotation.EnableTransactionManagement;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.bind.annotation.*;

import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

/**
 * @author dong
 * @title: HospitalController
 * @description: 医院相关控制器
 * @date 2019/6/18 10:43
 */
@RestController
public class HospitalController {
    protected static Logger logger = LoggerFactory.getLogger(HospitalController.class);

    @Autowired
    private RedisService redisService;

    @Autowired
    private IHospitalService hospitalService;

    /**
     　* @description: 医院设备登录
     　* @author dong
     　* @version: V1.0
     　* @date 2019/6/6 12:55
     　* @param
     　* @return
     　*/
    @PostMapping("/login")
    @Transactional
    public Result login(HttpServletResponse response, @RequestBody HospitalLogin account) throws IOException {
        Result result = new Result();
        if(StringUtils.isBlank(account.getLoginName())){
//            return result.error("loginName is null").setData(new HashMap<>());
            return result.setStatus(2, "loginName is null").setData(new HashMap<>());
        }
        if(StringUtils.isBlank(account.getPassword())){
//            return result.error("password is null").setData(new HashMap<>());
            return result.setStatus(2, "password is null").setData(new HashMap<>());
        }

        logger.info("用户登录当前用户为【{}】，用户密码【{}】",account.getLoginName(),account.getPassword());

        Device device = hospitalService.selectByLoginName((String) account.getLoginName());
        if(device == null){
            if(!account.getPassword().equals("123456")){
//                return result.error("初始密码不正确").setData(new HashMap<>());
                return result.setStatus(2, "初始密码不正确").setData(new HashMap<>());
            }
            hospitalService.deviceinit(account.getLoginName(), account.getPassword());
        }
        Device device1 = hospitalService.selectByLoginName((String) account.getLoginName());
        boolean istrue = ValidateUtil.validatePassword((String) account.getPassword(), device1.getPassword());
        if(istrue) {
            String token = JwtUtil.generateToken((String) account.getLoginName());
            Map resMap = new HashMap();
            resMap.put("token", token);
            resMap.put("loginName", account.getLoginName());
            redisService.set("hosptialdevicelogin:"+token, device, 36 * 60 * 60L);
            return result.ok().setData(resMap);
        }else {
//            return result.error("password error");
//            return result.setStatus(2, "password error");
            return result.setStatus(2, "密码错误");
        }
    }

    /**
    　* @description: 医院设备退出登录
    　* @author dong
    　* @version: V1.0
    　* @date 2019/7/3 10:22
    　* @param
    　* @return
    　*/
    @PostMapping("/api/loginout")
    public Result loginOut(){
        Result result = new Result();
        hospitalService.loginOut();
        return result.ok().setData(new HashMap<>());
    }

    /**
    　* @description: 修改医院设备密码
    　* @author dong
    　* @version: V1.0
    　* @date 2020/11/30 12:00
    　* @param
    　* @return
    　*/
    @PostMapping("/api/uppass")
    public Result uppass(@RequestBody Devpass account){
        Result result = new Result();
        logger.info("用户请求修改设备密码，设备号【{}】，原密码【{}】，新密码【{}】",account.getDeviceNo(),account.getOldPass(),account.getNewPass());
        if(StringUtils.isBlank(account.getDeviceNo())){
//            return result.error("deviceNo is null").setData(new HashMap<>());
            return result.setStatus(2, "设备号获取失败").setData(new HashMap<>());
        }
        if(StringUtils.isBlank(account.getOldPass())){
//            return result.error("oldPass is null").setData(new HashMap<>());
            return result.setStatus(2, "原密码为空").setData(new HashMap<>());
        }
        if(StringUtils.isBlank(account.getNewPass())){
//            return result.error("newPass is null").setData(new HashMap<>());
            return result.setStatus(2, "新密码为空").setData(new HashMap<>());
        }

        Device device = hospitalService.selectByLoginName((String) account.getDeviceNo());
        if(device == null){
//            return result.error("设备不存在").setData(new HashMap<>());
            return result.setStatus(2, "设备不存在").setData(new HashMap<>());
        }
        boolean istrue = ValidateUtil.validatePassword((String) account.getOldPass(), device.getPassword());
        if(istrue) {
            hospitalService.uppass(account);
            return result.ok().setData(new HashMap<>());
        }else {
//            return result.error("oldpassword error");
            return result.setStatus(2, "原密码错误");
        }
    }
}
