#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

int main() {
    int n, k, r;
    pid_t pid;

    //PIPES
    int p2c[2]; //parent to chidl
    int c2p[2]; //child to parent

    printf("Enter n (seconds), k (processes), r (iterations): ");
    if (scanf("%d %d %d", &n, &k, &r)!=3) {
        fprintf(stderr, "Invalid input.\n");
        return 1;
    }

    if (pipe(p2c)==-1 || pipe(c2p)==-1) {
        perror("Pipe failed");
        return 1;
    }

    //FORKING A CHILD PROCESS
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    //CHILD
    if (pid == 0) {

        // read from p2c and write to c2p:
        close(p2c[1]);
        close(c2p[0]);

        int target_pid;
        char ps_cmd[256];
        snprintf(ps_cmd, sizeof(ps_cmd), "ps -eo user,pid,%%mem,time --sort=-%%mem | head -n %d", k + 1);

        while (1) {
            for (int i = 0; i < r; i++) {
                sleep(n);

                printf("\n--- Memory Monitor (Iteration %d/%d) ---\n", i + 1, r);
                
                //running exec using grandchild
                pid_t printer = fork();
                if (printer == 0) {
                    execlp("sh", "sh", "-c", ps_cmd, NULL);
                    perror("Exec failed");
                    exit(1);
                }
                else wait(NULL);
            }

            char ready_sig = '1';
            write(c2p[1], &ready_sig, 1);

            //wait for parent
            if (read(p2c[0], &target_pid, sizeof(target_pid)) > 0) {
                if (target_pid == -2) {
                    printf("Child: Received -2. Exiting...\n");
                    close(p2c[0]);
                    close(c2p[1]);
                    exit(0);
                } else if (target_pid == -1) {
                    printf("Child: Skipping kill. Resuming monitor...\n");
                } else {
                    printf("Child: Attempting to kill PID %d...\n", target_pid);
                    if (kill(target_pid, SIGTERM) == 0) {
                        printf("Child: Process %d killed successfully.\n", target_pid);
                    } else {
                        perror("Child: Failed to kill process");
                    }
                    printf("Child: Resuming monitor...\n");
                }
            }
        }
    }

    //PARENT
    else {
        // write to p2c and read from c2p
        close(p2c[0]);
        close(c2p[1]);

        int input_pid;
        char sync_buf;

        while (1) {
            if (read(c2p[0], &sync_buf, 1) > 0) {
                
                printf("\n------------------------------------------\n");
                printf("Monitor paused.\n");
                printf("Enter PID to kill (-1 to skip, -2 to exit): ");
                scanf("%d", &input_pid);

                write(p2c[1], &input_pid, sizeof(input_pid));

                if (input_pid == -2) {
                    printf("Parent: Waiting for child to terminate...\n");
                    wait(NULL);
                    printf("Parent: Exiting.\n");
                    close(p2c[1]);
                    close(c2p[0]);
                    exit(0);
                }
            }
        }
    }

    return 0;
}