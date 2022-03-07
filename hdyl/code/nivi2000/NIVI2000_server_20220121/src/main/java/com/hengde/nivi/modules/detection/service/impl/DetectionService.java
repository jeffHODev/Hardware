package com.hengde.nivi.modules.detection.service.impl;

import com.hengde.nivi.common.config.HospitalConfig;
import com.hengde.nivi.common.redis.RedisService;
import com.hengde.nivi.common.util.IdGenerate;
import com.hengde.nivi.modules.analysis.service.IAnalysisApgService;
import com.hengde.nivi.modules.analysis.service.IAnalysisService;
import com.hengde.nivi.modules.create.dao.BasisMeasurementMapper;
import com.hengde.nivi.modules.detection.dao.OriginalWaveformMapper;
import com.hengde.nivi.modules.detection.entity.OriginalWaveform;
import com.hengde.nivi.modules.detection.entity.OriginalWaveformReq;
import com.hengde.nivi.modules.detection.service.IDetectionService;
import com.hengde.nivi.modules.hospital.entity.Device;
import com.hengde.nivi.modules.hospital.entity.Hospital;
import com.hengde.nivi.modules.maincvb.service.IMainCVBService;
import com.lowagie.text.ExceptionConverter;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.transaction.interceptor.TransactionAspectSupport;

import java.util.Date;
import java.util.HashMap;
import java.util.Map;

/**
 * @author dong
 * @title: DetectionService
 * @description: 检测相关业务
 * @date 2019/6/12 11:34
 */
@Service
public class DetectionService implements IDetectionService {
    protected static Logger logger = LoggerFactory.getLogger(DetectionService.class);
    @Autowired
    private OriginalWaveformMapper originalWaveformMapper;
    @Autowired
    private HospitalConfig hospitalConfig;
    @Autowired
    private IAnalysisService analysisService;
    @Autowired
    private IAnalysisApgService analysisApgService;
    @Autowired
    private IMainCVBService mainCVBService;
    @Autowired
    private BasisMeasurementMapper basisMeasurementMapper;

    @Override
    @Transactional
    public Map saveOriginalWaveform(OriginalWaveformReq originalWaveformReq, short IDL, short IDR) {
        Map resultMap = new HashMap();
        OriginalWaveform originalWaveform = originalWaveformMapper.selectByBasisMeasurementId(originalWaveformReq.getBasisMeasurementId());
        //Device device = hospitalConfig.getHospitalDevice(); //因业务需要，暂时先注释掉
        if(originalWaveform != null){
            //return result.error("该原始波形数据已存在").setData(new HashMap<>());
            logger.info("该原始波形数据已存在，开始一次性返回所有数据给前端。。。");
        }else{
            if(originalWaveformReq.getApgL() == null){
                resultMap.put("staCode", 519);
                return resultMap;
            }
            Map parmeMap = new HashMap();
            parmeMap.put("id", IdGenerate.uuid());
            parmeMap.put("createDate", new Date());
            parmeMap.put("delFlag", (byte)0);
            parmeMap.put("basisMeasurementId", originalWaveformReq.getBasisMeasurementId());
//            parmeMap.put("model", originalWaveformReq.getModel());
  //          basisMeasurementMapper.updateModel(parmeMap);
            Map<String, int[]> parMap = new HashMap();
            this.resMap(parMap, originalWaveformReq, null);

            for (Map.Entry<String, int[]> entry : parMap.entrySet()) {
                StringBuffer stringBuffer = new StringBuffer();
                int[] data = entry.getValue();
                int num = data.length;
                for (int i = 0; i < num; i++) {
                    if(i == num - 1){
                        stringBuffer.append(data[i]);
                    }else{
                        stringBuffer.append(data[i] + ",");
                    }
                }
                String s = stringBuffer.toString();
                parmeMap.put(entry.getKey(), s);
            }

            originalWaveformMapper.insert(parmeMap);

            Map parMapBa = new HashMap();
            parMapBa.put("basisMeasurementId", originalWaveformReq.getBasisMeasurementId());
            parMapBa.put("model", originalWaveformReq.getModel());
            basisMeasurementMapper.updateModel(parMapBa);

            //更新医院设备剩余检测次数
            //originalWaveformMapper.updateDetectCountByHospitalId(device.getId()); //因业务需要，暂时先注释掉
        }
        //resultMap.put("detectCount",originalWaveformMapper.selectDetectCountByHospitalId(device.getId())); //因业务需要暂时先注释掉
        //todo 之前没有以下代码，以下代码是保存完原始波形之后返回得竖线用于前端画图使用
        analysisService.ecgPcgApgSysDia(originalWaveformReq.getBasisMeasurementId(), resultMap);
        analysisApgService.preTranslateMessage(originalWaveformReq.getBasisMeasurementId(), IDL, IDR, resultMap, originalWaveformReq.getModel());
        mainCVBService.AnalData_Init(originalWaveformReq.getBasisMeasurementId(), resultMap);

        if(resultMap.containsKey("staCode")){
//            throw new NullPointerException("计算结果不规范：" + resultMap.get("staCode"));
            TransactionAspectSupport.currentTransactionStatus().setRollbackOnly(); //手动回滚事务
            logger.info("计算结果不规范：" + resultMap.get("staCode"));
        }

        return resultMap;
    }

