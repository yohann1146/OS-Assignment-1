#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>

long myfork(void){
  return syscall(451);    //got dis from syscall_64.tbl
}

int main(){
  long result = myfork();
  printf("Hello world. Current res: %ld\n", result);
  return 0;
}
