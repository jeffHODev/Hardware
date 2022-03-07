package com.hekangyuan.nivi1000.view.graphViewForLive;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

import com.hekangyuan.nivi1000.R;

/**
 * 在背景图片上根据x,y坐标进行绘制
 *
 * 传入参数包括backgroundImg, x, y
 */
public class QView extends View {
    //画笔
    protected Paint mPaint;

    float x = 0,y = 0;
    float xR = 0,yR = 0;

    //自身的大小
    public int backGroundWidth;
    public int backGroundHeight;



    Context mContext;
    private Bitmap mBitmap;

    public QView(Context context) {
        this(context, null);
        init(context);
    }

    public QView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
        init(context);
    }

    public QView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context);
    }

    private void init(Context context) {
        mPaint = new Paint();
        mContext = context;

    }

    public int getBackGroundWidth() {
        return backGroundWidth;
    }

    public void setBackGroundWidth(int backGroundWidth) {
        backGroundWidth = backGroundWidth;
    }

    public int getBackGroundHeight() {
        return backGroundHeight;
    }

    public void setBackGroundHeight(int backGroundHeight) {
        this.backGroundHeight = backGroundHeight;
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        int widthSpecMode = MeasureSpec.getMode(widthMeasureSpec);
        int widthSpecSize = MeasureSpec.getSize(widthMeasureSpec);
        int heightSpecMode = MeasureSpec.getMode(heightMeasureSpec);
        int heightSpecSize = MeasureSpec.getSize(heightMeasureSpec);
        backGroundWidth = widthSpecSize;
        backGroundHeight = heightSpecSize;

        Log.e("huang","backGround ：backGroundWidth "+backGroundWidth+"  :  "+backGroundHeight);

        setMeasuredDimension(backGroundWidth, backGroundHeight);
    }

    public void setData(float x, float y, float xR, float yR) {
        this.x = x;
        this.y = y;

        this.xR = xR;
        this.yR = yR;

        invalidate();
    }

    @Override
    protected void onDraw(Canvas canvas) {
//        clearCanvas(canvas);
        initBackground(canvas);

        drawPoint(canvas);
    }

    private void drawPoint(Canvas canvas) {
//        Bitmap pic = BitmapFactory.decodeResource(getResources(), R.drawable.b);

        Bitmap pic = BitmapFactory.decodeResource(getResources(), R.drawable.bb);
        Bitmap picScale = Bitmap.createScaledBitmap(pic, 15, 15, true);
        canvas.drawBitmap(picScale,x,backGroundHeight-y,mPaint); //左上角

        Bitmap pic1 = BitmapFactory.decodeResource(getResources(), R.drawable.bbb);
        Bitmap picScale1 = Bitmap.createScaledBitmap(pic1, 15, 15, true);
        canvas.drawBitmap(picScale1,xR,backGroundHeight-yR,mPaint); //左上角
    }

    private void clearCanvas(Canvas canvas) {
        canvas.drawColor(Color.WHITE, PorterDuff.Mode.CLEAR);
    }

    //绘制背景
    protected void initBackground(Canvas canvas) {


        Bitmap bmp = BitmapFactory.decodeResource(getResources(), R.mipmap.qimg);

        mBitmap = Bitmap.createScaledBitmap(bmp, backGroundWidth, backGroundHeight, true);

        canvas.drawBitmap(mBitmap,0,0,mPaint);


    }

}
