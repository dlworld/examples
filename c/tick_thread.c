#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include<sys/time.h>
#include<time.h>

typedef unsigned long long ticks;

static __inline__ ticks getticks(void)
{
	unsigned a, d;
	//     asm("cpuid");
	asm volatile("rdtscp" : "=a" (a), "=d" (d));

	return (((ticks)a) | (((ticks)d) << 32));
}


#define handle_error_en(en, msg) \
	   do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
	   do { perror(msg); exit(EXIT_FAILURE); } while (0)

struct thread_info {    /* Used as argument to thread_start() */
    pthread_t thread_id;        /* ID returned by pthread_create() */
    int       thread_num;       /* Application-defined thread # */
    cpu_set_t cpuset;
    unsigned long int cpu_bitmap;
    char     *argv_string;      /* From command-line argument */
};

static void *
thread_start(void *arg)
{
    struct thread_info *tinfo = arg;
    ticks tick, tick1, diff = 0, diff1;
    struct timespec ts, ts1;
	unsigned time =0; 
	int s, j;
  
	/* Check the actual affinity mask assigned to the thread */

	s = pthread_getaffinity_np(tinfo->thread_id, sizeof(cpu_set_t), &tinfo->cpuset);
	if (s != 0)
		handle_error_en(s, "pthread_getaffinity_np");

	printf("Thread %d: \n", tinfo->thread_num);
	for (j = 0; j < CPU_SETSIZE; j++) {
		if (CPU_ISSET(j, &tinfo->cpuset)) {
            tinfo->cpu_bitmap |= j;
		   printf("    CPU %d\n", j); 
        }
    }

	for (j = 0; j <= 500000; j++) {
        bzero((void*) &ts, sizeof(struct timespec));
        bzero((void*) &ts1, sizeof(struct timespec));

        clock_gettime(CLOCK_MONOTONIC, &ts);
        clock_gettime(CLOCK_MONOTONIC, &ts1);

		tick = getticks();
		tick1 = getticks();
		diff1 = tick1 - tick;

        if (ts1.tv_sec <= ts.tv_sec && ts1.tv_nsec <= ts.tv_nsec) {
			printf("ERROR[%d:0x%lx]Time backwards %ld.%ld %ld.%ld\nTick %Lu %Lu (%Lu)\n", 
                    tinfo->thread_num, tinfo->cpu_bitmap, 
                    ts.tv_sec, ts.tv_nsec,
                    ts1.tv_sec, ts1.tv_nsec,
                    tick, tick1, tick - tick1);
        }

		if (tick1 <= tick) {
			printf("ERROR[%d:0x%lx][%ld.%ld]Tick backwards %Lu %Lu (%Lu)\n", 
                    tinfo->thread_num, tinfo->cpu_bitmap, ts1.tv_sec, ts1.tv_nsec,
                    tick, tick1, tick - tick1);
		}

		if (diff1 > (diff * 1000)) {
			printf("[%d:0x%lx][%ld.%ld]Tick: %Ld %Ld, Diff: %Lu %Lu\n", 
					tinfo->thread_num, tinfo->cpu_bitmap, ts1.tv_sec, ts1.tv_nsec,
                    tick, tick1, diff, diff1);
		}
		diff = diff1;
	}

	return NULL;
}

int
main(int argc, char *argv[])
{
   int s, tnum, opt, num_threads;
   struct thread_info *tinfo;
	pthread_attr_t attr;
   cpu_set_t cpuset;
   void *res;
   
   /* The "-n" option specifies the number of threads */
   num_threads = -1;
   while ((opt = getopt(argc, argv, "n:")) != -1) {
	   switch (opt) {
	   case 'n':
		   num_threads = strtoul(optarg, NULL, 0);
		   break;
	   default:
		   fprintf(stderr, "Usage: %s [-n thread-nums] arg...\n",
				   argv[0]);
		   exit(EXIT_FAILURE);
	   }
   }
   
   /* Allocate memory for pthread_create() arguments */
   tinfo = calloc(num_threads, sizeof(struct thread_info));
   if (tinfo == NULL)
	   handle_error("calloc");

	/* Create one thread on each core */
	for (tnum = 0; tnum < num_threads; tnum++) {
	   tinfo[tnum].thread_num = tnum + 1;
	   tinfo[tnum].cpu_bitmap = 0;
	
       /* Initialize thread creation attributes */
       s = pthread_attr_init(&attr);
       if (s != 0)
           handle_error_en(s, "pthread_attr_init");
   
       CPU_ZERO(&cpuset);
	   CPU_SET(tnum, &cpuset);
	   s = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
		if (s != 0)
			handle_error_en(s, "pthread_setaffinity_np");
	   /* The pthread_create() call stores the thread ID into
		  corresponding element of tinfo[] */

	   s = pthread_create(&tinfo[tnum].thread_id, &attr,
						  &thread_start, &tinfo[tnum]);
	   if (s != 0)
		   handle_error_en(s, "pthread_create");
	}

   /* Now join with each thread, and display its returned value */
   for (tnum = 0; tnum < num_threads; tnum++) {
	   s = pthread_join(tinfo[tnum].thread_id, &res);
	   if (s != 0)
		   handle_error_en(s, "pthread_join");

	   printf("Joined with thread %d; returned value was %s\n",
			   tinfo[tnum].thread_num, (char *) res);
	   free(res);      /* Free memory allocated by thread */
   }

   free(tinfo);
   exit(EXIT_SUCCESS);
}
