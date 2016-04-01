/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbaran <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/03 15:00:20 by rbaran            #+#    #+#             */
/*   Updated: 2016/04/01 12:26:18 by rbaran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

static t_fd	*ft_checkfiles(int fd, t_fd *files)
{
	t_fd	*begin_files;

	begin_files = files;
	if (files)
	{
		if (files->fd == fd)
			return (files);
		while (files->fd != fd && files->next->fd != begin_files->fd)
		{
			if (files->fd == fd)
				return (files);
			files = files->next;
		}
		if (files->fd == fd)
			return (files);
	}
	return (NULL);
}

static t_fd	*ft_createfile(char *buf, int fd, t_fd *files)
{
	t_fd	*file;
	t_fd	*file_buf;

	if ((file = (t_fd*)ft_memalloc(sizeof(t_fd))))
	{
		file->line = ft_strdup(buf);
		file->begin_line = file->line;
		file->fd = fd;
		if (!files)
			file->next = file;
		else
		{
			file_buf = files->next;
			files->next = file;
			file->next = file_buf;
		}
	}
	return (file);
}

static int	ft_checkbuf(char *buf, t_fd **files, int fd)
{
	int		i;
	int		res;
	t_fd	*file;

	i = 0;
	res = 0;
	while (buf[i] != '\0' && buf[i] != '\n')
		i++;
	if (buf[i] == '\n')
		res = 1;
	if (!files || !(*files) || !(ft_checkfiles(fd, *files)))
		*files = ft_createfile(buf, fd, *files);
	else
	{
		file = ft_checkfiles(fd, *files);
		file->line = ft_strjoin(file->line, buf);
		free(file->begin_line);
		file->begin_line = file->line;
	}
	return (res);
}

static void	ft_fill_line(t_fd *file, char **line, int fd, t_fd **files)
{
	int		i;
	t_fd	*file_buf;

	i = 0;
	*line = file->line;
	while (file->line[i] != '\0' && file->line[i] != '\n')
		i++;
	file->line = (file->line) + i;
	if (*(file->line) == '\n')
	{
		*(file->line) = '\0';
		file->line = (file->line) + 1;
	}
	if (*(file->line) == 0)
	{
		file_buf = file;
		while (file->next->fd != fd)
			file = file->next;
		if (file->fd == file->next->fd)
			*files = NULL;
		file->next = file->next->next;
		free(file_buf);
	}
}

int			get_next_line(int const fd, char **line)
{
	char		buf[BUF_SIZE + 1];
	int			ret;
	int			res;
	static t_fd	*files = NULL;

	res = -1;
	if (fd < 0 || fd == 1 || fd == 2 || !line)
		return (-1);
	ret = read(fd, buf, BUF_SIZE);
	if (ret == -1 || (ret == 0 && (!ft_checkfiles(fd, files))))
		return (ret);
	buf[ret] = '\0';
	if (ret > 0)
		if (!(res = ft_checkbuf((char*)buf, &files, fd)))
			get_next_line(fd, line);
	if (((ret > 0 && res == 1) || ret == 0) && ft_checkfiles(fd, files))
		ft_fill_line(ft_checkfiles(fd, files), line, fd, &files);
	return (1);
}
