/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 19:51:27 by psharen           #+#    #+#             */
/*   Updated: 2022/08/31 19:55:06 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_H
# define CONSTANTS_H

# define SHELL_NAME "bear shell"
# define PROMPT "ʕ•ᴥ•ʔ ➜ "
# define HEREDOC "( ≖_≖) > "
# define THROW_TABLE "ʕノ•ᴥ•ʔノ ︵ ┻━┻ "

# define SPACE_CHARS " \t\n"
# define METACHARS " \t\n|<>"

// TODO don't forget to update help
# define HELP \
"ʕᵔᴥᵔʔ Bear shell, version 0.42\n\
Example usage: echo foo | tee > wat.txt\n\
Builtins: to be implemented!\n\
\n\
Limitations:\n\
- Doesn't support escape sequences.\n"

#endif