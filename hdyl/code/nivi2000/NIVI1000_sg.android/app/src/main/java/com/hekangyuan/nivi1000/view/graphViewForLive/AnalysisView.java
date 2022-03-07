package com.hekangyuan.nivi1000.view.graphViewForLive;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

import com.hekangyuan.nivi1000.R;
import com.hekangyuan.nivi1000.utils.Utils;
import com.nivi.nivifilter.Filter;
import com.nivi.nivifilter.iir.IIRFilter;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class AnalysisView extends View {
    //画笔
    protected Paint mPaint;
    protected Paint mPaint1;
    protected int flg;
    //折现的颜色
//    protected int MLINECOLOR = R.color.ecgline;
    protected int MLINECOLOR = R.color.yellow;
    protected int MLINECOLOR11 = R.color.ecgline;
//    protected int MLINECOLOR1 = Color.YELLOW;
protected int MLINECOLOR1 = Color.WHITE;
//    protected int MLINECOLOR2 = Color.GREEN;
//    protected int MLINECOLOR3 = Color.CYAN;
    protected int MLINECOLOR2 = Color.YELLOW;
    protected int MLINECOLOR3 = Color.YELLOW;
//    protected int MLINECOLOR4 = Color.LTGRAY;
//    protected int MLINECOLOR5 = Color.RED;
//    protected int MLINECOLOR6 = Color.MAGENTA;
    protected int MLINECOLOR4 = Color.GREEN;
    protected int MLINECOLOR5 = Color.RED;
    protected int MLINECOLOR6 = Color.WHITE;

    protected int MLINECOLOR7 = Color.MAGENTA;

    protected int WHITE = Color.WHITE;

    //背景颜色
    protected int mBackgroundColor = Color.BLACK;
    //自身的大小
    public static int backGroundWidth;
    public int backGroundHeight;

    protected float widthPerPoint;

    List<Float> mainPointList = new ArrayList<>();
    //心电rtops
    List<Float> rPointList = new ArrayList<>();
    //第一，第二心音
    List<Float> iPointList = new ArrayList<>();
    List<Float> iiPointList = new ArrayList<>();
    //us,dn,ut 起点，拐点，终点
    List<Float> usPointList = new ArrayList<>();
    List<Float> dnPointList = new ArrayList<>();
    List<Float> utPointList = new ArrayList<>();

    List<Integer> rtopPointList = new ArrayList<>();

    protected float viewHeight = 0;//通道高度

    Context mContext;
    public boolean isDrawPathOver = true;
    private float startX = 0;
    private float endX = 0;

    private boolean isClickable = false;

    private TouchDataCallBackListener listener;

    public float getStartX() {
        return startX;
    }

    public void setStartX(float startX) {
        this.startX = startX;
    }

    public float getEndX() {
        return endX;
    }

    public void setEndX(float endX) {
        this.endX = endX;
    }

    public TouchDataCallBackListener getListener() {
        return listener;
    }

    public void setListener(TouchDataCallBackListener listener) {
        this.listener = listener;
    }

    public AnalysisView(Context context) {
        this(context, null);
        init(context);
    }

    public AnalysisView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
        init(context);
    }

    public AnalysisView(Context context, AttributeSet attrs, int defStyleAttr) {
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

    public void setMainData(List<Float> pointList, int color) {
        flg = 1;
        if(pointList == null){
            return;
        }
        startX = 0;
        endX = 0;
        if (isDrawPathOver) {
            mPaint.setColor(getResources().getColor(color));
            isDrawPathOver = false;
            this.mainPointList.clear();
            this.mainPointList = new ArrayList<>(pointList);//每次都是新的list
        }
    }

    public void setMainData1(List<Float> pointList) {
        flg = 2;
        if(pointList == null){
            return;
        }
        startX = 0;
        endX = 0;
        if (isDrawPathOver) {
            isDrawPathOver = false;
            this.mainPointList.clear();
            this.mainPointList = new ArrayList<>(pointList);//每次都是新的list
        }
    }

    //设置线
    public void setrPointList(List<Float> rPointList) {
        this.rPointList = rPointList;
    }

    public void setiPointList(List<Float> iPointList) {
        this.iPointList = iPointList;
    }

    public void setIiPointList(List<Float> iiPointList) {
        this.iiPointList = iiPointList;
    }

    public void setUsPointList(List<Float> usPointList) {
        this.usPointList = usPointList;
    }

    public void setRtopPointList(List<Integer> rtopPointList) {
        this.rtopPointList = rtopPointList;
    }

    public void setDnPointList(List<Float> dnPointList) {
        this.dnPointList = dnPointList;
    }

    public void setUtPointList(List<Float> utPointList) {
        this.utPointList = utPointList;
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
//        if(mainPointList.size() == 5000) {
//            filter();
//        }

        float minData = Float.MAX_VALUE;
        float maxData = 0f;

        for (int i = 0; i < mainPointList.size(); i++) {
            if (mainPointList.get(i) < minData) minData = mainPointList.get(i);
            if (mainPointList.get(i) > maxData) maxData = mainPointList.get(i);
        }

        float yadj = (maxData - minData) / viewHeight; //计算y轴平均值



        for (int i = 0; i < mainPointList.size(); i++) {
            mainPointList.set(i, ((mainPointList.get(i) - minData) / yadj));
        }

    }

    private void filter() {
        float[] source = Utils.unWrap(mainPointList.toArray(new Float[mainPointList.size()]));
        float[] result = Filter.filter(source, (short) 5000, (short) 25);

        mainPointList = Arrays.asList(Utils.wrap(result));
    }

    @Override
    public void setClickable(boolean clickable) {
        isClickable = clickable;
    }

    private void drawPath(Canvas canvas) {
        mPaint.setStyle(Paint.Style.STROKE);
        mPaint.setStrokeWidth(1);
        mPaint1.setStyle(Paint.Style.STROKE);
        mPaint1.setStrokeWidth(1);

        //画波形图
        for (int p = 0; p < mainPointList.size() - 2; p++) {
            if(flg == 1){
//                mPaint.setColor(getResources().getColor(MLINECOLOR11));
                canvas.drawLine(
                        (p) * widthPerPoint, viewHeight - (mainPointList.get(p)),
                        (p + 1) * widthPerPoint, viewHeight - (mainPointList.get(p + 1)),
                        mPaint);
            }else if (flg == 2){
                if(p >= startX && p <= endX){
                    mPaint1.setColor(WHITE); //此区域设置成白色
                }
                else{
                    mPaint1.setColor(getResources().getColor(MLINECOLOR));
                }
                canvas.drawLine(
                        (p) * widthPerPoint, viewHeight - (mainPointList.get(p)),
                        (p + 1) * widthPerPoint, viewHeight - (mainPointList.get(p + 1)),
                        mPaint1);
            }

        }

        //画Rtops
        drawViceData(canvas, rPointList, MLINECOLOR1);
        //画第一心音
        drawViceData(canvas, iPointList, MLINECOLOR2);
        drawViceData(canvas, iiPointList, MLINECOLOR3);

        drawViceData(canvas, usPointList, MLINECOLOR4);
        drawViceData(canvas, dnPointList, MLINECOLOR5);
        drawViceData(canvas, utPointList, MLINECOLOR6);

        drawRtopData(canvas, rtopPointList, MLINECOLOR4);

        isDrawPathOver = true;
    }

    /**
     * 画线
     *
     * @param canvas
     * @param pointList
     * @param color
     */
    private void  drawViceData(Canvas canvas, List<Float> pointList, int color) {
        if (pointList != null && !pointList.isEmpty()) {

            mPaint.setColor(color);
            for (int i = 0; i < pointList.size(); i++) {
                canvas.drawLine(
                        pointList.get(i) * widthPerPoint, 0,
                        pointList.get(i) * widthPerPoint, viewHeight,
                        mPaint
                );
            }
        }
    }

    /**
     * 画线
     *
     * @param canvas
     * @param pointList
     * @param color
     */
    private void drawRtopData(Canvas canvas, List<Integer> pointList, int color) {
        if (pointList != null && !pointList.isEmpty()) {

            mPaint.setColor(color);
            for (int i = 0; i < pointList.size(); i++) {
                canvas.drawLine(
                        pointList.get(i) * widthPerPoint, 0,
                        pointList.get(i) * widthPerPoint, viewHeight,
                        mPaint
                );
            }
        }
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
        //宽度
        int perWidth = (int) (backGroundWidth / 6);
//        mPaint.setColor(getResources().getColor(MLINECOLOR11));
        mPaint.setStrokeWidth(2);
        //画竖线
//        for (int i = 0; i < 6; i++) {
//            canvas.drawLine(i * perWidth, 0, i * perWidth, backGroundHeight, mPaint);
//        }
    }


    /**
     * TODO 只需要画拐点DN 最高点UT
     * @param event
     * @return
     */
    @Override
    public boolean onTouchEvent(MotionEvent event) {

        if(event.getAction() == MotionEvent.ACTION_DOWN && isClickable){
            int x = (int) (event.getX() / widthPerPoint);
            Log.e("huang", "x----" + x );

            //判断x坐标位于哪个区域，找到区域起点和终点，并将区域绘制成白色
            if(rtopPointList == null || rtopPointList.size() == 0){
                return super.onTouchEvent(event);
            }

            int size = rtopPointList.size();
            int id = 0;
            for(int i=0; i<size-1; i++){
                if(x >= rtopPointList.get(i) && x <= rtopPointList.get(i+1)){
                    startX = rtopPointList.get(i);
                    endX = rtopPointList.get(i+1);
                    id = i + 1;
                    Log.e("huang", "startX----" + startX + "--endX---" + endX + "--id--" + id);
                    if(listener != null){
                        listener.callBack(mainPointList.subList((int)startX,(int)endX),id);
                    }
                    invalidate(); // 触发重绘
                    break;
                }
//                else if(x < rtopPointList.get(0)){
////                    startX = 0;
////                    endX = rtopPointList.get(0);
//                    startX = Float.MAX_VALUE;
//                    endX = Float.MIN_VALUE;
//                    id = 0;
//                    Log.e("huang", "startX----" + startX + "--endX---" + endX + "--id--" + id);
//
//                }else if(x > rtopPointList.get(size - 1)){
////                    startX = rtopPointList.get(size - 1);
////                    endX = 5000;
//                    startX = Float.MAX_VALUE;
//                    endX = Float.MIN_VALUE;
//                    id = size;
//                    Log.e("huang", "startX----" + startX + "--endX---" + endX + "--id--" + id);
//
//                }
            }



        }

        return super.onTouchEvent(event);
    }

    public void initViewSize(float widthPerPoint, float viewHeight) {
        this.viewHeight = viewHeight;
        this.widthPerPoint = widthPerPoint;
    }

    public interface TouchDataCallBackListener{
        void callBack(List<Float> mainPointList,int id);
    }
}
