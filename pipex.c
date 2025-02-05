#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int fd[2];
    pipe(fd);  // Create the pipe
    pid_t pid = fork();

    if (pid == 0) // Child process
    {    
        close(fd[1]);         // Close write end (child only reads)
        dup2(fd[0], STDIN_FILENO); // Redirect stdin (fd[0] becomes 0)
        close(fd[0]);         // Close fd[0] (already duplicated)
        
        char buffer[100];
        read(STDIN_FILENO, buffer, sizeof(buffer)); // Read from stdin (actually the pipe)
        printf("Child received: %s\n", buffer);
    }
    else // Parent process
    {
        close(fd[0]);         // Close read end (parent only writes)
        dup2(fd[1], STDOUT_FILENO); // Redirect stdout (fd[1] becomes 1)
        close(fd[1]);         // Close fd[1] (already duplicated)
        
        printf("Hello from parent\n"); // This will now be sent to the pipe
    }
    return (0);
}

