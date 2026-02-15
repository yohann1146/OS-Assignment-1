# Operating Systems Assignment-1
## Group Members
1) Sushant Subramanian - 2023A8PS0295H
2) Yohann Miglani - 2023A8PS1146H
3) Rishit Shah - 2023A3PS0376H
4) Aditya R Goel - 2023A3PS0323H

## Running Files:
To compile: `g++ monitor.c -o monitor.exe` <br>
To run: `./monitor.exe`

## Q1) Child Process Fork
> question1.c

### Functionality
- Implements a multi-process system where a parent process sends data to a child process.
- The parent randomly selects integers from a predefined array until every number has been chosen at least once.
- The child receives integers and immediately calculates and prints their mathematical factors.
- Uses user-defined input (n) to calculate dynamic sleep delays, controlling the execution speed of both processes.

### Coding
- Initializes a unidirectional data pipe (pipe()) before forking to establish communication.
- Uses fork() to split execution; the parent writes to the pipe (fd[1]) while the child reads from it (fd[0]).
- Tracks visited array indices to ensure full coverage, writes the value to the pipe, and sleeps for value % n seconds.
- Continuously reads from the pipe, computes factors via iteration, and sleeps for time % n seconds.
- The parent closes the write end after visiting all items, causing the child's read() to return 0 (EOF) and terminate safely.

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

## Q3) Grep line number extractor
> question3.c

### Functionality
- Call the compiled file using ./mygrep
- Format: ./mygrep <word_to_search> <files>....
- Outputs line numbers sorted in ascending order with uniques removed wherever a match is found.

### Coding
- Search term and file names manually extracted from given command line arguments argv[].
- Used the snprintf() command to format and store the command into another character buffer.
- The main command built uses **grep** to search the given text in the given files, then uses **cut** and ":" as the delimiter to cut out the number line only and **sort** to sort the numbers and keep uniques only.
- Used execlp() command to terminate and replace current program entirely and run the main command on the shell.
- Output is given directly on the terminal.

### Sample input given
<img width="468" height="102" alt="image" src="https://github.com/user-attachments/assets/eab65f88-788a-48d0-bf5d-1f4f67be2bd1" />
<img width="468" height="102" alt="image" src="https://github.com/user-attachments/assets/889eef4e-ae56-4634-8c10-c60fdc4ea25b" />

### Output
<img width="653" height="117" alt="image" src="https://github.com/user-attachments/assets/a8515b34-27e4-4b3a-ac42-66fb34922125" />


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

## Q5) Custom Syscall
> q5.c
> myfork.c

### Functionality
- Created a custom syscall in myfork.c that is the same as fork, but also prints PID of parent process.
- Integrated this into the Linux Kernel by modifying syscall tables and recompiling.
- Called the function `myfork()` in a different file to test its working.

### Coding
- `SYSCALL_DEFINE0` registers our myfork as a custom syscall that takes no args.
- It implements the builtin kernel_clone function, while setting the exit signal to `SIGCHLD` so the parent is notified when child exits.
- It executes a print statement to print the PID of parent.
- The main C file defines the myfork function as a syscall with a particular ID that we defined in the syscalls ID.
- It then calls the function and stores its result.
