#include<sys/time.h>
#include<time.h>
#include<stdio.h>
typedef unsigned long long ticks;

static __inline__ ticks getticks(void)
{
     unsigned a, d;
     asm("cpuid");
     asm volatile("rdtsc" : "=a" (a), "=d" (d));

     return (((ticks)a) | (((ticks)d) << 32));
}

int main(){
    int r = 0, i;
    ticks tick, tick1, diff = 0, diff1;
    unsigned time =0;

//    do {
        for (i = 0; i <= 500000; i++) {
            tick = getticks();
            tick1 = getticks();
            diff1 = tick1 - tick;

            if (tick1 <= tick) {
                printf("###Tick dupe %Ld %Ld\n", tick, tick1);
            }

            if (diff1 > (diff * 100)) {
                printf("Tick: %Ld %Ld, Diff: %Ld %Ld\n", tick, tick1, diff, diff1);
            }
            diff = diff1;
        }
        //sleep(1);

//              time = (unsigned)((tick1-tick)/1662543);
//              printf("\ntime in MS%u\n",time);
//    } while (1);

    return 0;
}

