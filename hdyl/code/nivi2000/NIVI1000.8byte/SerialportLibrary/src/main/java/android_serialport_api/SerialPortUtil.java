package android_serialport_api;

import android.os.Handler;
import android.os.Message;
import android.util.Log;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Random;

import callback.SerialPortCallBackUtils;
import util.ByteUtil;

/**
 * Created by Administrator on 2018/5/31.
 */

public class SerialPortUtil {

    public static String TAG = "huang";

    /**
     * 标记当前串口状态(true:打开,false:关闭)
     **/
    public static boolean isFlagSerial = false;

    public static SerialPort serialPort = null;
    public static InputStream inputStream = null;
    public static OutputStream outputStream = null;
    public static Thread receiveThread = null;
    public static SerialBuffer serialBuffer = new SerialBuffer();

    public static boolean isSerialOpen(){
        return isFlagSerial;
    }

    /**
     * 打开串口
     */
    public static boolean open(String pathname, int baudrate, int flags) {
//        Log.e("huangli","open----111");
        boolean isopen = false;
        if (isFlagSerial) {
            return false;
        }
        try {
            serialPort = new SerialPort(new File(pathname), baudrate, flags);
            inputStream = serialPort.getInputStream();
            outputStream = serialPort.getOutputStream();
            receive();
            isopen = true;
            isFlagSerial = true;
        } catch (IOException e) {
            e.printStackTrace();
            isopen = false;
        }
        return isopen;
    }

    /**
     * 关闭串口
     */
    public static boolean close() {
//        Log.e("huangli","close----222");

        if (!isFlagSerial) {
            return false;
        }
        boolean isClose = false;
        try {
            if (inputStream != null) {
                inputStream.close();
            }
            if (outputStream != null) {
                outputStream.close();
            }
            isClose = true;
            isFlagSerial = false;//关闭串口时，连接状态标记为false
        } catch (IOException e) {
            e.printStackTrace();
            isClose = false;
        }
        return isClose;
    }

    /**
     * 发送串口指令
     */
    public static void sendString(String data) {
        if (!isFlagSerial) {
            return;
        }
//        Log.e("huang","data----"+data);

        try {
            outputStream.write(ByteUtil.HexToByteArr(data));
            outputStream.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    /**
     * 接收串口数据的方法
     */

    public static void receive() {
        if (receiveThread != null && !isFlagSerial) {
            return;
        }
        receiveThread = new Thread() {
            @Override
            public void run() {

                while (isFlagSerial) {
                    try {
                        byte[] readData = new byte[32];
                        if (inputStream == null) {
                            return;
                        }
                        int size = 0;
                        while ((size = inputStream.read(readData) )!= -1){
                            String dataString = ByteUtil.bytesToHexString(readData);
//                            Log.e("huangli","dataString---------"+dataString);

                            serialBuffer.put(dataString);
                            try {
                                Thread.sleep(3); //240/44  44=1408/32一个包有44个readData，然后采样率为240ms一个包1608 67*3ms=201ms,
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        };
        receiveThread.start();
    }
}