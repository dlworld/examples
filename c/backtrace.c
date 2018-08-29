/*
 * # gcc -rdynamic -o bt backtrace.c
 * # ./bt
 * */

#include <stdio.h>
#include <signal.h>
#include <stdio.h>
#include <signal.h>
#include <execinfo.h>

void bt_sighandler() 
{
  void *trace[16] = { NULL };
  char **messages = NULL;
  int i, trace_size = 0;

  trace_size = backtrace(trace, 16);
  /* overwrite sigaction with caller's address */
  messages = backtrace_symbols(trace, trace_size);
  /* skip first stack frame (points here) */
  printf("[bt] Execution path:\n");
  for (i=1; i<trace_size; ++i)
  {
    printf("[bt] #%d %s\n", i, messages[i]);
  }
}


int func_a(int a, char b) 
{
  char *p = (char *)0xdeadbeef;

  a = a + b;
  bt_sighandler();
  *p = 10;  /* CRASH here!! */

  return 2*a;
}


int main() 
{

  int res, a = 5;

  res = 5 + func_a(a, 't');

  return res;
}
