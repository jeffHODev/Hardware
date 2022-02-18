package com.hekangyuan.nivi1000.broadcast;

import static com.blankj.utilcode.util.ActivityUtils.startActivity;

import android.app.Activity;
import android.content.BroadcastReceiver;

import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;
import android.support.v7.app.AppCompatActivity;
import android.text.TextUtils;
import android.util.Log;
import android.widget.Toast;

import com.github.mjdev.libaums.UsbMassStorageDevice;
import com.github.mjdev.libaums.fs.FileSystem;
import com.github.mjdev.libaums.fs.UsbFile;
import com.github.mjdev.libaums.fs.UsbFileInputStream;
import com.hekangyuan.nivi1000.GpioNative;
import com.hekangyuan.nivi1000.activity.LoginActivity;
import com.hekangyuan.nivi1000.activity.ModifyPasswordActivity;
import com.hekangyuan.nivi1000.activity.SettingActivity;
import com.hekangyuan.nivi1000.activity.UsbActivity;
import com.hekangyuan.nivi1000.utils.ThreadPoolWrapper;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

/*
 *@Description: U盘检测广播
 *@Author: hl
 *@Time: 2018/9/20 10:11
 */
public class USBBroadcastReceiver extends BroadcastReceiver {
    private static final String TAG = "USBBroadcastReceiver";
    @Override
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();
        switch (action) {
            case ACTION_USB_PERMISSION://接受到自定义广播
                UsbDevice usbDevice = intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
                //允许权限申请
                if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                    if (usbDevice != null) {
                        //用户已授权，可以进行读取操作
//                        readDevice(context, getUsbMass(usbDevice));
//                        Intent intent1 = new Intent(context, UsbActivity.class);
//                        startActivity(intent1);
                    } else {
//                        Toast.makeText(context, "没有插入U盘", Toast.LENGTH_LONG).show();
                    }
                } else {
//                    Toast.makeText(context, "未获取到U盘权限", Toast.LENGTH_LONG).show();
                }
                break;
            case UsbManager.ACTION_USB_DEVICE_ATTACHED://接收到U盘设备插入广播
                UsbDevice device_add = intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
                if (device_add != null) {

                    UsbInterface anInterface = device_add.getInterface(0);
                    int interfaceClass = anInterface.getInterfaceClass();
                    if(anInterface.getInterfaceClass()==7){
//                        sb.append(”此设备是打印机\n”);
                    }else if(anInterface.getInterfaceClass()==8){

                        Intent intent1 = new Intent(context, UsbActivity.class);
                        startActivity(intent1);
                    } else {
                        Toast.makeText(context, "耳机", Toast.LENGTH_LONG).show();
                    }
                }
                break;
            case UsbManager.ACTION_USB_DEVICE_DETACHED://接收到U盘设设备拔出广播
                Toast.makeText(context, "拔出", Toast.LENGTH_LONG).show();
//                if (UsbActivity.AActivity.isFinishing())
//                UsbActivity.AActivity.finish();
//                UsbActivity.finish();
                break;
        }

        ///< 另外一种乱七八糟拼凑判断U盘插入的方法，不是特别好用 这个地方本来是这样：intent.getExtras().getBoolean("connected") ，但是我发现需要修改为host_connected
        ///< 这种判断方式肯定不如上面的UsbManager.ACTION_USB_DEVICE_ATTACHED
        //        if (intent.getAction().equals(ACTION_USB_PERMISSION)) {
        //            UsbDevice usbDevice = intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
        //            //允许权限申请
        //            if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
        //                if (usbDevice != null) {
        //                    //用户已授权，可以进行读取操作
        //                    readDevice(context, getUsbMass(usbDevice));
        //                } else {
        //                    Toast.makeText(context, "没有插入U盘", Toast.LENGTH_LONG).show();
        //                }
        //            } else {
        //                Toast.makeText(context, "未获取到U盘权限", Toast.LENGTH_LONG).show();
        //            }
        //        } else if (intent.getAction().equals("android.hardware.usb.action.USB_STATE")) {
        //            Toast.makeText(context, "action=" + intent.getAction(), Toast.LENGTH_LONG).show();
        //            Bundle bundle = intent.getExtras();
        //            for (String key : bundle.keySet()) {
        //                Toast.makeText(context, "Key=" + key + " + content=" + bundle.get(key), Toast.LENGTH_LONG).show();
        //            }
        //            if (intent.getExtras().getBoolean("host_connected")) {
        //                // usb 插入
        //                Toast.makeText(context, "插入", Toast.LENGTH_LONG).show();
        //
        //                UsbDevice device_add = intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
        //                Toast.makeText(context, "device_add=" + device_add, Toast.LENGTH_LONG).show();
        //
        //                if (device_add != null) {
        //                    //接收到U盘插入广播，尝试读取U盘设备数据
        //                    redUDiskDevsList(context);
        //               }

        //                UsbMassStorageDevice[] devices =
        //                        UsbMassStorageDevice.getMassStorageDevices(context /* Context or Activity */);
        //                Toast.makeText(context, "devices.length=" + devices.length, Toast.LENGTH_LONG).show();
        //                for (UsbMassStorageDevice device : devices) {
        //
        //                    // before interacting with a device you  need to call init()!
        //                    try {
        //                        device.init();
        //                        // Only uses the first partition on the device
        //                        FileSystem currentFs = device.getPartitions().get(0).getFileSystem();
        //                        Log.d(TAG, "Capacity: " + currentFs.getCapacity());
        //                        Toast.makeText(context, "Capacity: " + currentFs.getCapacity(), Toast.LENGTH_LONG).show();
        //                        Log.d(TAG, "Occupied Space: " + currentFs.getOccupiedSpace());
        //                        Toast.makeText(context, "Occupied Space: " + currentFs.getOccupiedSpace(), Toast.LENGTH_LONG).show();
        //                        Log.d(TAG, "Free Space: " + currentFs.getFreeSpace());
        //                        Toast.makeText(context, "Free Space: " + currentFs.getFreeSpace(), Toast.LENGTH_LONG).show();
        //                        Log.d(TAG, "Chunk size: " + currentFs.getChunkSize());
        //                        Toast.makeText(context, "Chunk size: " + currentFs.getChunkSize(), Toast.LENGTH_LONG).show();
        //                    } catch (IOException e) {
        //                        //e.printStackTrace();
        //                        Toast.makeText(context, "IOException" + e.getMessage(), Toast.LENGTH_LONG).show();
        //                    }
        //                }
        //            } else {
        //                //   usb 拔出
        //                Toast.makeText(context, "拔出", Toast.LENGTH_LONG).show();
        //            }
        //        }
    }

    //当前处接U盘列表
    private UsbMassStorageDevice[] storageDevices;
    //自定义U盘读写权限
    private static final String ACTION_USB_PERMISSION = "com.android.example.USB_PERMISSION";
    //当前U盘所在文件目录
    private UsbFile cFolder;
    private final static String U_DISK_FILE_NAME = "u_disk.txt";

    /**
     * @description U盘设备读取
     * @author ldm
     * @time 2017/9/1 17:20
     */
    private void redUDiskDevsList(Context context) {
        //设备管理器
        UsbManager usbManager = (UsbManager) context.getSystemService(Context.USB_SERVICE);
        //获取U盘存储设备
        storageDevices = UsbMassStorageDevice.getMassStorageDevices(context);
        PendingIntent pendingIntent = PendingIntent.getBroadcast(context, 0,
                new Intent(ACTION_USB_PERMISSION), 0);
        //一般手机只有1个OTG插口
        for (UsbMassStorageDevice device : storageDevices) {
            //读取设备是否有权限
            if (usbManager.hasPermission(device.getUsbDevice())) {
//                readDevice(context, device);
                Toast.makeText(context, "插入", Toast.LENGTH_LONG).show();
//                        Toast.makeText(context, "u盘", Toast.LENGTH_LONG).show();
                Intent intent1 = new Intent(context, UsbActivity.class);
                startActivity(intent1);
            } else {
                //没有权限，进行申请
                usbManager.requestPermission(device.getUsbDevice(), pendingIntent);
            }
        }
        if (storageDevices.length == 0) {
//            Toast.makeText(context, "请插入可用的U盘", Toast.LENGTH_LONG).show();
        }
    }

    /**
     * 获取对应的U盘设备
     * @param usbDevice
     * @return
     */
    private UsbMassStorageDevice getUsbMass(UsbDevice usbDevice) {
        for (UsbMassStorageDevice device : storageDevices) {
            if (usbDevice.equals(device.getUsbDevice())) {
                return device;
            }
        }
        return null;
    }

    /**
     * 读取U盘信息
     * @param context
     * @param device
     */
    private void readDevice(Context context, UsbMassStorageDevice device) {
        try {
            device.init();
            // Only uses the first partition on the device
            FileSystem currentFs = device.getPartitions().get(0).getFileSystem();
            //Toast.makeText(context, "getRootDirectory: " + currentFs.getRootDirectory().getName(), Toast.LENGTH_LONG).show();
            Log.d(TAG, "Capacity: " + currentFs.getCapacity());
            //Toast.makeText(context, "Capacity: " + currentFs.getCapacity(), Toast.LENGTH_LONG).show();
            Log.d(TAG, "Occupied Space: " + currentFs.getOccupiedSpace());
            //Toast.makeText(context, "Occupied Space: " + currentFs.getOccupiedSpace(), Toast.LENGTH_LONG).show();
            Log.d(TAG, "Free Space: " + currentFs.getFreeSpace());
            //Toast.makeText(context, "Free Space: " + currentFs.getFreeSpace(), Toast.LENGTH_LONG).show();
            Log.d(TAG, "Chunk size: " + currentFs.getChunkSize());
            //Toast.makeText(context, "Chunk size: " + currentFs.getChunkSize(), Toast.LENGTH_LONG).show();
            //设置当前文件对象为根目录
            cFolder = currentFs.getRootDirectory();
            ///< 读取当前目录下的文件
            readFromUDisk(context);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 获取U盘文件信息
     * @param context
     */
    private void readFromUDisk(Context context) {
        UsbFile[] usbFiles = new UsbFile[0];
        try {
            usbFiles = cFolder.listFiles();
        } catch (IOException e) {
            e.printStackTrace();
        }
        if (null != usbFiles && usbFiles.length > 0) {
            for (UsbFile usbFile : usbFiles) {
                Toast.makeText(context, "usbFile.getName(): " + usbFile.getName(), Toast.LENGTH_LONG).show();
                if (usbFile.getName().equals(U_DISK_FILE_NAME)) {
                    readTxtFromUDisk(usbFile);
                }
            }
        }
    }

    /**
     * @description 保存数据到U盘，目前是保存到根目录的
     * @author ldm
     * @time 2017/9/1 17:17
     */
    private void saveText2UDisk(Context context, String content) {
        //项目中也把文件保存在了SD卡，其实可以直接把文本读取到U盘指定文件
        File file = null;///< FileUtil.getSaveFile(context.getPackageName() + File.separator + FileUtil.DEFAULT_BIN_DIR, U_DISK_FILE_NAME);
        try {
            FileWriter fw = new FileWriter(file);
            fw.write(content);
            fw.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        if (null != cFolder) {
            //FileUtil.saveSDFile2OTG(file, cFolder);
        }
    }

    /**
     * 读取U盘文件的内容
     * @param usbFile
     */
    private void readTxtFromUDisk(UsbFile usbFile) {
        UsbFile descFile = usbFile;
        //读取文件内容
        InputStream is = new UsbFileInputStream(descFile);
        //读取秘钥中的数据进行匹配
        StringBuilder sb = new StringBuilder();
        BufferedReader bufferedReader = null;
        try {
            bufferedReader = new BufferedReader(new InputStreamReader(is));
            String read;
            while ((read = bufferedReader.readLine()) != null) {
                sb.append(read);
            }
            //            Message msg = mHandler.obtainMessage();
            //            msg.what = 101;
            //            msg.obj = read;
            //            mHandler.sendMessage(msg);
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                if (bufferedReader != null) {
                    bufferedReader.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
