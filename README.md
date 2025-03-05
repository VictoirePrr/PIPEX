# Pipex Project 👭

## Overview
The Pipex project simulates the behavior of a simple shell pipeline. It implements piping between processes, handling file redirection, and executing commands using `execve()`. This project requires an in-depth understanding of system calls such as `fork()`, `execve()`, `waitpid()`, `pipe()`, and file descriptor management.

The project also emphasizes proper memory management, with a focus on freeing dynamically allocated memory using helper functions like `free_all()`.

## Objectives
- **Process Creation & Management**: Using `fork()` to create child processes and `waitpid()` to manage them in the parent process.
- **Pipes**: Implementing communication between child processes using pipes (`pipe()`).
- **Redirection**: Handling file redirection for both input and output.
- **Error Handling**: Ensuring robust error handling for process execution and system calls.
- **Memory Management**: Properly freeing dynamically allocated memory with helper functions like `free_all()`.

## Key Functions

## start_waitpid(pid_t pid)
Waits for the child process specified by `pid` to finish using `waitpid()`. It handles both the exit status and any signals that may have terminated the process.

```c
void start_waitpid(pid_t pid)
{
    int status;
    int exit_code;

    if (waitpid(pid, &status, 0) == -1)
    {
        perror("waitpid failed");
        exit(EXIT_FAILURE);
    }
    if (WIFEXITED(status))
    {
        exit_code = WEXITSTATUS(status);
        exit(exit_code);
    }
    else if (WIFSIGNALED(status))
        exit(128 + WTERMSIG(status));
}
```
## find_cmd(char *argv, char *envp)
This function attempts to locate the command (`argv`) in directories listed in the `PATH` environment variable (`envp`). It returns the full path to the executable if found, or `NULL` if not.

```c
char *find_cmd(char *argv, char *envp)
{
    int i = 0;
    char **dir;
    char *cmd;
    char *dir_join;

    if (!argv)
        return (NULL);
    dir = ft_split(envp, ':');
    cmd = ft_strjoin("/", argv);
    while (dir[i])
    {
        dir_join = ft_strjoin(dir[i], cmd);
        if (access(dir_join, F_OK) == 0)
        {
            free_all(dir);
            free(cmd);
            return (dir_join);
        }
        free(dir_join);
        i++;
    }
    free_all(dir);
    free(cmd);
    return (NULL);
}
```

## find_path(char **envp)
Searches for the `PATH` variable in the environment and returns the index of the line containing `PATH`. This is essential for determining where to look for executable files.

```c
int find_path(char **envp)
{
    int i = 0;
    int j;

    while (envp[i])
    {
        j = 0;
        if (envp[i][j] == 'P' && envp[i][j + 1] == 'A')
        {
            if (envp[i][j + 2] == 'T' && envp[i][j + 3] == 'H')
                return (i);
        }
        i++;
    }
    write(2, "ERROR: PATH not found\n", 23);
    exit(127);
}
```

## execute_cmd(char *cmd_path, char **args, char **envp)
Executes a command specified by `cmd_path` with arguments `args` using `execve()`. If the execution fails, it frees allocated memory and exits the program with an error message.

```c
void execute_cmd(char *cmd_path, char **args, char **envp)
{
    if (execve(cmd_path, args, envp) == ERROR)
    {
        free_all(args);
        free(cmd_path);
        perror("execve failed");
        exit(EXIT_FAILURE);
    }
}
```

## Error Handling

- **Command Not Found**: If the command cannot be found in the directories listed in `PATH`, the program exits with a message and a 127 exit status.
- **Execution Failure**: If `execve()` fails, an error message is displayed, memory is freed, and the program exits with `EXIT_FAILURE`.
- **waitpid() Failures**: If `waitpid()` fails to properly wait for a child process, an error message is shown, and the program exits.

## Example Usage

To run the `pipex` program, you need to pass the following arguments:

```bash
./pipex infile "cmd1" "cmd2" outfile
```

Where:
- `infile` is the file from which the first command should read its input.
- `"cmd1"` and `"cmd2"` are the commands to be executed in the pipeline.
- `outfile` is the file where the output of the final command will be written.

Example:
```bash
./pipex infile "ls -la" "wc -l" outfile
```

This will:

- Run `ls -la` on the current directory to list all files and directories in long format.
- Pass the output to `wc -l` to count the number of lines (i.e., the number of files and directories).
- Write the result (the count of files and directories) to `outfile`.

### Challenges Encountered

- **Argument Parsing**: Handling arguments like `{ print $2 }` properly, ensuring that `$2` is not interpreted as a shell variable.
- **Memory Management**: Managing memory properly, particularly when freeing dynamically allocated memory for arrays and strings using `free_all()`.
- **Pipe Handling**: Ensuring that pipes work correctly between child processes and that input/output redirection is handled properly.
- **Process Synchronization**: Using `waitpid()` effectively to wait for child processes to terminate and return appropriate exit codes.

### Conclusion

The Pipex project demonstrates the core concepts of process management, inter-process communication, and file manipulation in a Unix environment. By creating a simplified version of the pipe system, this project provides a solid foundation for understanding system calls and process management in C.
