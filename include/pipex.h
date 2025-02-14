/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:28:17 by vicperri          #+#    #+#             */
/*   Updated: 2025/02/14 13:48:47 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <fcntl.h>
# include <limits.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define ERROR -1
# define SUCCESS 0

// pipex
int		main(int argc, char **argv, char **envp);
void	find_and_execute_cmd(char *argv, char **envp);
void	start_child_one_process(char **argv, int *fd, char **envp);
void	start_child_two_process(char **argv, int *fd, char **envp);

// utils
int		find_path(char **envp);
char	*find_cmd(char *argv, char *envp);
char	**free_all(char **res);
void	start_waitpid(pid_t pid);
void	execute_cmd(char *cmd_path, char **args, char **envp);
#endif