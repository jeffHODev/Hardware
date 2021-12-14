package com.hospital.nivihospital.modules.dealers.controller;

import com.hospital.nivihospital.common.redis.RedisService;
import com.hospital.nivihospital.common.response.Result;
import com.hospital.nivihospital.modules.dealers.service.IDealersService;
import com.hospital.nivihospital.modules.hospital.entity.AppAccount;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * @author dong
 * @title: DealersController
 * @description: 经销商相关业务控制器
 * @date 2021/11/18 11:29
 */
@RestController
@RequestMapping("/api/dealers")
public class DealersController {
    @Autowired
    private RedisService redisService;

    @Autowired
    private IDealersService dealersService;

    /**
    　* @description: 3.0 获取经销商对应所有医院信息
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/18 11:35
    　* @param
    　* @return
    　*/
    @GetMapping("/getdealershospitallist")
    public Result getDealersHospitalList(@RequestParam(value = "pageNo",defaultValue = "1") int pageNo,
                                          @RequestParam(value = "pageSize",defaultValue ="10") int pageSize){
        Result result = new Result();

        AppAccount account = redisService.getAccount();
        if(account.getEntityType() != 3){
            return result.setStatus(2, "无权访问").setData(new HashMap<>());
        }

        return result.ok().setData(dealersService.getDealersHospitalList(account, pageNo, pageSize));
    }

    /**
    　* @description: 获取医院对应的设备信息
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/18 12:09
    　* @param
    　* @return
    　*/
    @GetMapping("/gethospitaldevicelist")
    public Result getHospitalDeviceList(@RequestParam String hospitalId, @RequestParam(value = "pageNo",defaultValue = "1") int pageNo,
                                         @RequestParam(value = "pageSize",defaultValue ="10") int pageSize){
        Result result = new Result();

        AppAccount account = redisService.getAccount();
        if(account.getEntityType() != 3){
            return result.setStatus(2, "无权访问").setData(new HashMap<>());
        }

        return result.ok().setData(dealersService.getHospitalDeviceList(hospitalId, pageNo, pageSize));
    }
}
