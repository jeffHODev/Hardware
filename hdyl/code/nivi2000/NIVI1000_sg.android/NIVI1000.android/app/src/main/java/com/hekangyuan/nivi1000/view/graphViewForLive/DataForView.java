package com.hekangyuan.nivi1000.view.graphViewForLive;

import android.util.Log;

/**
 * 为波形图提供数据的类
 * 以后对数据 处理都可以在 这个类中进行
 *
 * @author huang
 */
public class DataForView {

    private static final int MAX_DATA_NUM = 200;
    float minData = Float.MAX_VALUE;
    float maxData = 0f;

    //设置数据压缩比例
    private int scale = 10;

    //设置y轴偏离值 百分比
    private float yPivot = 0f;

    //用于指定最终位置的值
    int index = 0;

    float[] data;

    public DataForView(float[] data) {
        this.data = data;
    }

    public float[] getData() {
        return data;
    }

    public void setData(float[] data) {
        this.data = data;
    }

    public void setScale(int scale) {
        this.scale = scale;
    }

    public void setYPivot(float yPivot) {
        this.yPivot = yPivot;
    }


    /**
     * 返回指定数量的数据，同时将 指针移动到指定位置
     *
     * @param num
     */
    public float[] getNumOfData(int num, int viewHeight) {
        float[] tempArray = new float[num];

        for (int i = 0; i < num; i++) {
            if (data[index] < minData) minData = data[index];
            if (data[index] > maxData) maxData = data[index];

            index++;
        }

        float yadj = (maxData - minData) / viewHeight; //计算y轴平均值
//        Log.e("huang", "yadj----" + yadj);

        for (int i = 0; i < num; i++) {
            tempArray[i] = (data[i] - minData) / yadj;
//            Log.e("huang", "temp----" + tempArray[i]);

        }
        return tempArray;
    }

}
