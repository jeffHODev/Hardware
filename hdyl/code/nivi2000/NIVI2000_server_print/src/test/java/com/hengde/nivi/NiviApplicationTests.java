package com.hengde.nivi;

import com.hengde.nivi.common.global.TypeData;
import com.sun.jna.Library;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

import java.util.*;
import java.util.concurrent.LinkedBlockingDeque;

@RunWith(SpringRunner.class)
@SpringBootTest
public class NiviApplicationTests {
    @Autowired
    private TypeData.ByReference mainData;

    LinkedBlockingDeque<String> deque = new LinkedBlockingDeque(100);

    @Test
    public void contextLoads() {
//        float[] fbuf = {65.56f,66.89f,67.98f,68.82f,69.55f,70.37f};
//        StringBuffer stringBuffer = new StringBuffer();
//        for (int i = 0; i < fbuf.length; i++) {
//            stringBuffer.append(fbuf[i] + ",");
//        }
//        String s = stringBuffer.toString();
//        System.out.println(s);

//        System.out.println("时间：" + System.currentTimeMillis());
//        StringBuffer stringBuffer = new StringBuffer();
//        for (int i = 0; i < 15000; i++) {
//            if(i == 15000 - 1){
//                stringBuffer.append(fbuf[0]);
//            }else{
//                stringBuffer.append(fbuf[0] + ",");
//            }
//        }
//        String s = stringBuffer.toString();
//        System.out.println(s);
//
//
//
//
//
//        System.out.println(s.length());
//        String[] ss = s.split(",");
//
//        float[] newFbuf = new float[ss.length];
//        for (int i = 0; i < ss.length; i++){
//            newFbuf[i] = Float.parseFloat(ss[i]);
//        }
//        System.out.println(newFbuf);
//        for (int i = 0; i < newFbuf.length; i++){
//            System.out.print(newFbuf[i]);
//        }
//        System.out.println();
//        System.out.println("时间：" + System.currentTimeMillis());
//
        /*Map<String, Object> map = new HashMap();
        map.put("a", 1);
        map.put("b", 2);
        map.put("c", 3);
        map.put("d", null);
        map.put("e", "");
        map.put("a", 111);

        System.out.println(map);*/
//
//        for (Map.Entry<String, Object> entry : map.entrySet()) {
//            System.out.println("key= " + entry.getKey() + " and value= " + entry.getValue());
//        }

//        int a = 10/0;
        /*int[] a = {-1, 2345, 5489, 9823, 8734, 1290, 6745, 9999999};
        float[] b = new float[8];
        for (int i = 0; i < 8; i++)
        {
            b[i] = (float)a[i];
        }
        for (int y = 0; y < 8; y++){
            System.out.println(b[y]);
        }

        int aa = 30000;
        short bb = 4;
        System.out.println((short)aa);
        System.out.println((short)(aa + bb));
        System.out.println((int)(aa + bb));
        System.out.println((short)aa + bb);
        System.out.println(aa + (int)bb);*/

//        Short a = new Short((short) 0);
//        Short b = new Short((short) 0);
//
//        this.testss(a, b);
//        System.out.println(a);
//        System.out.println(b);

        /*mainData.setA1(123);
        mainData.a2 = 111;
        System.out.println(mainData.getA1());
        System.out.println(mainData.a2);
        this.testJna();
        System.out.println(mainData.getA1());
        System.out.println(mainData.a2);*/

        /*float jPwvBase = (float)Math.sqrt((float)49 / 45.0f) * 740.0f;
        float largerKe = 1 / 1.1141803f;
        float largerKeBase = 1 / 1.4511938f;
        float largerAco = (float)(1.09f * jPwvBase * (Math.sqrt(652.9022f / jPwvBase) * 0.8f + Math.sqrt(largerKe / largerKeBase) * 0.2f));
        float largerAcoBase = 1.09f * jPwvBase;

        float asoresult = largerAco / largerAcoBase;
        System.out.println("asoresult:" + asoresult);
        largerAco = 1.4f * (float)Math.pow( largerAco / 1.4f, 0.25f);
        System.out.println("结果：" + largerAco);*/

        /*System.out.println(Math.pow((-0.139519f * 0.2f), 0.125f));

        int [] data = {1,2,3,4,5,6,7,8,9};
        int [] newData;
        newData = Arrays.copyOfRange(data, 2, 7);
        for(int i:newData)
            System.out.print(i+" ");*/

        /*float a1 = 116.610565f;
//        double a2=Double.valueOf(String.valueOf(a1));
        double a2 = (double)a1;
        System.out.println("------------------" + a2);

        float a = 1.4f;
        System.out.println(Math.round(a));*/

        //测试多线程
        /*Thread worker = new Thread(new Runnable() {
            @Override
            public void run() {
                for (int i = 0; i < 100; i++) {
                    deque.offer(i + "");
                }
            }
        });
        worker.start();
        try {
            for (int y = 0; y < 105; y++){
                System.out.println(deque.take());
//                deque.clear();
//                System.out.println(deque.take());
            }
        }catch (Exception e){
        }*/
        /*float[][] buffer = new float[4][10];
        float[][] buffer1 = new float[4][10];
        buffer[0][1] = 2.3f;
        buffer1[0][1] = 8.3f;
        buffer = buffer1;
        System.out.println(buffer[0][1]);*/
//        System.out.println(":::::::::::::" + 195 % 4);

//        for(int i = 0; i < 21; i++){
//            SerialBeanFilter se = new SerialBeanFilter();
//            se.serialBeanFilter(i+1);
//        }
//        System.out.println("第一次：" + Arrays.toString(SerialBeanFilter.xBuf));
//        SerialBeanFilter se1 = new SerialBeanFilter();
//        System.out.println("第二次：" + Arrays.toString(se1.xBuf));
//        List l = new ArrayList();
//        l.add(1);
//        l.add(2);
//        l.add(3);
//        l.add(4);
//        System.out.println("11:" + Arrays.toString(l.toArray()));
//        System.out.println("22:" + l.size());
//        l.remove(0);
//        l.add(5);
//        System.out.println("33:" + Arrays.toString(l.toArray()));
//        System.out.println("44:" + l.size());

//        SerialBeanFilterNew se = null;
//        for (float i = 4000; i < 5000; i++){
//            se = new SerialBeanFilterNew();
//            SerialBeanFilterNew.serialBeanFilter(i);
//        }
//        System.out.println("se.m00:" + SerialBeanFilterNew.m00[0]);
//        System.out.println("se.m91:" + SerialBeanFilterNew.m91[0]);

//        switch (3){
//            case 0:
//                System.out.println("1111");
//                return;
////                System.out.println("2222");
////                break;
////                System.out.println("3333");
////                return;
//            case 1:
//                System.out.println("4444");
//                return;
////                System.out.println("5555");
////                break;
////                System.out.println("6666");
////                return;
//            case 2:
//                System.out.println("7777");
//                return;
////                System.out.println("8888");
////                break;
////                System.out.println("9999");
////                return;
//            case 3:
////                System.out.println("0000");
////                break;
////                System.out.println("1010");
////                break;
////                System.out.println("1212");
//                break;
//            case 4:
//                System.out.println("1313");
//                break;
////                System.out.println("1414");
////                break;
////                System.out.println("1515");
////                return;
//        }
//        System.out.println("aaaaaaa");
//
//        System.out.println((int)1.5);

        System.out.println(Float.isFinite(93.82938f));
        System.out.println(Float.isInfinite((10.0f / 0.0f)));
        System.out.println("测试：" + 10.0f / 0.0f);
        System.out.println(Float.isNaN(93.82938f));
        System.out.println(Float.isInfinite(93.82938f));
        System.out.println(Float.isNaN((10.0f / 0.0f)));
        System.out.println(Float.isFinite((10.0f / 0.0f)));

    }

    public void testss(Short a, Short b){
        int [] data = {1,2,3,4,5,6,7,8,9};
        int [] newData;
        newData = Arrays.copyOfRange(data, 2, 7);
        for(int i:newData)
            System.out.print(i+" ");
    }

    public void testJna(){
//        mainData.setA1(1234);
//        mainData.a2 = 222;

        System.out.println((int)1.5);
    }

}