    @Override
    public OriginalWaveformReq getOriginalWaveform(String basisMeasurementId) {
        OriginalWaveform originalWaveform = originalWaveformMapper.selectByBasisMeasurementId(basisMeasurementId);
        if(originalWaveform == null){
            return null;
        }
        Map<String, String> resMap = new HashMap();
        this.resMap(resMap, null, originalWaveform);

        Map<String, int[]> resuMap = new HashMap();
        for (Map.Entry<String, String> entry : resMap.entrySet()) {
            String[] s = entry.getValue().split(",");

            int[] newFbuf = new int[s.length];
            for (int i = 0; i < s.length; i++){
                newFbuf[i] = Integer.parseInt(s[i]);
            }
            resuMap.put(entry.getKey(), newFbuf);
        }
        OriginalWaveformReq originalWaveformReq = new OriginalWaveformReq();
        originalWaveformReq.setId(originalWaveform.getId());
        originalWaveformReq.setHypecgs(resuMap.get("hypecgs"));
        originalWaveformReq.setHyppcgs(resuMap.get("hyppcgs"));
        originalWaveformReq.setHypapgs(resuMap.get("hypapgs"));
        originalWaveformReq.setHypecgd(resuMap.get("hypecgd"));
        originalWaveformReq.setHyppcgd(resuMap.get("hyppcgd"));
        originalWaveformReq.setHypapgd(resuMap.get("hypapgd"));
        originalWaveformReq.setEcgPl(resuMap.get("ecgPl"));
        originalWaveformReq.setPcgPl(resuMap.get("pcgPl"));
        originalWaveformReq.setApgL(resuMap.get("apgL"));
        originalWaveformReq.setEcgPr(resuMap.get("ecgPr"));
        originalWaveformReq.setPcgPr(resuMap.get("pcgPr"));
        originalWaveformReq.setApgR(resuMap.get("apgR"));
        originalWaveformReq.setEcgTh(resuMap.get("ecgTh"));
        originalWaveformReq.setApgTh(resuMap.get("apgTh"));
        originalWaveformReq.setCreateDate(originalWaveform.getCreateDate());
        originalWaveformReq.setBasisMeasurementId(originalWaveform.getBasisMeasurementId());
        originalWaveformReq.setDelFlag(originalWaveform.getDelFlag());
        return originalWaveformReq;
    }

    public void resMap(Map parMap, OriginalWaveformReq originalWaveformReq, OriginalWaveform originalWaveform){
        if(originalWaveformReq == null){
            parMap.put("hypecgs", originalWaveform.getHypecgs());
            parMap.put("hyppcgs", originalWaveform.getHyppcgs());
            parMap.put("hypapgs", originalWaveform.getHypapgs());
            parMap.put("hypecgd", originalWaveform.getHypecgd());
            parMap.put("hyppcgd", originalWaveform.getHyppcgd());
            parMap.put("hypapgd", originalWaveform.getHypapgd());
            parMap.put("ecgPl", originalWaveform.getEcgPl());
            parMap.put("pcgPl", originalWaveform.getPcgPl());
            parMap.put("apgL", originalWaveform.getApgL());
            parMap.put("ecgPr", originalWaveform.getEcgPr());
            parMap.put("pcgPr", originalWaveform.getPcgPr());
            parMap.put("apgR", originalWaveform.getApgR());
            parMap.put("ecgTh", originalWaveform.getEcgTh());
            parMap.put("apgTh", originalWaveform.getApgTh());
        }else{
            parMap.put("hypecgs", originalWaveformReq.getHypecgs());
            parMap.put("hyppcgs", originalWaveformReq.getHyppcgs());
            parMap.put("hypapgs", originalWaveformReq.getHypapgs());
            parMap.put("hypecgd", originalWaveformReq.getHypecgd());
            parMap.put("hyppcgd", originalWaveformReq.getHyppcgd());
            parMap.put("hypapgd", originalWaveformReq.getHypapgd());
            parMap.put("ecgPl", originalWaveformReq.getEcgPl());
            parMap.put("pcgPl", originalWaveformReq.getPcgPl());
            parMap.put("apgL", originalWaveformReq.getApgL());
            parMap.put("ecgPr", originalWaveformReq.getEcgPr());
            parMap.put("pcgPr", originalWaveformReq.getPcgPr());
            parMap.put("apgR", originalWaveformReq.getApgR());
            parMap.put("ecgTh", originalWaveformReq.getEcgTh());
            parMap.put("apgTh", originalWaveformReq.getApgTh());
        }
    }
}
