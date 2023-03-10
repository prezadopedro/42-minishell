/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: progerio <progerio@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:47:52 by sjhony-x          #+#    #+#             */
/*   Updated: 2023/03/08 16:48:53 by progerio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exit_child(int sig)
{
	if (sig == SIGINT)
		ft_putendl_fd("", STDIN_FILENO);
	if (sig == SIGQUIT)
		ft_putendl_fd("Quit", STDIN_FILENO);
}

pid_t	create_child_process(void)
{
	pid_t	pid;

	signal(SIGINT, exit_child);
	signal(SIGQUIT, exit_child);
	pid = fork();
	if (pid < 0)
	{
		perror("Error: ");
		exit(EXIT_FAILURE);
	}
	return (pid);
}

pid_t	execute_child_process(t_data *data)
{
	pid_t	pid;
	char	**env;

	env = get_env(data->builtin_vars);
	pid = create_child_process();
	if (pid == 0)
	{
		dup2(data->fd_in, STDIN_FILENO);
		dup2(data->fd_out, STDOUT_FILENO);
		ft_close_fds(data->fds);
		if (execve(data->args[0], data->args,
				env) == -1)
		{
			free_children_main(data, env);
			exit(EXIT_FAILURE);
		}
		ft_free_tab(env);
	}
	ft_free_tab(env);
	return (pid);
}

void	heredoc_children(t_parser *parser_data, int fd[], char **redirection)
{
	here_doc(fd, redirection[1]);
	close(fd[STDIN_FILENO]);
	close(fd[STDOUT_FILENO]);
	free_children(parser_data, redirection);
	exit(EXIT_SUCCESS);
}
