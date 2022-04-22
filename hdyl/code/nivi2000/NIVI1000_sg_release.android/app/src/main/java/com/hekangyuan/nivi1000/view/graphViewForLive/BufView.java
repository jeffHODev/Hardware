package com.hekangyuan.nivi1000.view.graphViewForLive;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

import com.hekangyuan.nivi1000.R;

import java.util.ArrayList;
import java.util.List;

public class BufView extends View {
    //画笔
    protected Paint mPaint;
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

    public BufView(Context context) {
        this(context, null);
        init(context);
    }

    public BufView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
        init(context);
    }

    public BufView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context);
    }

    private void init(Context context) {
        mPaint = new Paint();
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
        startX = 0;
        endX = 0;
        if (isDrawPathOver) {
            isDrawPathOver = false;
            this.mainPointList.clear();
            this.mainPointList = new ArrayList<>(pointList);//每次都是新的list
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
        float minData = Float.MAX_VALUE;
        float maxData = 0f;

        for (int i = 0; i < mainPointList.size(); i++) {
            if (mainPointList.get(i) == 0 && mainPointList.get(i + 1) == 0) { // 连续两个为0的数据
                nums = i + 1; //总个数
                break;
            }
            if (mainPointList.get(i) < minData) minData = mainPointList.get(i);
            if (mainPointList.get(i) > maxData) maxData = mainPointList.get(i);
        }

        float yadj = (maxData - minData) / viewHeight; //计算y轴平均值


        for (int i = 0; i < mainPointList.size(); i++) {
            mainPointList.set(i, ((mainPointList.get(i) - minData) / yadj));
        }

    }

    private void drawPath(Canvas canvas) {
        if (mainPointList.size() == 0) {
            return;
        }

        mPaint.setStyle(Paint.Style.STROKE);
        mPaint.setStrokeWidth(3);

        //画波形图
        for (int p = 0; p < mainPointList.size() - 2; p++) {

            if (p >= startX && p <= endX) {
                mPaint.setColor(WHITE); //此区域设置成白色
            } else {
                mPaint.setColor(getResources().getColor(MLINECOLOR));
            }
            canvas.drawLine(
                    (p) * viewWidth / nums, viewHeight - (mainPointList.get(p)),
                    (p + 1) * viewWidth / nums, viewHeight - (mainPointList.get(p + 1)),
                    mPaint);

        }


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

    public void initViewSize(float widthBuff, int viewHeight) {
        this.viewWidth = widthBuff;
        this.viewHeight = viewHeight;
    }

}
