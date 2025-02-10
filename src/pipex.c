/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:40:41 by vicperri          #+#    #+#             */
/*   Updated: 2025/02/10 17:13:00 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**free_all(char **res)
{
	size_t	i;

	i = 0;
	while (res[i])
	{
		free(res[i]);
		res[i] = NULL;
		i++;
	}
	free(res);
	return (0);
}
int	find_and_execute_cmd(char *argv, char **envp)
{
	int		envp_pos;
	char	*cmd_path;
	char	**args;

	envp_pos = find_path(envp);
	if (envp_pos == ERROR)
		return (ERROR);
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
	execve(cmd_path, args, envp);
	free_all(args);
	free(cmd_path);
	exit(EXIT_FAILURE);
}

char	*find_cmd(char *argv, char *envp)
{
	int		i;
	char	**dir;
	char	*cmd;
	char	*dir_join;

	i = 0;
	dir = ft_split(envp, ':');
	cmd = ft_strjoin("/", argv);
	while (dir[i])
	{
		dir_join = ft_strjoin(dir[i], cmd);
		if (access(dir_join, F_OK) == 0)
		{
			printf("new dir : %s\n", dir_join);
			free(cmd);
			free_all(dir);
			return (dir_join);
		}
		free(dir_join);
		i++;
	}
	free_all(dir);
	free(cmd);
	return (NULL);
}

int	find_path(char **envp)
{
	int	i;
	int	j;

	i = 0;
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
	return (ERROR); // a voir le return
}

void	start_child_one_process(char *argv1, int *fd, char **envp)
{
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	find_and_execute_cmd(argv1, envp);
	//	perror("execve failed"); // Execve only returns on error
}

void	start_child_two_process(char *argv2, int *fd, char **envp)
{
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	find_and_execute_cmd(argv2, envp);
	// perror("execve failed");
}
void	start_parent_process(char **argv, int *fd, char **envp)
{
	pid_t	pid1;
	pid_t	pid2;
	// int		status;

	pid1 = fork();
	if (pid1 == 0)
		start_child_one_process(argv[1], fd, envp);
	else
	{
		// if (waitpid(pid1, &status, 0) == -1)
		// {
		// 	perror("waitpid failed");
		// 	exit(EXIT_FAILURE);
		// }
		pid2 = fork();
		if (pid2 == 0)
			start_child_two_process(argv[2], fd, envp);
		// else
		// 	waitpid(pid2, &status, 0);
		// {
		// 	perror("waitpid failed");
		// 	exit(EXIT_FAILURE);
		// }
	}
}

int	main(int argc, char **argv, char **envp)
{
	int	fd[2];

	if (argc)
		pipe(fd);
	start_parent_process(argv, fd, envp);
	return (0);
}
