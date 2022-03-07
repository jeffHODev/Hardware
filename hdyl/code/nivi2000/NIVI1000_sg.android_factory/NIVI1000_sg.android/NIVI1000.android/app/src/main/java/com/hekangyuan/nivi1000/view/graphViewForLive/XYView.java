package com.hekangyuan.nivi1000.view.graphViewForLive;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.drawable.BitmapDrawable;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;

import com.hekangyuan.nivi1000.R;

import java.util.ArrayList;
import java.util.List;

/**
 * 在背景图片上根据x,y坐标进行绘制
 *
 * 传入参数包括backgroundImg, x, y
 */
public class XYView extends View {
    //画笔
    protected Paint mPaint;

    float x = 0,y = 0;

    //自身的大小
    public int backGroundWidth;
    public int backGroundHeight;



    Context mContext;
    private Bitmap mBitmap;

    public XYView(Context context) {
        this(context, null);
        init(context);
    }

    public XYView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
        init(context);
    }

    public XYView(Context context, AttributeSet attrs, int defStyleAttr) {
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

    public void setData(float x, float y) {
        this.x = x-5;
        this.y = y;

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
//
//        Bitmap picScale = Bitmap.createScaledBitmap(pic, 30, 30, true);
//        canvas.drawBitmap(picScale,x,backGroundHeight-y,mPaint); //左上角
        mPaint.setColor(Color.BLUE);
        canvas.drawCircle(x,backGroundHeight-y,7, mPaint); //左上角
    }

    private void clearCanvas(Canvas canvas) {
        canvas.drawColor(Color.WHITE, PorterDuff.Mode.CLEAR);
    }

    //绘制背景
    protected void initBackground(Canvas canvas) {


        Bitmap bmp = BitmapFactory.decodeResource(getResources(), R.drawable.carotidpulsebac);

        mBitmap = Bitmap.createScaledBitmap(bmp, backGroundWidth, backGroundHeight, true);

        canvas.drawBitmap(mBitmap,0,0,mPaint);


    }

}
