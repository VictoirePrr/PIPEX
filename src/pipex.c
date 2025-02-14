/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:40:41 by vicperri          #+#    #+#             */
/*   Updated: 2025/02/13 16:17:56 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	find_and_execute_cmd(char *argv, char **envp)
{
	int		envp_pos;
	char	*cmd_path;
	char	**args;

	envp_pos = find_path(envp);
	args = ft_split(argv, ' ');
	cmd_path = find_cmd(args[0], envp[envp_pos]);
	if (cmd_path == NULL)
	{
		write(2, "ERROR: command not found\n", 26);
		free_all(args);
		free(cmd_path);
		exit(127);
	}
	execute_cmd(cmd_path, args, envp);
}

void	execute_cmd(char *cmd_path, char **args, char **envp)
{
	// if(ft_strcmp(args[0], "awk") == SUCCESS)
	// 	args[1] = ft_strtrim(args[1], "'");
	if (execve(cmd_path, args, envp) == ERROR)
	{
		free_all(args);
		free(cmd_path);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
}

void	start_child_one_process(char **argv, int *fd, char **envp)
{
	int	infile_fd;

	infile_fd = open(argv[1], O_RDONLY);
	if (infile_fd == ERROR)
	{
		perror("ERROR");
		exit(EXIT_FAILURE);
	}
	close(fd[0]);
	dup2(infile_fd, STDIN_FILENO);
	close(infile_fd);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	find_and_execute_cmd(argv[2], envp);
}

void	start_child_two_process(char **argv, int *fd, char **envp)
{
	int	outfile_fd;

	outfile_fd = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (outfile_fd == ERROR)
	{
		perror("ERROR");
		exit(EXIT_FAILURE);
	}
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	dup2(outfile_fd, STDOUT_FILENO);
	close(outfile_fd);
	find_and_execute_cmd(argv[3], envp);
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
		start_child_one_process(argv, fd, envp);
	pid2 = fork();
	if (pid2 == ERROR)
	{
		perror("pid2 failed");
		exit(EXIT_FAILURE);
	}
	if (pid2 == 0)
		start_child_two_process(argv, fd, envp);
	close(fd[0]);
	close(fd[1]);
	start_waitpid(pid1);
	start_waitpid(pid2);
}

int	main(int argc, char **argv, char **envp)
{
	int	fd[2];

	if (argc < 5)
		return (SUCCESS);
	// if (argc < 5)
	// {
	// 	write(2, "ERROR: missing arguments\n", 25);
	// 	return (SUCCESS);
	// }
	// if (argc > 5 && ft_strcmp(argv[3], "awk") != SUCCESS)
	// {
	// 	write(2, "ERROR: too much arguments\n", 27);
	// 	return (ERROR);
	// }
	if (pipe(fd) == ERROR)
	{
		perror("pipe failed");
		exit(EXIT_FAILURE);
	}
	start_parent_process(argv, fd, envp);
	return (SUCCESS);
}
