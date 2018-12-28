import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

/*
 * http://www.runoob.com/memcached/java-memcached.html
 * */
import java.util.concurrent.Future;
import net.spy.memcached.MemcachedClient;
import java.net.*;
 
class MemcachedTask implements Runnable {
    private String name;
    private String host;
    private int port ;
    private String key;
    private MemcachedClient mcc;

    public MemcachedTask(String host, int port, String key) {
        this.host = host;
        this.port = port;
        this.key  = key;
        this.name = host + ":" + port;

          try{
             // 本地连接 Memcached 服务
             this.mcc = new MemcachedClient(new InetSocketAddress(host, port));
             System.out.println("Connection to server " + name + " sucessful.");
          }catch(Exception ex){
             System.out.println( ex.getMessage() );
          }
    }

   private String get() {
       String value = null;
      try{
         // 添加数据
         //Future fo = mcc.set("runoob", 900, "Free Education");

         // 输出执行 set 方法后的状态
         //System.out.println("set status:" + fo.get());

         // 使用 get 方法获取数据
         value = mcc.get(key).toString();
      }catch(Exception ex) {
         System.out.println(ex.getMessage());
      }

      return value;
   }

   private void set(String[] args) {
      try{
         // 存储数据
         Future fo = mcc.set("runoob", 900, "Free Education");

         // 查看存储状态
         System.out.println("set status:" + fo.get());

         // 输出值
         System.out.println("runoob value in cache - " + mcc.get("runoob"));
      }catch(Exception ex){
         System.out.println( ex.getMessage() );
      }
   }

   private void add() {
      try{
         // 添加数据
         Future fo = mcc.set("runoob", 900, "Free Education");

         // 打印状态
         System.out.println("set status:" + fo.get());

         // 输出
         System.out.println("runoob value in cache - " + mcc.get("runoob"));

         // 添加
         fo = mcc.add("runoob", 900, "memcached");

         // 打印状态
         System.out.println("add status:" + fo.get());

         // 添加新key
         fo = mcc.add("codingground", 900, "All Free Compilers");

         // 打印状态
         System.out.println("add status:" + fo.get());

         // 输出
         System.out.println("codingground value in cache - " + mcc.get("codingground"));
      }catch(Exception ex){
         System.out.println(ex.getMessage());
      }
   }

    public void run() {
        String value;

        for(int i = 0; i < 10; i++) {
            value = get();

            System.out.println(System.nanoTime() + " " + name +  " Get: " + value);
            try {
                Thread.sleep(1);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        shutdown();
    }

    public void shutdown() {
        mcc.shutdown();
    }
}

@SuppressWarnings("unused")
class CustomThreadPool
{
    //Thread pool size
    private final int poolSize;

    //Internally pool is an array
    private final WorkerThread[] workers;

    // FIFO ordering
    private final LinkedBlockingQueue<Runnable> queue;

    public CustomThreadPool(int poolSize)
    {
        this.poolSize = poolSize;
        queue = new LinkedBlockingQueue<Runnable>();
        workers = new WorkerThread[poolSize];

        for (int i = 0; i < poolSize; i++) {
            workers[i] = new WorkerThread();
            workers[i].start();
        }
    }

    public void execute(Runnable task) {
        synchronized (queue) {
            queue.add(task);
            queue.notify();
        }
    }

    private class WorkerThread extends Thread {
        public void run() {
            Runnable task;

            while (true) {
                synchronized (queue) {
                    while (queue.isEmpty()) {
                        try {
                            queue.wait();
                        } catch (InterruptedException e) {
                            System.out.println("An error occurred while queue is waiting: " + e.getMessage());
                        }
                    }
                    task = (Runnable) queue.poll();
                }

                try {
                    task.run();
                } catch (RuntimeException e) {
                    System.out.println("Thread pool is interrupted due to an issue: " + e.getMessage());
                }
            }
        }
    }

    public void shutdown() {
        System.out.println("Shutting down thread pool");
        for (int i = 0; i < poolSize; i++) {
            workers[i] = null;
        }
    }
}

public class MemcachedExample {
    public static void main(String[] args)
    {
        CustomThreadPool customThreadPool = new CustomThreadPool(16);

        MemcachedTask task1 = new MemcachedTask("10.58.151.23", 12048, "cl:176248672");

        customThreadPool.execute(task1);
    }
}

