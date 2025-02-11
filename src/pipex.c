/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:40:41 by vicperri          #+#    #+#             */
/*   Updated: 2025/02/11 14:26:36 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	find_and_execute_cmd(char *argv, char **envp)
{
	int		envp_pos;
	char	*cmd_path;
	char	**args;

	envp_pos = find_path(envp);
	if (envp_pos == ERROR)
		exit(127);
	args = ft_split(argv, ' ');
	cmd_path = find_cmd(args[0], envp[envp_pos]);
	if (!cmd_path) // if cmd is not found
	{
		free_all(args);
		free(cmd_path);
		exit(127); // error that tells cmd not found
	}
	else
		printf("File exists!\n");
	if (execve(cmd_path, args, envp) == ERROR)
	{
		free_all(args);
		free(cmd_path);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
}

void	start_child_one_process(char *argv1, int *fd, char **envp)
{
	int	infile_fd;

	infile_fd = open("infile", O_RDONLY);
	close(fd[0]);
	dup2(infile_fd, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	close(infile_fd);
	find_and_execute_cmd(argv1, envp);
}

void	start_child_two_process(char *argv2, int *fd, char **envp)
{
	int	outfile_fd;

	outfile_fd = open("outfile", O_WRONLY | O_CREAT, 0644);
	close(fd[1]);
	dup2(outfile_fd, STDOUT_FILENO);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(outfile_fd);
	find_and_execute_cmd(argv2, envp);
}

void	start_parent_process(char **argv, int *fd, char **envp)
{
	pid_t	pid1;
	pid_t	pid2;

	pid1 = fork();
	if (pid1 == ERROR)
	{
		perror("pid1 failed");
		exit(EXIT_FAILURE);
	}
	if (pid1 == 0)
		start_child_one_process(argv[1], fd, envp);
	start_waitpid(pid1);
	pid2 = fork();
	if (pid2 == ERROR)
	{
		perror("pid2 failed");
		exit(EXIT_FAILURE);
	}
	if (pid2 == 0)
		start_child_two_process(argv[2], fd, envp);
	close(fd[0]);
	close(fd[1]);
	start_waitpid(pid2);
}

int	main(int argc, char **argv, char **envp)
{
	int	fd[2];

	if (argc)
		if (pipe(fd) == ERROR)
		{
			perror("pipe failed");
			exit(EXIT_FAILURE);
		}
	start_parent_process(argv, fd, envp);
	return (0);
}
