package com.hekangyuan.nivi1000.serialport_api;

import android.util.Log;

import com.hekangyuan.nivi1000.model.SBPBean;
import com.hekangyuan.nivi1000.model.SRealPressureBean;
import com.hekangyuan.nivi1000.model.SerialBean;

import android_serialport_api.SerialBuffer;
import android_serialport_api.SerialPortUtil;

public class SerialParseUtil {

    private static int sn = 0;

    private static int getSn() {
        return sn++;
    }

    public static String packetSend(int head, byte[] data){
        return packetSend(head,getSn(),data);
    }

//    public static String packetSend(int head, byte[] data, int result){
//        return packetSend(head,getSn(),data,result);
//    }

    //head是byte，串口协议head都小于128
    public static String packetSend(int head, int sn,byte[] data){
        StringBuilder buf = new StringBuilder();
        buf.append(HexUtil.Dec2HexString(head));
        buf.append(HexUtil.Dec2HexString(sn));
        if(data == null)
        {
            buf.append(HexUtil.Dec2HexString(0));
            buf.append(HexUtil.Dec2HexString(0));
        }else{
            buf.append(HexUtil.Dec2HexString((data.length >> 8) & 0xFF));  //取高8位
            buf.append(HexUtil.Dec2HexString(data.length & 0xFF)); //取低8位
            buf.append(HexUtil.encodeHexStr(data));
        }
        byte[] tempData = HexUtil.HexString2Bytes(buf.toString());

        int crc = HexUtil.calcCrc16(tempData, 0, tempData.length);

        buf.append(HexUtil.Dec2HexString(crc >> 8));
        buf.append(HexUtil.Dec2HexString(crc & 0xFF));
//        Log.e("packetSend","buf---------  "+buf.toString());
        return buf.toString();
    }

    //head是byte，串口协议head都小于128
//    public static String packetSend(int head, int sn,byte[] data,int result){
//        StringBuilder buf = new StringBuilder();
//        buf.append(HexUtil.Dec2HexString(head));
//        buf.append(HexUtil.Dec2HexString(sn));
//        if(data == null)
//        {
//            buf.append(HexUtil.Dec2HexString(0));
//            buf.append(HexUtil.Dec2HexString(0));
//        }else{
//            buf.append(HexUtil.Dec2HexString((data.length >> 8) & 0xFF));  //取高8位
//            buf.append(HexUtil.Dec2HexString(data.length & 0xFF)); //取低8位
//            buf.append(""+result);
//            buf.append(HexUtil.encodeHexStr(data));
//        }
//        byte[] tempData = HexUtil.HexString2Bytes(buf.toString());
//
//        int crc = HexUtil.calcCrc16(tempData, 0, tempData.length);
//
//        buf.append(HexUtil.Dec2HexString(crc >> 8));
//        buf.append(HexUtil.Dec2HexString(crc & 0xFF));
//        Log.e("packetSend","buf---------@@@@@@@@@"+buf.toString());
//        return buf.toString();
//    }

    //全局buffer，开始只有10000个
    //1、是否是因为处理速度不快，导致inputStream读取数据的时候不连续
    //2、在解析数据的时候数据本身处理位置有问题，导致数据错误
    public static void parseData(ParseSerialCallBack parseSerialCallBack,ParseBPCallBack parseBPCallBack,
                                ParseRealBPCallBack parseRealBPCallBack) {
        doParse(parseSerialCallBack,parseBPCallBack,parseRealBPCallBack);
    }

