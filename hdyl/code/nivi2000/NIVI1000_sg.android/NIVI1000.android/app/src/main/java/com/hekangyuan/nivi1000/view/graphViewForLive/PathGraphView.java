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
import com.hekangyuan.nivi1000.utils.Utils;
import com.nivi.nivifilter.Filter;
import com.nivi.nivifilter.iir.IIRFilter;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;


/**
 * 1.pointList 每次加入25个点，放到这个集合中，
 * <p>
 * 2.curPosition 当前画到某个点，从当前点开始往后画 pointList个点
 * <p>
 * 3. invalidate从curPosition开始，往后加pointList个点
 * <p>
 * 4.如果curPosition达到最大，curPosition回到初始点
 */
public class PathGraphView extends View {
    //画笔
    protected Paint mPaint;
    protected Paint mPaint1;
    //折现的颜色
    protected int mLineColor = R.color.ecgline;
    protected int mLineColor1 = R.color.lawngreen;
    protected int mLineColor2 = R.color.yellow;
    //背景颜色
    protected int mBackgroundColor = Color.BLACK;
    //自身的大小
    public static int backGroundWidth;
    public int backGroundHeight;

    protected float viewHeght = 0;//通道高度

    protected float widthPerPoint;
    List<Float> pointList = new ArrayList<>();

    Context mContext;
    public boolean isDrawPathOver = true;

    public PathGraphView(Context context) {
        this(context, null);
        init(context);
    }

