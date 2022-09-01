/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 19:49:39 by psharen           #+#    #+#             */
/*   Updated: 2022/09/01 12:02:08 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_H
# define SCANNER_H

# include <stdbool.h>

typedef enum e_token_type {
	T_WORD,
	T_REDIR,
	T_REDIR_OUT,
	T_REDIR_OUT_APPEND,
	T_REDIR_IN,
	T_HEREDOC,
	T_PIPE,
	T_ERR_UNBALANCED_QUOTES,
	T_NONE
}	t_token_type;

typedef struct s_token {
	t_token_type	type;
	char			*value;
}	t_token;

typedef struct s_scanner {
	char	*line;
	char	*cursor;
	t_token	*token;
}	t_scanner;

// scanner
bool		is_eof(t_scanner *sc);
t_scanner	*init_scanner(char *line);
void		scan_next_token(t_scanner *sc);
void		scan_pipe(t_scanner *sc);
void		scan_redirect(t_scanner *sc);
bool		try_scan_word(t_scanner *sc);

#endif
