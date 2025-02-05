# What concepts is the project trying to teach me ? 
- inter-process communication
- Reproducing the use of the shell pipe operator in a C program
- How does a process work : what is a parent and its child and how they work together.
- How processes work ? What is going on when I run multiple commands on a program ?
- What is the difference between a process and a program ?
- File manipulations with fd


So if I want to duplicate a process I can you the function fork.
My initial process will become a parent and the copy of its process will becore a child.
The child starts to work in your program from the moment he is called. He will not re-read your program from the start. So the parent and the child technically work on the same things at the same time. 

Changes done on the parent won´t affect the child and changes made on the child would not affect the parent. 

The parent must wait (with wait or waitpid) that its child finished working to remove its child entry from the process table and then finish. Otherwise, the child will become a zombie, a body with no brain in a way. 

A child can terminate because of an exit(num) or because of a return in the main or by a segfault.

The waitpid   function will help you in knowing which child terminated because of itś RETURN but you can also retrieve its STATUS and compare the status with macros given to check how your child exited.

You can kill a child for multiple reasons, the obvious one would be to not having to take care of him for the next 18 years in the crisis we live in, but suprisingly it is most of the time because the child process is unresponsive, there is an error or simply because there is a timeout. To do this, you just have to call the kill function which will send the right signal to the ight process. The parent MUST wait for the child to terminate to finish. This way the child does not become a zombie and is cleaned up properly.

A file descriptor table is a tab where informations about files that are opened by a process are stored. Each fd is a integer. Every processes inherit a fd table with 0 for the stdin, 1 for stdout and 2 for stderr. Then if the process open a file, the file will be given a fd too. It is a way of representing opened files.

### Using pipe :
< infile grep a1 | wc -w > outfile 
    --> will grep the word a1 in the infile and the pipe will take this information to the wc -w command to count the num of words and print this info on the outfile. 

### The command line look something like that : 
< infile grep a1 | wc -w > outfile

### Yours will look like this :
./pipex infile "grep hello" "wc -w" outfile



## Contraints :
The way pipes are described generally is that it redirects the output of one command to the input of the next command. That is correct. But there's a catch, when you build pipex, you have to launch a new child process for each command you want to do, and they are all made at the same time.

That means all commands are run at the same time, it's just that they will wait for the writing end of the pipe to be closed before reading from the pipe.



# Functions that will be usefull :

## int access(const char *pathname, int mode);
- access() checks whether the program can access the file pathname. 
- The mode specifies the accessibility check(s) to be performed, and is either the value F_OK, or a mask consisting of the bitwise OR of one or more of R_OK, W_OK, and X_OK. F_OK tests for the existence of the file. R_OK, W_OK, and X_OK test whether the file exists and grants read, write, and execute permissions, respectively.
- On success (all requested permissions granted), zero is returned. On error (at least one bit in mode asked for a permission that is denied, or some other error occurred), -1 is returned, and errno is set appropriately.

## int dup2(int oldfd, int newfd);
- So that the commands like grep will be able to get informations in the file we want instead of the stdin.
- "Now, we use the execve() function to execute the grep. When grep is launched without any argument, it reads text from the standard input before executing.
BUT we replaced the stdin file descriptor by in, so grep will read from the standard input, the standard input now reads from the in file, so grep will execute on whatever the content of the in file is."

## int pipe(int pipefd[2]);
- to create a pipe
- Definition : A pipe redirects the output of the command on the left to the input of the command on the right. 

## pid_t fork(void);
- duplicate a process, creates a child

## pid_t waitpid(pid_t pid, int *status, int options);
- You can choose which child to wait for
- makes the parent wait for its child to finish before finishing
- gives back the PID of the child and its exit status to the parent so that the child would be cleaned up by the kernel.

## pid_t wait(int *status);
- Same as waitpid but will make the parent wait for the first child to finish before finishing.

## int execve(const char *filename, char *const argv[], char *const envp[]);

## int unlink(const char *pathname);