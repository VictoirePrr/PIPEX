/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:21:13 by vicperri          #+#    #+#             */
/*   Updated: 2025/02/12 15:29:57 by vicperri         ###   ########lyon.fr   */
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

void	ft_putchar_err(char *c)
{
	int	i;

	i = 0;
	while (c[i])
	{
		write(2, &c[i], 1);
		i++;
	}
	write(2, "\n", 1);
}

int	check_files(char **argv, int argc)
{
	if (argc < 5)
	{
		write(2, "ERROR: missing argument\n", 25);
		return (ERROR);
	}
	if (argc > 5 && ft_strcmp(argv[3], "awk") != SUCCESS)
	{
		write(2, "ERROR: too much arguments\n", 27);
		return (ERROR);
	}
	return (SUCCESS);
}