    public PathGraphView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
        init(context);
    }

    public PathGraphView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context);
    }

    private void init(Context context) {
        mContext = context;
        mPaint = new Paint();
        mPaint1 = new Paint();
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

        Log.e("test", " widthSpecMode: " + widthSpecMode + " heightSpecMode: " + heightSpecMode +
                " widthSpecSize: " + widthSpecSize + " heightSpecSize: " + heightSpecSize);

        setMeasuredDimension(backGroundWidth, backGroundHeight);

    }

    public void setData(List<Float> pointList, float widthPerPoint, float viewHeght, int curPos, int manualYadj) {
        this.viewHeght = viewHeght;

        setData(pointList, widthPerPoint, curPos, manualYadj);

    }

    public void setData1(List<Float> pointList, float widthPerPoint, float viewHeght, int curPos, int manualYadj) {
        this.viewHeght = viewHeght;

        setData1(pointList, widthPerPoint, curPos, manualYadj);

    }

    public void setData2(List<Float> pointList, float widthPerPoint, float viewHeght, int curPos, int manualYadj) {
        this.viewHeght = viewHeght;

        setData2(pointList, widthPerPoint, curPos, manualYadj);

    }

    public void setData3(List<Float> pointList, float widthPerPoint, float viewHeght, int curPos, int manualYadj) {
        this.viewHeght = viewHeght;

        setData3(pointList, widthPerPoint, curPos, manualYadj);

    }

    public void setData4(List<Float> pointList, float widthPerPoint, float viewHeght, int curPos, int manualYadj) {
        this.viewHeght = viewHeght;

        setData4(pointList, widthPerPoint, curPos, manualYadj);

    }

    public void setData(List<Float> pointList, float widthPerPoint, int curPos, int manualYadj) {
        if (isDrawPathOver) {
            isDrawPathOver = false;
            this.pointList.clear();
            this.pointList = new ArrayList<>(pointList);//每次都是新的list
            this.widthPerPoint = widthPerPoint;
            mPaint1.setColor(getResources().getColor(mLineColor));
//            if (curPos == 0){ //每240ms執行一次
            pcgProcessData(manualYadj);
//            }

            invalidate();
        }
    }

    public void setData1(List<Float> pointList, float widthPerPoint, int curPos, int manualYadj) {
        if (isDrawPathOver) {
            isDrawPathOver = false;
            this.pointList.clear();
            this.pointList = new ArrayList<>(pointList);//每次都是新的list
            this.widthPerPoint = widthPerPoint;
            mPaint1.setColor(getResources().getColor(mLineColor1));
//            if (curPos == 0){ //每240ms執行一次
            ecgProcessData(manualYadj);
//            }

            invalidate();
        }
    }

    public void setData2(List<Float> pointList, float widthPerPoint, int curPos, int manualYadj) {
        if (isDrawPathOver) {
            isDrawPathOver = false;
            this.pointList.clear();
            this.pointList = new ArrayList<>(pointList);//每次都是新的list
            this.widthPerPoint = widthPerPoint;
            mPaint1.setColor(getResources().getColor(mLineColor2));
//            if (curPos == 0){ //每240ms執行一次
//            processData();
            headProcessData(manualYadj);
//            }

            invalidate();
        }
    }

    public void setData3(List<Float> pointList, float widthPerPoint, int curPos, int manualYadj) {
        if (isDrawPathOver) {
            isDrawPathOver = false;
            this.pointList.clear();
            this.pointList = new ArrayList<>(pointList);//每次都是新的list
            this.widthPerPoint = widthPerPoint;
//            mPaint1.setColor(getResources().getColor(mLineColor));
            mPaint1.setColor(getResources().getColor(mLineColor2));
//            if (curPos == 0){ //每240ms執行一次
            processData(manualYadj);
//            }

            invalidate();
        }
    }

    public void setData4(List<Float> pointList, float widthPerPoint, int curPos, int manualYadj) {
        if (isDrawPathOver) {
            isDrawPathOver = false;
            this.pointList.clear();
            this.pointList = new ArrayList<>(pointList);//每次都是新的list
            this.widthPerPoint = widthPerPoint;
            mPaint1.setColor(getResources().getColor(mLineColor));
//            if (curPos == 0){ //每240ms執行一次
//            processData(manualYadj);
            headProcessData(manualYadj);
//            }

            invalidate();
        }
    }

    /**
     * 计算yadj
     */
    private void processData(int manualYadj) {
//        if(pointList.size() == 5000) {
//            filter();
//        }


        float minData = Float.MAX_VALUE;
        float maxData = 0f;

        for (int i = 0; i < pointList.size(); i++) {
            if (pointList.get(i) < minData) minData = pointList.get(i);
            if (pointList.get(i) > maxData) maxData = pointList.get(i);

        }

        float yadj;
        if(manualYadj != 0){
            yadj = manualYadj / viewHeght; //计算y轴平均值
            for (int i = 0; i < pointList.size(); i++) {
//                pointList.set(i, (pointList.get(i) - minData) / yadj);
                pointList.set(i, ( (pointList.get(i) - minData) + (manualYadj/2 - (maxData - minData)/2) ) / yadj);
            }
        }else{
            yadj = (maxData - minData) / viewHeght; //计算y轴平均值
            for (int i = 0; i < pointList.size(); i++) {
                pointList.set(i, (pointList.get(i) - minData) / yadj);
            }
        }

//        if (yadj > 5) {
//            yadj = 5;
//        }
//
//        if (yadj < 2) {
//            yadj = 2;
//        }

//        Log.e("huangli", "yadj----" + yadj);

//        for (int i = 0; i < pointList.size(); i++) {
//            pointList.set(i, (pointList.get(i) - minData) / yadj);
//        }

    }

    /**
     * 计算心电yadj
     */
    private void ecgProcessData(int manualYadj) {
//        if(pointList.size() == 5000) {
//            filter();
//        }

        float minData = Float.MAX_VALUE;
        float maxData = 0f;

        for (int i = 0; i < pointList.size(); i++) {
            if (pointList.get(i) < minData) minData = pointList.get(i);
            if (pointList.get(i) > maxData) maxData = pointList.get(i);

        }

        float yadj;
        if(manualYadj != 0){
            yadj = manualYadj / viewHeght; //计算y轴平均值
            for (int i = 0; i < pointList.size(); i++) {
//                pointList.set(i, (pointList.get(i) - minData) / yadj);
            pointList.set(i, ( (pointList.get(i) - minData) + (manualYadj/2 - (maxData - minData)/2) ) / yadj);
            }
        }else{
            yadj = (maxData - minData) / viewHeght; //计算y轴平均值
            for (int i = 0; i < pointList.size(); i++) {
                pointList.set(i, (pointList.get(i) - minData) / yadj);
//                pointList.set(i, ( (pointList.get(i) - minData) + (manualYadj/2 - (maxData - minData)/2) ) / yadj);
            }
        }

//        float yadj = 3000 / viewHeght; //计算y轴平均值
//        if (yadj > 5) {
//            yadj = 5;
//        }
//
//        if (yadj < 2) {
//            yadj = 2;
//        }

//        Log.e("huangli", "yadj----" + yadj);

//        for (int i = 0; i < pointList.size(); i++) {
//            pointList.set(i, (pointList.get(i) - minData) / yadj);
////            pointList.set(i, ( (pointList.get(i) - minData) + (3000/2 - (maxData - minData)/2) ) / yadj);
//        }

    }

    /**
     * 计算心音yadj
     */
    private void pcgProcessData(int manualYadj) {
//        if(pointList.size() == 5000) {
//            filter();
//        }

        float minData = Float.MAX_VALUE;
        float maxData = 0f;

        for (int i = 0; i < pointList.size(); i++) {
            if (pointList.get(i) < minData) minData = pointList.get(i);
            if (pointList.get(i) > maxData) maxData = pointList.get(i);

        }

        float yadj;
        if(manualYadj != 0){
            yadj = manualYadj / viewHeght; //计算y轴平均值
            for (int i = 0; i < pointList.size(); i++) {
//                pointList.set(i, (pointList.get(i) - minData) / yadj);
            pointList.set(i, ( (pointList.get(i) - minData) + (manualYadj/2 - (maxData - minData)/2) ) / yadj);
            }
        }else{
            yadj = (maxData - minData) / viewHeght; //计算y轴平均值
            for (int i = 0; i < pointList.size(); i++) {
                pointList.set(i, (pointList.get(i) - minData) / yadj);
//                pointList.set(i, ( (pointList.get(i) - minData) + (manualYadj/2 - (maxData - minData)/2) ) / yadj);
            }
        }

//        float yadj = 2700 / viewHeght; //计算y轴平均值
//        float yadj = 3000 / viewHeght; //计算y轴平均值
//        if (yadj > 5) {
//            yadj = 5;
//        }
//
//        if (yadj < 2) {
//            yadj = 2;
//        }

//        Log.e("huangli", "yadj----" + yadj);

//        for (int i = 0; i < pointList.size(); i++) {
//            pointList.set(i, (pointList.get(i) - minData) / yadj);
////            pointList.set(i, pointList.get(i) / yadj);
////            pointList.set(i, ( (pointList.get(i) - minData) + (2700/2 - (maxData - minData)/2) ) / yadj);
//        }

    }

    /**
     * 计算颈动脉yadj
     */
    private void headProcessData(int manualYadj) {
//        if(pointList.size() == 5000) {
//            filter();
//        }

        float minData = Float.MAX_VALUE;
        float maxData = 0f;

        for (int i = 0; i < pointList.size(); i++) {
            if (pointList.get(i) < minData) minData = pointList.get(i);
            if (pointList.get(i) > maxData) maxData = pointList.get(i);

        }

        float yadj;
        if(manualYadj != 0){
            yadj = manualYadj / viewHeght; //计算y轴平均值
            for (int i = 0; i < pointList.size(); i++) {
//                pointList.set(i, (pointList.get(i) - minData) / yadj);
            pointList.set(i, ( (pointList.get(i) - minData) + (manualYadj/2 - (maxData - minData)/2) ) / yadj);
            }
        }else{
            yadj = (maxData - minData) / viewHeght; //计算y轴平均值
            for (int i = 0; i < pointList.size(); i++) {
                pointList.set(i, (pointList.get(i) - minData) / yadj);
//                pointList.set(i, ( (pointList.get(i) - minData) + (manualYadj/2 - (maxData - minData)/2) ) / yadj);
            }
        }

//        float yadj = 8000 / viewHeght; //计算y轴平均值
//        float yadj = 3000 / viewHeght; //计算y轴平均值
//        if (yadj > 5) {
//            yadj = 5;
//        }
//
//        if (yadj < 2) {
//            yadj = 2;
//        }

//        Log.e("huangli", "yadj----" + yadj);

//        for (int i = 0; i < pointList.size(); i++) {
//            pointList.set(i, (pointList.get(i) - minData) / yadj);
////            pointList.set(i, pointList.get(i) / yadj);
////            pointList.set(i, ( (pointList.get(i) - minData) + (8000/2 - (maxData - minData)/2) ) / yadj);
//        }

    }

    private void filter() {
        float[] source = Utils.unWrap(pointList.toArray(new Float[pointList.size()]));
//        float[] result = Filter.filter(source, (short) 5000, (short) 25);
        float[] result = IIRFilter.filter(source);

        pointList = Arrays.asList(Utils.wrap(result));
    }


    private void drawPath(Canvas canvas) {
        isDrawPathOver = true;
        if (pointList.size() == 0) {
            return;
        }

        mPaint1.setStyle(Paint.Style.STROKE);
        mPaint1.setStrokeWidth(1);
//        mPaint.setColor(getResources().getColor(mLineColor));

        for (int p = 0; p < pointList.size() - 2; p++) {
//            canvas.drawLine((p) * widthPerPoint, (pointList.get(p)), (p + 1) * widthPerPoint, (pointList.get(p + 1)), mPaint);//画所有点
            canvas.drawLine((p) * widthPerPoint, viewHeght - (pointList.get(p)), (p + 1) * widthPerPoint, viewHeght - (pointList.get(p + 1)), mPaint1);//画所有点

        }

        pointList.clear();
        isDrawPathOver = true;
    }

    @Override
    public void setAlpha(float alpha) {
        super.setAlpha(alpha);
    }

    @Override
    protected void onDraw(Canvas canvas) {
//        Log.e("test", "onDraw");
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
        //宽度
        int perWidth = (int) (backGroundWidth / 6);
        mPaint.setColor(getResources().getColor(mLineColor));
        mPaint.setStrokeWidth(2);
        //画竖线
//        for (int i = 0; i < 6; i++) {
//            canvas.drawLine(i * perWidth, 0, i * perWidth, backGroundHeight, mPaint);
//        }
    }
}
