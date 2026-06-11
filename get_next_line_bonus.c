/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omatalla <omatalla@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 12:40:07 by omatalla          #+#    #+#             */
/*   Updated: 2026/06/11 13:05:51 by omatalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*append_buffer(char *line_container, char *buffer)
{
	char	*tmp_line_container;

	tmp_line_container = ft_strjoin(line_container, buffer);
	free(line_container);
	return (tmp_line_container);
}

char	*read_and_store(int fd, char *line_container)
{
	char	*buffer;
	ssize_t	bytes_read;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	if (!line_container)
		line_container = ft_strdup("");
	if (!line_container)
		return (free(buffer), NULL);
	while (bytes_read > 0 && !ft_strchr(line_container, '\n'))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
			return (free(buffer), free(line_container), NULL);
		buffer[bytes_read] = '\0';
		line_container = append_buffer(line_container, buffer);
		if (!line_container)
			return (free(buffer), NULL);
	}
	free(buffer);
	return (line_container);
}

char	*extract_line(char *line_container)
{
	char	*line;
	size_t	line_length;

	if (!line_container || line_container[0] == '\0')
		return (NULL);
	line_length = 0;
	while (line_container[line_length] != '\n'
		&& line_container[line_length] != '\0')
		line_length++;
	if (line_container[line_length] == '\n')
		line_length++;
	line = malloc((line_length + 1) * sizeof(char));
	if (!line)
		return (NULL);
	ft_memcpy(line, line_container, line_length);
	line[line_length] = '\0';
	return (line);
}

char	*update_line_container(char *line_container)
{
	char	*updated_container;
	size_t	i;

	i = 0;
	while (line_container[i] != '\n'
		&& line_container[i] != '\0')
		i++;
	if (line_container[i] == '\n')
		i++;
	if (line_container[i] == '\0')
		return (free(line_container), NULL);
	updated_container = ft_strdup(&line_container[i]);
	free(line_container);
	return (updated_container);
}

char	*get_next_line(int fd)
{
	static char	*line_container[1024];
	char		*extracted_line;

	if (fd < 0 || fd >= 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	if (read(fd, 0, 0) < 0)
	{
		free(line_container[fd]);
		line_container[fd] = NULL;
		return (NULL);
	}
	line_container[fd] = read_and_store(fd, line_container[fd]);
	if (!line_container[fd])
		return (NULL);
	extracted_line = extract_line(line_container[fd]);
	if (!extracted_line)
	{
		free(line_container[fd]);
		line_container[fd] = NULL;
		return (NULL);
	}
	line_container[fd] = update_line_container(line_container[fd]);
	return (extracted_line);
}

/*Why 1024? 
Because that's the maximum number of file descriptors
that can be open simultaneously.
Historically, Unix systems often used 1024 
as the default maximum number of 
simultaneously open file descriptors per process*/
