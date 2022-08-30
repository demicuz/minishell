/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 15:02:07 by psharen           #+#    #+#             */
/*   Updated: 2022/08/30 19:24:28 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <stdio.h> // TODO remove this
#include <unistd.h>
#include <wait.h>

// char	*find_pathname()
// {
// 	char *path = getenv("PATH");
// 	printf("getenv(\"PATH\"): %s\n", path);
// }

#define DEBUG_PIPES

// Yes.
void	close_pipe(int p[])
{
	if (p[0] != -1)
		close(p[0]);
	if (p[1] != -1)
		close(p[1]);
}

void	exec_cmd(t_cmd *cmd, t_state *state, int left_pipe[], int right_pipe[])
{
	// TODO char *pathname = find_pathname(state);
	// TODO redirects, heredocs

	if (left_pipe[0] != -1)
		dup2(left_pipe[0], STDIN_FILENO);
	if (right_pipe[1] != -1)
		dup2(right_pipe[1], STDOUT_FILENO);
	close_pipe(left_pipe);
	close_pipe(right_pipe);

	char **argv = lst_to_string_array(cmd->args);
	execve(argv[0], argv, state->envp);
	perror(argv[0]);
	exit(EXIT_FAILURE);
}

void	shift_pipes(int left_pipe[], int right_pipe[])
{
	close_pipe(left_pipe);
	left_pipe[0] = right_pipe[0];
	left_pipe[1] = right_pipe[1];
	right_pipe[0] = -1;
	right_pipe[1] = -1;
}

// TODO save last exit code in `$?` variable, probably through `export` bulitin
// TODO should abort execution gracefully if fork() or pipe() fails.
// @param pipeline - a list of t_cmd's, one or more
int	exec_pipeline(t_list *pipeline, t_state *state)
{
	int		left_pipe[2];
	int		right_pipe[2];
	pid_t	pid;

	left_pipe[0]  = -1;
	left_pipe[1]  = -1;
	right_pipe[0] = -1;
	right_pipe[1] = -1;

	if (ft_lstsize(pipeline) > 1)
		pipe(right_pipe); // TOOD catch errors

	pid = fork();
	if (pid == 0)
		exec_cmd(pipeline->data, state, left_pipe, right_pipe);

	pipeline = pipeline->next;
	while (pipeline)
	{
		shift_pipes(left_pipe, right_pipe);
		if (pipeline->next)
			pipe(right_pipe); // TODO catch errors

		pid = fork();
		if (pid == 0)
			exec_cmd(pipeline->data, state, left_pipe, right_pipe);

		pipeline = pipeline->next;
	}

	close_pipe(left_pipe);
	close_pipe(right_pipe);

	pid = wait(NULL);
	while (pid > 0)
	{
		// printf("Done with process: %d\n", pid);
		pid = wait(NULL);
	}
	return (-1);
}
