#include "kernel/types.h"
#include "kernel/sysinfo.h"
#include "user/user.h"

void
testcall(void)
{
  struct sysinfo info;
  if(sysinfo(&info) < 0){
    printf("FAILED: sysinfo() returned error\n");
    exit(1);
  }
  printf("testcall: OK (freemem=%lu nproc=%lu)\n",
         info.freemem, info.nproc);
}

void
testmem(void)
{
  struct sysinfo before, after;

  sysinfo(&before);

  char *p = sbrk(4096);
  if(p == (char*)-1){
    printf("FAILED: sbrk failed\n");
    exit(1);
  }

  sysinfo(&after);
  if(after.freemem >= before.freemem){
    printf("FAILED: freemem did not decrease after allocation\n");
    exit(1);
  }

  sbrk(-4096);
  sysinfo(&after);
  if(after.freemem != before.freemem){
    printf("FAILED: freemem did not recover after free\n");
    exit(1);
  }

  printf("testmem: OK\n");
}

void
testproc(void)
{
  struct sysinfo before, after;
  int pid;

  sysinfo(&before);

  pid = fork();
  if(pid < 0){
    printf("FAILED: fork failed\n");
    exit(1);
  }
  if(pid == 0){
    exit(0);
  }

  sysinfo(&after);
  if(after.nproc <= before.nproc){
    printf("FAILED: nproc did not increase after fork\n");
    wait(0);
    exit(1);
  }

  wait(0);
  printf("testproc: OK\n");
}

int
main(void)
{
  printf("sysinfotest: start\n");
  testcall();
  testmem();
  testproc();
  printf("sysinfotest: OK\n");
  exit(0);
}
