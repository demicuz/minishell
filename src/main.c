/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 17:31:24 by psharen           #+#    #+#             */
/*   Updated: 2022/08/30 19:23:18 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>

#include <libft.h>
#include <minishell.h>

// TODO remove this
#include <unistd.h>

// According to POSIX, this is the way you should access environment variables:
extern char **environ;

void	print_pipeline(t_list *lst)
{
	char *redir_types[] = {"T_REDIR_OUT", "T_REDIR_OUT_APPEND", "T_REDIR_IN", "T_HEREDOC"};

	while (lst)
	{
		t_cmd *cmd = lst->data;

		if (cmd->args)
			printf("args:");
		t_list *args = cmd->args;
		while (args)
		{
			printf(" %s", (char *) args->data);
			args = args->next;
		}
		if (cmd->args)
			printf("\n");

		if (cmd->redirects)
			printf("redirects: ");
		t_list *redirects = cmd->redirects;
		while (redirects)
		{
			t_redirect *r = redirects->data;
			printf("type: %s, value: %s, ", redir_types[r->type - 2], (char *) r->name);
			redirects = redirects->next;
		}
		if (cmd->redirects)
			printf("\n");
		lst = lst->next;
	}
	printf("\n");
}

int	main(int argc, const char *argv[])
{
	// char *line = "   Hello 123 \"abc\"  | ec\"ho wo\"rld  ";
	// t_list	*pipeline;
	// pipeline = parse(line);
	// printf("Line to parse: [%s]\n", line);
	// print_pipeline(pipeline);

	char	*line;
	t_state	state;
	// TODO:
	// state.envp = copy_environ(environ);
	state.envp = environ;

	// TODO if non-tty, run with get-next-line

	line = readline(PROMPT);
	while (line != NULL)
	{
		if (strlen(line) > 0)
			add_history(line);
		// printf("[%s]\n", line);
		t_list *pipeline = parse(line);
		print_pipeline(pipeline);

		exec_pipeline(pipeline, &state);

		free(line);
		ft_lstclear(&pipeline, free_cmd_data);
		// puts("rl_line_buffer:");
		// puts(rl_line_buffer);
		line = readline(PROMPT);
	}
	return (0);
}
