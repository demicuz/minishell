/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 14:59:34 by psharen           #+#    #+#             */
/*   Updated: 2022/08/30 16:58:49 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <libft.h>

// TODO move these to minishell_constants.h maybe?
# define PROMPT "ʕ•ᴥ•ʔ ➜ "
# define THROW_TABLE "ʕノ•ᴥ•ʔノ ︵ ┻━┻ "

# define SPACE_CHARS " \t\n"

// TODO move this to a corresponding source file maybe?
# define METACHARS " \t\n|<>"
# define OPERATOR_CHARS "|<>"

typedef enum e_token_type {
	T_WORD,
	T_REDIR,
	T_REDIR_OUT,
	T_REDIR_OUT_APPEND,
	T_REDIR_IN,
	T_HEREDOC,
	T_PIPE,
	T_ERR_UNBALANCED_QUOTES,
	T_NONE // TODO do I really need this?
}	t_token_type;

typedef struct s_token {
	t_token_type	type;
	char			*value;
}	t_token;

// NOTICE
// If you change t_redirect or t_cmd, don't forget to update their free()
// functions
typedef struct s_redirect {
	t_token_type	type;
	char			*name;
}	t_redirect;

typedef struct s_cmd {
	// char	*name;
	t_list	*args;      // a list of strings (includes command name)
	t_list	*redirects; // a list of t_redirect's
	// int		in_fd;
	// int		out_fd;
}	t_cmd;

typedef struct s_scanner {
	char	*line;
	char	*cursor;
	t_token	*token;
}	t_scanner;

typedef struct s_state {
	int		last_exit_code;
	char	**envp;
}	t_state;

// parser
t_list		*parse(char *expr);
t_list		*parse_command(t_scanner *sc);
t_list		*parse_pipeline(t_scanner *sc);
t_list		*parse_expr(t_scanner *sc);

// scanner
bool		isEOF(t_scanner *sc);
t_scanner	*init_scanner(char *line);
void		scan_next_token(t_scanner *sc);

// utils
bool		in(const char *s, char c);
t_list		*set_token_type(t_list *lst_token, t_token_type type);
void		*clear_data_and_abort(t_list **lst_to_clear);
void		fail(const char *message);
bool		strequal(const char *s1, const char *s2);
void		free_redirect_data(void *redirect);
void		free_cmd_data(void *cmd);
bool		is_help(const char *expr);
char		**lst_to_string_array(t_list *lst);

void		print_string_array(const char *arr[]);

// executer
int			exec_pipeline(t_list *pipeline, t_state *state);

#endif
