package com.hospital.nivihospital.modules.hospital.controller;

import com.hospital.nivihospital.common.auth.JwtUtil;
import com.hospital.nivihospital.common.redis.RedisService;
import com.hospital.nivihospital.common.response.Result;
import com.hospital.nivihospital.common.util.ValidateUtil;
import com.hospital.nivihospital.modules.dealers.dao.SysDealersMapper;
import com.hospital.nivihospital.modules.doctor.dao.SysDoctorMapper;
import com.hospital.nivihospital.modules.hospital.dao.SysHospitalMapper;
import com.hospital.nivihospital.modules.hospital.entity.Accountpass;
import com.hospital.nivihospital.modules.hospital.entity.AccountLogin;
import com.hospital.nivihospital.modules.hospital.entity.AppAccount;
import com.hospital.nivihospital.modules.hospital.service.IHospitalService;
import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

/**
 * @author dong
 * @title: HospitalController
 * @description: 医院相关控制器
 * @date 2021/10/10 10:43
 */
@RestController
public class HospitalController {
    protected static Logger logger = LoggerFactory.getLogger(HospitalController.class);

    @Autowired
    private RedisService redisService;

    @Autowired
    private IHospitalService hospitalService;

    @Autowired
    private SysHospitalMapper sysHospitalMapper;

    @Autowired
    private SysDoctorMapper sysDoctorMapper;

    @Autowired
    private SysDealersMapper sysDealersMapper;

    /**
     　* @description: 三端实体登录
     　* @author dong
     　* @version: V1.0
     　* @date 2021/10/10 12:55
     　* @param
     　* @return
     　*/
    @PostMapping("/login")
    @Transactional
    public Result login(HttpServletResponse response, @RequestBody AccountLogin account) throws IOException {
        Result result = new Result();
        if(StringUtils.isBlank(account.getLoginName())){
            return result.setStatus(2, "loginName is null").setData(new HashMap<>());
        }
        if(StringUtils.isBlank(account.getPassword())){
            return result.setStatus(2, "password is null").setData(new HashMap<>());
        }

        logger.info("用户登录当前用户为【{}】，用户密码【{}】",account.getLoginName(),account.getPassword());

        AppAccount appAccount = hospitalService.selectByLoginName((String) account.getLoginName());
        if(appAccount == null){
            return result.setStatus(2, "账户信息不存在").setData(new HashMap<>());
        }
        boolean istrue = ValidateUtil.validatePassword((String) account.getPassword(), appAccount.getPassword());
        if(istrue) {
            String token = JwtUtil.generateToken((String) account.getLoginName());
            Map resMap = new HashMap();
            resMap.put("token", token);
            resMap.put("loginName", account.getLoginName());
            int type = appAccount.getEntityType(); //从数据库中，根据账户查询实体类型
            resMap.put("type", type);
            if(type == 1){ //医院--实体类型（1医院 2医生 3经销商）
                resMap.put("hospitalName", sysHospitalMapper.selectHospitalByPrimaryKey(appAccount.getEntityId()).get("name"));
            }else if(type == 2){
                resMap.put("doctorName", sysDoctorMapper.selectByPrimaryKey(appAccount.getEntityId()).getName());
            }else if(type == 3){
                resMap.put("dealersName", sysDealersMapper.selectByPrimaryKey(appAccount.getEntityId()).getName());
            }else{
                return result.setStatus(2, "实体不存在").setData(new HashMap<>());
            }

            redisService.set("appAccount:"+token, appAccount, 36 * 60 * 60L);
            return result.ok().setData(resMap);
        }else {
            return result.setStatus(2, "密码错误");
        }
    }

    /**
    　* @description: 注册
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/30 11:39
    　* @param
    　* @return
    　*/
    @PostMapping("/registered")
    public Result registered() {
        Result result = new Result();
        String pass = ValidateUtil.entryptPassword("123456");
        hospitalService.registered(pass);
        return result.ok().setData(new HashMap<>());
    }

    /**
    　* @description: 退出登录
    　* @author dong
    　* @version: V1.0
    　* @date 2021/10/10 10:22
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
    　* @date 2021/10/10 12:00
    　* @param
    　* @return
    　*/
    @PostMapping("/api/uppass")
    public Result uppass(@RequestBody Accountpass account){
        Result result = new Result();
        logger.info("用户请求修改设备密码，账号【{}】，原密码【{}】，新密码【{}】",account.getAccount(),account.getOldPass(),account.getNewPass());
        if(StringUtils.isBlank(account.getAccount())){
            return result.setStatus(2, "账号获取失败").setData(new HashMap<>());
        }
        if(StringUtils.isBlank(account.getOldPass())){
            return result.setStatus(2, "原密码为空").setData(new HashMap<>());
        }
        if(StringUtils.isBlank(account.getNewPass())){
            return result.setStatus(2, "新密码为空").setData(new HashMap<>());
        }

        AppAccount appAccount = hospitalService.selectByLoginName((String) account.getAccount());
        if(appAccount == null){
            return result.setStatus(2, "账户不存在").setData(new HashMap<>());
        }
        boolean istrue = ValidateUtil.validatePassword((String) account.getOldPass(), appAccount.getPassword());
        if(istrue) {
            hospitalService.uppass(account);
            return result.ok().setData(new HashMap<>());
        }else {
            return result.setStatus(2, "原密码错误");
        }
    }
}