    public static int dataIndex = 0;
    private static void doParse(ParseSerialCallBack parseSerialCallBack,ParseBPCallBack parseBPCallBack,
                                ParseRealBPCallBack parseRealBPCallBack) {
        //buffer,append处理buffer剩余的数据+serialPortData
        byte[] pData = HexUtil.HexString2Bytes(SerialBuffer.stringBuffer.toString());
        SerialBean serialBean = new SerialBean();
        SBPBean sbpBean = new SBPBean();
        SRealPressureBean realPressureBean = new SRealPressureBean();

        while(true) {
            if(pData.length < 6){
                break;
            }
            //0.脏数据处理
            if(pData[0] != (byte)0x55 && pData[0] != (byte)0x5A){
                //1.要拷贝复制的原始数据
                //2.原始数据的读取位置(从原始数据哪个位置开始拷贝)
                //3.存放要拷贝的原始数据的目的地
                //4.开始存放的位置()
                //5.要读取的原始数据长度(拷贝多长)
                pData = removeFirstByte(pData);
                SerialBuffer.stringBuffer.delete(0,2);
                continue;
            }

            //1.计算数据长度
            int length = (((pData[2] & 0xff) << 8) | (pData[3] & 0xff) & 0xffff)  + 6;
//            Log.e("huang","length "+ length);
            if(length > 2048) {
                pData = removeFirstByte(pData);
                SerialBuffer.stringBuffer.delete(0,2);
                continue;
            }
            if(pData.length < length) {
                break;
            }
//            Log.e("huang","pData.length "+ pData.length);

            //2.crc校验
            int localCrc = HexUtil.calcCrc16(pData, 0, length-2);
            int crc = (((pData[length-2] & 0xff) << 8) | (pData[length-1] & 0xff)) & 0xffff;
//            Log.e("huang","crc = "+crc + " localCrc= " + localCrc);
//            Log.e("huang","length "+ length);

            if(localCrc != crc) {
                pData = removeFirstByte(pData);
                SerialBuffer.stringBuffer.delete(0,2);
                continue;
            }

            //3.协议解析 0x55基础协议，else为响应
            if(pData[0] == (byte)0x55){
                switch (pData[4]) {
                    case (byte)0x01:
                        SerialPortUtil.sendString(packetSend(0x5A, pData[1]&0xff, null));
                        int i = 6;
                        int index = 0;
                        for(int j=0;j<(pData[5]&0xff);j++) {
                            if(i+7 > pData.length) {
                                break;
                            }
                            float data = ((pData[0+i] & 0xff) << 12) | ((pData[1+i] & 0xff) << 4) | ((pData[2+i] & 0xff) >> 4);
//                            data = (data - 0xB964 / 2) * 4 * 240000 / 0xB964 / 7;
                            data = (data - 0xf300 / 2) * 4.0f * 240000 / 0xf300 / 7;
                            int headData = (((pData[2+i] & 0xff) & 0x0F) << 8) | (pData[3+i] & 0xff);
                            int pcgData = ((pData[4+i] & 0xff) << 4) | ((pData[5+i] & 0xff) >> 4);
                            int tstData = (((pData[5+i] & 0xff) & 0x0F) << 8) | (pData[6+i] & 0xff);

//                            serialBean.getEcgdata()[index] = Math.abs(data);
                            serialBean.getEcgdata()[index] = data;
                            serialBean.getPcgdata()[index] = Math.abs(headData);
//                            serialBean.getHeadata()[index] = Math.abs(5000-pcgData); //5000是绝对值上限
                            serialBean.getHeadata()[index] = -pcgData; //5000是绝对值上限

//                            serialBean.getTstdata()[index] = Math.abs(tstData);
                            serialBean.getTstdata()[index] = tstData;
                            index++;
                            i = i+7;
                        }
//                        Log.e("huang","index======================================"+index+"  dataIndex= " +dataIndex++);
//                        Log.e("huang","ecgdata"+serialBean.getEcgdata()[0]);
//                        Log.e("huang","headata"+serialBean.getHeadata()[0]);
//                        Log.e("huang","headata"+serialBean.getHeadata().toString());
//                        Log.e("huang","pcgdata"+serialBean.getPcgdata()[0]);
//                        Log.e("huang","tstdata"+serialBean.getTstdata()[0]);
                        if(parseSerialCallBack != null) {
                            parseSerialCallBack.onParseSuccess(serialBean);
                        }

                        break;
                    case (byte)0x04:
                        sbpBean.setSbp(pData[5] & 0xff); //收缩压
                        sbpBean.setDbp(pData[6] & 0xff); //舒张压
                        sbpBean.setAbp(pData[7] & 0xff); //平均压
                        sbpBean.setHeartRate(pData[8] & 0xff); //心率
                        if(parseBPCallBack != null){
                            parseBPCallBack.onParseSuccess(sbpBean);
                        }

                        break;
                    default:
                        break;
                }
            }else {//实时压力 TODO 需要将实时压力显示在柱状图上面
                switch (pData[4]) {
                    case (byte) 0x05:
                        Log.e("huangli",3333+""+pData[5]);
                        realPressureBean.setrPMMR(pData[5]);

                        break;
                    case (byte) 0x06:
                        Log.e("huangli",3333+"06"+pData[5]);
                        realPressureBean.setrPCuff(pData[5]);
                        if(parseRealBPCallBack != null){
                            parseRealBPCallBack.onParseSuccess(realPressureBean);
                        }
                        break;
                    default:
                        break;
                }


            }
            SerialBuffer.stringBuffer.delete(0,length*2); //delete方法后面不包括

            return;
        }
    }

    //1.要拷贝复制的原始数据
    //2.原始数据的读取位置(从原始数据哪个位置开始拷贝)
    //3.存放要拷贝的原始数据的目的地
    //4.开始存放的位置()
    //5.要读取的原始数据长度(拷贝多长)
    private static byte[] removeFirstByte(byte[] pData) {
        byte[] pData1 = new byte[pData.length-1];
        System.arraycopy(pData, 1, pData1, 0, pData.length-1);
        pData = pData1;
        return pData;
    }
}
