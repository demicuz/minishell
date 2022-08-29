/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 08:52:25 by psharen           #+#    #+#             */
/*   Updated: 2022/08/28 16:19:17 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>

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

// t_list	*set_token_type(t_list *lst_token, t_token_type type)
// {
// 	t_token	*token;

// 	token = lst_token->data;
// 	token->type = type;
// 	return (lst_token);
// }

void	*clear_data_and_abort(t_list **lst_to_clear)
{
	ft_lstclear(lst_to_clear, free);
	return (NULL);
}

void	parse_error(t_parse_error error, t_token *token)
{
	if (error == ERR_T)
		printf("syntax error: unexpected end of file\n");
	else if (error == ERR_EOF)
		printf("syntax error near unexpected token '%s'\n", token->value);
	// TODO cleanup?
}

void	fail(const char *message)
{
	printf("%s", message);
	exit(EXIT_FAILURE);
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
	free(c->name);
	ft_lstclear(&c->args, free);
	ft_lstclear(&c->redirects, free_redirect_data);
	free(c);
}

// Just trim 'expr' and compare it to "help". Don't want to use ft_strtrim,
// because it uses malloc. Don't need that really.
bool	is_help(const char *expr)
{
	while (in(SPACE_CHARS, *expr))
		expr++;
	return (ft_starts_with("help", expr) && (expr[4] == '\0' ||
		in(SPACE_CHARS, expr[4])));
}
