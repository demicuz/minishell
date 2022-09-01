/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2000/01/01 20:20:20 by psharen           #+#    #+#             */
/*   Updated: 2022/08/31 20:19:35 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include <minishell.h>
#include <get_next_line.h>

// Copies a string, returns a pointer to the end of the result
// If str_end is NULL, doesn't use it and copies the whole src
// Could also have been implemented like this:
// while (*src && (!str_end || src < str_end))
// {
// 	*dest = *src;
// 	dest++;
// 	src++;
// }
char	*ft_strput(char *dest, char *src, char *str_end)
{
	if (!str_end)
	{
		while (*src)
		{
			*dest = *src;
			dest++;
			src++;
		}
	}
	else
	{
		while (*src && src < str_end)
		{
			*dest = *src;
			dest++;
			src++;
		}
	}
	*dest = '\0';
	return (dest);
}

// O(n)
// TODO maybe return the added element?
int	ft_lst_add(t_list **lst, char *word_start, char *word_end)
{
	t_list	*elem;
	t_list	*p;

	elem = malloc(sizeof(t_list));
	if (!elem)
		fail("Out of memory!");
	elem->data = malloc(sizeof(char) * (word_end - word_start + 1));
	if (!elem->data)
		fail("Out of memory!");
	ft_strput(elem->data, word_start, word_end);
	elem->next = NULL;
	p = *lst;
	if (!p)
	{
		*lst = elem;
		return (1);
	}
	while (p->next)
		p = p->next;
	p->next = elem;
	return (1);
}

char	*ft_consume(t_fd_list *s, size_t *line_len)
{
	s->word_start = s->b_pos;
	while (*s->b_pos && *s->b_pos != '\n')
	{
		(*line_len)++;
		s->b_pos++;
	}
	if (*s->b_pos == '\n')
	{
		(*line_len)++;
		s->b_pos++;
	}
	return (NULL);
}

// Combines words from lst and a word from buffer into one string
// *line_start here points to the beginning of the line (first char),
// *line_end points to the char after the last char (acts like '\0')
// len is the total line length (words from lst + (line_end - line_start))
char	*ft_join(t_list *lst, char *line_start, char *line_end, size_t len)
{
	char	*result;
	t_list	*lst_p;
	char	*str_p;

	if (!lst && !*line_start)
		return (NULL);
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		fail("Out of memory!");
	str_p = result;
	lst_p = lst;
	while (lst_p)
	{
		str_p = ft_strput(str_p, lst_p->data, NULL);
		lst_p = lst_p->next;
	}
	str_p = ft_strput(str_p, line_start, line_end);
	*str_p = '\0';
	ft_lstclear(&lst, free);
	return (result);
}
