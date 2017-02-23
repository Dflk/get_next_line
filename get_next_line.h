/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbaran <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/02 14:22:50 by rbaran            #+#    #+#             */
/*   Updated: 2016/04/15 15:17:40 by rbaran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>
# include <stdlib.h>
# include <libft.h>
# define BUFF_SIZE 2048

typedef struct	s_fd
{
	int			fd;
	char		*line;
	size_t		start;
	struct s_fd	*next;
}				t_fd;

int				get_next_line(int const fd, char **line);

#endif
