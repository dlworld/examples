#include <glib.h>

int main(int argc, char **argv)
{
        GMutex *mutex;

        g_thread_init(NULL);
        mutex = g_mutex_new();

        return 1;
}

