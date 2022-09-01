/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 12:07:45 by psharen           #+#    #+#             */
/*   Updated: 2022/09/01 12:29:37 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <minishell.h>

void	run_tty(t_state *state)
{
	char	*line;

	// Disable readline signal handlers. Not sure if it's necessary.
	rl_catch_signals = 0;
	line = readline(PROMPT);
	while (line != NULL)
	{
		if (strlen(line) > 0)
			add_history(line);

		// TODO should be a builtin
		if (is_help(line))
		{
			printf(HELP);
			free(line);
			continue;
		}

		t_list *pipeline = parse(line);

		#ifdef DEBUG_PIPELINE
		print_pipeline(pipeline);
		#endif

		exec_pipeline(pipeline, state);
		free(line);
		ft_lstclear(&pipeline, free_cmd_data);
		line = readline(PROMPT);
	}
}

void	run_non_tty(t_state *state)
{
	puts("Non-tty mode is not implemented!");
	exit(EXIT_FAILURE);
}
