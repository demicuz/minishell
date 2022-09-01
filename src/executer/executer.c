/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 15:02:07 by psharen           #+#    #+#             */
/*   Updated: 2022/09/01 13:03:56 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <wait.h>

#include <minishell.h>

static char	**lst_to_string_array(t_list *lst)
{
	char	**arr;
	char	**p;

	arr = malloc(sizeof(*arr) * (ft_lstsize(lst) + 1));
	if (!arr)
	{
		perror("lst_to_string_array()");
		exit(EXIT_FAILURE);
	}
	p = arr;
	while (lst)
	{
		*p = lst->data;
		lst = lst->next;
		p++;
	}
	*p = NULL;
	return (arr);
}

void	exec_cmd(t_cmd *cmd, t_state *state, int l_pipe[], int r_pipe[])
{
	char	*pathname;
	char	*exec_name;

	if (!process_redirects(cmd))
		exit(EXIT_FAILURE);

	exec_name = cmd->args[0].data;
	if (!exec_name)
		exit(EXIT_SUCCESS);
	else if (exec_name[0] == '/')
		pathname = exec_name;
	else
		pathname = get_pathname(my_getenv(state->envp, "PATH"), exec_name);
	if (!pathname)
		epic_fail(SHELL_NAME, exec_name, "command not found...");

	if (cmd->in_fd)
		dup2(cmd->in_fd, STDIN_FILENO);
	if (cmd->out_fd)
		dup2(cmd->out_fd, STDOUT_FILENO);
	close_unused_fds(cmd, l_pipe, r_pipe);

	char **argv = lst_to_string_array(cmd->args);
	execve(pathname, argv, state->envp);
	perror(argv[0]);
	exit(EXIT_FAILURE);
}

void	prepare_cmd(t_cmd *cmd, int l_pipe[], int r_pipe[])
{
	if (l_pipe[0])
		cmd->in_fd = l_pipe[0];
	if (r_pipe[1])
		cmd->out_fd = r_pipe[1];
	// TODO perform variable expasion inside double quotes!
}

bool	try_fork(t_cmd *cmd, t_state *state, int l_pipe[], int r_pipe[])
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		prepare_cmd(cmd, l_pipe, r_pipe);
		exec_cmd(cmd, state, l_pipe, r_pipe);
	}
	else if (pid == -1)
	{
		close_pipe(l_pipe);
		close_pipe(r_pipe);
		return (false);
	}
	return (true);
}

// TODO save last exit code in `$?` variable, probably through `export` bulitin
// @param pipeline - a list of t_cmd's, one or more
// We can't use fcntl() to check if a file descriptor is valid, so I'm using
// 0 to mark invalid ones.
bool	exec_pipeline(t_list *pipeline, t_state *state)
{
	int		l_pipe[2];
	int		r_pipe[2];
	pid_t	pid;

	l_pipe[0] = 0;
	l_pipe[1] = 0;
	r_pipe[0] = 0;
	r_pipe[1] = 0;
	while (pipeline)
	{
		next_pipe(l_pipe, r_pipe);
		if (pipeline->next && pipe(r_pipe) == -1)
			return (perror_and_false("pipe"));
		if (!try_fork(pipeline->data, state, l_pipe, r_pipe))
			return (perror_and_false("fork"));
		pipeline = pipeline->next;
	}
	close_pipe(l_pipe);
	close_pipe(r_pipe);

	pid = wait(NULL);
	while (pid > 0)
	{
		// printf("Done with process: %d\n", pid);
		pid = wait(NULL);
	}
	return (true);
}
