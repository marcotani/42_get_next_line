/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtani <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 14:00:33 by mtani             #+#    #+#             */
/*   Updated: 2023/11/15 16:44:19 by mtani            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
//#include <unistd.h>
//#include <stdlib.h>
//#include <fcntl.h>

char	*join_free(char *buffer, char *readbuf)
{
	char	*tmp;
	int		i;

	i = 0;
	if (buffer == NULL)
	{
		tmp = malloc(sizeof(char) * (ft_strlen(readbuf) + 1));
		if (tmp == NULL)
			return (NULL);
		while (readbuf[i])
		{
			tmp[i] = readbuf[i];
			i++;
		}
		tmp[i] = '\0';
		return (tmp);
	}
	tmp = ft_strjoin(buffer, readbuf);
	if (tmp == NULL)
		return (NULL);
	free(buffer);
	buffer = NULL;
	return (tmp);
}

char	*clean_buf(char *buffer)
{
	char	*tmp;
	int		start;
	int		size;

	start = 0;
	if (buffer == NULL)
		return (NULL);
	while (buffer[start] != '\n' && buffer[start] != '\0')
		start++;
	if (buffer[start] == '\0')
	{
		free (buffer);
		return (NULL);
	}
	size = start + 1;
	while (buffer[size] != '\0')
		size++;
	tmp = ft_calloc(size - start + 1, sizeof(char));
	if (tmp == NULL)
		return (NULL);
	size = 0;
	while (buffer[start + 1] != '\0')
		tmp[size++] = buffer[(start++) + 1];
	free(buffer);
	return (tmp);
}

char	*extract_line(char *buffer)
{
	char	*line;
	int		start;
	int		i;

	i = 0;
	start = 0;
	if (buffer == NULL || buffer[0] == '\0')
		return (NULL);
	while (buffer[start] != '\n' && buffer[start] != '\0')
		start++;
	line = ft_calloc(start + 1 + (buffer[start] == '\n'), sizeof(char));
	if (line == NULL)
		return (NULL);
	if (buffer[start] == '\n')
		start++;
	while (i < start && buffer[i] != '\0')
	{
		line[i] = buffer[i];
		i++;
	}
	return (line);
}

char	*read_line(char *buffer, char *readbuf, int fd)
{
	int	read_check;

	read_check = 1;
	while (read_check > 0)
	{
		read_check = read(fd, readbuf, BUFFER_SIZE);
		if (read_check < 0)
			return (NULL);
		if (read_check == 0)
			break ;
		readbuf[read_check] = '\0';
		buffer = join_free(buffer, readbuf);
		if (ft_strchr(buffer, '\n') != NULL)
			break ;
	}
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer[4096];
	char		*readbuf;
	char		*line;

	if (fd < 0 || BUFFER_SIZE < 1 || read(fd, 0, 0) < 0)
	{
		if (buffer[fd] != NULL)
		{
			free(buffer[fd]);
			buffer[fd] = NULL;
		}
		return (NULL);
	}
	readbuf = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (readbuf == NULL)
		return (NULL);
	if (ft_strchr(buffer[fd], '\n') == NULL)
		buffer[fd] = read_line(buffer[fd], readbuf, fd);
	free (readbuf);
	readbuf = NULL;
	line = extract_line(buffer[fd]);
	buffer[fd] = clean_buf(buffer[fd]);
	return (line);
}

/*int main()
{
	int	fd0 = open("test0.txt", O_RDONLY);
	char *line;

	int fd1 = open("test1.txt", O_RDONLY);
	int fd2 = open("test2.txt", O_RDONLY);
	while ((line = get_next_line(fd0)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	line = get_next_line(fd1);
	printf("%s", line);
	free(line);
	while ((line = get_next_line(fd2)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	while ((line = get_next_line(fd1)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd0);
	close(fd1);
	close(fd2);
}*/
