/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 15:02:07 by psharen           #+#    #+#             */
/*   Updated: 2022/08/31 01:47:31 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <stdio.h> // TODO remove this
#include <string.h>
#include <unistd.h>
#include <wait.h>

// Don't even try to understand this
char	*construct_pathname(char *PATH, char *exec_name)
{
	char	*path_end;
	size_t	len;
	char	*pathname;

	path_end = ft_strchr(PATH, ':');
	if (!path_end)
		path_end = ft_strchr(PATH, '\0');
	len = (path_end - PATH) + ft_strlen(exec_name) + 1; // + 1 for slash
	pathname = ft_calloc(len + 1, sizeof(char));
	if (!pathname)
		fail("Out of memory!");
	ft_strlcpy(pathname, PATH, (path_end - PATH) + 1);
	pathname[path_end - PATH] = '/';
	ft_strlcat(pathname, exec_name, len + 1);
	return (pathname);
}

// E.g.: `exec_name` is "echo", then return "/usr/bin/echo". NULL of not found.
// Note: directory with a colon in the name cannot be added to PATH (bc POSIX)
char	*get_pathname(char *PATH, char *exec_name)
{
	char	*pathname;

	if (!PATH)
		return (NULL);
	while (PATH)
	{
		pathname = construct_pathname(PATH, exec_name);
		if (access(pathname, X_OK) == 0)
			return (pathname);
		free(pathname);
		PATH = ft_strchr(PATH, ':');
		if (PATH)
			PATH++;
	}
	return (NULL);
}

// Yes.
void	close_pipe(int p[])
{
	if (p[0] != -1)
		close(p[0]);
	if (p[1] != -1)
		close(p[1]);
}

void	exec_cmd(t_cmd *cmd, t_state *state, int l_pipe[], int r_pipe[])
{
	// TODO redirects, heredocs, double quotes expansion, process them FIRST!
	char	*pathname;
	char	*exec_name;

	exec_name = cmd->args[0].data;
	if (!exec_name)
		exit(EXIT_SUCCESS);
	else if (exec_name[0] == '/')
		pathname = exec_name;
	else
		pathname = get_pathname(my_getenv(state->envp, "PATH"), exec_name);
	if (!pathname)
		epic_fail(SHELL_NAME, exec_name, "command not found...");

	if (l_pipe[0] != -1)
		dup2(l_pipe[0], STDIN_FILENO);
	if (r_pipe[1] != -1)
		dup2(r_pipe[1], STDOUT_FILENO);
	close_pipe(l_pipe);
	close_pipe(r_pipe);

	char **argv = lst_to_string_array(cmd->args);
	execve(pathname, argv, state->envp);
	perror(argv[0]);
	exit(EXIT_FAILURE);
}

void	next_pipe(int l_pipe[], int r_pipe[])
{
	close_pipe(l_pipe);
	l_pipe[0] = r_pipe[0];
	l_pipe[1] = r_pipe[1];
	r_pipe[0] = -1;
	r_pipe[1] = -1;
}

bool	try_fork(t_cmd *cmd, t_state *state, int l_pipe[], int r_pipe[])
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		exec_cmd(cmd, state, l_pipe, r_pipe);
	else if (pid == -1)
	{
		close_pipe(l_pipe);
		close_pipe(r_pipe);
		return (false);
	}
	return (true);
}

// TODO save last exit code in `$?` variable, probably through `export` bulitin
// TODO should abort execution gracefully if fork() or pipe() fails.
// @param pipeline - a list of t_cmd's, one or more
// We can't use fcntl() to check if a file descriptor is valid, so I'm using
// -1 to mark invalid ones.
bool	exec_pipeline(t_list *pipeline, t_state *state)
{
	int		l_pipe[2];
	int		r_pipe[2];
	pid_t	pid;

	l_pipe[0] = -1;
	l_pipe[1] = -1;
	r_pipe[0] = -1;
	r_pipe[1] = -1;
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
