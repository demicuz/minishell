/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 01:37:20 by psharen           #+#    #+#             */
/*   Updated: 2022/09/01 13:01:22 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <stdio.h>
#include <unistd.h>

void	fail(const char *msg)
{
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	epic_error(const char *shell, const char *exec_name, const char *msg)
{
	if (shell)
	{
		ft_putstr_fd(shell, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (exec_name)
	{
		ft_putstr_fd(exec_name, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	epic_fail(const char *shell, const char *exec_name, const char *msg)
{
	epic_error(shell, exec_name, msg);
	exit(EXIT_FAILURE);
}

bool	perror_and_false(const char *exec_name)
{
	perror(exec_name);
	return (false);
}

// TODO maybe not needed?
void	*clear_data_and_abort(t_list **lst_to_clear)
{
	ft_lstclear(lst_to_clear, free);
	return (NULL);
}
