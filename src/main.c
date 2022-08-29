/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 17:31:24 by psharen           #+#    #+#             */
/*   Updated: 2022/08/29 13:48:38 by psharen          ###   ########.fr       */
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

void	print_argv(int argc, const char *argv[])
{
	while (argc)
	{
		printf("%s\n", *argv);
		argv++;
		argc--;
	}
}

void	print_pipeline(t_list *lst)
{
	while (lst)
	{
		t_cmd *cmd = lst->data;
		printf("\nname: %s\n", cmd->name);
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
			printf("type: %d, value: %s, ", r->type, (char *) r->name);
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

	line = readline(PROMPT);
	while (line != NULL)
	{
		if (strlen(line) > 0)
			add_history(line);
		// printf("[%s]\n", line);
		t_list *pipeline = parse(line);
		print_pipeline(pipeline);
		free(line);
		ft_lstclear(&pipeline, free_cmd_data);
		// puts("rl_line_buffer:");
		// puts(rl_line_buffer);
		line = readline(PROMPT);
	}
	return (0);
}
