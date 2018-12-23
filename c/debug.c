
#include <sys/syscall.h>
#define MY_DEBUG(format, ...) printf("%d %d : %s "#format "\n", (int)getpid(), (int)syscall(SYS_gettid), __FUNCTION__, ##__VA_ARGS__)


