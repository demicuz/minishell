/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 17:33:15 by psharen           #+#    #+#             */
/*   Updated: 2022/09/01 00:22:38 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdbool.h>

#include <libft.h>
#include <minishell.h>

static void	skip_spaces(t_scanner *sc)
{
	while (in(SPACE_CHARS, *sc->cursor))
		sc->cursor++;
}

// `token` could've been a part of scanner, but stuff like sc->token.type is meh
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

bool	is_eof(t_scanner *sc)
{
	return (*sc->cursor == '\0');
}

// TODO catch errors, check all the references
// Puts next token into `sc->token`. Returns `false` if scanning failed due to
// unclosed quotes. Returns `true` even on EOF, as it's not an error.
void	scan_next_token(t_scanner *sc)
{
	char			*start;
	t_token			*token;

	sc->token->type = T_NONE;
	sc->token->value = NULL;
	if (is_eof(sc))
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
			fail("Out of memory!");
	}
	skip_spaces(sc);
}
