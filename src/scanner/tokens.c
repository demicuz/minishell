/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 19:59:24 by psharen           #+#    #+#             */
/*   Updated: 2022/09/01 00:22:35 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include <minishell.h>

void	scan_pipe(t_scanner *sc)
{
	sc->token->type = T_PIPE;
	sc->token->value = "|";
	sc->cursor++;
}

void	scan_redirect(t_scanner *sc)
{
	sc->token->type = T_REDIR;
	if (ft_strncmp(sc->cursor, ">>", 2) == 0)
	{
		sc->token->type = T_REDIR_OUT_APPEND;
		sc->token->value = ">>";
		sc->cursor += 2;
	}
	else if (ft_strncmp(sc->cursor, "<<", 2) == 0)
	{
		sc->token->type = T_HEREDOC;
		sc->token->value = "<<";
		sc->cursor += 2;
	}
	else if (*sc->cursor == '>')
	{
		sc->token->type = T_REDIR_OUT;
		sc->token->value = ">";
		sc->cursor++;
	}
	else
	{
		sc->token->type = T_REDIR_IN;
		sc->token->value = "<";
		sc->cursor++;
	}
}

// Should probably implement escape sequences with `\`.
static bool	try_scan_quoted(t_scanner *sc, char quote_char)
{
	sc->cursor++;
	while (*sc->cursor != quote_char && !is_eof(sc))
		sc->cursor++;
	if (is_eof(sc))
	{
		printf("unexpected eof while looking for matching `%c`\n", quote_char);
		return (false);
	}
	sc->cursor++; // consume closing quote
	return (true);
}

// Tries to move `sc->cursor` to the end of the word. Returns `false` if there
// are unclosed quotes.
bool	try_scan_word(t_scanner *sc)
{
	sc->token->type = T_WORD;
	while (!is_eof(sc) && !in(METACHARS, *sc->cursor))
	{
		if (in("'\"", *sc->cursor))
		{
			if (!try_scan_quoted(sc, *sc->cursor))
			{
				sc->token->type = T_ERR_UNBALANCED_QUOTES;
				return (false);
			}
		}
		else
			sc->cursor++;
	}
	return (true);
}
