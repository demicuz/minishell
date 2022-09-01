/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 14:59:34 by psharen           #+#    #+#             */
/*   Updated: 2022/09/01 13:04:03 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <libft.h>

# include <scanner.h>
# include <constants.h>
# include <builtins.h>

// NOTICE
// If you change t_redirect or t_cmd, don't forget to update their free()
// functions
typedef struct s_redirect {
	t_token_type	type;
	char			*name;
}	t_redirect;

typedef struct s_cmd {
	t_list	*args;      // a list of strings (includes command name)
	t_list	*redirects; // a list of t_redirect's
	int		in_fd;
	int		out_fd;
}	t_cmd;

typedef struct s_state {
	int		last_exit_code;
	char	**envp;
}	t_state;

// parser
t_list	*parse(char *expr);
bool	is_redirect(t_token_type t);
bool	expect(t_token *token, t_token_type expected_type);
t_list	*parse_command(t_scanner *sc);
t_list	*parse_pipeline(t_scanner *sc);

// run
void	run_tty(t_state *state);
void	run_non_tty(t_state *state);

// executer
bool	exec_pipeline(t_list *pipeline, t_state *state);

// exec_helpers
char	*get_pathname(char *PATH, char *exec_name);
void	close_pipe(int p[]);
void	close_unused_fds(t_cmd *cmd, int l_pipe[], int r_pipe[]);
void	next_pipe(int l_pipe[], int r_pipe[]);

// redirects
bool	process_redirects(t_cmd *cmd);

// errors
void	fail(const char *msg);
void	epic_error(const char *shell, const char *exec_name, const char *msg);
void	epic_fail(const char *shell, const char *exec_name, const char *msg);
bool	perror_and_false(const char *exec_name);
void	*clear_data_and_abort(t_list **lst_to_clear);

// utils
bool	in(const char *s, char c);
t_list	*set_token_type(t_list *lst_token, t_token_type type);

bool	strequal(const char *s1, const char *s2);
void	free_redirect_data(void *redirect);
void	free_cmd_data(void *cmd);
bool	is_help(const char *expr);
char	*my_getenv(char *envp[], char *var);
int		my_open(const char *file, int flags, mode_t mode);
void	my_close(int *fd);
char	**copy_string_arr(char *arr[]);

void	print_string_array(const char *arr[]);

#endif
