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

typedef struct s_token
{
	t_token_type	type;
	char			*value;
}	t_token;

// NOTICE
// If you change t_redirect or t_cmd, don't forget to update their free()
// functions
typedef struct s_redirect
{
	t_token_type	type;
	char			*name;
}	t_redirect;

typedef struct s_cmd
{
	char	*name;
	t_list	*args;      // a list of strings
	t_list	*redirects; // a list of t_redirect's
	// int		in_fd;
	// int		out_fd;
	// t_list	*redir_in;
	// t_list	*redir_heredoc;
	// t_list	*redir_out;
	// t_list	*redir_out_append;
}	t_cmd;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR
}	t_node_type;

// If 'type' is NODE_CMD, then *cmd is a valid structure. Unised otherwise.
typedef struct s_node
{
	t_node_type	type;
	t_cmd		*cmd;
}	t_node;

// TODO maybe not even needed?
// Create types for preserving type safety (don't use 'void *' as t_list does),
// and keeping compatibility with list functiuons from libft. It's guaranteed
// that the compiler won't reorder struct fields. But! But it's not guaranteed
// by the standard that 'void *' and 'struct *' are equal sizes. But on all
// modern architectures they are, so who cares?
typedef struct s_token_list
{
	t_token				*token;
	struct s_token_list	*next;
}	t_token_list;

typedef struct s_scanner {
	char	*line;
	char	*cursor;
	t_token	*token;
}	t_scanner;

// typedef struct s_parser {
// 	t_token			*next_token;
// 	t_token_type	next_token_type;
// }	t_parser;

// typedef struct s_node
// {
// 	enum e_node_type {
// 		OP_AND,
// 		OP_OR,
// 		PIPELINE
// 	}	type;
// 	char				*value;

// 	struct s_node		*left;
// 	struct s_node		*right;
// }	t_node;

typedef enum e_parse_error
{
	ERR_T,
	ERR_EOF
}	t_parse_error;

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
void		parse_error(t_parse_error error, t_token *token);
void		fail(const char *message);
bool		strequal(const char *s1, const char *s2);
void		free_redirect_data(void *redirect);
void		free_cmd_data(void *cmd);
bool		is_help(const char *expr);

#endif
