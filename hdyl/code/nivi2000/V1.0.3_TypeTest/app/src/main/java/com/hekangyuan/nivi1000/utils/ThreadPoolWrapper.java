package com.hekangyuan.nivi1000.utils;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;


/**
 * corePoolSize:指定了线程池中的线程数量，它的数量决定了添加的任务是开辟新的线程去执行，还是放到workQueue任务队列中去；
 *
 * maximumPoolSize:指定了线程池中的最大线程数量，这个参数会根据你使用的workQueue任务队列的类型，决定线程池会开辟的最大线程数量；
 *
 * keepAliveTime:当线程池中空闲线程数量超过corePoolSize时，多余的线程会在多长时间内被销毁；
 *
 * unit:keepAliveTime的单位
 *
 * workQueue:任务队列，被添加到线程池中，但尚未被执行的任务；它一般分为直接提交队列、有界任务队列、无界任务队列、优先任务队列几种；
 *
 * threadFactory:线程工厂，用于创建线程，一般用默认即可；
 *
 * handler:拒绝策略；当任务太多来不及处理时，如何拒绝任务；
 *
 */
public class ThreadPoolWrapper {
	private static final int COREPOOLSIZE = 20; //核心线程数，一开始启动线程就用它
	private static final int MAXIMUM_POOLSIZE = 30; //最大线程数，核心线程数满了之后，会添加到队列里面，但是最大只能到这个数
	private static final long KEEP_ALIVE_TIME = 30L;
	private static ThreadPoolWrapper instance;
	private BlockingQueue<Runnable> bq;
	private static ThreadPoolExecutor executor;

	private ThreadPoolWrapper() {
		executor = null;
		bq = new ArrayBlockingQueue<Runnable>(1000); //有界队列，超过core时，所有新加入的线程会加入到阻塞队列中，最大线程满了之后还有拒绝策略
		executor = new ThreadPoolExecutor(COREPOOLSIZE,
				MAXIMUM_POOLSIZE, KEEP_ALIVE_TIME,
				TimeUnit.SECONDS, bq);
	}

	public static ThreadPoolWrapper getThreadPool() {
		if (instance == null) {
			synchronized (ThreadPoolWrapper.class){
				if(instance == null){
					instance = new ThreadPoolWrapper();
				}
			}
		}
		return instance;

	}

	public void executeTask(Runnable runnable) {
		executor.execute(runnable);
	}

	public static void removeTask(Runnable runnable) {
		executor.remove(runnable);
	}
	
	public static boolean isThreadPoolActive(){
		if(executor.getActiveCount()>=1){
			return true;
		}
		return false;
	}

	public static void shutdown() {
		executor.shutdown();
		instance = null;
	}
}
