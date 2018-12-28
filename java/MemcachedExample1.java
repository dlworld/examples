import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

/*
 * http://www.runoob.com/memcached/java-memcached.html
 * */
import java.util.concurrent.Future;
import net.spy.memcached.MemcachedClient;
import net.spy.memcached.AddrUtil;
import net.spy.memcached.BinaryConnectionFactory;
import java.net.*;
 
class MemcachedTask implements Runnable {
    private String name;
    private String hosts;
    private int port ;
    private String key;
    private MemcachedClient mcc;

    public MemcachedTask(String hosts, String key) {
        this.hosts = hosts;
        this.key  = key;

          try{
             // 本地连接 Memcached 服务
             this.mcc = new MemcachedClient(new BinaryConnectionFactory(),
                     AddrUtil.getAddresses(hosts));
             System.out.println("Connection to server " + hosts + " sucessful.");
          }catch(Exception ex){
             System.out.println( ex.getMessage() );
          }
    }

   private Object get() {
       Object value = null;
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

   private void set(String value) {
      try{
         // 存储数据
         Future fo = mcc.set(key, 900, value);

         // 查看存储状态
         System.out.println("set status:" + fo.get());
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
      }catch(Exception ex){
         System.out.println(ex.getMessage());
      }
   }

    public void run() {
        Object value = null, old_value = null;

        //set("test");

        for(int i = 0; i < 10; i++) {
            value = get();

            if (value && !value.equals(old_value)) {
                System.out.println(System.nanoTime() + " Get: " + value);
            }

            try {
                Thread.sleep(1000 * 10);
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

public class MemcachedExample1 {
    public static void main(String[] args)
    {
        String hosts1 = "10.58.151.35:12048 10.58.151.35:12049 10.58.151.35:12050 10.58.151.33:12048 10.58.151.33:12049 10.58.151.33:12050 10.58.151.32:12048 10.58.151.32:12049 10.58.151.32:12050 10.58.151.31:12048 10.58.151.31:12049 10.58.151.31:12050 10.58.151.30:12048 10.58.151.30:12049 10.58.151.30:12050 10.58.151.23:12048 10.58.151.23:12049 10.58.151.23:12050 10.58.151.21:12048 10.58.151.21:12049 10.58.151.21:12050 10.58.151.20:12048 10.58.151.20:12049 10.58.151.20:12050";

        CustomThreadPool customThreadPool = new CustomThreadPool(16);

        String value = "cl:176248672"
        if (args.length == 1) {
            value = args[0]
        }

        MemcachedTask task1 = new MemcachedTask(hosts1, value);

        customThreadPool.execute(task1);
    }
}

