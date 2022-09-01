/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2000/01/01 20:20:20 by psharen           #+#    #+#             */
/*   Updated: 2022/08/31 03:53:37 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <libft.h>

typedef struct s_fd_list
{
	int					fd;
	char				*buffer;
	char				*b_pos;
	char				*word_start;
	struct s_fd_list	*next;
}	t_fd_list;

char	*get_next_line(int fd);

char	*ft_strput(char *dest, char *src, char *str_end);
int		ft_lst_add(t_list **lst, char *word_start, char *word_end);
char	*ft_consume(t_fd_list *s, size_t *line_len);
char	*ft_join(t_list *lst, char *line_start, char *line_end, size_t len);

#endif