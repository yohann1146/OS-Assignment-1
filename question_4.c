#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_INPUT 1024
#define DEFAULT_FREQ 800

int current_frequency = DEFAULT_FREQ;

void handle_sigint(int sig) {
    printf("\n[WARNING] Carrier Interrupt Signal Received\n");
    current_frequency = DEFAULT_FREQ;
    printf("Frequency reset to default safe mode (%d MHz)\n", DEFAULT_FREQ);
    printf("station-controller$ ");
    fflush(stdout);
}

int main() {
    char input[MAX_INPUT];
    char *args[64];
    
    signal(SIGINT, handle_sigint);

    while (1) {
        printf("station-controller$ ");
        if (fgets(input, MAX_INPUT, stdin) == NULL) break;

        input[strcspn(input, "\n")] = 0;

        int i = 0;
        args[i] = strtok(input, " ");
        while (args[i] != NULL) {
            args[++i] = strtok(NULL, " ");
        }

        if (args[0] == NULL) continue;

        
        if (strcmp(args[0], "quit") == 0) {
            break;
        } 
        else if (strcmp(args[0], "get_freq") == 0) {
            printf("Current Frequency: %d MHz\n", current_frequency);
        } 
        else if (strcmp(args[0], "set_freq") == 0) {
            if (args[1] != NULL) {
                int new_freq = atoi(args[1]);
                if (new_freq > 0) {
                    current_frequency = new_freq;
                } else {
                    printf("Error: Frequency must be a positive integer.\n");
                }
            } else {
                printf("Usage: set_freq <MHz>\n");
            }
        } 
        

        
        else if (strcmp(args[0], "top") == 0 || strcmp(args[0], "ping") == 0) {
            pid_t pid = fork();

            if (pid == 0) {
                if (strcmp(args[0], "ping") == 0) { 
                    args[i++] = "-c";
                    args[i++] = "4";  //execute exactly 4 times
                    args[i] = NULL;
                }

                if (execvp(args[0], args) == -1) {
                    perror("Execution failed");
                }
                exit(EXIT_FAILURE);
            } else if (pid < 0) {
                perror("Fork failed");
            } else {
                wait(NULL);
            }
        } 
        else {
            printf("Error: Command '%s' not defined.\n", args[0]);
        }
    }

    return 0;
}
