package com.hekangyuan.nivi1000;

import com.hekangyuan.nivi1000.utils.ThreadPoolWrapper;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

/**
 * Example local unit test, which will execute on the development machine (host).
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
public class ExampleUnitTest {
    @Test
    public void addition_isCorrect() {
        assertEquals(4, 2 + 2);

        int i = 0;


        class TestRunnable implements Runnable {

            private int name;

            public TestRunnable(int name) {
                this.name = name;
            }

            @Override
            public void run() {
                System.out.println("Thread Name: " + Thread.currentThread().getName() + "  class name--" + name);
                while (true) {

                }
            }
        }

        TestRunnable runnable1 = new TestRunnable(1);
        TestRunnable runnable2 = new TestRunnable(2);
        TestRunnable runnable3 = new TestRunnable(3);
        TestRunnable runnable4 = new TestRunnable(4);
        TestRunnable runnable5 = new TestRunnable(5);
        TestRunnable runnable6 = new TestRunnable(6);

        //小于核心线程数，开始创建线程
        ThreadPoolWrapper.getThreadPool().executeTask(runnable1);
        ThreadPoolWrapper.getThreadPool().executeTask(runnable2);
        //阻塞队列
        ThreadPoolWrapper.getThreadPool().executeTask(runnable3);
        ThreadPoolWrapper.getThreadPool().executeTask(runnable4);
        //最大线程数，重新创建新的线程
        ThreadPoolWrapper.getThreadPool().executeTask(runnable5);
        ThreadPoolWrapper.getThreadPool().executeTask(runnable6);


    }
}