#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void	find_and_execute_cmd(envp)
{
	char	*cmd_path;

	cmd_path = find_cmd_path(envp);
	execve(cmd_path[0], cmd_path, envp);
	// msg error if it does not execute ??
	// free(cmd_path) ???
}

/*Goal : find the path of the cmd in envp.*/
char	*find_cmd_path(char *envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
	}
	return ();
}

/*Goal : the copy of the process (the child) needs
to execute the first argv cmd.
To do so :
1. Close the end of the pipe you don't need (fd[0], read end)
2. You redirect the output of the terminal in the fd[1],
the write end of the pipe. --> execve print to the stdout normally ?
3. You close fd[1] --> why this soon ?
4. You find the path of the cmd
5. you execute the cmd with execve
6. exit ?????? */
void	start_child_process(char *argv1, int *fd, char *envp)
{
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	find_and_execute_cmd(envp);
	perror("execve failed"); // Execve only returns on error
}

/*Goal : read from the write end of the pipe,
and execute the argv[2] cmd.
To do so :
1. Close the write end of the pipe fd[1]
1bis. waitpid ??????
2. Redirect the input of the keyboard to the read end of the pipe fd[0].
3. Close fd[0] -> why this soon ?
4. Execute the second command. --> how can execve knows to execute the command ?
on the first command, where tf is the link ?
*/
void	start_parent_process(char *argv2, int *fd, char *envp)
{
	close(fd[1]);
	// waitpid ?????????
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	find_and_execute_cmd(envp);
	perror("execve failed");
}

/* Goal : code a program that will allow
a command to work on another command just like the way a pipe works.
To do so :
1 : create a tab of int fd[2], 0 for the read end, 1 for the write end
2 : fork the process and retrieve the PID
3 : depending on the PID give instructions to the child and the parent
4 : output everything on the terminal.*/
int	main(int argc, char *argv[], char *envp[])
{
	int		fd[2];
	pid_t	pid;

	pipe(fd); // Create pipe
	pid = fork();
	if (pid == 0)
	{
		start_child_process(argv[1], fd, envp);
		exit(1);
	}
	else
	{
		start_parent_process(argv[2], fd, envp);
		exit(1);
	}
	return (0);
}
