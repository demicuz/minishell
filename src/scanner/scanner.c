/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 17:33:15 by psharen           #+#    #+#             */
/*   Updated: 2022/08/29 13:54:07 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#include <minishell.h>
#include <libft.h>

void	skip_spaces(t_scanner *sc)
{
	while (in(SPACE_CHARS, *sc->cursor))
		sc->cursor++;
}

// 'token' could've been a part of scanner, but stuff like sc->token.type is meh
// (mixed `->` and `.`). I think the `->` syntax makes things only harder.
// Would've been better to have only `.`.
t_scanner	*init_scanner(char *line)
{
	static t_scanner	sc;
	static t_token		token;

	sc.line = line;
	sc.cursor = line;
	token.type = T_NONE;
	sc.token = &token;
	skip_spaces(&sc);
	return (&sc);
}

bool	isEOF(t_scanner *sc)
{
	return (*sc->cursor == '\0');
}

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
bool	try_scan_quoted(t_scanner *sc, char quote_char)
{
	sc->cursor++;
	while (*sc->cursor != quote_char && !isEOF(sc))
		sc->cursor++;
	if (isEOF(sc))
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
	while (!isEOF(sc) && !in(METACHARS, *sc->cursor))
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

// TODO catch errors, check all the references
// Puts next token into `sc->token`. Returns `false` if scanning failed due to
// unclosed quotes. NOTICE: returns `true` even on EOF, as it's not an error.
void	scan_next_token(t_scanner *sc)
{
	char			*start;
	t_token			*token;

	sc->token->type = T_NONE;
	sc->token->value = NULL;
	if (isEOF(sc))
		return ;

	start = sc->cursor;
	token = sc->token;

	if (*start == '|')
		scan_pipe(sc);
	else if (*start == '>' || *start == '<')
		scan_redirect(sc);
	else if (!try_scan_word(sc))
		return ;

	if (token->type == T_WORD)
	{
		token->value = ft_substr_between(start, sc->cursor);
		if (!token->value)
			fail("Out of memory");
	}
	skip_spaces(sc);
}
