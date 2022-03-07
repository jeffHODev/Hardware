package com.hekangyuan.nivi1000.view;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.text.TextUtils;
import android.content.Context;
import android.util.Log;

public class BitmapUtils {

    public static int IMG_DEFAULT_WIDTH ;      //背景图的宽度
    public static int IMG_DEFAULT_HEIGHT;    //背景图的高度

    public static int QRCODE_X = 370;               //图片位置位置X
    public static int QRCODE_Y = 420;               //图片位置位置Y

    public static int TEXT_CENTER_X = 375;          //小图片居中位置
    public static int TEXT_Y = 1262;                //小图片y

    /**
     *
     * @param context
     * @param backBitmap 背景图
     * @param frontBitmap 上层图
     * @param inviteCode  需要绘制上去的文字
     * @param defBackImg  默认背景图
     * @return
     *
     *
     * 1.算出x坐标，每坐标对应的宽度 （图片宽度/坐标差），用坐标值乘以前面那个值就可以算出在那个位置需要画点了
     *
     * 2.y同理
     */
    public static Bitmap mergeBitmap(Context context, Bitmap backBitmap, Bitmap frontBitmap [], String inviteCode, int defBackImg,int x[],int y[],int w,int h) {

        IMG_DEFAULT_WIDTH=w;
        IMG_DEFAULT_HEIGHT=h;
        if (backBitmap == null || backBitmap.isRecycled() || frontBitmap == null || frontBitmap.length>0) {
            backBitmap = BitmapFactory.decodeResource(context.getResources(), defBackImg);
        }
        backBitmap =scaleBitmap(backBitmap,IMG_DEFAULT_WIDTH,IMG_DEFAULT_HEIGHT,context);
        //合并bitmap
        Bitmap bitmap = Bitmap.createBitmap(backBitmap.getWidth(), backBitmap.getHeight(), Bitmap.Config.RGB_565);
        Canvas canvas = new Canvas(bitmap);
        canvas.drawColor(Color.WHITE);
        canvas.drawBitmap(backBitmap, 0, 0, null);



        for (int i=0;i<frontBitmap.length;i++)
        {
            canvas.drawBitmap(frontBitmap[i], x[i], y[i], null);
        }
//        if(true){
//            canvas.drawBitmap(frontBitmap, QRCODE_X, QRCODE_Y, null);
//        }
        if(!TextUtils.isEmpty(inviteCode)){
            Paint paint = new Paint();
            paint.setTextSize(sp2px(context, 20));
            paint.setColor(Color.WHITE);
            paint.setTextAlign(Paint.Align.CENTER);
            canvas.drawText(inviteCode, TEXT_CENTER_X, TEXT_Y, paint);
        }

        return bitmap;
    }


    public static Bitmap mergeBitmap1(Context context, Bitmap backBitmap, Bitmap frontBitmap, String inviteCode, int defBackImg,double x,double y,int w,int h) {

        IMG_DEFAULT_WIDTH=w;
        IMG_DEFAULT_HEIGHT=h;
        if (backBitmap == null || backBitmap.isRecycled() || frontBitmap == null) { //加载默认图
            backBitmap = BitmapFactory.decodeResource(context.getResources(), defBackImg);
        }
        Log.e("huang","backBitmap before width ：height "+backBitmap.getWidth()+":"+backBitmap.getHeight());
        backBitmap =scaleBitmap(backBitmap,IMG_DEFAULT_WIDTH,IMG_DEFAULT_HEIGHT,context);
        Log.e("huang","backBitmap width ：height "+backBitmap.getWidth()+":"+backBitmap.getHeight());

        //合并bitmap
        Bitmap bitmap = Bitmap.createBitmap(backBitmap.getWidth(), backBitmap.getHeight(), Bitmap.Config.RGB_565);
        Canvas canvas = new Canvas(bitmap);
        canvas.drawColor(Color.BLACK);
        canvas.drawBitmap(backBitmap, 0, 0, null);

        int realX = 0,realY = 0;
        realX = calculateX(x,backBitmap.getWidth());
        realY = calculateX(2.2-y,backBitmap.getHeight());
        canvas.drawBitmap(frontBitmap,realX,realY,null);

        if(!TextUtils.isEmpty(inviteCode)){
            Paint paint = new Paint();
            paint.setTextSize(sp2px(context, 20));
            paint.setColor(Color.WHITE);
            paint.setTextAlign(Paint.Align.CENTER);
            canvas.drawText(inviteCode, TEXT_CENTER_X, TEXT_Y, paint);
        }

        return bitmap;
    }

    private static int calculateX(double x, int width) {
        float perWidth = (float) (width / 2.2);
        return (int) (perWidth * x);
    }

    private static Bitmap scaleBitmap(Bitmap bitmap,int defWith,int defHeght,Context context){
        //缩放
        float scaleX = 1.0f;
        float scaleY = 1.0f;
        Log.e("huang","width ：height "+bitmap.getWidth()+":"+bitmap.getHeight());
        Log.e("huang","width ：height "+defWith+":"+defHeght);
        float density = context.getResources().getDisplayMetrics().density;
        defHeght = (int) (defWith * density);
        defWith = (int) (defWith * density);

        Log.e("huang","scaledDensity defHeght ：defWith "+defHeght+":"+defWith);
        if (bitmap.getWidth() != defWith) {
            scaleX = (defWith * 1.0f / bitmap.getWidth());
        }
        if (bitmap.getHeight() != defHeght) {
            scaleY = (defHeght * 1.0f / bitmap.getHeight());
        }
        Log.e("huang","scaleX ：scaleY "+scaleX+":"+scaleY);

        Matrix matrix = new Matrix();
        matrix.postScale(scaleX, scaleY);
        return Bitmap.createBitmap(bitmap, 0, 0, bitmap.getWidth(), bitmap.getHeight(), matrix, false);
    }

    private static float sp2px(Context context, float sp) {
        final float scale = context.getResources().getDisplayMetrics().scaledDensity;
        return sp * scale;
    }

}
