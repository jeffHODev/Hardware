package com.hekangyuan.nivi1000.view.graphViewForLive;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

import com.hekangyuan.nivi1000.R;

import java.util.ArrayList;
import java.util.List;

public class SideView extends View {
    //画笔
    protected Paint mPaint;
    protected Paint mPaint1;
    protected int flg;
    //折现的颜色
//    protected int MLINECOLOR = R.color.ecgline;
    protected int MLINECOLOR = R.color.yellow;

    protected int WHITE = Color.WHITE;

    //背景颜色
    protected int mBackgroundColor = Color.BLACK;
    //自身的大小
    public static int backGroundWidth;
    public int backGroundHeight;

    protected float viewWidth;

    List<Float> mainPointList = new ArrayList<>();

    protected float viewHeight = 0;//通道高度

    Context mContext;
    public boolean isDrawPathOver = true;
    private int startX = 0;
    private int endX = 0;

    private int nums = 0;

    private boolean isClickable = false;
    private Float TsL,UtL,TsR,UtR;

    public SideView(Context context) {
        this(context, null);
        init(context);
    }

    public SideView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
        init(context);
    }

    public SideView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context);
    }

    private void init(Context context) {
        mPaint = new Paint();
        mPaint1 = new Paint();
        mContext = context;

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

        setMeasuredDimension(backGroundWidth, backGroundHeight);

    }

    public void setMainData(List<Float> pointList) {
        flg = 2;
        startX = 0;
        endX = 0;
        if (isDrawPathOver) {
            isDrawPathOver = false;
            this.mainPointList.clear();
            this.mainPointList = new ArrayList<>(pointList);//每次都是新的list
        }
    }

    public void setMainDataL(List<Float> pointList, Float TsL, Float UtL) {
        flg = 1;
        startX = 0;
        endX = 0;
        if (isDrawPathOver) {
            isDrawPathOver = false;
            this.mainPointList.clear();
            this.mainPointList = new ArrayList<>(pointList);//每次都是新的list
            this.TsL = TsL;
            this.UtL = UtL;
        }
    }

    public void setMainDataR(List<Float> pointList, Float TsR, Float UtR) {
        flg = 1;
        startX = 0;
        endX = 0;
        if (isDrawPathOver) {
            isDrawPathOver = false;
            this.mainPointList.clear();
            this.mainPointList = new ArrayList<>(pointList);//每次都是新的list
            this.TsR = TsR;
            this.UtR = UtR;
        }
    }

    /**
     * 画截取之后的左右颈动脉的竖线
     *
     * @param canvas
     * @param point
     * @param color
     */
    private void drawUtDnData(Canvas canvas, Float point, int color) {
        if (point != null && point != 0) {
            mPaint.setColor(color);
            canvas.drawLine(
                    point * viewWidth / nums, 0,
                    point * viewWidth / nums, viewHeight,
                    mPaint
            );
        }
    }


    /**
     * 执行绘制
     */
    public void performDraw() {
        processData();

        invalidate();
    }

    /**
     * 数据处理
     */
    private void processData() {
        processMainData();
    }


    /**
     * 处理主波形数据
     */
    private void processMainData() {
        nums = mainPointList.size();
        float minData = Float.MAX_VALUE;
        float maxData = 0f;

        for (int i = 0; i < mainPointList.size(); i++) {

            if (mainPointList.get(i) < minData) minData = mainPointList.get(i);
            if (mainPointList.get(i) > maxData) maxData = mainPointList.get(i);
        }

        float yadj = (maxData - minData) / viewHeight; //计算y轴平均值



        for (int i = 0; i < mainPointList.size(); i++) {
            mainPointList.set(i,  ((mainPointList.get(i) - minData) / yadj));
        }

    }

    @Override
    public void setClickable(boolean clickable) {
        isClickable = clickable;
    }

    private void drawPath(Canvas canvas) {
        if (mainPointList.size() == 0) {
            return;
        }
        Log.e("huang", "sideView viewWidth----" + viewWidth + "sideView viewHeight----" + viewHeight );

        mPaint.setStyle(Paint.Style.STROKE);
        mPaint.setStrokeWidth(3);
        mPaint1.setStyle(Paint.Style.STROKE);
        mPaint1.setStrokeWidth(3);

        //画波形图
        for (int p = 0; p < mainPointList.size() - 2; p++) {
            if (flg == 1){
                mPaint1.setColor(Color.YELLOW);
                canvas.drawLine(
                        (p) * viewWidth / nums, viewHeight - (mainPointList.get(p)),
                        (p + 1) * viewWidth / nums, viewHeight - (mainPointList.get(p + 1)),
                        mPaint1);
            }else if (flg == 2){
                if(p >= startX && p <= endX){
                    mPaint.setColor(WHITE); //此区域设置成白色
                }else{
                    mPaint.setColor(getResources().getColor(MLINECOLOR));
                    canvas.drawLine(
                            (p) * viewWidth / nums, viewHeight - (mainPointList.get(p)),
                            (p + 1) * viewWidth / nums, viewHeight - (mainPointList.get(p + 1)),
                            mPaint);
                }
            }





//            float y = viewHeight - mainPointList.get(p);
//            float y1 = viewHeight - mainPointList.get(p+1);
//            if(y1 - y > 50){
//                continue;
//            }
//
//            if(p >= 240 && p<= 280){
//                Log.e("huang", "sideView y----" + (viewHeight - (mainPointList.get(p))));
//                mPaint.setColor(Color.RED); //此区域设置成白色
//            }


        }

        drawUtDnData(canvas, UtL, Color.WHITE);
        drawUtDnData(canvas, TsL, Color.RED);
        drawUtDnData(canvas, UtR, Color.WHITE);
        drawUtDnData(canvas, TsR, Color.RED);

        isDrawPathOver = true;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        clearCanvas(canvas);
        initBackground(canvas);
        drawPath(canvas);
    }

    private void clearCanvas(Canvas canvas) {
        canvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR);
    }

    //绘制背景
    protected void initBackground(Canvas canvas) {
        canvas.drawColor(mBackgroundColor);
        mPaint.setColor(getResources().getColor(MLINECOLOR));
        mPaint.setStrokeWidth(2);

    }

    public void initViewSize(float viewWidth, float viewHeight) {
        this.viewWidth = viewWidth;
        this.viewHeight = viewHeight;

//        Log.e("huang", "sideView viewWidth----" + viewWidth + "sideView viewHeight----" + viewHeight );

    }
}
