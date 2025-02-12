/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 10:04:09 by vicperri          #+#    #+#             */
/*   Updated: 2025/02/12 16:27:40 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	start_waitpid(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid failed");
		exit(EXIT_FAILURE);
	}
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
			printf("new dir : %s\n", dir_join); // A SUPP
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
	write(2, "ERROR: PATH not found\n", 23);
	exit(127);
}
