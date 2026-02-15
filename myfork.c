#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/sched/task.h>

SYSCALL_DEFINE0(myfork){
  struct kernel_clone_args args={ .exit_signal=SIGCHLD};

  pr_info("myfork executed successfully. PID of parent is: %d\n", current->pid);

  return kernel_clone(&args);
}
