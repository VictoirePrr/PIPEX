/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:21:13 by vicperri          #+#    #+#             */
/*   Updated: 2025/02/13 15:58:29 by vicperri         ###   ########lyon.fr   */
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

