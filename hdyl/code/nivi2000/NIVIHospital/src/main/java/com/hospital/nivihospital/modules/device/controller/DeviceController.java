package com.hospital.nivihospital.modules.device.controller;

import com.hospital.nivihospital.common.redis.RedisService;
import com.hospital.nivihospital.common.response.Result;
import com.hospital.nivihospital.modules.device.service.IDeviceService;
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
 * @title: DeviceController
 * @description: 医院设备相关控制器
 * @date 2021/9/17 15:44
 */
@RestController
@RequestMapping("/api/device")
public class DeviceController {

    @Autowired
    private RedisService redisService;

    @Autowired
    private IDeviceService deviceService;

    /**
    　* @description: 获取医院对应设备列表
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/17 9:34
    　* @param
    　* @return
    　*/
    @GetMapping("/getdetectcount")
    public Result getdetectcount(@RequestParam(value = "pageNo",defaultValue = "1") int pageNo,
                                          @RequestParam(value = "pageSize",defaultValue ="10") int pageSize){
        Result result = new Result();

        AppAccount account = redisService.getAccount();
        if(account.getEntityType() != 1){
            return result.setStatus(2, "无权访问").setData(new HashMap<>());
        }

        return result.ok().setData(deviceService.getdetectcount(account.getEntityId(), pageNo, pageSize));
    }

    /**
    　* @description: 获取医院下当天、当周、当月设备数据
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/17 9:35
    　* @param
    　* @return
    　*/
    @GetMapping("/getdaydevice")
    public Result getdaydevice(@RequestParam(value = "pageNo",defaultValue = "1") int pageNo,
                               @RequestParam(value = "pageSize",defaultValue ="10") int pageSize, @RequestParam String startDate, String endDate){
        Result result = new Result();
        //todo 查询先从缓存中拿到 hospitalId
        if(null == endDate || endDate.equals("") || endDate.length() == 0){ //endDate没传默认查询startDate当天数据

        }

        Map hospitalDevice = new HashMap();
        hospitalDevice.put("deviceId", "sadfadsfadsfadsf");
        hospitalDevice.put("evaluationCount", 100);
        hospitalDevice.put("deviceNo", "42W4D1KUIH");
        hospitalDevice.put("date", "2021-08-25");

        Map hospitalDevice1 = new HashMap();
        hospitalDevice1.put("deviceId", "sadfadsfadsfadsf");
        hospitalDevice1.put("evaluationCount", 100);
        hospitalDevice1.put("deviceNo", "42W4D1KUIH");
        hospitalDevice1.put("date", "2021-08-25");

        List list = new ArrayList();
        list.add(hospitalDevice);
        list.add(hospitalDevice1);

        Map resMap = new HashMap();
        resMap.put("dataList", list);
        resMap.put("allCount", 2);
        return result.ok().setData(resMap);
    }

    /**
    　* @description: 获取当前医院下的所有设备信息
    　* @author dong
    　* @version: V1.0
    　* @date 2021/11/17 11:08
    　* @param
    　* @return
    　*/
    @GetMapping("/gethospitaldevice")
    public Result getHospitalDevice(@RequestParam(value = "pageNo",defaultValue = "1") int pageNo,
                               @RequestParam(value = "pageSize",defaultValue ="10") int pageSize){
        Result result = new Result();
        //todo 查询先从缓存中拿到 hospitalId

        Map hospitalDevice = new HashMap();
        hospitalDevice.put("deviceId", "sadfadsfadsfadsf");
        hospitalDevice.put("detectCount", 100); //剩余检测次数
        hospitalDevice.put("useCount", 50); //累计使用次数
        hospitalDevice.put("deviceNo", "42W4D1KUIH");

        Map hospitalDevice1 = new HashMap();
        hospitalDevice1.put("deviceId", "sadfadsfadsfadsf");
        hospitalDevice1.put("detectCount", 100); //剩余检测次数
        hospitalDevice1.put("useCount", 50); //累计使用次数
        hospitalDevice1.put("deviceNo", "42W4D1KUIH");

        List list = new ArrayList();
        list.add(hospitalDevice);
        list.add(hospitalDevice1);

        Map resMap = new HashMap();
        resMap.put("dataList", list);
        resMap.put("allCount", 2);
        return result.ok().setData(resMap);
    }
}
