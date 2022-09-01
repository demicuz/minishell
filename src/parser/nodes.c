/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 12:00:54 by psharen           #+#    #+#             */
/*   Updated: 2022/09/01 13:02:15 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>

#include <libft.h>
#include <minishell.h>

// Parses and writes arguments to cmd->args. The first argument is command name.
static bool parse_and_save_arg(t_scanner *sc, t_cmd *cmd)
{
	t_list	*lst_elem;

	if (!expect(sc->token, T_WORD))
		return (false);
	lst_elem = ft_lstnew(sc->token->value);
	if (!lst_elem)
		fail("Out of memory!");
	ft_lstadd_back(&cmd->args, lst_elem); // TODO O(n^2)
	scan_next_token(sc);
	return (true);
}

// redirect = (">" | "<" | ">>" | "<<") word
// Adds redirects to cmd->redirects. Each redirect is of type t_redirect.
static t_list	*parse_redirect(t_scanner *sc)
{
	t_redirect	*redir;
	t_list		*lst_elem;

	if (!expect(sc->token, T_REDIR))
		return (NULL);
	redir = malloc(sizeof(*redir));
	redir->type = sc->token->type;
	scan_next_token(sc);
	if (!expect(sc->token, T_WORD))
	{
		free(redir);
		return (NULL);
	}
	redir->name = sc->token->value;
	lst_elem = ft_lstnew(redir);
	if (!lst_elem)
		fail("Out of memory!");
	scan_next_token(sc);
	return (lst_elem);
}

// Parse and save either a word, or a redirect.
static bool	parse_and_save_cmd_part(t_scanner *sc, t_cmd *cmd)
{
	t_list	*current_redir;

	if (sc->token->type == T_WORD)
	{
		if (!parse_and_save_arg(sc, cmd))
			return (false);
	}
	else
	{
		current_redir = parse_redirect(sc);
		if (!current_redir)
			return (false);
		ft_lstadd_back(&cmd->redirects, current_redir); // TODO O(n^2)
	}
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
	if (!parse_and_save_cmd_part(sc, cmd))
	{
		ft_lstclear(&lst_elem, free_cmd_data);
		return (NULL);
	}
	while (sc->token->type == T_WORD || is_redirect(sc->token->type)) {
		if (!parse_and_save_cmd_part(sc, cmd))
		{
			ft_lstclear(&lst_elem, free_cmd_data);
			return (NULL);
		}
	}
	return (lst_elem);
}