/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 16:13:47 by psharen           #+#    #+#             */
/*   Updated: 2022/09/01 12:59:40 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <libft.h>
#include <minishell.h>

// Don't even try to understand this.
static char	*construct_pathname(char *PATH, char *exec_name)
{
	char	*path_end;
	size_t	len;
	char	*pathname;

	path_end = ft_strchr(PATH, ':');
	if (!path_end)
		path_end = ft_strchr(PATH, '\0');
	len = (path_end - PATH) + ft_strlen(exec_name) + 1; // + 1 for `/`
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

void	close_pipe(int p[])
{
	my_close(&p[0]);
	my_close(&p[1]);
}

void	close_unused_fds(t_cmd *cmd, int l_pipe[], int r_pipe[])
{
	if (cmd->in_fd == l_pipe[0])
		cmd->in_fd = 0;
	if (cmd->out_fd == r_pipe[1])
		cmd->out_fd = 0;
	close_pipe(l_pipe);
	close_pipe(r_pipe);
	my_close(&cmd->in_fd);
	my_close(&cmd->out_fd);
}

// Move r_pipe descriptors to l_pipe.
void	next_pipe(int l_pipe[], int r_pipe[])
{
	close_pipe(l_pipe);
	l_pipe[0] = r_pipe[0];
	l_pipe[1] = r_pipe[1];
	r_pipe[0] = 0;
	r_pipe[1] = 0;
}
