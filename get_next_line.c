/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbaran <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/03 15:00:20 by rbaran            #+#    #+#             */
/*   Updated: 2016/04/15 10:05:57 by rbaran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_fd	*ft_checkfiles(int fd, t_fd *files)
{
	t_fd	*begin_files;

	begin_files = files;
	if (files)
	{
		while (files->next->fd != begin_files->fd)
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
		file->start = 0;
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
	char	*str_buf;
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
		str_buf = file->line;
		file->line = ft_strjoin(file->line, buf);
		free(str_buf);
	}
	return (res);
}

static void	ft_fill_line(t_fd *file, char **line, int fd, t_fd **files)
{
	size_t	i;
	t_fd	*file_buf;

	i = file->start;
	while (file->line[i] != '\0' && file->line[i] != '\n')
		i++;
	*line = ft_strsub(file->line, file->start, i - file->start);
	file->start = i;
	if (file->line[i] == '\n')
		file->start++;
	if (file->line[file->start] == 0)
	{
		file_buf = file;
		if (file->fd == file->next->fd)
			*files = NULL;
		else
			while (file->next->fd != fd)
				file = file->next;
		file->next = file->next->next;
		if ((*files) && (*files)->fd == file_buf->fd)
			*files = (*files)->next;
		free(file_buf->line);
		free(file_buf);
	}
}

int			get_next_line(int const fd, char **line)
{
	char		buf[BUFF_SIZE + 1];
	int			ret;
	int			res;
	static t_fd	*files = NULL;

	res = -1;
	if (fd < 0 || fd == 1 || fd == 2 || !line)
		return (-1);
	ret = read(fd, buf, BUFF_SIZE);
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
