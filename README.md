# Operating Systems Assignment-1

## Running Files:
To compile: `g++ monitor.c -o monitor.exe` <br>
To run: `./monitor.exe`

## Q1)

## Q2) Memory Monitor
> monitor.c

### Functionality
- The user inputs:<br>
**n**: time in seconds between 2 consecutive prints<br>
**k**: processes displayed in each print<br>
**r**: iterations after which a process is killed

- The top **k** processes sorted by %memory usage are listed with their respective users, process identifers and timestamps.

- After running **r** times (every **n** seconds), parent process sends a PID:
`-2 for exit, -1 for skip`.

### Coding
- Created pipes for parent to child and child to parent read/write. Then the parent forks a child process.
- First, the child process *reads from p2c* and *writes to c2p*. Then it runs the **ps** (process status Linux Command): `ps -eo user,pid,%mem,time --sort=-%mem | head -n <iters>`
- A temporary grandchild process is used to execute the above command for **r** iterations. After that, it sends a ready signal to parent and either waits to skip the given PID or kills the process.
- The parent process *writes to p2c* and *reads from c2p*. It scans the inputs from the user and sends it to the child.

### Output
![A screenshot of the output terminal:](https://github.com/yohann1146/OS-Assignment-1/blob/main/imgs/Screenshot%202026-02-13%20000206.png)

## Q3)

## Q4) Cellular Base Station Shell
> question_4.c

### Functionality
- The user inputs:<br>
**set_freq <MHz>**: updates the local frequency variable if the input is a positive integer.<br>
**get_freq**: displays the current base station carrier frequency.<br>
**quit**: allows the user to exit the shell safely.<br>

- The shell provides a custom interface with the prompt station-controller$.<br>
- External Commands such as top and ping are supported:<br>

**top**: displays real-time system resource usage and running processes.<br>
**ping**: attempts to ping the specified address exactly 4 times.<br>

- The shell handles the SIGINT (Ctrl+C) signal:
- It does not terminate on Ctrl+C. Instead, it prints a warning and resets the frequency to a safe mode of 800 MHz.

### Coding
- Created a standard command loop that prints the prompt, reads user input with fgets(), and parses strings using strtok().
- Implemented Internal Commands by using strcmp() to catch specific keywords and modify local shell variables directly.
- Implemented External Commands using the fork() and execvp() system calls:
- The child process handles the execution of the system command.
- For the ping command, the code programmatically appends the -c 4 argument to ensure it runs exactly four times.
- The parent process uses wait() to stay in sync with the child process termination.
- Integrated Signal Handling using the signal() function to catch SIGINT. A custom handler function performs the frequency reset logic and clears the input buffer.
- Designed to handle a maximum command input of 1023 characters.
