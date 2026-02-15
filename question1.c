#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <stdbool.h>

int main() {
    int array[] = {3, 15, 4, 6, 7, 17, 9, 2};
    int array_size = sizeof(array) / sizeof(array[0]);
    int n;
    int pipe_fd[2]; // descriptors for the pipe

    //input
    printf("Enter value for n (n > 0): ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Invalid input. n must be > 0.\n");
        return 1;
    }

    // pipeline creation
    if (pipe(pipe_fd) == -1) {
        perror("Pipe creation error");
        return 1;
    }

    // fork process
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed in child process");
        return 1;
    }

    // CHILD PROCESS 

    if (pid == 0) {
        close(pipe_fd[1]); // unused write end
        int x;

        // loops reads until pipe is closed by parent (read returns 0)
        while (read(pipe_fd[0], &x, sizeof(x)) > 0) {
            printf("[Child] Received: %d. Factors: ", x);
            
            // Calculate and print factors
            for (int i = 1; i <= x; i++) {
                if (x % i == 0) {
                    printf("%d ", i);
                }
            }
            printf("\n");


            int sleep_time = time(NULL) % n;
            sleep(sleep_time); 
        }

        close(pipe_fd[0]);
        exit(0);
    } 
    
    // PARENT PROCESS 
    else {
        close(pipe_fd[0]); // Close unused read end
        
        // Track visited numbers 
        bool visited[8] = {false}; 
        int visit_cnt = 0;
        
        // Seed random number generator
        srand(time(NULL));

        while (visit_cnt < array_size) {
            int index = rand() % array_size;
            int x = array[index];

            // Mark as visited
            if (!visited[index]) {
                visited[index] = true;
                visit_cnt++;
            }

            printf("[Parent] Selected: %d (Visited count: %d/%d)\n", x, visit_cnt, array_size);

            // Pass to child via pipe
            write(pipe_fd[1], &x, sizeof(x));

            sleep(x % n);
        }

        printf("[Parent] All integers visited. Terminating.\n");
        
        close(pipe_fd[1]); 
        
        wait(NULL); 
    }

    return 0;
}