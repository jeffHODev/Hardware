package com.hekangyuan.nivi1000.print;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Build;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.print.PrintAttributes;
import android.print.PrintManager;
import android.support.annotation.RequiresApi;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v4.print.PrintHelper;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.view.View;

import com.hekangyuan.nivi1000.MyApplication;
import com.hekangyuan.nivi1000.R;
import com.hekangyuan.nivi1000.activity.api.PrintPDFView;
import com.hekangyuan.nivi1000.activity.presenter.PringtPDFPresenter;
import com.hekangyuan.nivi1000.base.BaseActivity;
import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.file.FileUtils;
import com.hekangyuan.nivi1000.model.PDFBean;
import com.hekangyuan.nivi1000.utils.Constants;
import com.hekangyuan.nivi1000.utils.NetWorkUtils;
import com.hekangyuan.nivi1000.utils.ToastUtils;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

//Reminder before you start use this demo
/*
you should an service from app market to search printer in the same network, such as HP Print Service.
some android phones are not support this kind of service, these phones can not use this function.
*/
public class PrintActivity extends BaseActivity<PringtPDFPresenter> implements PrintPDFView,View.OnClickListener{

    private PDFBean pdfBean;


    public void requestPermission(){
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED){
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_EXTERNAL_STORAGE,
                            Manifest.permission.MOUNT_UNMOUNT_FILESYSTEMS},
                    3);
        }

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED){
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.ACCESS_COARSE_LOCATION, Manifest.permission.BLUETOOTH,
                            Manifest.permission.BLUETOOTH_ADMIN},
                    5);
        }

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.MOUNT_UNMOUNT_FILESYSTEMS) != PackageManager.PERMISSION_GRANTED){
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_EXTERNAL_STORAGE,
                            Manifest.permission.MOUNT_UNMOUNT_FILESYSTEMS},
                    4);
        }

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.INTERNET) != PackageManager.PERMISSION_GRANTED){
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.INTERNET}, 6);
        }
    }

    @Override
    protected PringtPDFPresenter createPresenter() {
        return new PringtPDFPresenter(this);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestPermission();

        findViewById(R.id.printPicture).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    doPhotoPrint();
                } catch (IOException e) {
                    Log.i("blb", "--------print picture error");
                }
            }
        });

        findViewById(R.id.printDocument).setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.KITKAT)
            @Override
            public void onClick(View v) {
//                doDocPrint("blb" + System.currentTimeMillis());
//                File dataFile=new File(getCacheDir(),"state.pdf");
                doPdfPrint(getPdfPath());

            }
        });

        findViewById(R.id.getPDF).setOnClickListener(this);

        final Handler mHandler =  new Handler(){
            @Override
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
            }
        };

        new Thread(){
            @Override
            public void run() {
                super.run();
                mHandler.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            Thread.sleep(20000);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }

                        Log.e("huang","test------");

                    }
                },20000);
            }
        }.start();

    }

    @Override
    protected int getLayoutId() {
        return R.layout.activity_print;
    }

    @Override
    protected void initData() {

    }

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    private void doDocPrint(String jobName) {
        PrintManager printManager = (PrintManager) getSystemService(Context.PRINT_SERVICE);
        PrintAttributes.Builder builder = new PrintAttributes.Builder();
        builder.setColorMode(PrintAttributes.COLOR_MODE_COLOR);
//        builder.setDuplexMode(DUPLEX_MODE_SHORT_EDGE);
//        builder.setResolution();
        PrintAttributes.MediaSize temp = PrintAttributes.MediaSize.ISO_A4;
        temp.asLandscape();
        Log.i("blb", "--------is portrait:" + temp.isPortrait());
        builder.setMediaSize(temp);//invalid param
//        builder.setDuplexMode();
//        builder.setMediaSize(M);
//        PrintAttributes.Margins margins = new PrintAttributes.Margins(10, 10, 10, 10)//invalid param
        builder.setResolution(new PrintAttributes.Resolution("white", "whiteRadish", 300, 300));//invalid param
        MyPrintAdapter myPrintAdapter = new MyPrintAdapter(this);

        printManager.print(jobName, myPrintAdapter, builder.build());
    }

    //print photo from assert directory
    private void doPhotoPrint() throws IOException {
        PrintHelper photoPrinter = new PrintHelper(this);
        photoPrinter.setScaleMode(PrintHelper.SCALE_MODE_FIT);
//        photoPrinter.setColorMode(PrintHelper.COLOR_MODE_COLOR);
        photoPrinter.setColorMode(PrintHelper.COLOR_MODE_MONOCHROME);
        photoPrinter.setOrientation(PrintHelper.ORIENTATION_LANDSCAPE);//invalid param
        Bitmap bitmap = BitmapFactory.decodeStream(getAssets().open("testPicture.jpg"));
//                BitmapFactory.decodeResource(getResources(),
//                R.mipmap.ic_launcher);
        photoPrinter.printBitmap("print_picture_" + (int)(Math.random()*100), bitmap);
    }

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    private void doPdfPrint(String filePath) {
        PrintManager printManager = (PrintManager) getSystemService(Context.PRINT_SERVICE);
        MyPrintAdapter2 myPrintAdapter = new MyPrintAdapter2(filePath);
        printManager.print("jobName", myPrintAdapter, null);
    }


    private String getPdfPath() {
        String pdfPath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/" + "io.pdf";
        Log.e("hl",pdfPath);
        return pdfPath;

    }


    @Override
    public void onMainSuccess(BaseJsonMsg<PDFBean> o) {
        pdfBean = o.getData();

        Log.e("huang",pdfBean.getPDFByte());

        String pdfPath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/" + "io.pdf";
        //将字符串写成文件保存到本地
        FileUtils.Companion.createOrExistsFile(pdfPath);
        File file = FileUtils.Companion.getFileByPath(pdfPath);

        FileOutputStream outputStream = null;
        try {
            outputStream = new FileOutputStream(file);
            outputStream.write(Base64.decode(pdfBean.getPDFByte().getBytes(),Base64.DEFAULT));
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }finally {
            try {
                outputStream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }


    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {

            case R.id.getPDF:

                if (NetWorkUtils.isNetworkAvailable(PrintActivity.this)) {

                    PDFBean pdfBean=new PDFBean();
                    System.out.println("222打印PDF当前测试号为：" + MyApplication.ecgMonitorBean.getBasisMeasurementId());
                    mPresenter.getPdf(pdfBean, MyApplication.ecgMonitorBean.getBasisMeasurementId());
                } else {
                    ToastUtils.showToast(PrintActivity.this, R.string.no_wifi);
                }
                break;

            default:
                break;
        }
    }

}
