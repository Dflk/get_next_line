/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbaran <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/03 15:00:20 by rbaran            #+#    #+#             */
/*   Updated: 2016/03/15 09:17:41 by rbaran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

static void	ft_fillline(char *files[FILES], char **line, int const fd)
{
	if (files[fd])
	{
		*line = ft_strdup(files[fd]);
		free(files[fd]);
		files[fd] = NULL;
	}
}

static int	ft_checkfile(char *files[FILES], char **line, int const fd)
{
	int		i;
	int		nl;

	i = -1;
	nl = 0;
	while (nl == 0 && files[fd][++i] != '\0')
		if (files[fd][i] == '\n')
			nl = 1;
	if (nl == 1)
	{
		files[fd][i] = '\0';
		*line = files[fd];
		if (files[fd][i + 1] != '\0')
			files[fd] = ft_strdup(files[fd] + i + 1);
		else
		{
			free(files[fd]);
			files[fd] = NULL;
		}
	}
	return (nl);
}

static void	ft_strfreejoin(char *files[FILES], char *buf, int const fd)
{
	char	*str_tmp;

	str_tmp = files[fd];
	files[fd] = ft_strjoin(files[fd], buf);
	free(str_tmp);
}

int			get_next_line(int const fd, char **line)
{
	char		buf[BUF_SIZE + 1];
	int			ret;
	int			i;
	static char	*files[FILES];

	if (fd < 0 || fd == 1 || fd == 2 || fd >= FILES || !line)
		return (-1);
	i = -1;
	ret = read(fd, buf, BUF_SIZE);
	if (ret == -1 || (ret == 0 && !files[fd]))
		return (ret);
	buf[ret] = '\0';
	if (!files[fd] && ret > 0)
	{
		if ((files[fd] = (char*)ft_memalloc(sizeof(char) * (ret + 1))))
			files[fd] = ft_strcpy(files[fd], (char*)buf);
	}
	else if (files[fd] && ret > 0)
		ft_strfreejoin(files, buf, fd);
	if (ret > 0)
		ret = get_next_line(fd, line);
	if (ret == 0 && !ft_checkfile(files, line, fd))
		ft_fillline(files, line, fd);
	return (1);
}
