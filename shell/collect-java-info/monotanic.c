// This function should execute and never pass the word 'dupe' to stdout
// It was created as a proof-of-concept to show that the feature is broken
// in Red Hat 7.x
#include <stdio.h>
#include <time.h>
#include <strings.h>

int main(int argc, char** argv) {
   struct timespec ts1, ts2;
   long n;
   for(n=0; n < 10000000; n++)
   {
      bzero((void*) &ts1, sizeof(struct timespec));
      bzero((void*) &ts2, sizeof(struct timespec));

      // capture time
      clock_gettime(CLOCK_MONOTONIC, &ts1);
      clock_gettime(CLOCK_MONOTONIC, &ts2);

      // compare that they are not duplicates
      if (ts1.tv_sec >= ts2.tv_sec && ts1.tv_nsec >= ts2.tv_nsec)
      {
         printf("time backwards: %Ld %Ld\n", ts1.tv_nsec, ts2.tv_nsec);
      }
   } // end for loop

   return 0;
}
