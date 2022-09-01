/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 17:33:07 by psharen           #+#    #+#             */
/*   Updated: 2022/09/01 12:05:29 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include <libft.h>
#include <minishell.h>

// THIS IS NOT HOW YOU SHOULD WRITE PARSERS!
// It basically works like this: youtube.com/shorts/YkLo2wg667Q

// Grammar in eBNF notation:
// pipeline = cmd {"|" cmd}
// cmd      = word | redirect {word | redirect}
// redirect = (">" | "<" | ">>" | "<<") word
// word     = (* hard to describe, refer to Bash spec :) *)

t_list	*parse(char *expr)
{
	t_scanner	*sc;

	sc = init_scanner(expr);
	if (is_eof(sc))
		return (NULL);
	scan_next_token(sc);
	return (parse_pipeline(sc));
}

bool	is_redirect(t_token_type t)
{
	return (t == T_REDIR_IN || t == T_REDIR_OUT || t == T_HEREDOC ||
		t == T_REDIR_OUT_APPEND);
}

// Check if token is of certain type. If not, print error and return `false`.
// T_REDIR is a helper type, which is used to check for T_REDIR_OUT,
// T_REDIR_OUT_APPEND, T_REDIR_IN and T_HEREDOC.
bool	expect(t_token *token, t_token_type expected_type)
{
	if (token->type == T_ERR_UNBALANCED_QUOTES)
		return (false);
	if (token->type == T_NONE)
	{
		ft_putstr_fd(SHELL_NAME, STDERR_FILENO);
		ft_putstr_fd(": syntax error: unexpected end of file\n", STDERR_FILENO);
		return (false);
	}
	if ((expected_type == T_REDIR && !is_redirect(token->type)) ||
		(expected_type != T_REDIR && token->type != expected_type))
	{
		ft_putstr_fd(SHELL_NAME, STDERR_FILENO);
		ft_putstr_fd(": syntax error near unexpected token '", STDERR_FILENO);
		ft_putstr_fd(token->value, STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
		return (false);
	}
	return (true);
}

// pipeline = cmd {"|" cmd}
// Pipeline is a list of t_cmd's.
t_list	*parse_pipeline(t_scanner *sc)
{
	t_list	*pipeline;
	t_list	*parsed_cmd;

	pipeline = parse_command(sc);
	if (!pipeline)
		return (NULL);

	while (sc->token->type == T_PIPE)
	{
		scan_next_token(sc);
		parsed_cmd = parse_command(sc);
		if (!parsed_cmd)
		{
			ft_lstclear(&pipeline, free_cmd_data);
			return (NULL);
		}
		ft_lstadd_back(&pipeline, parsed_cmd); // TODO O(n^2)
	}
	if (sc->token->type == T_ERR_UNBALANCED_QUOTES)
	{
		ft_lstclear(&pipeline, free_cmd_data);
		return (NULL);
	}
	return (pipeline);
}
