/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 17:33:07 by psharen           #+#    #+#             */
/*   Updated: 2022/08/29 14:13:47 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h> // TODO maybe not needed
#include <errno.h>

#include <libft.h>
#include <minishell.h>
#include <minishell_constants.h>

// THIS IS NOT HOW YOU SHOULD WRITE PARSERS!

// Grammar in eBNF:
// pipeline = cmd {"|" cmd}
// cmd      = word | redirect {word | redirect}
// redirect = (">" | "<" | ">>" | "<<") word
// word     = (* hard to describe, refer to Bash spec :) *)

// TODO looks ugly
t_list	*parse(char *expr)
{
	t_scanner	*sc;
	// t_list		*pipeline;

	if (is_help(expr))
	{
		printf(HELP); // TODO
		return (NULL);
	}
	sc = init_scanner(expr);
	if (isEOF(sc))
		return (NULL);
	scan_next_token(sc);
	// pipeline = parse_pipeline(sc);
	return (parse_expr(sc));
}

bool	is_redirect(t_token_type t)
{
	return (t == T_REDIR_IN || t == T_REDIR_OUT || t == T_HEREDOC ||
		t == T_REDIR_OUT_APPEND);
}

// TODO nice error messages
bool	expect(t_token *token, t_token_type expected_type)
{
	if (token->type == T_NONE)
	{
		// parse_error(ERR_EOF, token);
		printf("%s\nsyntax error: unexpected end of file\n", THROW_TABLE);
		// free(token->value);
		return (false);
	}
	if ((expected_type == T_REDIR && !is_redirect(token->type)) ||
		(expected_type != T_REDIR && token->type != expected_type))
	{
		// parse_error(ERR_T, token);
		printf("syntax error near unexpected token '%s'\n", token->value);
		// free(token->value);
		return (false);
	}
	if (token->type == T_ERR_UNBALANCED_QUOTES)
		return (false);
	// next(sc);
	return (true);
}

bool parse_and_save_cmd_arg(t_scanner *sc, t_cmd *cmd)
{
	t_list	*lst_elem;

	if (!expect(sc->token, T_WORD))
		return (false);
	if (!cmd->name)
		cmd->name = sc->token->value;
	else
	{
		// TODO maybe make a wrapper for these three lines? This pattern is
		// very common
		lst_elem = ft_lstnew(sc->token->value);
		if (!lst_elem)
			fail("Out of memory!");
		ft_lstadd_back(&cmd->args, lst_elem); // TODO O(n^2)
	}
	scan_next_token(sc);
	return (true);
}

// redirect = (">" | "<" | ">>" | "<<") word
t_list	*parse_redirect(t_scanner *sc)
{
	t_redirect	*redirect;
	t_list		*lst_elem;

	if (!expect(sc->token, T_REDIR))
		return (NULL);
	redirect = malloc(sizeof(*redirect));
	redirect->type = sc->token->type;
	scan_next_token(sc);
	if (!expect(sc->token, T_WORD))
	{
		free(redirect);
		return (NULL);
	}
	redirect->name = sc->token->value;
	lst_elem = ft_lstnew(redirect);
	if (!lst_elem)
		fail("Out of memory!");
	scan_next_token(sc);
	return (lst_elem);
}

bool	parse_and_save_cmd_elem(t_scanner *sc, t_cmd *cmd)
{
	t_list	*current_redir;

	if (sc->token->type == T_WORD)
	{
		if (!parse_and_save_cmd_arg(sc, cmd))
			return (false);
	}
	// else if (is_redirect(sc->token->type))
	else
	{
		current_redir = parse_redirect(sc);
		if (!current_redir)
			return (false);
		ft_lstadd_back(&cmd->redirects, current_redir); // TODO O(n^2)
	}
	// else
		// return (false);
	return (true);
}

// cmd = word | redirect {word | redirect}
// (yes, "> wat" is a valid command in Bash)
t_list	*parse_command(t_scanner *sc)
{
	t_cmd	*cmd;
	t_list	*lst_elem;

	cmd = ft_calloc(1, sizeof(*cmd));
	lst_elem = ft_lstnew(cmd);
	if (!cmd || !lst_elem)
		fail("Out of memory!");
	lst_elem->data = cmd;
	if (!parse_and_save_cmd_elem(sc, cmd))
	{
		free_cmd_data(cmd);
		free(lst_elem); // TODO ugly
		return (NULL);
	}
	while (sc->token->type == T_WORD || is_redirect(sc->token->type)) {
		if (!parse_and_save_cmd_elem(sc, cmd))
		{
			free_cmd_data(cmd);
			free(lst_elem); // TODO ugly
			return (NULL);
		}
	}
	return (lst_elem);
}

// pipeline = cmd {"|" cmd}
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
		ft_lstadd_back(&pipeline, parsed_cmd);
	}
	return (pipeline);
}

t_list	*parse_expr(t_scanner *sc)
{
	t_list	*pipeline;

	pipeline = parse_pipeline(sc);
	if (sc->token->type == T_ERR_UNBALANCED_QUOTES)
	{
		ft_lstclear(&pipeline, free_cmd_data);
		return (NULL);
	}
	return (pipeline);
}
