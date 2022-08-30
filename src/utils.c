/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 08:52:25 by psharen           #+#    #+#             */
/*   Updated: 2022/08/31 02:34:23 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include <minishell.h>

// Same as strchr, but:
// - returns booleans
// - if c is '\0', returns false
bool	in(const char *s, char c)
{
	while (*s)
	{
		if (*s == c)
			return (true);
		s++;
	}
	return (false);
}

// TODO maybe not needed?
void	*clear_data_and_abort(t_list **lst_to_clear)
{
	ft_lstclear(lst_to_clear, free);
	return (NULL);
}

bool	strequal(const char *s1, const char *s2)
{
	if (ft_strcmp(s1, s2) == 0)
		return (true);
	return (false);
}

void	free_redirect_data(void *redirect)
{
	t_redirect	*r;

	r = redirect;
	free(r->name);
	free(r);
}

void	free_cmd_data(void *cmd)
{
	t_cmd	*c;

	c = cmd;
	// free(c->name);
	ft_lstclear(&c->args, free);
	ft_lstclear(&c->redirects, free_redirect_data);
	free(c);
}

// Just trim 'expr' and compare it to "help". Don't want to use ft_strtrim,
// because it uses malloc.
bool	is_help(const char *expr)
{
	while (in(SPACE_CHARS, *expr))
		expr++;
	return (ft_starts_with("help", expr) && (expr[4] == '\0' ||
		in(SPACE_CHARS, expr[4])));
}

// TODO move to executer-related stuff maybe?
char	**lst_to_string_array(t_list *lst)
{
	char	**arr;
	char	**p;

	arr = malloc(sizeof(*arr) * (ft_lstsize(lst) + 1));
	// TODO correct error handling, maybe don't exit (if in parent process)?
	if (!arr)
	{
		perror("lst_to_string_array");
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

char	*my_getenv(char *envp[], char *var)
{
	while (envp)
	{
		if (ft_starts_with(var, *envp) && (*envp)[ft_strlen(var)] == '=')
			return (*envp + ft_strlen(var) + 1);
		envp++;
	}
	return (NULL);
}

char	**copy_string_arr(char *arr[])
{
	size_t	size;
	size_t	i;
	char	**copy;

	if (!arr)
		return (NULL);
	size = 0;
	while (arr[size])
		size++;
	copy = malloc((size + 1) * sizeof(char *));
	if (!copy)
		fail("Out of memory!");
	i = 0;
	while (arr[i])
	{
		copy[i] = ft_strdup(arr[i]);
		if (!copy[i])
			fail("Out of memory!");
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

// TODO remove this
void	print_string_array(const char *arr[])
{
	if (!arr)
		return ;
	while (*arr)
	{
		printf("%s\n", *arr);
		arr++;
	}
}
