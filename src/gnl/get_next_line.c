/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2000/01/01 20:20:20 by psharen           #+#    #+#             */
/*   Updated: 2022/08/31 20:20:11 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include <minishell.h>
#include <get_next_line.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1024
#endif

// Returns fd state, if there's an entry for it.
// If not, adds it to the head of the *fd_states and returns it.
// Yes, it's O(n) and is called every time get_next_line() is called,
// but it works fast if we don't have a whole lot of open files to read from
t_fd_list	*ft_get_fd_state(t_fd_list **fd_states, int fd)
{
	t_fd_list	*p;

	p = *fd_states;
	while (p)
	{
		if (p->fd == fd)
			return (p);
		p = p->next;
	}
	p = malloc(sizeof(t_fd_list));
	if (!p)
		fail("Out of memory!");
	p->buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!p->buffer)
		fail("Out of memory!");
	p->fd = fd;
	p->b_pos = p->buffer;
	*(p->b_pos) = '\0';
	p->next = *fd_states;
	*fd_states = p;
	return (p);
}

// It's guaranteed that *fd_states is non-empty, and has fd
void	ft_remove_fd(int fd, t_fd_list **fd_states)
{
	t_fd_list	*elem;
	t_fd_list	*prev;

	elem = *fd_states;
	if ((*fd_states)->fd == fd)
		*fd_states = elem->next;
	else
	{
		while (elem->fd != fd)
		{
			prev = elem;
			elem = elem->next;
		}
		prev->next = elem->next;
	}
	free(elem->buffer);
	free(elem);
}

// reads from s->fd to s->buffer
ssize_t	ft_read_fd(t_fd_list *s)
{
	ssize_t	chars_read;

	chars_read = read(s->fd, s->buffer, BUFFER_SIZE);
	if (chars_read >= 0)
	{
		s->buffer[chars_read] = '\0';
		s->b_pos = s->buffer;
	}
	return (chars_read);
}

// It's guaranteed there's at least one char of ascii text in s->buffer
char	*ft_spit_line(t_fd_list *s)
{
	t_list	*lst;
	size_t	line_len;
	ssize_t	chars_read;

	lst = NULL;
	line_len = 0;
	ft_consume(s, &line_len);
	while (s->b_pos - s->buffer == BUFFER_SIZE && *(s->b_pos - 1) != '\n')
	{
		if (!ft_lst_add(&lst, s->word_start, s->b_pos))
		{
			ft_lstclear(&lst, free);
			return (NULL);
		}
		chars_read = ft_read_fd(s);
		if (chars_read == 0)
			break ;
		if (chars_read < 0)
		{
			ft_lstclear(&lst, free);
			return (NULL);
		}
		ft_consume(s, &line_len);
	}
	return (ft_join(lst, s->word_start, s->b_pos, line_len));
}

char	*get_next_line(int fd)
{
	static t_fd_list	*fd_states = NULL;
	t_fd_list			*s;
	char				*result;

	if (fd < 0)
		return (NULL);
	s = ft_get_fd_state(&fd_states, fd);
	if (!s)
		return (NULL);
	if (!*(s->b_pos) && ft_read_fd(s) <= 0)
	{
		ft_remove_fd(fd, &fd_states);
		return (NULL);
	}
	result = ft_spit_line(s);
	if (!result)
		ft_remove_fd(fd, &fd_states);
	return (result);
}
